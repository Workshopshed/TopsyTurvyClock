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

//Todo: Replace with calls to RTC
int Clock::hour()
{
    return this->ihour;
}

int Clock::minute()
{
  return this->iminute;
}

int Clock::second()
{
  return 0;
}

void Clock::display()
{
   Serial.print(this->ihour);
   Serial.print(':');
   Serial.println(this->iminute);
};

//Extend to seconds?
boolean Clock::parseTime(String t)
{
  if (t.length() != 5) return false;
  if (t.charAt(2) != ':') return false;
  this->ihour=t.substring(0,2).toInt();
  if (this->ihour>23) return false;
  this->iminute=t.substring(3,5).toInt();
  if (this->iminute>59) return false;
  
  //Todo replace iminutes and ihour with local var
  set(this->ihour,this->iminute,0);

  return true;
}

void Clock::set(int h,int m,int s)
{
  tmElements_t tm;
  time_t t;
  
  tm.Hour = h;  
  tm.Minute = m;
  tm.Second = s;
  t = makeTime(tm);
  
  RTC.set(t);   // set the RTC and the system time to the received value
  setTime(t); 
}

boolean Clock::parseDate(String t)
{ 
  // dd/mm/yyyy
  int day;
  int month;
  int year;
  if (t.length() != 10) return false;
  if (t.charAt(2) != '/' || t.charAt(5) != '/') return false;
  day=t.substring(0,2).toInt();
  month=t.substring(3,5).toInt();
  year=t.substring(6,10).toInt();
  if (day==0 || month==0 || year==0) return false;
  //Todo: Set RTC here
  Serial.println(day);
  Serial.println(month);
  Serial.println(year);
  return true;
}
