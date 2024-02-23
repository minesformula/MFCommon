#include "DAQLine.h"


template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(){
    _DAQLine.begin();
    _DAQLine.setBaudRate(1000000);
    _DAQLine.setMaxMB(64);
    _DAQLine.enableMBInterrupts();
    _DAQLine.setMB(MB0,RX);
    for(int i = 1; i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,RX);
    }
    _DAQLine.setMBFilter(REJECT_ALL);
    _DAQLine.distribute();
    _DAQLine.mailboxStatus();

    _activeMB = 0;
}


template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(int baudrate){
    _DAQLine.begin();
    _DAQLine.setBaudRate(baudrate);
    _DAQLine.setMaxMB(64);
    _DAQLine.enableMBInterrupts();
    for(int i = 0; i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,RX);
    }
    _DAQLine.setMBFilter(REJECT_ALL);
    _DAQLine.distribute();
    _DAQLine.mailboxStatus();

    _activeMB = 0;
}

template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(int TXBoxes){
    _DAQLine.begin();
    _DAQLine.setBaudRate(1000000);
    _DAQLine.setMaxMB(64);
    _DAQLine.enableMBInterrupts();

    int i = 0;
    for (i; i < TXBoxes && i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,TX);
    }
    for(i; i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,RX);
    }

    _DAQLine.setMBFilter(REJECT_ALL);
    _DAQLine.distribute();
    _DAQLine.mailboxStatus();

    _activeMB = TXBoxes;
}

template<CAN_DEV_TABLE T>
DAQLine<T>::DAQLine(int TXBoxes, int baudrate){
    _DAQLine.begin();
    _DAQLine.setBaudRate(1000000);
    _DAQLine.setMaxMB(64);
    _DAQLine.enableMBInterrupts();

    int i = 0;
    for (i; i < TXBoxes && i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,TX);
    }
    for(i; i < 64; i++){
        _DAQLine.setMB((FLEXCAN_MAILBOX)i,RX);
        _DAQLine.setMBFilter((FLEXCAN_MAILBOX)i, REJECT_ALL);
    }

    _DAQLine.distribute();
    _DAQLine.mailboxStatus();

    _activeMB = TXBoxes;
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::addCallback(int ID, _callback function){
    _DAQLine.setMBFilter((FLEXCAN_MAILBOX)_activeMB, ID);
    _DAQLine.onReceive((FLEXCAN_MAILBOX)_activeMB, function);
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::write(CAN_message_t msg){
    _DAQLine.write(msg)
}

template<CAN_DEV_TABLE T>
void DAQLine<T>::update(){
    _DAQLine.events();
}