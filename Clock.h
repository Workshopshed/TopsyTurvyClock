/* Class for setting and reading the clock */
#include "Time.h"
#include "Timezone.h"   //https://github.com/JChristensen/Timezone
#include <Wire.h>
#include "DS1307.h" // Modified version of DFRobot http://www.dfrobot.com/index.php?route=product/product&product_id=879

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef CLOCK_H
#define CLOCK_H

class Clock {
  public:
    Clock(Timezone *tz,DS1307 *rtc);
    boolean parseTime(String t);
    boolean parseDate(String d); 
    void display();
    void init();
    int chour();
    int cminute();
    int csecond();
  private:
    Timezone *_tz;
    DS1307 *_rtc;
    void set(int y,int m,int d,int h,int n,int s);
};

#endif
