#include "Arduino.h"

#include <EEPROM.h>

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

int16_t safe_check = 1111;

struct config_type
{
  int16_t eeprom_safe_check;
  int16_t eeprom_correct_min[4];
  int16_t eeprom_correct_max[4];
  uint8_t eeprom_Joy_deadzone_val;
#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
  boolean eeprom_mode_mpu;
#endif
  boolean eeprom_mode_protocol;
  uint8_t eeprom_mwc_channal;
  uint8_t eeprom_nrf_channal;
  boolean eeprom_tft_theme;
  boolean eeprom_tft_rotation;
  boolean eeprom_mcu_voltage;
};

//======================================
boolean eeprom_read()
{
  //EEPROM读取赋值
  config_type config_readback;
  EEPROM_read(0, config_readback);

  if (config_readback.eeprom_safe_check != 1111)
    return false;

  for (uint8_t a = 0; a < 4; a++)
  {
    joy_correct_min[a] = config_readback.eeprom_correct_min[a];
    joy_correct_max[a] = config_readback.eeprom_correct_max[a];
  }
  Joy_deadzone_val = config_readback.eeprom_Joy_deadzone_val;

  mode_protocol = config_readback.eeprom_mode_protocol;
#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
  mode_mpu = config_readback.eeprom_mode_mpu;
#endif

  mwc_channal = config_readback.eeprom_mwc_channal;
  nrf_channal = config_readback.eeprom_nrf_channal;
  tft_theme = config_readback.eeprom_tft_theme;
  tft_rotation = config_readback.eeprom_tft_rotation;
  mcu_voltage = config_readback.eeprom_mcu_voltage;

  return true;
}

void eeprom_write(bool _sta)
{
  if (!_sta)    //default
  {
    safe_check = 1111;

    for (uint8_t a = 0; a < 4; a++)
    {
      joy_correct_min[a] = -512;
      joy_correct_max[a] = 512;
    }
    /*
        Joy_deadzone_val = 10;

        mode_protocol = 0;
      #if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
        mode_mpu = 0;
      #endif
      #if defined(__AVR_ATmega128RFA1__)
        mwc_channal = 4;
      #else
        mwc_channal = 70;
      #endif

        nrf_channal = 70;
        tft_theme = 0;
        tft_rotation = 1;
        mcu_voltage = 1;
    */
  }

  // 定义结构变量config，并定义config的内容
  config_type config;

  config.eeprom_safe_check = safe_check;

  for (uint8_t a = 0; a < 4; a++)
  {
    config.eeprom_correct_min[a] = joy_correct_min[a];
    config.eeprom_correct_max[a] = joy_correct_max[a];
  }
  config.eeprom_Joy_deadzone_val = Joy_deadzone_val;

  config.eeprom_mode_protocol = mode_protocol;
#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
  config.eeprom_mode_mpu = mode_mpu;
#endif

  config.eeprom_mwc_channal = mwc_channal;
  config.eeprom_nrf_channal = nrf_channal;
  config.eeprom_tft_theme = tft_theme;
  config.eeprom_tft_rotation = tft_rotation;
  config.eeprom_mcu_voltage = mcu_voltage;

  // 变量config存储到EEPROM，地址0写入
  EEPROM_write(0, config);
}
