/*
MPU6050 code
*/
#include <Wire.h>
#include <math.h>
#include <stdio.h>
const int MPU = 0x68; // I2C address


float Xacc, Yacc, Zacc;
float Xgyro, Ygyro, Zgyro;
float XaccAngle, YaccAngle, XgyroAngle, YgyroAngle, ZgyroAngle;
float roll, pitch, yaw;
float XaccError, YaccError, XgyroError, YgyroError, ZgyroError;
float currentTime, previousTime, elapsedTime;


int i = 0;


void setup(){


  Serial.begin(19200);
  Wire.begin();
  Wire.beginTransmission(MPU); //INIT I2C for MPU




//PWR_MGMT_1
  Wire.write(0x6B); // reg 6B
  Wire.write(0x8); // 00001000 reset values wakes up mpu and disables temperature sensor
  Wire.endTransmission(true);


//ACCEL_CONFIG
  Wire.beginTransmission(MPU);
  Wire.write(0x1C); // reg 1C
  Wire.write(0x10); // 00010000 sets range to +/- 8g
  Wire.endTransmission(true);


// GYRO_CONFIG
  Wire.beginTransmission(MPU);
  Wire.write(0x1B); // reg 1B
  Wire.write(0x100); // 00000000 sets range to +/- 250 deg/sec
  Wire.endTransmission(true);


// Calculate errors
  MPU_offset_test();


  delay(20);
  Serial.print("acc error X: ");
  Serial.println(XaccError);


  Serial.print("acc error Y: ");
  Serial.println(YaccError);


  Serial.print("gyro error X: ");
  Serial.println(XgyroError);


  Serial.print("gyro error Y: ");
  Serial.println(YgyroError);


  Serial.print("gyro error Z: ");
  Serial.println(ZgyroError);


  currentTime = millis();
}






void loop(){


//Read acceleration measurements
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // read 6 regs


  Xacc = (Wire.read() << 8 | Wire.read()) /4096.0; //bitwise or combines them
  Yacc = (Wire.read() << 8 | Wire.read()) /4096.0;
  Zacc = (Wire.read() << 8 | Wire.read()) /4096.0;


  XaccAngle = (atan(Yacc / sqrt(pow(Xacc,2) + pow(Zacc,2))) *180/PI) - XaccError;
  YaccAngle = (atan(-1 * Xacc / sqrt(pow(Yacc,2) + pow(Zacc,2))) *180/PI) - YaccError;


//Read gyroscope measurements
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000; //in seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // read 6 regs


  Xgyro = (Wire.read() << 8 | Wire.read()) /131.0; //bitwise or combines them
  Ygyro = (Wire.read() << 8 | Wire.read()) /131.0;
  Zgyro = (Wire.read() << 8 | Wire.read()) /131.0;


  Xgyro = Xgyro - XgyroError;
  Ygyro = Ygyro - YgyroError;
  Zgyro = Zgyro - ZgyroError;


  XgyroAngle = XgyroAngle + Xgyro * elapsedTime;
  YgyroAngle = YgyroAngle + Ygyro * elapsedTime;
  yaw = yaw + Zgyro * elapsedTime;


// complementary filter
  XgyroAngle = 0.95 * XgyroAngle + 0.05 * XaccAngle;
  YgyroAngle = 0.95 * YgyroAngle + 0.05 * YaccAngle;


  roll = XgyroAngle;
  pitch = YgyroAngle;


  delay(50);
  Serial.print("roll:");
  Serial.print(roll,2);
  Serial.print(",pitch:");
  Serial.print(pitch,2);
  Serial.print(",yaw:");
  Serial.print(yaw,2);
  Serial.println("");
}






void MPU_offset_test(){
// Test ACCEL Measurements
  while(i < 250){


    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); // read 6 regs


    Xacc = (Wire.read() << 8 | Wire.read()) /4096.0; //bitwise or combines them
    Yacc = (Wire.read() << 8 | Wire.read()) /4096.0;
    Zacc = (Wire.read() << 8 | Wire.read()) /4096.0;


    XaccError = (atan(Yacc / sqrt(pow(Xacc,2) + pow(Zacc,2))) *180/PI);
    YaccError = (atan(-1 * Xacc / sqrt(pow(Yacc,2) + pow(Zacc,2))) *180/PI);
    i++;
  }


  XaccError = XaccError/250;
  YaccError = YaccError/250;
  i = 0;


// Test GYRO Measurements
 while(i < 250){


    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); // read 6 regs


    Xgyro = (Wire.read() << 8 | Wire.read()) /131.0; //bitwise or combines them
    Ygyro = (Wire.read() << 8 | Wire.read()) /131.0;
    Zgyro = (Wire.read() << 8 | Wire.read()) /131.0;  


    XgyroError = XgyroError + Xgyro;
    YgyroError = YgyroError + Ygyro;
    ZgyroError = ZgyroError + Zgyro;
    i++;
 }
  XgyroError = XgyroError/250;
  YgyroError = YgyroError/250;
  ZgyroError = ZgyroError/250;
 


}









