#include "Arduino.h"
#include "com_nrf.h"
#include "com_mwc.h"

//debug--------------------------
void comBegin() {
  if (mode_protocol) {  //Robot
    SPI.begin();
    radio.begin();
    network.begin(/*channel*/ nrf_channal, /*node address*/ this_node);
  }
  else {  //QuadCopter
    unsigned long _channel;
#if !defined(RF_PORT)
    switch (mwc_channal) {
      case 0:
        _channel = 9600;
        break;
      case 1:
        _channel = 19200;
        break;
      case 2:
        _channel = 38400;
        break;
      case 3:
        _channel = 57600;
        break;
      case 4:
        _channel = 115200;
        break;
    }
#else
    _channel = mwc_channal;
#endif
    mwc_port.begin(_channel);
  }
}

unsigned long node_clock = 0;
boolean nodeClock() {
  node_clock++;
  return (node_clock > 20) ? false : true;
}

boolean comSend(int16_t _channal[8]) {
  if (mode_protocol) {
    if (nrf_send(_channal))   //Robot
      return true;
  }
  else {
    if (mwc_send(_channal))  //QuadCopter
      return true;
  }
  return false;
}

boolean comRece() {
  uint32_t _c;
  if (mode_protocol) {
    delay(25);
    _c = nrf_rece();
  }
  else {
    _c = mwc_rece();
  }
  if (_c) {
    node_clock = 0;
    return true;
  }
  else
    return false;
}
