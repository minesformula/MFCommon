#include "Sensor.hpp"

#pragma once

namespace ECU{
    //Produces RPM (pos0) and WaterTemp (pos1) data
    class EngineStatus : public Sensor {
        public:
        EngineStatus() : Sensor("ENG", 0) {};
        EngineStatus(uint8_t num) : Sensor("ENG", num) {};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static Sensor* create(uint8_t num){return new EngineStatus(num);}
    };

    void EngineStatus::query(){
        return;
    }

    //Indicates status of fuelpump (pos0), waterpump (pos1) and fan (pos2)
    class PumpStatus: public Sensor {
        public:
        PumpStatus() : Sensor("PMP", 0) {};
        PumpStatus(uint8_t num) : Sensor("PMP", num) {};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static Sensor* create(uint8_t num){return new PumpStatus(num);}
    };

    //pos0 is zero if neutral
    class GearStatus: public Sensor {
        public:
        GearStatus() : Sensor("GEA", 0) {};
        GearStatus(uint8_t num) : Sensor("GEA", num) {};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static Sensor* create(uint8_t num){return new GearStatus(num);}
    };

    //Records Battery Voltage
    class BatteryStatus: public Sensor {
        public:
        BatteryStatus() : Sensor("BTV", 0) {};
        BatteryStatus(uint8_t num) : Sensor("BTV", num) {};

        void query();

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static Sensor* create(uint8_t num){return new BatteryStatus(num);}
    };

    CAN_message_t EngineStatus::writeToMsg(){
        CAN_message_t msg;
        msg.len = 8;
        
        uint8_t* temp = static_cast<uint8_t*>(static_cast<void*>(&data[0]));
        for (size_t i = 0; i < sizeof(float); i++){
            msg.buf[i] = temp[i];
        }

        uint8_t* temp = static_cast<uint8_t*>(static_cast<void*>(&data[1]));
        for (size_t i = 0; i < sizeof(float); i++){
            msg.buf[i+4] = temp[i];
        }

        return msg;
    }

    void EngineStatus::readFromMsg(const CAN_message_t& msg){
        data[0] = msg.buf[0] * 100;
        data[1] = msg.buf[3] * 100;
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
}