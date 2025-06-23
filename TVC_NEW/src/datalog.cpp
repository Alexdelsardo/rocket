#include "datalog.h"
#include <SD.h>

File logFile;

void initDataLog() {
  if (!SD.begin(10)) {
    Serial.println("SD init failed!");
    return;
  }
  logFile = SD.open("flight.csv", FILE_WRITE);
  if (logFile) {
    logFile.println("Time_ms,Pitch,Yaw,AX,AY,AZ");
    logFile.flush();
  }
}

void logFlightData(unsigned long ms, float pitch, float yaw, float ax, float ay, float az) {
  if (logFile) {
    logFile.print(ms); logFile.print(",");
    logFile.print(pitch); logFile.print(",");
    logFile.print(yaw); logFile.print(",");
    logFile.print(ax); logFile.print(",");
    logFile.print(ay); logFile.print(",");
    logFile.println(az);
    logFile.flush();
  }
}

void closeDataLog() {
  if (logFile) logFile.close();
}
