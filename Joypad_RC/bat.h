int _V_bat=_V_min;

#define MCU_VOLTAGE 3.3   // 5.0 or 3.3
#define _V_fix 0.2  //fix battery voltage
#define _V_math(Y) 10*(_V_fix+((Y*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f))))

void vobat()
{
  //_V_bat=10*((voltage*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f)));
  _V_bat=_V_math(MCU_VOLTAGE);
  if(_V_bat<_V_min) _V_bat=_V_min;
  if(_V_bat>_V_max) _V_bat=_V_max;

#ifdef Serial_DEBUG
  Serial.print("_V_bat: ");
  Serial.println(_V_bat);
#endif
}

