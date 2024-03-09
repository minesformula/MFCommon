#include <Arduino.h>
#include <DAQLine.hpp>
#include <ECUSensors.hpp>
#include <SensorDefinitions.h>

MF::DAQLine<CAN1> ECULine;

void setup() {
  Serial.begin(9600); delay(400);
  ECULine.begin();
  ECULine.SDLoggingMode();
  ECULine.enableDynamicSensors();
  ECULine.addSensor(1280, BATTERY_STATUS, 0);
}

void loop() {
  ECULine.update();
}