#include "Arduino.h"

byte inBuf[16];

int16_t outBuf[8] =
{
  Joy_MID, Joy_MID, Joy_MID, Joy_MID, Joy_MID, Joy_MID, Joy_MID, Joy_MID
};

boolean AUX[4] = {0, 0, 0, 0};
//======================================
void data_begin()
{
  Joy();

  if (mode_protocol)   //Robot
  {
    if (!sw_l)
    {
      Joy_x = Joy_MID;
      Joy_y = Joy_MID;
      Joy1_x = Joy_MID;
      Joy1_y = Joy_MID;
    }
  }
  else        //QuadCopter
  {
    if (!sw_l)
      Joy_y = Joy_MID - Joy_maximum;
  }

  //but---------------------------------
  for (uint8_t a = 0; a < 4; a++)
  {
    if (key_get(a, 1))  AUX[a] = !AUX[a];
  }

  outBuf[0] = Joy1_x;
  outBuf[1] = Joy1_y;
  outBuf[2] = Joy_x;
  outBuf[3] = Joy_y;
  outBuf[4] = map(AUX[0], 0, 1, Joy_MID - Joy_maximum, Joy_MID + Joy_maximum);
  outBuf[5] = map(AUX[1], 0, 1, Joy_MID - Joy_maximum, Joy_MID + Joy_maximum);
  outBuf[6] = map(AUX[2], 0, 1, Joy_MID - Joy_maximum, Joy_MID + Joy_maximum);
  outBuf[7] = map(AUX[3], 0, 1, Joy_MID - Joy_maximum, Joy_MID + Joy_maximum);
}

