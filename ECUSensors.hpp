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
        void query(uint8_t pin);

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
        void query(uint8_t pin);

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
        void query(uint8_t pin);

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
        void query(uint8_t pin);

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new BatteryStatus(num);}
        static identifier getIdentity()
            {return { .abbr = BATTERY_STATUS, .constructor = create, .info = getInfo()};}
    };

    class ThrottleStatus : public Sensor {
        public:
        ThrottleStatus() : Sensor(THROTTLE_STATUS, 0) {};
        ThrottleStatus(uint8_t num) : Sensor(THROTTLE_STATUS, num) {};
        ~ThrottleStatus(){};

        void query();
        void query(uint8_t pin);

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new ThrottleStatus(num);}
        static identifier getIdentity()
            {return { .abbr = THROTTLE_STATUS, .constructor = create, .info = getInfo()};}
    };

    class EngineRuntimeStats : public Sensor {
        public:
         EngineRuntimeStats() : Sensor(ENGINE_RUNTIME, 0) {};
         EngineRuntimeStats(uint8_t num) : Sensor(ENGINE_RUNTIME, num) {};
        ~ EngineRuntimeStats(){};

        void query();
        void query(uint8_t pin);

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new  EngineRuntimeStats(num);}
        static identifier getIdentity()
            {return { .abbr = ENGINE_RUNTIME, .constructor = create, .info = getInfo()};}
    };

    class BrakeStatus : public Sensor {
    public:
    BrakeStatus() : Sensor(BRAKE_STATUS, 0) {};
    BrakeStatus(uint8_t num) : Sensor(BRAKE_STATUS, num) {};
    ~BrakeStatus(){};

    void query();
    void query(uint8_t pin);

    CAN_message_t writeToMsg();

    void readFromMsg(const CAN_message_t& msg);

    static String getInfo();

    static Sensor* create(uint8_t num){return new BrakeStatus(num);}
    static identifier getIdentity()
        {return { .abbr = BRAKE_STATUS, .constructor = create, .info = getInfo()};}
    };

    class EngineTemp : public Sensor {
        public:
        EngineTemp() : Sensor(ENGINE_TEMPERATURE, 0) {};
        EngineTemp(uint8_t num) : Sensor(ENGINE_TEMPERATURE, num) {};
        ~EngineTemp(){};

        void query();
        void query(uint8_t pin);

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new EngineTemp(num);}
        static identifier getIdentity()
            {return { .abbr = ENGINE_TEMPERATURE, .constructor = create, .info = getInfo()};}
    };

    void EngineStatus::query(){
        return;
    }

    void EngineStatus::query(uint8_t pin){
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
        data[1] = msg.buf[1];
        data[2] = msg.buf[2];
        data[3] = msg.buf[3];
        data[4] = msg.buf[4];
        data[5] = msg.buf[5];
        data[6] = msg.buf[6];
        data[7] = msg.buf[7];
    }

    String EngineStatus::getInfo(){
        return "Engine_RPM(RPM),Throttle_Pos(%),Vehicle_Speed(kmh),Water_Temp(C),Oil_Temp(C),Fuel_Temp(C),Transmission_Temp(C),Differential_Temp(C)";
    }

    void PumpStatus::query(){
        return;
    }

    void PumpStatus::query(uint8_t pin){
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

    void GearStatus::query(uint8_t pin){
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

    void BatteryStatus::query(uint8_t pin){
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
        if (msg.buf[0] != 48) return;
        data[0] = float(msg.buf[2]) * 0.1216;
    }

    String BatteryStatus::getInfo(){
        return "Battery_Voltage";
    }

    void ThrottleStatus::query(){
        return;
    }

    void ThrottleStatus::query(uint8_t pin){
        return;
    }

    CAN_message_t ThrottleStatus::writeToMsg(){
        CAN_message_t msg;
        msg.len = 4;
        for (size_t currPoint = 0; currPoint < 4; currPoint++){
            uint8_t* temp = static_cast<uint8_t*>(static_cast<void*>(&data[currPoint]));
            for (size_t i = 0; i < sizeof(float); i++){
                msg.buf[i] = temp[i];
            }
        }

        return msg;
    }

    void ThrottleStatus::readFromMsg(const CAN_message_t& msg){
        data[0] = msg.buf[0] << 8 | msg.buf[1];
        data[1] = float(msg.buf[2] << 8 | msg.buf[3])/10;
        data[2] = float(reinterpret_cast<int>(msg.buf[4] << 8 | msg.buf[5]))/10;
        data[3] = float(msg.buf[6] << 8 | msg.buf[7])/10;
    }

    String ThrottleStatus::getInfo(){
        return "Engine_Speed(RPM),Inlet_Manifold_Pressure(kPa),Inlet_Air_Temp(C),Throttle_Pos(%)";
    }

    void EngineRuntimeStats::query(){
        return;
    }

    void EngineRuntimeStats::query(uint8_t pin){
        return;
    }

    //TODO: Write this function
    CAN_message_t EngineRuntimeStats::writeToMsg(){
        return CAN_message_t();
    }

    void EngineRuntimeStats::readFromMsg(const CAN_message_t& msg){
        data[0] = msg.buf[0];
        data[1] = msg.buf[1];
        data[2] = msg.buf[2];
        data[3] = msg.buf[3];
        data[4] = msg.buf[4];
        data[5] = (msg.buf[5] & 0b1100) >> 2;
        data[6] = msg.buf[5] & 0b0011;
        data[7] = float(msg.buf[6] << 8 | msg.buf[7])/68.95;

    }

    String EngineRuntimeStats::getInfo(){
        return "Ignition_Output_Cut_Count,Fuel_Output_Count,Ignition_Output_Cut_Average(%),Fuel_Output_Cut_Average(%),Cylinder1_Primary_Output_Pulse_Width(mS),Ignition_Cut_State,Ignition_Timing_State,Engine_Oil_Pressure(PSI)";
    }

    void BrakeStatus::query(){
        return;
    }

    void BrakeStatus::query(uint8_t pin){
        return;
    }

    //TODO:Write Function Implementation;
    CAN_message_t BrakeStatus::writeToMsg(){
        return CAN_message_t();
    }

    void BrakeStatus::readFromMsg(const CAN_message_t& msg){
        data[0] = float(reinterpret_cast<int>(msg.buf[0] << 8 | msg.buf[1]))/100;
        data[1] = float(reinterpret_cast<int>(msg.buf[2] << 8 | msg.buf[3]))/100;
        data[2] = float(reinterpret_cast<int>(msg.buf[4] << 8 | msg.buf[5]))/10;
        data[3] = float(reinterpret_cast<int>(msg.buf[6] << 8 | msg.buf[7]))/100;
    }

    String BrakeStatus::getInfo(){
        return "Brake_Pressure_Front(bar),Brake_Pressure_Rear(bar),Coolant_Pressure(kPa),Power_Steering(bar)";
    }

    void EngineTemp::query(){
        return;
    }

    void EngineTemp::query(uint8_t pin){
        return;
    }

    CAN_message_t EngineTemp::writeToMsg(){
        return CAN_message_t();
    }

    void EngineTemp::readFromMsg(const CAN_message_t& msg){
        data[0] = int(msg.buf[0]) - 40;
        data[1] = int(msg.buf[1]) - 40;
        data[2] = int(msg.buf[2]) - 40;
        data[3] = int(msg.buf[3]) - 40;
        data[4] = int(msg.buf[4]) - 40;
        data[5] = float(msg.buf[5])/10;
        data[6] = float(msg.buf[6] << 8 | msg.buf[7])/100;
    }

    String EngineTemp::getInfo(){
        return "Coolant_Temperature(C),Engine_Oil_Temperature(C),Fuel Temperature(C),Ambient Temperature(C),Airbox_Temperature(C),ECU_Battery_Voltage(V),Fuel_Used(L)";
    }
}