#include <Arduino.h>
#include <Wire.h>
#include "Time.h"
#include "DS1307.h"
 
int ledPin = 13;
DS1307 rtc;

void setup()
{
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  delay(5000);
/*
#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year 
#define  CalendarYrToTm(Y)   ((Y) - 1970)
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)   
*/

  tmElements_t tm; //TM structure values are unsigned integeres aka a byte so the value is from 0 to 255
  tm.Year = CalendarYrToTm(2014);
  tm.Month = 9;
  tm.Day = 19;
  tm.Hour = 20;
  tm.Minute = 15;
  tm.Second = 30;
  rtc.set(makeTime(tm));
}
 
void loop()
{
  int i;
  Serial.println("--------------");
  time_t t = rtc.get();
  Display(t);
 
    digitalWrite(ledPin, HIGH); 
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);

}

void Display(time_t t)
{
  Serial.print (hour(t));
  Serial.print (':');
  Serial.print (minute(t));
  Serial.print (':');
  Serial.println (second(t));
  Serial.print (day(t));
  Serial.print ('/');
  Serial.print (month(t));
  Serial.print ('/');
  Serial.println (year(t));  
  
}



