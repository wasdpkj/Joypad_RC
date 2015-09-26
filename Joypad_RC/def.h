#include "Arduino.h"
//MCU-------------

//DEBUG-----------
#define Serial_DEBUG

//TFT-------------
#define TFT_ROTATION
#define THEME 0    //0 is white,1 is black

#if THEME == 0
#define TFT_BUT ST7735_WHITE
#define TFT_TOP ST7735_BLACK
#endif

#if THEME == 1
#define TFT_BUT ST7735_BLACK
#define TFT_TOP ST7735_WHITE
#endif

//MWC-------------
#if defined(__AVR_ATmega128RFA1__)
#define mwc_port_speed 11  //RF channel
#else
#define mwc_port_speed 115200  //Serial baud
#endif

#if  defined(__AVR_ATmega32U4__)
#define mwc_port Serial1    //Serial1 is D0 D1
#elif defined(__AVR_ATmega128RFA1__)
#define mwc_port ZigduinoRadio    //RF
#else
#define mwc_port Serial    //Serial is D0 D1
#endif

//Key-------------
#define PIN_key {CH_SWITCH_1,CH_SWITCH_2,CH_SWITCH_3,CH_SWITCH_4}	//按键1 2 3 4

//Battery---------
#define PIN_bat A7	//BAT

#define _V_max 42		//锂电池满电电压4.2V
#define _V_min 37		//锂电池没电电压3.7V

#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
//MPU-------------
#define MPU_maximum 70
#endif

//nRF-------------
#define interval_debug  2000	//节点查错间隔
#define nRF_channal 70    //0~125

//Joy-------------
//1000~2000
#define joy_DEAD_ZONE 10
#define Joy_s_maximum 200 //MAX 500
#define Joy_maximum 450 //MAX 500
#define Joy_MID 1500  //1500

//Time------------
#define interval_TIME1 2000    //setup delay
#define interval_time2 40      //send interval
#define interval_time3 1000    //battery interval
