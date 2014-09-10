/* Class for setting and reading the clock */
#include "Time.h"
#include "Timezone.h"   //https://github.com/JChristensen/Timezone
#include <Wire.h>
#include "DS1307RTC.h" // http://www.pjrc.com/teensy/td_libs_DS1307RTC.html

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef CLOCK_H
#define CLOCK_H

class Clock {
  public:
    Clock(Timezone *tz,DS1307RTC *rtc);
    boolean parseTime(String t);
    boolean parseDate(String d); 
    void display();
    void init();
    int hour();
    int minute();
    int second();
  private:
    /* Temp, replace with Time.h functions etc. */
    int ihour;
    int iminute;
    Timezone *_tz;
    DS1307RTC *_rtc;
    void set(int h,int m,int s);
};

#endif
