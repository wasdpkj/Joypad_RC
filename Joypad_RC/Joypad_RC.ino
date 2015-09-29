#include "Arduino.h"
#include "def.h"
#include "time.h"
#include "bat.h"
#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#include "mpu.h"
#endif
#include "joy.h"
#include "key.h"
#include "data.h"
#include "nrf.h"
#include "mwc.h"
#include "tft.h"
#include "eep.h"

#if defined(__AVR_ATmega128RFA1__)
#include <ZigduinoRadio.h>
#endif

//joypad================================
#include <Joypad.h>
//eeprom================================
#include <EEPROM.h>
//TFT===================================
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific 
#include <SPI.h>
//rf====================================
#include <RF24Network.h>
#include <RF24.h>

#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
//MPU===================================
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#endif

//spi===================================
#include <SPI.h>

void setup()
{
  // initialize serial communication at 115200 bits per second:

#ifdef Serial_DEBUG
  Serial.begin(115200);
  delay(100);
  Serial.println("========hello========");
#endif

  //---------------
  key_init();

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r EEPROM READ...");
#endif
  eeprom_read();

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT INIT...");
#endif
  TFT_init(true, tft_rotation);

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT BEGIN...");
#endif
  TIME1 = millis();
  while (millis() - TIME1 < interval_TIME1)
  {
    TFT_begin();

    if (!Joypad.readButton(CH_SWITCH_1))
    {
#ifdef Serial_DEBUG
      Serial.println("\n\rCorrect IN...");
#endif

      //---------------
#ifdef Serial_DEBUG
      Serial.println("\n\r TFT INIT...");
#endif
      TFT_init(false, tft_rotation);

      while (1)
      {
        if (!TFT_config())
          break;
      }
#ifdef Serial_DEBUG
      Serial.println("\n\rCorrect OUT...");
#endif

      //---------------
#ifdef Serial_DEBUG
      Serial.println("\n\r EEPROM WRITE...");
#endif
      eeprom_write();
    }
  }

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT CLEAR...");
#endif
  TFT_clear();

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT READY...");
#endif
  TFT_ready();

  //---------------.l
  if (mode_protocol)   //Robot
  {
    SPI.begin();		//初始化SPI总线
    radio.begin();
    network.begin(/*channel*/ nrf_channal, /*node address*/ this_node);
  }
  else          //QuadCopter
  {
    unsigned long _channel;
#if !defined(__AVR_ATmega128RFA1__)
    switch (mwc_channal)
    {
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
#else if
    _channel = mwc_channal;
#endif
    mwc_port.begin(_channel);
  }

  //---------------
#ifdef Serial_DEBUG
  Serial.println("===========start===========");
#endif

#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
  if (mode_mpu) initMPU(); //initialize device
#endif
}

void loop()
{
  //  unsigned long time = millis();

#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
  //MPU--------------------------------
  if (mode_mpu)
    getMPU();
#endif

  //DATA_begin------------------------------
  data_begin();

  //DATA_send-------------------------------
  if (millis() < time2) time2 = millis();
  if (millis() - time2 > interval_time2)
  {
    if (mode_protocol) nrf_send();    //Robot
    else data_send();           //QuadCopter

    time2 = millis();
  }

  //节点查错-------------------------------
  vodebug();

  //BAT--------------------------------
  if (time3 > millis()) time3 = millis();
  if (millis() - time3 > interval_time3)
  {
    vobat();
    time3 = millis();
  }

  //TFT------------------------------------
  TFT_run();

  //===================================
  //  time = millis() - time;

  //  Serial.println(time, DEC);    //loop time
}
