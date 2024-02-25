#include <Arduino.h>
#include <DAQLine.hpp>
#include <ECUSensors.hpp>

void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> _DAQLine;

void setup() {
  Serial.begin(115200); delay(400);
  _DAQLine.begin();
  _DAQLine.setBaudRate(1000000);
  _DAQLine.setMaxMB(64);

  for (int i = 0; i < 64; i++){
    _DAQLine.setMB((FLEXCAN_MAILBOX)i, RX, EXT);
  }

  _DAQLine.setMBFilter(REJECT_ALL);
  _DAQLine.enableMBInterrupts();
  _DAQLine.onReceive((FLEXCAN_MAILBOX)1, canSniff);
  _DAQLine.setMBFilter((FLEXCAN_MAILBOX)1, 0x18A19D00);
}

void loop() {
  _DAQLine.events();
}