/* Class to control motion of clock */

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "Clock.h"

#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
  private:
    String commandbuffer;
    Clock clock;
    float CalcMinutePos(int m);
    float CalcHourPos(int h); 
    bool running;
  public:
    void init();
    void readinput();
    void run();
};

#endif
