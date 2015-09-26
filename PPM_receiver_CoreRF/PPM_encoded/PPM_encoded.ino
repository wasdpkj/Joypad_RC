#include <ZigduinoRadio.h>

#include "def.h"
#include "RC.h"
#include "RCdata.h"

//-------------------------
//#include  <Servo.h>
#include <RCEncoder.h>

//===================
void setup()
{
  ZigduinoRadio.begin(11);
#ifdef _DEBUG
  DEBUG.begin(115200);
#endif

  RC_begin();
}

// the loop routine runs over and over again forever:
void loop()
{
  protocol();

  RC_safe_mode();
}
