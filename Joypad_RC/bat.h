//Battery---------
#define PIN_bat A7  //BAT

#define _V_max 42   //�4.2V
#define _V_min 33   //�3.7V

int8_t _V_bat = _V_min;
int8_t _V_bat_RC = _V_min;

#if F_CPU == 16000000
#if defined(RF_PORT)
#define mcu_voltage 33 // 3.3
#else
#define mcu_voltage 50 // 5.0
#endif
#else
#define mcu_voltage 33 // 3.3
#endif

#define _V_fix 3  //fix mcu voltage,+0.3v

uint8_t batVoltage() {
  return constrain(((((mcu_voltage + _V_fix) * analogRead(PIN_bat) / 1023.0f) / (33.0f / (51.0f + 33.0f)))), _V_min, _V_max);
}
