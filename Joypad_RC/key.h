#include "arduino.h"

uint8_t key_pin[4] = {CH_SWITCH_1, CH_SWITCH_2, CH_SWITCH_3, CH_SWITCH_4}; //按键1 2 3 4

boolean key_status[4];			//按键
boolean key_cache[4];		//检测按键松开缓存

void key_init()
{
  for (uint8_t a = 0; a < 4; a++)
  {
    key_status[a] = LOW;
    key_cache[a] = HIGH;
  }
}

boolean key_get(uint8_t _key_num, boolean _key_type)
{
  key_cache[_key_num] = key_status[_key_num];		//缓存作判断用

  key_status[_key_num] = !Joypad.readButton(key_pin[_key_num]);	//触发时

  switch (_key_type)
  {
    case 0:
      if (!key_status[_key_num] && key_cache[_key_num])		//按下松开后
        return true;
      else
        return false;
      break;
    case 1:
      if (key_status[_key_num] && !key_cache[_key_num])		//按下松开后
        return true;
      else
        return false;
      break;
  }
}
