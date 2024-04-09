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

        static String getReadOut();
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
        types[4] = ECU::EngineRuntimeStats::getIdentity();
        types[5] = ECU::ThrottleStatus::getIdentity();
        types[6] = ECU::BrakeStatus::getIdentity();
        types[7] = ECU::EngineTemp::getIdentity();

        numTypes = 8;
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

    String SensorFactory::getReadOut(){
        String message = "0,";
        for (uint8_t i = 0; i < numTypes; i++){
            message.append(types[i].abbr);
            message.append(",");
            message.append(types[i].info);
        }
        return message;
    }

    void SensorFactory::sendReadOut(HardwareSerial &serial){
        serial.println(getReadOut());
        serial.flush();
    }
}