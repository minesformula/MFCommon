#include <Arduino.h>
#include <DAQLine.hpp>
#include <ECUSensors.hpp>
#include <SensorDefinitions.h>

MF::DAQLine<CAN2> DAQLine;
int prevTime = 0;

void setup() {
  Serial.begin(9600); 
  Serial2.begin(57600);

  DAQLine.begin();
  DAQLine.SDLoggingMode();
  DAQLine.enableDynamicSensors();
  DAQLine.enableLiveTelemetry(Serial2);
  DAQLine.addSensor(1280, BATTERY_STATUS, 0);
}

void loop() {
  DAQLine.update();

  if (millis()-prevTime > 10000){
    MF::SensorFactory::sendReadOut(Serial2);
    prevTime = millis();
  }
  delay(1);
}