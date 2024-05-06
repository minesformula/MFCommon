#include <Arduino.h>

union {
    int8_t integer;
    uint8_t bytes[1];
}converter8Bit;

union {
    int16_t integer;
    uint8_t bytes[2];
}converter16Bit;

union {
    int integer;
    float floatingPoint;
    uint8_t bytes[4];
    }converter32Bit;