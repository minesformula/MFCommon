#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Sensor.hpp>
#include <SensorFactory.hpp>

#include <SD.h>
#include <SPI.h>

#include <TimeLib.h>

#pragma once

namespace MF {
    typedef void (*_callback)(const CAN_message_t &msg);
    template <CAN_DEV_TABLE T>
    class DAQLine {
        public:
        DAQLine();
        DAQLine(int baudrate);
        ~DAQLine();

        static void begin();

        static bool addSensor(uint32_t ID, const String &abbr, uint8_t num);

        static void writeMsg(const CAN_message_t &msg);
        static void update();

        static void SDLoggingMode();
        static void SDLoggingMode(bool set);

        private:
        static void processFrame(const CAN_message_t &msg);

        static void writeBytes(const String &filename, int size, uint8_t* buffer);

        static FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> _DAQLine;
        static String _CANNum;

        static int _baudrate;

        static String knownDataFile;
        static String unknownDataFile;

        static bool _SDMode;

        static SensorCAN _sensor[200];
        static uint8_t _sensorNum;
    };

    template<CAN_DEV_TABLE T>
    FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> DAQLine<T>::_DAQLine;

    template<CAN_DEV_TABLE T>
    String DAQLine<T>::_CANNum;

    template<CAN_DEV_TABLE T>
    int DAQLine<T>::_baudrate;

    template<CAN_DEV_TABLE T>
    String DAQLine<T>::knownDataFile;

    template<CAN_DEV_TABLE T>
    String DAQLine<T>::unknownDataFile;

    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::_SDMode;

    template<CAN_DEV_TABLE T>
    SensorCAN DAQLine<T>::_sensor[200];

    template<CAN_DEV_TABLE T>
    uint8_t DAQLine<T>::_sensorNum;

    template<CAN_DEV_TABLE T>
    DAQLine<T>::DAQLine(){
        DAQLine<T>::_baudrate = 1000000;
        _sensorNum = 0;

        MF::SensorFactory::load();

        if (T == CAN1){
            _CANNum = "CAN1";
        } else if (T == CAN2){
            _CANNum = "CAN2";
        } else if (T == CAN3){
            _CANNum = "CAN3";
        } 
    }

    template<CAN_DEV_TABLE T>
    DAQLine<T>::DAQLine(int baudrate){
        DAQLine<T>::_baudrate = baudrate;
        _sensorNum = 0;

        MF::SensorFactory::load();

        if (T == CAN1){
            _CANNum = "CAN1";
        } else if (T == CAN2){
            _CANNum = "CAN2";
        } else if (T == CAN3){
            _CANNum = "CAN3";
        } 
    }

    template<CAN_DEV_TABLE T>
    DAQLine<T>::~DAQLine(){
        for (int i = 0; i < _sensorNum; i++){
            delete _sensor[i].sensor;
        }
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


    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::addSensor(uint32_t ID, const String &abbr, uint8_t num){
        if(_sensorNum == 200){
            Serial.println("Max Sensors Reached");
            return false;
        }

        _sensor[_sensorNum]={.sensor=MF::SensorFactory::createFromAbbr(abbr, num), .CANID=ID};
        _sensorNum++;

        return true;
    }

    template<CAN_DEV_TABLE T>
    void DAQLine<T>::writeMsg(const CAN_message_t &msg){
        _DAQLine.write(msg);
    }

    template<CAN_DEV_TABLE T>
    void DAQLine<T>::update(){
        _DAQLine.events();
    }

    //TODO: Add datetime filenaming when SDLoggingMode is enabled
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::SDLoggingMode(){
        String temp = String(year()) + "_" + String(month()) + 
            "_" + String(day()) + "/" + String(hour()) + ":" + String(minute()) + ":" + String(second());


        SD.mkdir(temp.c_str());
        knownDataFile = temp + "/SensorInfo.dat";
        unknownDataFile = temp + "/" + _CANNum + "Msgs.dat";

        _SDMode = true;
    }

    template<CAN_DEV_TABLE T>
    void DAQLine<T>::SDLoggingMode(bool set){
        if (set){String temp = String(year()) + "_" + String(month()) + 
            "_" + String(day()) + "/" + String(hour()) + ":" + String(minute()) + ":" + String(second());

        SD.mkdir(temp.c_str());
        knownDataFile = temp + "/SensorInfo.dat";
        unknownDataFile = temp + "/" + _CANNum + "Msgs.dat";
        }

        _SDMode = set;
    }

    template<CAN_DEV_TABLE T>
    void DAQLine<T>::processFrame(const CAN_message_t &msg){
        if (_SDMode){
            for (uint16_t i = 0; i < 200; i++){
                if (_sensor[i].CANID == msg.id){
                    _sensor[i].sensor->readFromMsg(msg);
                    SensorData temp = _sensor[i].sensor->getDataPackage();
                    uint8_t* data = reinterpret_cast<uint8_t*>(reinterpret_cast<void*>(temp.data));
                    const char* c_str = temp.abbr.c_str();

                    writeBytes(knownDataFile, temp.abbr.length(), (uint8_t*) &c_str);
                    writeBytes(knownDataFile, sizeof(float)*8, data);
                    return;
                }
            }

            uint8_t* id = reinterpret_cast<uint8_t*>(reinterpret_cast<void*>(msg.id));
            writeBytes(unknownDataFile, sizeof(msg.id), (uint8_t*) &id);
            writeBytes(unknownDataFile, 8, (uint8_t*) &msg.buf);

        } else {
            for (uint16_t i = 0; i < 200; i++){
                if (_sensor[i].CANID == msg.id){
                    _sensor[i].sensor->readFromMsg(msg);
                    return;
                }
            }
        }
    }

    template<CAN_DEV_TABLE T>
    void DAQLine<T>::writeBytes(const String &filename, int size, uint8_t* buffer){
        File temp = SD.open(filename.c_str(), FILE_WRITE);
        
        for (int i = 0; i < size; i++){
            temp.write(buffer[i]);
        }

        temp.close();
    }
}
