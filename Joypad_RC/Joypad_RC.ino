#include "Arduino.h"
#include "def.h"
#if defined(LARGE_FLASH)
#include "mpu.h"
#endif
#include "com.h"
#include "data.h"
#include "tft.h"
#include "eep.h"

//eeprom================================
#include <EEPROM.h>

#if defined(LARGE_FLASH)
//MPU===================================
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#endif

void setup() {
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
  if (!eeprom_read())
    eeprom_write(false);

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
  while (millis() - TIME1 < interval_TIME1) {
    TFT_begin();

    if (key_get(0, 1)) {
#ifdef Serial_DEBUG
      Serial.println("\n\rCorrect IN...");
#endif

      //---------------
#ifdef Serial_DEBUG
      Serial.println("\n\r TFT INIT...");
#endif
      TFT_init(false, tft_rotation);

      while (1) {
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
      eeprom_write(true);
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
  TFT_ready(false);

  //---------------
  comBegin();

#if defined(LARGE_FLASH)
  if (mode_mpu) initMPU(); //initialize device
#endif

#ifdef Serial_DEBUG
  Serial.println("===========start===========");
#endif
}

void loop() {
#if defined(LARGE_FLASH)
  if (mode_mpu) getMPU();
#endif

  //DATA_begin------------------------------
  buf_code = true;
  if (!data_begin(&outBuf[0], &outBuf[1], &outBuf[2], &outBuf[3], &outBuf[4], &outBuf[5], &outBuf[6], &outBuf[7])) {
    buf_code = false;
    TFT_clear();
    TFT_ready(true);
    time4 = millis() + 500;
  }

  //BAT--------------------------------
  if (time3 > millis()) time3 = millis();
  if (millis() - time3 > interval_time3) {
    _V_bat = batVoltage();
    time3 = millis();
  }

  //DATA_send-------------------------------
  if (millis() < time2) time2 = millis();
  if (millis() - time2 > interval_time2) {
    if (comSend(buf_code ? buf_code_A : buf_code_B, outBuf)) comRece();
    time2 = millis();
  }

  //TFT------------------------------------
  if (millis() < time4) time4 = millis();
  if (millis() - time4 > interval_time4) {
    TFT_run();
    time4 = millis();
  }
}
