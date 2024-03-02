#include <Arduino.h>
#include <DAQLine.hpp>
#include <ECUSensors.hpp>
#include <SensorDefinitions.h>

MF::DAQLine<CAN1> ECULine;

void setup() {
  Serial.begin(115200); delay(400);
  MF::SensorFactory::load();
  ECULine.begin();
  ECULine.SDLoggingMode();
  ECULine.addSensor(1280, BATTERY_STATUS, 0);
}

void loop() {
  ECULine.update();
}