#include <Arduino.h>

#include "Sensor.hpp"
#include "ECUSensors.hpp"

#include "thirdParty/FlexCAN_T4/FlexCAN_T4.h"

#pragma once

namespace MF{
    class SensorFactory{
        public:
        static SensorFactory* getInstance(){return new SensorFactory();}

        static void load();

        static Sensor* createFromMsg(const CAN_message_t &msg);
        static Sensor* createFromAbbr(String abbr, uint8_t num);

        static void sendReadOut(HardwareSerial &serial);

        private:
        explicit SensorFactory(){load();}

        static identifier types[50];
        static uint8_t numTypes;
    };

    identifier SensorFactory::types[50];
    uint8_t SensorFactory::numTypes;

    void SensorFactory::load(){
        types[0] = ECU::EngineStatus::getIdentity();
        types[1] = ECU::GearStatus::getIdentity();
        types[2] = ECU::PumpStatus::getIdentity();
        types[3] = ECU::BatteryStatus::getIdentity();

        numTypes = 4;
    }

    Sensor* SensorFactory::createFromMsg(const CAN_message_t &msg){
        String abbr = char(msg.buf[0]);
        abbr.concat(char(msg.buf[1]));
        abbr.concat(char(msg.buf[2]));

        return createFromAbbr(abbr, msg.buf[3]);
    }

    Sensor* SensorFactory::createFromAbbr(String abbr, uint8_t num){
        for(int i = 0; i < numTypes; i++){
            if(types[i].abbr == abbr){
                return types[i].constructor(num);
            }
        }

        return nullptr;
    }

    void SensorFactory::sendReadOut(HardwareSerial &serial){
        for (uint8_t i = 0; i < numTypes; i++){
            String message = "0,";
            message.append(types[i].abbr);
            message.append(",");
            message.append(types[i].info);
            serial.println(message);
        }
    }
}