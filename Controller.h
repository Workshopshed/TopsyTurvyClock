/* Class to control motion of clock */

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "CustomStepper.h"
#include "Clock.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
  public:
    Controller(CustomStepper *stepper1,CustomStepper *stepper2,Clock *clock);
    void init();
    void readinput();
    void run();
    
  private:
    String commandbuffer;
    Clock *_clock;
    CustomStepper *stepperH;
    CustomStepper *stepperM;
    float CalcMinutePos(int m);
    float CalcHourPos(int h, int m); 
    bool running;
      
};

#endif
