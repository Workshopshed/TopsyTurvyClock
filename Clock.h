/* Class for setting and reading the clock */

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef CLOCK_H
#define CLOCK_H

class Clock {
  private:
    /* Temp, replace with Time.h functions etc. */
    int ihour;
    int iminute;
  public:
    boolean parseTime(String t);
    boolean parseDate(String d); 
    void display();
    int hour();
    int minute();
};

#endif
