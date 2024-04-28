#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#include "thirdParty/FlexCAN_T4/FlexCAN_T4.h"
#include "MFCommon.h"
#include "Sensor.hpp"
#include "SensorFactory.hpp"

#include <TimeLib.h>

#pragma once

namespace MF {
    typedef void (*_callback)(const CAN_message_t &msg);

    union {
    int integer;
    float floatingPoint;
    uint8_t bytes[4];
    }converter;

    template <CAN_DEV_TABLE T>
    class DAQLine {
        public:
        DAQLine();
        DAQLine(int baudrate);
        ~DAQLine();

        static void begin();

        static bool addSensor(uint32_t ID, const String &abbr, uint8_t num);
        static bool addSensor(Sensor* sensor, uint32_t ID);

        static void writeMsg(const CAN_message_t &msg);
        static void update();

        static void SDLoggingMode();
        static void SDLoggingMode(bool set);

        static void flushSD();

        static void enableLiveTelemetry(HardwareSerial &radio);

        static void enableDynamicSensors();
        static void enableDynamicSensors(uint32_t receiverID);

        static void sendReadOut(HardwareSerial &serial);
        static void sendReadOut(HardwareSerial *serial, Sensor* sensor);

        private:
        static void processFrame(const CAN_message_t &msg);

        static void writeBytes(File &filename, int size, uint8_t* buffer);

        static FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> _DAQLine;
        static String _CANNum;

        static int _baudrate;

        static char knownFilename[23];
        static char unknownFilename[23];

        static File knownDataFile;
        static File unknownDataFile;

        static HardwareSerial* _radio;

        static bool _SDMode;
        static bool _liveMode;

        static bool _dynamicMode;
        static uint32_t _initializerID;

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
    char DAQLine<T>::knownFilename[23];

    template<CAN_DEV_TABLE T>
    char DAQLine<T>::unknownFilename[23];

    template<CAN_DEV_TABLE T>
    File DAQLine<T>::knownDataFile;

    template<CAN_DEV_TABLE T>
    File DAQLine<T>::unknownDataFile;

    template<CAN_DEV_TABLE T>
    HardwareSerial* DAQLine<T>::_radio;

    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::_SDMode = false;

    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::_liveMode = false;

    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::_dynamicMode = false;

    template<CAN_DEV_TABLE T>
    uint32_t DAQLine<T>::_initializerID;

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

        if (T == CAN1){
            _CANNum = "CAN1";
        } else if (T == CAN2){
            _CANNum = "CAN2";
        } else if (T == CAN3){
            _CANNum = "CAN3";
        } 
    }

    /// @brief Creates a sensor at a given CANID
    /// @tparam T CANLine
    /// @param ID CANID for sensor creation
    /// @param abbr Corresponding Sensor Abbreviation
    /// @param num Sensor Number for identification
    /// @return Boolean indicating success or failure
    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::addSensor(uint32_t ID, const String &abbr, uint8_t num){
        return addSensor(MF::SensorFactory::createFromAbbr(abbr, num), ID);
    }

    /// @brief Creates a sensor at a given CANID
    /// @tparam T CANLine
    /// @param Sensor Pointer to a sensor instance
    /// @param ID CANID for sensor creation
    /// @return Boolean indicating success or failure
    template<CAN_DEV_TABLE T>
    bool DAQLine<T>::addSensor(Sensor* Sensor, uint32_t ID){
        if(_sensorNum == 200){
            Serial.println("Max Sensors Reached");
            return false;
        }

        Serial.print(_CANNum);
        Serial.print(": Creating Sensor at: ");
        Serial.println(ID);
        _sensor[_sensorNum]={.sensor=Sensor, .CANID=ID};
        _sensorNum++;

        return true;
    }

    /// @brief Send a message over associated CAN line
    /// @tparam T CANLine
    /// @param msg Flexcan_T4 msg to be shipped over CAN
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::writeMsg(const CAN_message_t &msg){
        _DAQLine.write(msg);
    }

    /// @brief Check CAN line for new messages
    /// @tparam T CANLine
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::update(){
        _DAQLine.events();
        //Serial2.println("Updating");
    }

    /// @brief Enable DAQ style SDCard logging. By default creates files with the current date and time.
    /// @tparam T CANLine
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::SDLoggingMode(){
        _SDMode = true;
    }

    /// @brief Toggle DAQ style SDCard logging. By default creates files with the current date and time.
    /// @tparam T CANLine
    /// @param set Boolean for enabling/disabling SDLogging Mode
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::SDLoggingMode(bool set){
        if (set){

            if (!SD.exists(VERSION)){
                File temp = SD.open(VERSION, FILE_WRITE);
                temp.println("Ver" + String(VERSION) + " Config Data:");
                temp.println(SensorFactory::getReadOut());
                temp.close();
            }

            int i = 0;
            sprintf(knownFilename, "%slogFile0.data", _CANNum.c_str());
            sprintf(unknownFilename, "%sFile0.data", _CANNum.c_str());

            while (SD.exists(knownFilename)){
                i++;

                sprintf(knownFilename, "%slogFile%d.data", _CANNum.c_str(), i);
                sprintf(unknownFilename, "%sFile%d.data", _CANNum.c_str(), i);
            }
        

            knownDataFile = SD.open(knownFilename, FILE_WRITE);
            unknownDataFile = SD.open(unknownFilename, FILE_WRITE);

            knownDataFile.println("Ver:" + String(VERSION) + " Known Data:");
            unknownDataFile.println("CAN: " + _CANNum + " Ver: " + String(VERSION) + " Unknown Data:");

            knownDataFile.close();
            unknownDataFile.close();

            knownDataFile = SD.open(knownFilename, FILE_WRITE);
            unknownDataFile = SD.open(unknownFilename, FILE_WRITE);

            Serial.print(_CANNum);
            Serial.println(": Starting SDLogging");
        }

        _SDMode = set;
    }

    template<CAN_DEV_TABLE T>
    void DAQLine<T>::flushSD(){
        knownDataFile.flush();
        unknownDataFile.flush();
    }

    /// @brief Enables live telemetry transmissions
    /// @tparam T CANLine
    /// @param radio Serial pins to write transmissions to
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::enableLiveTelemetry(HardwareSerial &radio){
        _radio = &radio;
        _liveMode = true;
    }

    /// @brief Enables dynamic sensor allocation allowing sensors to be created from CAN ID 0 messages
    /// @tparam T CANLine
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::enableDynamicSensors(){
        _initializerID = 0;
        _dynamicMode = true;
    }

    /// @brief Enables dynamic sensor allocation allowing sensors to be created messages with a certain CAN ID
    /// @tparam T CANLine
    /// @param receiverID CAN ID to use for dynamic sensor allocation
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::enableDynamicSensors(uint32_t receiverID){
        _initializerID = receiverID;
        _dynamicMode = true;
    }

    /// @brief Prints every datapoint currently available to serial with comma delimiters
    /// @tparam T CANLine
    /// @param serial Serial to transmit data through
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::sendReadOut(HardwareSerial &serial){
        for(uint16_t i = 0; i < _sensorNum; i++){
            SensorData temp = _sensor[i].sensor->getDataPackage();

            //Sends messages as '1' when sending data.
            //Live telemetry mode sends a 0 when transmitting sensor info allowing late connection
            serial.print("1,");
            serial.print(temp.abbr);
            serial.print(",");
            serial.print(temp.sensorNum);
            serial.print(",");
            for (uint8_t j = 0; j < DATA_SIZE; j++){
            serial.print(temp.data[j], 5);
            if (j == DATA_SIZE-1){
                serial.println();
            } else {
                serial.print(",");
            }
        }
            serial.flush();
        }
    }

    /// @brief Sends data from a specified sensor through a specified serial line
    /// @tparam T CANLine
    /// @param serial Serial to transmit data through
    /// @param sensor Pointer to sensor to transmit data from
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::sendReadOut(HardwareSerial *serial, Sensor* sensor){
        SensorData temp = sensor->getDataPackage();

        //Sends messages as '1' when sending data.
        //Live telemetry mode sends a 0 when transmitting sensor info allowing late connection
        serial->print("1,");
        serial->print(temp.abbr);
        serial->print(",");
        serial->print(temp.sensorNum);
        serial->print(",");
        for (uint8_t j = 0; j < DATA_SIZE; j++){
            serial->print(temp.data[j], 5);
            if (j == DATA_SIZE-1){
                serial->println();
            } else {
                serial->print(",");
            }
        }
        serial->flush();
    }

    /// @brief CAN callback for processing received messages
    /// @tparam T CANLine
    /// @param msg Message to process
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::processFrame(const CAN_message_t &msg){
        Serial.print(_CANNum);
        Serial.print(": Received: ");
        Serial.println(msg.id);
        //Serial2.println(msg.id);

        if (_SDMode){
            //Serial2.println("Write Unknown");
            unknownDataFile.print(millis());
            unknownDataFile.print(",");
            unknownDataFile.print(msg.id);
            unknownDataFile.print(",");

            for(size_t dataPos = 0; dataPos < 8; dataPos++){
                unknownDataFile.print(msg.buf[dataPos]);

                if(dataPos != 7){
                    unknownDataFile.print(",");
                }
            }

            unknownDataFile.println();
        }

        if (_dynamicMode && _initializerID == msg.id){
            addSensor(SensorFactory::createFromMsg(msg), msg.buf[4]);

        } else {

            for (uint16_t i = 0; i < _sensorNum; i++){
                if (_sensor[i].CANID == msg.id){
                    _sensor[i].sensor->readFromMsg(msg);

                    if (_SDMode){
                        //Serial2.println("Write Known");

                        SensorData temp = _sensor[i].sensor->getDataPackage();
                        
                        knownDataFile.print(millis());
                        knownDataFile.print(",");
                        knownDataFile.print(temp.abbr);
                        knownDataFile.print(",");
                        knownDataFile.print(temp.sensorNum);
                        knownDataFile.print(",");
    
                        for (size_t dataPos = 0; dataPos < DATA_SIZE; dataPos++){
                            knownDataFile.print(temp.data[dataPos]);

                            if (dataPos != DATA_SIZE-1){
                                knownDataFile.print(",");
                            }
                        }

                        knownDataFile.println();
                    }

                    if (_liveMode){
                        sendReadOut(_radio, _sensor[i].sensor);
                    }

                    return;
                }
            }
        }
    }

    /// @brief Write given bytes buffer to a given file
    /// @tparam T CANLine
    /// @param file SDCard File to be written to
    /// @param size Buffer size
    /// @param buffer Buffer containing bytes to write to the file
    template<CAN_DEV_TABLE T>
    void DAQLine<T>::writeBytes(File &file, int size, uint8_t* buffer){

        for (int i = 0; i < size; i++){
            file.write(buffer[i]);
        }

        file.println();
    }
}
