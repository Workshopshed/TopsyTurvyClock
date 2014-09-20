#include "Clock.h"

Clock::Clock(Timezone *tz,DS1307 *rtc)
{
    _tz = tz;
    _rtc = rtc;
}

void Clock::init(){
  delay(1000); // Give RTC time to start up
  if(timeStatus()!= timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time"); 
}

//Todo: Replace with calls to RTC converted to local time with TimeZone
int Clock::chour()
{
    return hour();
}

int Clock::cminute()
{
  return minute();
}

int Clock::csecond()
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

boolean Clock::parseTime(String t)
{
  int h;
  int m;
  int s;
  s=0;
  if (t.length() != 5 && t.length() != 8) return false;
  if (t.charAt(2) != ':') return false;
  h=t.substring(0,2).toInt();
  if (h>23) return false;
  m=t.substring(3,5).toInt();
  if (m>59) return false;
  if (t.length() == 8) {
    if (t.charAt(5) != ':') return false;
    s = t.substring(6,8).toInt();
  }
  
  //Todo adjust for local time?
  set(year(),month(),day(),h,m,s);

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
  tm.Year = CalendarYrToTm(y); //TM.Year is a byte so can't take year > 255
  t = makeTime(tm);
  
  this->_rtc->set(t);   // set the RTC and the system time to the received value
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
