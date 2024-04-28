#include "Sensor.hpp"
#include "SensorDefinitions.h"

#pragma once

namespace Motion{
    union {
        int16_t integer;
        uint8_t bytes[2];
    }conv16_t;

    class Accelerometer : public Sensor {
        public:
        Accelerometer() : Sensor(ACCELEROMETER, 0) {};
        Accelerometer(uint8_t num) : Sensor(ACCELEROMETER, num) {};
        ~Accelerometer(){};

        void query();
        void query(uint8_t pin);

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new Accelerometer(num);}
        static identifier getIdentity()
            {return { .abbr = ACCELEROMETER, .constructor = create, .info = getInfo()};}
    };

    class Gyroscope : public Sensor {
        public:
        Gyroscope() : Sensor(GYROSCOPE, 0) {};
        Gyroscope(uint8_t num) : Sensor(GYROSCOPE, num) {};
        ~Gyroscope(){};

        void query();
        void query(uint8_t pin);

        CAN_message_t writeToMsg();

        void readFromMsg(const CAN_message_t& msg);

        static String getInfo();

        static Sensor* create(uint8_t num){return new Gyroscope(num);}
        static identifier getIdentity()
            {return { .abbr = GYROSCOPE, .constructor = create, .info = getInfo()};}
    };

    void Accelerometer::query(){
        //INSTRUCTIONS TO QUERY SENSOR
    }

    void Accelerometer::query(uint8_t pin){
        //OPTIONAL INSTRUCTIONS TO QUERY SENSOR FROM PIN
    }

    CAN_message_t Accelerometer::writeToMsg(){
        //PARSE DATA To CAN_message_t
    }

    void Accelerometer::readFromMsg(const CAN_message_t& msg){
        conv16_t.bytes[0] = msg.buf[0];
        conv16_t.bytes[1] = msg.buf[1];
        data[0] = float(conv16_t.integer)/100;

        conv16_t.bytes[0] = msg.buf[2];
        conv16_t.bytes[1] = msg.buf[3];
        data[1] = float(conv16_t.integer)/100;

        conv16_t.bytes[0] = msg.buf[4];
        conv16_t.bytes[1] = msg.buf[5];
        data[2] = float(conv16_t.integer/10);
    }

    String Accelerometer::getInfo(){
        return "X_Acceleration(m/s^2),Y_Acceleration(m/s^2),Z_Acceleration(m/s^2)";
    }

    void Gyroscope::query(){
        //INSTRUCTIONS TO QUERY SENSOR
    }

    void Gyroscope::query(uint8_t pin){
        //OPTIONAL INSTRUCTIONS TO QUERY SENSOR FROM PIN
    }

    CAN_message_t Gyroscope::writeToMsg(){
        //PARSE DATA To CAN_message_t
    }

    void Gyroscope::readFromMsg(const CAN_message_t& msg){
        conv16_t.bytes[0] = msg.buf[0];
        conv16_t.bytes[1] = msg.buf[1];
        data[0] = float(conv16_t.integer)/100;

        conv16_t.bytes[0] = msg.buf[2];
        conv16_t.bytes[1] = msg.buf[3];
        data[1] = float(conv16_t.integer)/100;

        conv16_t.bytes[0] = msg.buf[4];
        conv16_t.bytes[1] = msg.buf[5];
        data[2] = float(conv16_t.integer)/100;
    }

    String Gyroscope::getInfo(){
        return "X_Roll(deg/s),Y_Roll(deg/s),Z_Roll(deg/s)";
    }

}