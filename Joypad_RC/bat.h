//Battery---------
#define PIN_bat A7  //BAT

#define _V_max 41   //�4.2V
#define _V_min 35   //�3.7V

int8_t _V_bat = _V_min;
int8_t _V_bat_RC = _V_min;

#if F_CPU == 16000000
#if defined(RF_PORT)
boolean mcu_voltage = 33; // 3.3
#else
boolean mcu_voltage = 50; // 5.0
#endif
#else
boolean mcu_voltage = 33; // 3.3
#endif

#define _V_fix 0.2  //fix battery voltage
#define _V_math(Y) (_V_fix+((Y*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f))))

uint8_t batVoltage() {
  return constrain(_V_math(mcu_voltage), _V_min, _V_max);
}
