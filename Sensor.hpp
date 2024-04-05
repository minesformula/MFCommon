#include <Arduino.h>
#include "thirdParty/FlexCAN_T4/FlexCAN_T4.h"

#pragma once

#define DATA_SIZE 8

/*
//Template class for easy copy/pasting

class TEMPLATE : public Sensor {
    public:
    TEMPLATE() : Sensor(ABBR_SENSOR_DEF, 0) {};
    TEMPLATE(uint8_t num) : Sensor(ABBR_SENSOR_DEF, num) {};
    ~TEMPLATE(){};

    void query();

    CAN_message_t writeToMsg();

    void readFromMsg(const CAN_message_t& msg);

    static String getInfo();

    static Sensor* create(uint8_t num){return new TEMPLATE(num);}
    static identifier getIdentity()
        {return { .abbr = ABBR_SENSOR_DEF, .constructor = create, .info = getInfo()};}
};

void TEMPLATE::query(){
    //INSTRUCTIONS TO QUERY SENSOR
}

void TEMPLATE::query(){
    //INSTRUCTIONS TO QUERY SENSOR
}

void TEMPLATE::query(uint8_t pin){
    //OPTIONAL INSTRUCTIONS TO QUERY SENSOR FROM PIN
}

CAN_message_t TEMPLATE::writeToMsg(){
    //PARSE DATA To CAN_message_t
}

void TEMPLATE::readFromMsg(const CAN_message_t& msg){
    //PARSE CAN_message_t INTO HUMAN READABLE DATA
}

String TEMPLATE::getInfo(){
    //RETURNS A STRING FOR USE IN FILES
}
*/
struct SensorData {
    String abbr;
    int sensorNum;
    float* data;
};

class Sensor{
    public:
    explicit Sensor(String abbr, uint8_t num);
    virtual ~Sensor(){};

    virtual void query() = 0;
    virtual void query(uint8_t pin){data[0]=analogRead(pin);}

    float getData();
    float getData(uint8_t position);
    SensorData getDataPackage(){ return {.abbr=abbr, .sensorNum=num, .data=data}; };

    virtual CAN_message_t writeToMsg() = 0;
    virtual void readFromMsg(const CAN_message_t& msg) = 0;

    String getAbbreviation(){return abbr;}

    uint8_t getNumber(){return num;}
    void setNumber(uint8_t newNumber){this->num = newNumber;}

    protected:
    String abbr;
    uint8_t num;

    float data[DATA_SIZE];
    String dataNames[DATA_SIZE];
};

Sensor::Sensor(String abbr, uint8_t number){
    this->abbr = abbr;
    this->num = number;
    
    for (int i = 0; i < DATA_SIZE; i++){
        this->data[i] = 0;
    }
}


float Sensor::getData(){
    return data[0];
}

float Sensor::getData(uint8_t position){
    if (position > DATA_SIZE){
        Serial.print("Overrun, failed to getData");
        return -1;
    }

    return data[position];
}

typedef Sensor* (*creator)(uint8_t num);

//Identifier class to allow easy identification of Sensors. 
//Includes their abbreviation, constructor and comma delimited position names as "info"
struct identifier {
    String abbr;
    creator constructor;
    String info;
};

//Unifies a sensor and it's respective canID
struct SensorCAN {
    Sensor* sensor;
    u_int32_t CANID;
};
