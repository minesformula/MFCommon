#include <Arduino.h>
#include <FlexCAN_T4.h>

#pragma once

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

    String getInfo();

    static Sensor* create(uint8_t num){return new TEMPLATE(num);}
    static identifier getIdentity(){return { .abbr = ABBR_SENSOR_DEF, .constructor = create};}
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
    SensorData getDataPackage(){ return {.abbr=abbr, .data=data}; };

    virtual CAN_message_t writeToMsg() = 0;
    virtual void readFromMsg(const CAN_message_t& msg) = 0;

    String getAbbreviation(){return abbr;}

    uint8_t getNumber(){return num;}
    void setNumber(uint8_t newNumber){this->num = newNumber;}

    virtual String getInfo() = 0;

    protected:
    String abbr;
    uint8_t num;

    float data[8];
    String dataNames[8];
};

Sensor::Sensor(String abbr, uint8_t number){
    this->abbr = abbr;
    this->num = number;
}


float Sensor::getData(){
    return data[0];
}

float Sensor::getData(uint8_t position){
    return data[position];
}

typedef Sensor* (*creator)(uint8_t num);

struct identifier {
    String abbr;
    creator constructor;
};

struct SensorCAN {
    Sensor* sensor;
    u_int32_t CANID;
};
