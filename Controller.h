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
    Controller(CustomStepper *stepperH,CustomStepper *stepperM,Clock *clock);
    void init();
    void readinput();
    void run();
    void dump();
  private:
    String commandbuffer;
    Clock *_clock;
    CustomStepper *_stepperH;
    CustomStepper *_stepperM;
    float CalcMinutePos(int m, int s);
    float CalcHourPos(int h, int m);
    void Adjust();
    float hourAdjust;
    float minuteAdjust;
    bool running;
    float _positions[12];
};

#endif
