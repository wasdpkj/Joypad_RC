int8_t _V_bat = _V_min;

boolean mcu_voltage = true; // 5.0 or 3.3
#define _V_fix 0.2  //fix battery voltage
#define _V_math(Y) (_V_fix+((Y*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f))))

void vobat()
{
  //_V_bat=10*((voltage*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f)));
  _V_bat = _V_math(mcu_voltage ? 50 : 33);
  _V_bat = constrain(_V_bat, _V_min, _V_max);

#ifdef Serial_DEBUG
  Serial.print("_V_bat: ");
  Serial.println(_V_bat);
#endif
}

