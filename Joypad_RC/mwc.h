#include "Arduino.h"

#if defined(__AVR_ATmega128RFA1__)
#include <ZigduinoRadio.h>
#endif

int16_t RCin[8], RCoutA[8], RCoutB[8];

int16_t p;
uint16_t read16()
{
  uint16_t r = (inBuf[p++] & 0xFF);
  r += (inBuf[p++] & 0xFF) << 8;
  return r;
}

uint16_t t, t1, t2;
uint16_t write16(boolean a)
{
  if (a)
  {
    t1 = outBuf[p++] >> 8;
    t2 = outBuf[p - 1] - (t1 << 8);
    t = t1;
  }
  else
    t = t2;
  return t;
}

typedef  unsigned char byte;
byte getChecksum(byte length, byte cmd, byte mydata[])
{
  //三个参数分别为： 数据长度  ，  指令代码  ，  实际数据数组
  byte checksum = 0;
  checksum ^= (length & 0xFF);
  checksum ^= (cmd & 0xFF);
  for (uint8_t i = 0; i < length; i++)
  {
    checksum ^= (mydata[i] & 0xFF);
  }
  return checksum;
}

void data_rx()
{
  //  s_struct_w((int*)&inBuf,16);
  p = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    RCin[i] = read16();
    /*
    Serial.print("RC[");
     Serial.print(i+1);
     Serial.print("]:");

     Serial.print(inBuf[2*i],DEC);
     Serial.print(",");
     Serial.print(inBuf[2*i+1],DEC);

     Serial.print("---");
     Serial.println(RCin[i]);
     */
    //    delay(50);        // delay in between reads for stability
  }
}

void data_tx()
{
  p = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    RCoutA[i] = write16(1);
    RCoutB[i] = write16(0);

    /*
    Serial.print("RC[");
     Serial.print(i+1);
     Serial.print("]:");

     Serial.print(RCout[i]);

     Serial.print("---");

     Serial.print(RCoutA[i],DEC);
     Serial.print(",");
     Serial.print(RCoutB[i],DEC);

     Serial.println("");
     */
    //    delay(50);        // delay in between reads for stability
  }
}

/*
if Core RF
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte] [body,1byte,getChecksum()]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05 E3
 */
void data_send()
{
  data_tx();

#if !defined(__AVR_ATmega128RFA1__)
  static byte buf_head[3];
  buf_head[0] = 0x24;
  buf_head[1] = 0x4D;
  buf_head[2] = 0x3C;
#endif

#define buf_length 0x10   //16
#define buf_code 0xC8     //200

  static byte buf_data[buf_length];
  for (uint8_t a = 0; a < (buf_length / 2); a++)
  {
    buf_data[2 * a] = RCoutB[a];
    buf_data[2 * a + 1] = RCoutA[a];
  }

  static byte buf_body;
  buf_body = getChecksum(buf_length, buf_code, buf_data);

  //----------------------
#if defined(__AVR_ATmega128RFA1__)
  mwc_port.beginTransmission();
  mwc_port.write(0xaa);
  mwc_port.write(0xbb);
#else
  for (uint8_t a = 0; a < 3; a++) {
    mwc_port.write(buf_head[a]);
  }
  mwc_port.write(buf_length);
#endif
  mwc_port.write(buf_code);
  for (uint8_t a = 0; a < buf_length; a++) {
    mwc_port.write(buf_data[a]);
  }
  mwc_port.write(buf_body);
#if defined(__AVR_ATmega128RFA1__)
  mwc_port.endTransmission();
#endif
}
