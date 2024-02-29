#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Sensor.hpp>
#include <ECUSensors.hpp>

#pragma once

class SensorFactory{
    public:
    static SensorFactory* getInstance(){return new SensorFactory();}

    static Sensor* createFromMsg(const CAN_message_t &msg);
    static Sensor* createFromAbbr(String abbr, uint8_t num);

    private:
    explicit SensorFactory(){load();}
    static void load();

    static identifier types[200];
};

void SensorFactory::load(){
    types[0] = ECU::EngineStatus::getIdentity();
    types[1] = ECU::GearStatus::getIdentity();
    types[2] = ECU::PumpStatus::getIdentity();
    types[3] = ECU::BatteryStatus::getIdentity();
}

Sensor* SensorFactory::createFromMsg(const CAN_message_t &msg){
    String abbr = msg.buf[0];
    abbr.concat(msg.buf[1]);
    abbr.concat(msg.buf[2]);

    return createFromAbbr(abbr, msg.buf[3]);
}

Sensor* SensorFactory::createFromAbbr(String abbr, uint8_t num){
    for(int i = 0; i < 200; i++){
        if(types[i].abbr == abbr){
            return types[i].constructor(num);
        }
    }

    return nullptr;
}