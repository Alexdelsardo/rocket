#ifndef DATALOG_H
#define DATALOG_H

#include <Arduino.h>

void initDataLog();
void logFlightData(unsigned long ms, float pitch, float yaw, float ax, float ay, float az);
void closeDataLog();

#endif
