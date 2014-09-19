#include <Wire.h>
#include "CustomStepper.h"  //http://playground.arduino.cc/Main/CustomStepper
#include "Delay.h"
#include "Controller.h"

CustomStepper stepperm(15,14,16,10,18,19, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 10, CW);
CustomStepper stepperh(6,7,8,20,5,4, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 10, CW);

TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; //British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};  //Standard Time
Timezone tz(BST, GMT);

DS1307 rtc;

Clock clock(&tz,&rtc);

Controller con(&stepperh,&stepperm,&clock);

NonBlockDelay d;
bool motorsinit;

time_t timesync(){
  return rtc.get();
}

void setup()
{
  Serial.begin(9600);
  //while (!Serial) {
  //; // wait for serial port to connect. Needed for Leonardo only
  //}
  
  //Setup Time provider#
  setSyncProvider(timesync); 
  setSyncInterval(59);
  con.init();
}

void loop()
{
  con.readinput();
  con.run();
  
  if (d.Timeout()) {
        con.dump();
  d.Delay(5000);
  }  
  

}








