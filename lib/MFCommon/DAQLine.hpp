#include <Arduino.h>
#include <FlexCAN_T4.h>

#pragma once

typedef void (*_callback)(const CAN_message_t &msg);
template <CAN_DEV_TABLE T>
class DAQLine {
    public:
    DAQLine(); //All RX Mailboxes. No writing capability
    DAQLine(int baudrate);
    DAQLine(int baudrate, int TXBoxes);

    static void begin();

    static void addCallback(int ID, _callback function);
    static void addCallback(int ID, _callback function, bool extended);

    static void write(CAN_message_t msg);
    static void update();

    private:
    static FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> _DAQLine;

    static int _activeMB;
    static int _baudrate;
    static int _TXBoxes;
    
};

template<CAN_DEV_TABLE T>
FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> DAQLine<T>::_DAQLine;

template<CAN_DEV_TABLE T>
int DAQLine<T>::_baudrate;

template<CAN_DEV_TABLE T>
int DAQLine<T>::_TXBoxes;

template<CAN_DEV_TABLE T>
int DAQLine<T>::_activeMB = 0;

template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(){
    _baudrate = 1000000;
    _TXBoxes = 0;
}


template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(int baudrate){
    _baudrate = baudrate;
    _TXBoxes = 0;
}

template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(int baudrate, int TXBoxes){
    _baudrate = baudrate;
    _TXBoxes = TXBoxes;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::begin(){
    _DAQLine.begin();
    _DAQLine.setBaudRate(1000000);
    _DAQLine.setMaxMB(64);

    int i = 0;
    for(;i < _TXBoxes; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,TX,EXT);
    }
    for(;i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,RX,EXT);
    }
    _DAQLine.setMBFilter(REJECT_ALL);
    _DAQLine.enableMBInterrupts();

    _activeMB = _TXBoxes;
}

//Adds a mailbox for a specified callback. Intended for use with Sensor msg handling
template<CAN_DEV_TABLE T>
void DAQLine<T>::addCallback(int ID, _callback function){
    if (_activeMB >= 64){
        Serial.println("\e[1;31mError: Maximum mailboxes reached\e[1;37m");
        return;
    }

    _DAQLine.onReceive((FLEXCAN_MAILBOX)_activeMB, function);
    _DAQLine.setMBFilter((FLEXCAN_MAILBOX)_activeMB, ID);
    _activeMB++;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::addCallback(int ID, _callback function, bool extended){
    if (_activeMB >= 64){
        Serial.println("\e[1;31mError: Maximum mailboxes reached\e[1;37m");
        return;
    }

    if (extended){
        _DAQLine.setMB((FLEXCAN_MAILBOX)_activeMB, RX, EXT);
    } else {
        _DAQLine.setMB((FLEXCAN_MAILBOX)_activeMB, RX, STD);
    }

    _DAQLine.onReceive((FLEXCAN_MAILBOX)_activeMB, function);
    _DAQLine.setMBFilter((FLEXCAN_MAILBOX)_activeMB, ID);
    _activeMB++;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::write(CAN_message_t msg){
    _DAQLine.write(msg);
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::update(){
    _DAQLine.events();
}