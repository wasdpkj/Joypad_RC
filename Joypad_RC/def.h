#include "Arduino.h"
#include "time.h"
#include "bat.h"

//DEBUG-----------
//#define Serial_DEBUG

#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define LARGE_FLASH
#endif
#if defined(__AVR_ATmega128RFA1__)
#define RF_PORT
#endif

//mode------------
#if defined(LARGE_FLASH)
boolean mode_mpu = false;
#endif
boolean mode_protocol = false; //{(mode_protocol: 0 is mwc, 1 is nrf),(mode_mpu: 0 is no mpu, 1 is mpu)}

//RSSI------------
int8_t RSSI = -40;

//MWC-------------
#if defined(RF_PORT)
uint8_t mwc_channal = 11; //RF channel
#else
uint8_t mwc_channal = 4; //RF channel
#endif

#if  defined(__AVR_ATmega32U4__)
#define mwc_port Serial1    //Serial1 is D0 D1
#elif defined(RF_PORT)
#define mwc_port ZigduinoRadio    //RF
#else
#define mwc_port Serial    //Serial is D0 D1
#endif

//nRF-------------
#define interval_debug  100
uint8_t nrf_channal = 70;  //0~125

#if defined(LARGE_FLASH)
//MPU-------------
#define MPU_maximum 70
#endif

//Time------------
#define interval_TIME1 1000    //setup delay
#define interval_time2 40      //send interval
#define interval_time3 1000    //battery interval
#define interval_time4 50      //tft interval

