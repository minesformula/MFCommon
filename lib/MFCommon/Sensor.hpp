#include <Arduino.h>
#include <FlexCAN_T4.h>

class Sensor{
    public:
    explicit Sensor(String abbr, uint8_t num);

    virtual void Query() = 0;
    virtual double getData(uint8_t position) = 0;
    virtual CAN_message_t writeToMsg() = 0;
    virtual CAN_message_t writeToMsg(double value) = 0;

    private:
    String abbr;
    uint8_t num;
};

Sensor::Sensor(String abbr, uint8_t num){
    this->abbr = abbr;
    this->num = num;
}