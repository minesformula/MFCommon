#include <Arduino.h>

#include "Sensor.hpp"
#include "ECUSensors.hpp"
#include "MotionSensors.hpp"

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
        inline explicit SensorFactory(){load();}

        static identifier types[50];
        static uint8_t numTypes;
    };

    identifier SensorFactory::types[50];
    uint8_t SensorFactory::numTypes;

    /// @brief Loads all implemented sensors allowing Sensor Factory to create sensors
    void SensorFactory::load(){
        types[0] = ECU::EngineStatus::getIdentity();
        types[1] = ECU::GearStatus::getIdentity();
        types[2] = ECU::PumpStatus::getIdentity();
        types[3] = ECU::BatteryStatus::getIdentity();
        types[4] = ECU::EngineRuntimeStats::getIdentity();
        types[5] = ECU::ThrottleStatus::getIdentity();
        types[6] = ECU::BrakeStatus::getIdentity();
        types[7] = ECU::EngineTemp::getIdentity();
        types[8] = Motion::Accelerometer::getIdentity();
        types[9] = Motion::Gyroscope::getIdentity();

        numTypes = 10;
    }

    /// @brief Reads the first three data values from a CAN message as an abbreviation and 
    /// attempts to find/create a matching sensor. Returns nullptr on failure. CAN message
    /// should be formatted such that the first three values are the abbreviation and the fourth
    /// value is the sensor number. The 4 remaining CAN values are open and may be used by other
    /// functions.
    /// @param msg CAN message to process
    /// @return Pointer to created Sensor
    Sensor* SensorFactory::createFromMsg(const CAN_message_t &msg){
        String abbr = char(msg.buf[0]);
        abbr.concat(char(msg.buf[1]));
        abbr.concat(char(msg.buf[2]));

        return createFromAbbr(abbr, msg.buf[3]);
    }

    /// @brief Attempts to create a sensor given an abbreviation. Returns nullptr on failure.
    /// @param abbr Sensor abbreviation (Check SensorDefinitions.hpp for a list)
    /// @param num Sensor Number for distinguishing sensors
    /// @return Pointer to created sensor
    Sensor* SensorFactory::createFromAbbr(String abbr, uint8_t num){
        for(int i = 0; i < numTypes; i++){
            if(types[i].abbr == abbr){
                return types[i].constructor(num);
            }
        }

        return nullptr;
    }

    /// @brief Returns a string containing every sensor's abbreviation and the names
    /// of each value in the data section of each sensor. Every line is preceded by a
    /// zero. Intended for use in dynamic communication protocols.
    /// @return String of sensors with identifying features
    String SensorFactory::getReadOut(){
        String message = "0,";
        for (uint8_t i = 0; i < numTypes; i++){
            message.append(types[i].abbr);
            message.append(",");
            message.append(types[i].info);
            message.append('\n');

            if (numTypes-1 != i){
                message.append("0,");
            }
        }
        return message;
    }

    /// @brief Prints output of getReadOut() to a given serial line.
    /// @param serial Serial to print to.
    void SensorFactory::sendReadOut(HardwareSerial &serial){
        serial.println(getReadOut());
        serial.flush();
    }
}