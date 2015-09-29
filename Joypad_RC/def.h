#include "Arduino.h"

//DEBUG-----------
#define Serial_DEBUG

//MWC-------------
uint8_t mwc_channal = 11; //RF channel

#if  defined(__AVR_ATmega32U4__)
#define mwc_port Serial1    //Serial1 is D0 D1
#elif defined(__AVR_ATmega128RFA1__)
#define mwc_port ZigduinoRadio    //RF
#else
#define mwc_port Serial    //Serial is D0 D1
#endif

//nRF-------------
#define interval_debug  2000  //节点查错间隔
uint8_t nrf_channal = 70;  //0~125

//Battery---------
#define PIN_bat A7	//BAT

#define _V_max 41		//锂电池满电电压4.2V
#define _V_min 36		//锂电池没电电压3.7V

#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
//MPU-------------
#define MPU_maximum 70
#endif


//Time------------
#define interval_TIME1 2000    //setup delay
#define interval_time2 40      //send interval
#define interval_time3 1000    //battery interval
