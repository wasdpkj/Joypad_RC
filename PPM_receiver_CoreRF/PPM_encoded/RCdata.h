#include "arduino.h"
//======================
/*
[head,2byte,0xAA 0xBB] [type,1byte,0xCC] [data,16byte] [body,2byte,0x0D,0x0A]
 Example:
 AA BB CC 1A 01 1A 01 1A 01 2A 01 3A 01 4A 01 5A 01 6A 01 0D 0A
 */

//-------------------------
int16_t RC_data[8];
int type;

//=============================
int p;
int16_t read16(byte* _Buf) {
  byte _Buf1 = _Buf[p++];
  byte _Buf2 = _Buf[p++];
  int16_t r = (_Buf1 & 0xff) | ((_Buf2 << 8) & 0xff00);
  return r;
}

void read_data(int _num, byte* _buf)
{
  p = 0;
  _num = _num / 2;

  int16_t _bufin[_num];

  for (int i = 0; i < _num; i++)
  {
    _bufin[i] = read16(_buf);
  }

#ifdef _DEBUG
  DEBUG.println("\n\r");
#endif

#ifdef _DEBUG
  DEBUG.print("DATA_PPM: ");
#endif
  int16_t RC_data_c[16];
  for (int a = 0; a < _num; a++)
  {
#ifdef _DEBUG
    DEBUG.print(_bufin[a]);
    DEBUG.print(",");
#endif
    RC_data_c[a] = _bufin[a];
  }

  for (int a = 0; a < _num/2; a++)
  {
    RC_data[a] = RC_data_c[PPM_LINE[a]];
  }


#ifdef _DEBUG
  DEBUG.println(" ");
#endif
}

byte inChar, inCache;
byte buffer[256];
unsigned long num = 0;

boolean sta = false;
boolean error = false;

void protocol()
{
  if (FROM.available() > 0) {
    inCache = inChar;
    inChar = FROM.read();

    buffer[num] = inChar;
#ifdef _DEBUG
    //DEBUG.print(inCache,HEX);
    //DEBUG.print(inChar,HEX);
#endif
    num++;
    delayMicroseconds(200);
  }

  if (sta)
  {
    sta = false;

    switch (inChar)
    {
      case 0xCC:
        error = false;
        type = 1;
#ifdef _DEBUG
        DEBUG.println("\n\rType 1 RAW: ");
#endif
        break;
      default:
        error = true;
        type = 0;
#ifdef _DEBUG
        DEBUG.println("\n\rType ERROR ");
#endif
    }
    num = 0;
  }

  if (inChar == 0xbb && inCache == 0xaa)
  {
    sta = true;
#ifdef _DEBUG
    DEBUG.println("\n\r");
    DEBUG.println("\n\r----START----");
#endif
  }


  if (inChar == 0x0a && inCache == 0x0d)
  {
    inChar = NULL;
    inCache = NULL;

    num -= 2;

#ifdef _DEBUG
    DEBUG.print("NUM[");
    DEBUG.print(num);
    DEBUG.print("]:");
    for (long a = 0; a < num; a++)
    {
      DEBUG.print(buffer[a], HEX);
      DEBUG.print(" ");
    }
    DEBUG.println(" ");
#endif

    if (error || num!=16)
    {
#ifdef _DEBUG
      DEBUG.println("DATA ERROR");
#endif
    }
    else
    {
#ifdef _DEBUG
      DEBUG.println("DATA OK");
#endif
      read_data(num, buffer);

      //---------------------
#ifdef _DEBUG
      DEBUG.println("\n\rsendDATA READY");
#endif

      RC_write(RC_data);
      ///      delay(8);
    }
#ifdef _DEBUG
    //DEBUG.print("\n\r");
    //DEBUG.println(num);
#endif
    num = 0;

#ifdef _DEBUG
    DEBUG.println("\n\r----END----");
#endif
  }
}
