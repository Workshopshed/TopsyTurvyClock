#include "Clock.h"

Clock::Clock(Timezone *tz,DS1307RTC *rtc)
{
    _tz = tz;
    _rtc = rtc;
}

void Clock::init(){
  //Setup Time provider
  setSyncProvider(_rtc->get); 
  setSyncInterval(59);
  if(timeStatus()!= timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time"); 
}

//Todo: Replace with calls to RTC converted to local time with TimeZone
int Clock::hour()
{
    return hour();
}

int Clock::minute()
{
  return minute();
}

int Clock::second()
{
  return second();
}

void Clock::display()
{
   Serial.print(hour());
   Serial.print(':');
   Serial.println(minute());
   
   Serial.print(' ');
   Serial.print(day());
   Serial.print('/');
   Serial.print(month());
   Serial.print('/');
   Serial.println(year());
   
};

//Extend to seconds?
boolean Clock::parseTime(String t)
{
  int h;
  int m;
  if (t.length() != 5) return false;
  if (t.charAt(2) != ':') return false;
  h=t.substring(0,2).toInt();
  if (h>23) return false;
  m=t.substring(3,5).toInt();
  if (m>59) return false;
  
  //Todo adjust for local time?
  set(year(),month(),day(),h,m,0);

  return true;
}

void Clock::set(int y,int m,int d,int h,int n,int s)
{
  tmElements_t tm;
  time_t t;
  
  tm.Hour = h;  
  tm.Minute = n;
  tm.Second = s;
  tm.Month = m;
  tm.Day = d;
  tm.Year = y;
  t = makeTime(tm);
  
  RTC.set(t);   // set the RTC and the system time to the received value
  setTime(t); 
}

boolean Clock::parseDate(String t)
{ 
  // dd/mm/yyyy
  int d;
  int m;
  int y;
  if (t.length() != 10) return false;
  if (t.charAt(2) != '/' || t.charAt(5) != '/') return false;
  d=t.substring(0,2).toInt();
  m=t.substring(3,5).toInt();
  y=t.substring(6,10).toInt();
  if (d ==0 || m==0 || y==0) return false;

  //Todo adjust for local time?
  set(y,m,d,hour(),minute(),second());

  return true;
}
