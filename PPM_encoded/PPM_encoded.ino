#include "def.h"
#include <Microduino_Protocol.h>
ProtocolZig protocolA(16); //数据长度为16个字节

#include <Microduino_PPM.h>
PPM PPM;

uint16_t rcData[8];
uint16_t recData[8];
uint8_t recCmd;

uint32_t safe_time = 0;

void setup() {
#ifdef DEBUG
  DEBUG.begin(115200);
#endif
  protocolA.begin(CHANNEL_ZIG);  //括号内参数为CoreRF通道号
  PPM.beginWrite(OUTPUT_PIN, CHANNEL_NUM);  //引脚,通道数（不填默认8）
  PPM.setFix(-5);    //发送数据修正,单位us
}

void loop() {
  if (protocolA.available()) {
    protocolA.readWords(&recCmd, recData, 8);
#ifdef DEBUG
    DEBUG.print("recCmd: ");
    DEBUG.print(recCmd);
    DEBUG.print("  Data:");
#endif
    for (uint8_t i = 0; i < 8; i++) {
#ifdef DEBUG
      DEBUG.print(" ");
      DEBUG.print(recData[i]);
#endif
      rcData[i] = recData[PPM_LINE[i]];
    }
    for (uint8_t i = 0; i < 8; i++) {
      if (rcData[i] >= 1000 && rcData[i] <= 2000) {
        //myservo[i].writeMicroseconds(RC_write_i[i]);
        PPM.Write(i, rcData[i]);
      }
    }
#ifdef DEBUG
    DEBUG.println();
#endif
    safe_time = millis();
  }


  if (millis() < safe_time) safe_time = millis();
  if (millis() - safe_time > SAFE_TIME_OUT) {
#ifdef DEBUG
    DEBUG.println("\n\rSAFE_TIME_OUT");
#endif
    for (int i = 0; i < 8; i++) {
      //myservo[i].writeMicroseconds(SAFE_CHANNEL_VAL);
      PPM.Write(i, SAFE_CHANNEL_VAL);
    }
  }
}
