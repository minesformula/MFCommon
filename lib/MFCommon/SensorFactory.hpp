#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Sensor.hpp>
#include <ECUSensors.hpp>

#pragma once

typedef Sensor* (*creator)(uint8_t num);

class SensorFactory{
    public:
    static SensorFactory* getInstance(){return new SensorFactory();}

    static Sensor* createFromMsg(const CAN_message_t &msg);
    static Sensor* createFromAbbr(String abbr);

    private:
    SensorFactory(){load();}
    static void load();

    static creator types[200];
};

void SensorFactory::load(){
    types[0] = ECU::EngineStatus::create;
    types[1] = ECU::GearStatus::create;
    types[2] = ECU::PumpStatus::create;
    types[3] = ECU::BatteryStatus::create;
}