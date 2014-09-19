#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include "DS1307RTC.h"

DS1307RTC rtc;
 

void setup()
{
  time_t t;
  tmElements_t tm;
  tm.Year = 2014;
  tm.Month = 9;
  tm.Day = 19;
  tm.Hour = 19;
  tm.Minute = 15;
  tm.Second = 30;
  t = makeTime(tm);
  rtc.set(t);
}
 
void loop()
{
  time_t t = rtc.get();
  Serial.print (hour(t));
  Serial.print (':');
  Serial.print (minute(t));
  Serial.print (':');
  Serial.println (second(t));
}

