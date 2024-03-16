#include <Arduino.h>
#include <DAQLine.hpp>
#include <ECUSensors.hpp>
#include <SensorDefinitions.h>

MF::DAQLine<CAN2> ECULine;
int prevTime = 0;

void setup() {
  Serial.begin(9600); 
  Serial2.begin(57600);

  ECULine.begin();
  ECULine.SDLoggingMode();
  ECULine.enableDynamicSensors();
  ECULine.addSensor(1280, BATTERY_STATUS, 0);
}

void loop() {
  ECULine.update();
  ECULine.sendReadOut(Serial2);

  if (millis()-prevTime > 50000){
    MF::SensorFactory::sendReadOut(Serial2);
  }
  delay(1);
}