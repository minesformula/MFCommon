#ifndef MFCAN_H
#define MFCAN_H

#include <FlexCAN_T4.h>

template <CAN_DEV_TABLE T>
class DAQLine {
    public:
    DAQLine(); //All RX Mailboxes. No writing capability
    DAQLine(int baudrate);

    DAQLine(int TXBoxes);
    DAQLine(int TXBoxes, int baudrate);

    typedef (*_callback)(const CAN_message_t &msg);
    void addCallback(int ID, _callback function);

    void write(CAN_message_t msg);
    void update();

    private:
    static FlexCAN_T4<T, RX_SIZE_256, TX_SIZE_16> _DAQLine;

    int _activeMB;
    
};

#endif