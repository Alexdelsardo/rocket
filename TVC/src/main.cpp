#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

// Create IMU and servo objects/class
MPU6050 MPU;
Servo PitchServo;
Servo YawServo;

// Configure pin connections and servo range
const int PitchServoPin = 9;    // Connect pitch servo signal to D9
const int YawServoPin   = 10;   // Connect yaw servo signal to D10
const int ServoNeutral  = 80;   // Neutral position for servos
const int ServoRange    = 10;   // Max angle adjustment (±10°)

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Initializing MPU...");
  MPU.initialize();
  Serial.println("MPU initialized...");

  if (!MPU.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (true); // Stop if MPU not found
  }

  Serial.println("MPU6050 connected successfully");

  // Attach and center servos
  PitchServo.attach(PitchServoPin);
  YawServo.attach(YawServoPin);
  PitchServo.write(ServoNeutral);
  YawServo.write(ServoNeutral);

  Serial.println("Servos attached and centered.");
}

void loop() {
  // Read raw data from MPU
  int16_t ax, ay, az, gx, gy, gz;
  MPU.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // Estimate pitch and yaw angles
  float pitch = atan2(ax, az) * 180.0 / PI;
  float yaw   = atan2(ay, az) * 180.0 / PI;
  // Convert angles to servo range
  float mappedPitch = map(pitch, -20, 20, ServoNeutral - ServoRange, ServoNeutral + ServoRange);
  float mappedYaw   = map(yaw,   -20, 20, ServoNeutral - ServoRange, ServoNeutral + ServoRange);
  // Keep angles within limit
  mappedPitch = constrain(mappedPitch, ServoNeutral - ServoRange, ServoNeutral + ServoRange);
  mappedYaw   = constrain(mappedYaw,   ServoNeutral - ServoRange, ServoNeutral + ServoRange);

  PitchServo.write(mappedPitch);
  YawServo.write(mappedYaw);

  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print("  ServoP: "); Serial.print(mappedPitch);
  Serial.print("  Yaw: "); Serial.print(yaw);
  Serial.print("  ServoY: "); Serial.println(mappedYaw);

  delay(5); // Loop every 5ms
}


/*
//Test Code Below (Rotates motor in a circle)

#include <Servo.h>
#include <math.h>

Servo PitchServo;
Servo YawServo;

const int center = 80;      // Neutral angle
const int radius = 10;      // Max deviation from center (circle radius in degrees)

const float stepSize = 0.1; // Smaller = smoother, larger = faster
float angle = 0;            // Angle in radians

void setup() {
  PitchServo.attach(9);     // Pitch servo
  YawServo.attach(10);      // Yaw servo
}

void loop() {
  // Compute positions as circular path
  float pitchOffset = sin(angle) * radius;
  float yawOffset   = cos(angle) * radius;

  PitchServo.write(center + pitchOffset);
  YawServo.write(center + yawOffset);

  angle += stepSize;
  if (angle >= 2 * PI) angle = 0;

  delay(5);  // Adjust delay for speed
}
*/