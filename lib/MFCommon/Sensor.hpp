#include <Arduino.h>
#include <FlexCAN_T4.h>

#pragma once
class Sensor{
    public:
    explicit Sensor(String abbr, uint8_t num);

    virtual void query() = 0;
    float getData();
    float getData(uint8_t position);
    virtual CAN_message_t writeToMsg() = 0;
    virtual void readFromMsg(const CAN_message_t& msg) = 0;

    String getAbbreviation(){return abbr;}

    uint8_t getNumber(){return num;}
    void setNumber(uint8_t newNumber){this->num = newNumber;}

    protected:
    String abbr;
    uint8_t num;
    float data[8];
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