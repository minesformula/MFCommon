#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Sensor.hpp>

#include <SD.h>
#include <SPI.h>

#include <TimeLib.h>

#pragma once

typedef void (*_callback)(const CAN_message_t &msg);
template <CAN_DEV_TABLE T>
class DAQLine {
    public:
    DAQLine();
    DAQLine(int baudrate);

    static void begin();

    static void addSensor(int ID, String abbr);

    static void write(CAN_message_t msg);
    static void update();

    static void SDLoggingMode();
    static void SDLoggingMode(bool set);

    private:
    static void processFrame(CAN_message_t &msg);

    void writeBytes(String filename, int size, uint8_t* buffer);

    static FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> _DAQLine;

    static int _baudrate;

    static String knownDataFile;
    static String unknownDataFile;

    static bool _SDMode;

    static SensorCAN _sensor[200];
};

template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(){
    DAQLine<T>::_baudrate = 1000000;
}

template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(int baudrate){
    DAQLine<T>::_baudrate = baudrate;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::begin(){
    _DAQLine.begin();
    _DAQLine.setBaudRate(DAQLine<T>::_baudrate);
    _DAQLine.setMaxMB(16);
    _DAQLine.enableFIFO();
    _DAQLine.enableFIFOInterrupt();
    _DAQLine.onReceive(processFrame);
}

//TODO: Add datetime filenaming when SDLogginMode is enabled
template<CAN_DEV_TABLE T>
void DAQLine<T>::SDLoggingMode(){
    _SDMode = true;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::SDLoggingMode(bool set){
    _SDMode = set;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::processFrame(CAN_message_t &msg){
    if (_SDMode){
        for (uint16_t i = 0; i < 200; i++){
            if (_sensor[i].CANID == msg.id){
                _sensor[i].sensor->readFromMsg(msg);
                SensorData temp = _sensor[i].sensor->getDataPackage();
                writeBytes(knownDataFile, temp.abbr.length(), temp.abbr.c_str());
                writeBytes(knownDataFile, sizeof(float)*8, temp.abbr.c_str());
                return;
            }
        }

        writeBytes(unknownDataFile, sizeof(msg.id), msg.id);
        writeBytes(unknownDataFile, 8, msg.buf);
    }
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::writeBytes(String filename, int size, uint8_t* buffer){
    File temp = SD.open(filename.c_str(), FILE_WRITE);
    
    for (int i = 0; i < size; i++){
        temp.write(buffer[i]);
    }

    temp.close();
}
