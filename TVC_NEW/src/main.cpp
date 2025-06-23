#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>
#include "datalog.h"


MPU6050 MPU;
Servo PitchServo;
Servo YawServo;

// Servo and control configuration
const int PitchServoPin = 9;
const int YawServoPin   = 10;
const int PitchCenter   = 112;
const int YawCenter     = 99;
const int PitchRange    = 10;
const int YawRange      = 10;
const int parachutePin  = 6;

// Flight control states
enum FlightState { IDLE, ASCENT, COAST, APOGEE, DESCENT };
FlightState currentState = IDLE;

// User-selected mode (via serial input)
enum Mode { WAITING, FLIGHT, SERVO_SWEEP, CENTER_YAW, CENTER_PITCH };
Mode selectedMode = WAITING;

// Flight timing and parachute control
unsigned long launchTime = 0;
bool parachuteDeployed = false;
unsigned long chuteFireTime = 0;

// PID and filtering state
float pitch = 0, yaw = 0;
float lastPitchError = 0, lastYawError = 0;
float pitchErrorSum = 0, yawErrorSum = 0;
unsigned long lastTime = 0;

// PID tuning parameters
const float kp = 2.5, ki = 0.0, kd = 0.8;

// Complementary filter blending factor
const float alpha = 0.98;

// Test Modes

void sweepTest() {
  float angle = 0;
  const float stepSize = 0.1;
  Serial.println(">> SERVO SWEEP TEST - Press 'e' to exit");
  while (true) {
    int pitchVal = PitchCenter + int(sin(angle) * PitchRange * 2);
    int yawVal   = YawCenter + int(cos(angle) * YawRange * 2);
    PitchServo.write(pitchVal);
    YawServo.write(yawVal);
    Serial.print("PitchServo: "); Serial.print(pitchVal);
    Serial.print("  YawServo: "); Serial.println(yawVal);
    angle += stepSize;
    if (angle >= 2 * PI) angle -= 2 * PI;
    if (Serial.available() && Serial.read() == 'e') {
      Serial.println(">> EXITED SWEEP MODE");
      selectedMode = WAITING;
      break;
    }
    delay(20);
  }
}

void centerYawTest() {
  Serial.println(">> CENTER YAW - Press 'e' to exit");
  YawServo.write(YawCenter);
  while (true) {
    for (int i = -YawRange; i <= YawRange; i++) {
      int val = YawCenter + i;
      YawServo.write(val);
      Serial.print("YawServo: "); Serial.println(val);
      delay(200);
      if (Serial.available() && Serial.read() == 'e') {
        Serial.println(">> EXITED CENTER YAW TEST");
        selectedMode = WAITING;
        return;
      }
    }
  }
}

void centerPitchTest() {
  Serial.println(">> CENTER PITCH - Press 'e' to exit");
  PitchServo.write(PitchCenter);
  while (true) {
    for (int i = -PitchRange; i <= PitchRange; i++) {
      int val = PitchCenter + i;
      PitchServo.write(val);
      Serial.print("PitchServo: "); Serial.println(val);
      delay(200);
      if (Serial.available() && Serial.read() == 'e') {
        Serial.println(">> EXITED CENTER PITCH TEST");
        selectedMode = WAITING;
        return;
      }
    }
  }
}

// Setup & Loop

void setup() {
  Serial.begin(115200);
  Wire.begin();
  MPU.initialize();
  if (!MPU.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (true);
  }
  PitchServo.attach(PitchServoPin);
  YawServo.attach(YawServoPin);
  PitchServo.write(PitchCenter);
  YawServo.write(YawCenter);
  pinMode(parachutePin, OUTPUT);
  digitalWrite(parachutePin, LOW);
  Serial.println("Select mode: [f]light  [1]sweep  [2]center YAW  [3]center PITCH");
  lastTime = millis();

  initDataLog();

}

void loop() {
  // Serial command mode
  if (selectedMode == WAITING) {
    if (Serial.available()) {
      char input = Serial.read();
      if (input == 'f') selectedMode = FLIGHT;
      else if (input == '1') selectedMode = SERVO_SWEEP;
      else if (input == '2') selectedMode = CENTER_YAW;
      else if (input == '3') selectedMode = CENTER_PITCH;
    }
    return;
  }

  //  Test modes
  if (selectedMode == SERVO_SWEEP) { sweepTest(); return; }
  if (selectedMode == CENTER_YAW)  { centerYawTest(); return; }
  if (selectedMode == CENTER_PITCH){ centerPitchTest(); return; }

  // Sensor Read
  int16_t ax, ay, az, gx, gy, gz;
  MPU.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Manual override
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'l' && currentState == IDLE) {
      currentState = ASCENT;
      launchTime = millis();
      Serial.println(">> ASCENT (manual)");
    }
    if (input == 'e') {
      currentState = IDLE;
      selectedMode = WAITING;
      Serial.println(">> EXITED FLIGHT MODE");
      return;
    }
  }

  // Time Change
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // Complementary Filter
  float accPitch = atan2(ay, az) * 180.0 / PI;
  float accYaw   = atan2(ax, az) * 180.0 / PI;
  float gyroPitchRate = gy / 131.0;
  float gyroYawRate   = gx / 131.0;
  pitch = alpha * (pitch + gyroPitchRate * dt) + (1 - alpha) * accPitch;
  yaw   = alpha * (yaw   + gyroYawRate   * dt) + (1 - alpha) * accYaw;

  logFlightData(millis(), pitch, yaw, ax, ay, az);


  // Launch/Flight State Machine
  float accMag = sqrt(ax * ax + ay * ay + az * az) / 16384.0;
  switch (currentState) {
    case IDLE:
      if (accMag > 1.5) {
        currentState = ASCENT;
        launchTime = millis();
        Serial.println(">> ASCENT");
      }
      break;
    case ASCENT:
      if (accMag < 0.5) {
        currentState = COAST;
        Serial.println(">> COAST");
      }
      break;
    case COAST:
      if (millis() - launchTime > 10000) {
        currentState = APOGEE;
        Serial.println(">> APOGEE");
      }
      break;
    case APOGEE:
      if (!parachuteDeployed) {
        digitalWrite(parachutePin, HIGH);
        chuteFireTime = millis();
        parachuteDeployed = true;
        Serial.println(">> PARACHUTE DEPLOYED");
      }
      currentState = DESCENT;
      break;
    case DESCENT:
      break;
  }

  // Parachute
  if (parachuteDeployed && millis() - chuteFireTime > 1000) {
    digitalWrite(parachutePin, LOW);

    closeDataLog();
  }

  // PID Flight Control
  if (currentState == ASCENT || currentState == COAST) {
    float pitchError = 0 - pitch;
    float yawError   = 0 - yaw;
    pitchErrorSum += pitchError * dt;
    yawErrorSum   += yawError * dt;
    float pitchDerivative = (pitchError - lastPitchError) / dt;
    float yawDerivative   = (yawError   - lastYawError) / dt;
    lastPitchError = pitchError;
    lastYawError   = yawError;
    float pitchOutput = kp * pitchError + ki * pitchErrorSum + kd * pitchDerivative;
    float yawOutput   = kp * yawError   + ki * yawErrorSum   + kd * yawDerivative;
    int pitchServoPos = PitchCenter + constrain(pitchOutput, -PitchRange, PitchRange);
    int yawServoPos   = YawCenter   + constrain(yawOutput,   -YawRange,   YawRange);
    PitchServo.write(pitchServoPos);
    YawServo.write(yawServoPos);
    Serial.print("PitchServo: "); Serial.print(pitchServoPos);
    Serial.print("  YawServo: "); Serial.println(yawServoPos);
  } else {
    PitchServo.write(PitchCenter);
    YawServo.write(YawCenter);
  }

  delay(5);
}