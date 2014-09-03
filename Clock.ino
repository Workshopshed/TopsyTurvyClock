#include "CustomStepper.h"
#include "Delay.h"
#include "Controller.h"
#include "RTC.h" //http://www.pjrc.com/teensy/td_libs_DS1307RTC.html

//Full constructor, just the first 4 parameters are necessary, they are the pins connected to the motor,
//the others are optional, and default to the following below
// 5 and 6 refer to the LED and Opto transistor pins
//the 7th paramater is the steps sequence, where the 1st element of the array is the number of steps
//it can have a maximum of 8 steps
//the 8th parameter is the SPR (Steps Per Rotation)
//the 9th parameter is the RPM
//the 10th parameter is the rotation orientation
CustomStepper stepper1(15,14,16,10,18,19, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CW);
CustomStepper stepper2(6,7,8,20,5,4, (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CW);
Controller con1;

NonBlockDelay d;
bool motorsinit;

void setup()
{
// My motors seem to be capable of handling upto about 16RPM but have good torque around 10rpm
  con1.init(&stepper1,&stepper2);
}

void loop()
{
  con1.run();
  
  if (d.Timeout()) {
        con1.dump();
  d.Delay(2000);
  }  
  

}








