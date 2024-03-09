#include "Sensor.hpp"
#include "SensorDefinitions.h"

#pragma once

namespace ECU{
    //Produces RPM (pos0) and WaterTemp (pos1) data
    class EngineStatus : public Sensor {
        public:
        EngineStatus() : Sensor(ENGINE_STATUS, 0) {};
        EngineStatus(uint8_t num) : Sensor(ENGINE_STATUS, num) {};
        ~EngineStatus(){};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new EngineStatus(num);}
        static identifier getIdentity()
            {return { .abbr = ENGINE_STATUS, .constructor = create, .info = getInfo()};}
    };


    //Indicates status of fuelpump (pos0), waterpump (pos1) and fan (pos2)
    class PumpStatus: public Sensor {
        public:
        PumpStatus() : Sensor(PUMP_STATUS, 0) {};
        PumpStatus(uint8_t num) : Sensor(PUMP_STATUS, num) {};
        ~PumpStatus(){};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new PumpStatus(num);}
        static identifier getIdentity()
            {return { .abbr = PUMP_STATUS, .constructor = create, .info = getInfo()};}
    };

    //pos0 is zero if neutral
    class GearStatus: public Sensor {
        public:
        GearStatus() : Sensor(GEAR_STATUS, 0) {};
        GearStatus(uint8_t num) : Sensor(GEAR_STATUS, num) {};
        ~GearStatus(){};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new GearStatus(num);}
        static identifier getIdentity()
            {return { .abbr = GEAR_STATUS, .constructor = create, .info = getInfo()};}
    };

    //Records Battery Voltage
    class BatteryStatus: public Sensor {
        public:
        BatteryStatus() : Sensor(BATTERY_STATUS, 0) {};
        BatteryStatus(uint8_t num) : Sensor(BATTERY_STATUS, num) {};
        ~BatteryStatus(){};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new BatteryStatus(num);}
        static identifier getIdentity()
            {return { .abbr = BATTERY_STATUS, .constructor = create, .info = getInfo()};}
    };

    void EngineStatus::query(){
        return;
    }

    CAN_message_t EngineStatus::writeToMsg(){
        CAN_message_t msg;
        msg.len = 8;
        
        uint8_t* temp = static_cast<uint8_t*>(static_cast<void*>(&data[0]));
        for (size_t i = 0; i < sizeof(float); i++){
            msg.buf[i] = temp[i];
        }

        temp = static_cast<uint8_t*>(static_cast<void*>(&data[1]));
        for (size_t i = 0; i < sizeof(float); i++){
            msg.buf[i+4] = temp[i];
        }

        return msg;
    }

    void EngineStatus::readFromMsg(const CAN_message_t& msg){
        data[0] = msg.buf[0] * 100;
        data[1] = msg.buf[3] * 100;
    }

    String EngineStatus::getInfo(){
        return "Engine_RPM,WaterTemp";
    }

    void PumpStatus::query(){
        return;
    }

    CAN_message_t PumpStatus::writeToMsg(){
        CAN_message_t msg;
        msg.len = 3;  
        msg.buf[0] = data[0];
        msg.buf[1] = data[1];
        msg.buf[2] = data[2];

        return msg;
    }

    void PumpStatus::readFromMsg(const CAN_message_t& msg){
        if (msg.buf[0] != 0 && msg.buf[0] != 1){
            return;
        }

        data[0] = msg.buf[0];
        data[1] = msg.buf[1];
        data[2] = msg.buf[3];
    }

    String PumpStatus::getInfo(){
        return "FuelPumpBool,WaterPumpBool,FanBool";
    }

    void GearStatus::query(){
        return;
    }

    CAN_message_t GearStatus::writeToMsg(){
        CAN_message_t msg;
        msg.len = 1;  
        msg.buf[0] = data[0];

        return msg;
    }

    void GearStatus::readFromMsg(const CAN_message_t& msg){
        data[0] = (msg.buf[4] & 0x10 ) << 0x10;
    }

    String GearStatus::getInfo(){
        return "NeutralBool";
    }

    void BatteryStatus::query(){
        return;
    }

    CAN_message_t BatteryStatus::writeToMsg(){
        CAN_message_t msg;
        msg.len = 4;
        
        uint8_t* temp = static_cast<uint8_t*>(static_cast<void*>(&data[0]));
        for (size_t i = 0; i < sizeof(float); i++){
            msg.buf[i] = temp[i];
        }

        return msg;
    }

    void BatteryStatus::readFromMsg(const CAN_message_t& msg){
        data[0] = float(msg.buf[2]) * 0.1216;
    }

    String BatteryStatus::getInfo(){
        return "Battery_Voltage";
    }
}