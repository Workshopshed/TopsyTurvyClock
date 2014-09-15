#include "Controller.h"
#include "Clock.h"

Controller con;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  con.init();
}

void loop()
{
  con.readinput();
}
