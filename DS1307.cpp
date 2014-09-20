#include "DS1307.h"

DS1307::DS1307()
{
  Wire.begin();
}

// PRIVATE FUNCTIONS

// Aquire data from the RTC chip in BCD format
// refresh the buffer
void DS1307::read(void)
{
  // use the Wire lib to connect to tho rtc
  // reset the resgiter pointer to zero
  Wire.beginTransmission(DS1307_CTRL_ID);
    Wire.write((uint8_t)0x00);
  Wire.endTransmission();

  // request the 7 bytes of data    (secs, min, hr, dow, date. mth, yr)
  Wire.requestFrom(DS1307_CTRL_ID, 7);
  for(int i=0; i<7; i++)
  {
    // store data in raw bcd format
    rtc_bcd[i]=Wire.read();
  }
}

// update the data on the IC from the bcd formatted data in the buffer
void DS1307::save(void)
{
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write((uint8_t)0x00); // reset register pointer
  for(int i=0; i<7; i++)
  {
    Wire.write(rtc_bcd[i]);
  }
  Wire.endTransmission();
}


// PUBLIC FUNCTIONS
void DS1307::get(int *rtc, boolean refresh)   // Aquire data from buffer and convert to int, refresh buffer if required
{
  if(refresh) read();
  for(int i=0;i<7;i++)  // cycle through each component, create array of data
  {
	rtc[i]=get(i, 0);
  }
}

time_t DS1307::get()   // Aquire data from buffer and convert to time_t
{
  read(); //Refresh
  tmElements_t tm;
  tm.Second = get(DS1307_SEC,0);
  tm.Minute = get(DS1307_MIN,0);
  tm.Hour = get(DS1307_HR,0);
  tm.Wday = get(DS1307_DOW,0);
  tm.Day = get(DS1307_DATE,0);
  tm.Month = get(DS1307_MTH,0);
  tm.Year = y2kYearToTm(get(DS1307_YR,0));
  return(makeTime(tm));
}

void DS1307::set(time_t t)
{
  stop();
  tmElements_t tm;
  breakTime(t, tm);
  set(DS1307_SEC,tm.Second);
  set(DS1307_MIN,tm.Minute);
  set(DS1307_HR,tm.Hour);
  set(DS1307_DOW,tm.Wday);
  set(DS1307_DATE,tm.Day);
  set(DS1307_MTH,tm.Month);
  set(DS1307_YR,tmYearToY2k(tm.Year));
  save();
  start();
}


int DS1307::get(int c, boolean refresh)  // aquire individual RTC item from buffer, return as int, refresh buffer if required
{
  if(refresh) read();
  return bcd2dec(rtc_bcd[c]);
}

void DS1307::set(int c, int v)  // Update buffer, then update the chip
{
  switch(c)
  {
  case DS1307_SEC:
    if(v<60 && v>-1)
    {
	//preserve existing clock state (running/stopped)
	int state=rtc_bcd[DS1307_SEC] & DS1307_CLOCKHALT;
	rtc_bcd[DS1307_SEC]=state | dec2bcd(v);
    }
    break;
  case DS1307_MIN:
    if(v<60 && v>-1)
    {
	rtc_bcd[DS1307_MIN]=dec2bcd(v);
    }
    break;
  case DS1307_HR:
  // TODO : AM/PM  12HR/24HR
    if(v<24 && v>-1)
    {
	rtc_bcd[DS1307_HR]=dec2bcd(v);
    }
    break;
  case DS1307_DOW:
    if(v<8 && v>-1)
    {
	rtc_bcd[DS1307_DOW]=v;
    }
    break;
  case DS1307_DATE:
    if(v<31 && v>-1)
    {
	rtc_bcd[DS1307_DATE]=dec2bcd(v);
    }
    break;
  case DS1307_MTH:
    if(v<13 && v>-1)
    {
	rtc_bcd[DS1307_MTH]=dec2bcd(v);
    }
    break;
  case DS1307_YR:
    if(v<80 && v>-1)
    {
	rtc_bcd[DS1307_YR]=dec2bcd(v);
    }
    break;
  } // end switch
}

void DS1307::stop(void)
{
	// set the ClockHalt bit high to stop the rtc
	// this bit is part of the seconds byte
    rtc_bcd[DS1307_SEC]=rtc_bcd[DS1307_SEC] | DS1307_CLOCKHALT;
    save();
}

void DS1307::start(void)
{
	// unset the ClockHalt bit to start the rtc
	// TODO : preserve existing seconds
    rtc_bcd[DS1307_SEC]=rtc_bcd[DS1307_SEC] & (~DS1307_CLOCKHALT);
	save();
}


void DS1307::SetOutput(uint8_t c)  
{
  uint8_t out;
  switch(c)
  {
  case HIGH :
	out=DS1307_HIGH_BIT;
  break;
  case LOW :
	out=DS1307_LOW_BIT;
  break;
  case DS1307_SQW1HZ :
	out=DS1307_SQW1HZ_BIT;
  break;
  case DS1307_SQW4KHZ :
	out=DS1307_SQW4KHZ_BIT;
  break;
  case DS1307_SQW8KHZ :
	out=DS1307_SQW8KHZ_BIT;
  break;
  case DS1307_SQW32KHZ :
	out=DS1307_SQW32KHZ_BIT;
  break;
  default: 
	out=DS1307_LOW_BIT;
  }
  
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write((uint8_t)0x07); 
  Wire.write(out);
  Wire.endTransmission();
}

uint8_t DS1307::GetOutput(void)  
{
  Wire.beginTransmission(DS1307_CTRL_ID);
  Wire.write((uint8_t)0x07);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_CTRL_ID, 1);
  uint8_t out=Wire.read();
  
  int c=-1;
  switch(out)
  {
  case DS1307_HIGH_BIT :
	c=HIGH;
  break;
  case DS1307_LOW_BIT :
	c=LOW;
  break;
  case DS1307_SQW1HZ_BIT :
	c=DS1307_SQW1HZ;
  break;
  case DS1307_SQW4KHZ_BIT :
	c=DS1307_SQW4KHZ;
  break;
  case DS1307_SQW8KHZ_BIT :
	c=DS1307_SQW8KHZ;
  break;
  case DS1307_SQW32KHZ_BIT :
	c=DS1307_SQW32KHZ;
  break;
  }
  return c;
}

// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t DS1307::dec2bcd(uint8_t num)
{
  return ((num / 10)<<4) + (num % 10);
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t DS1307::bcd2dec(uint8_t num)
{
  return (10*((num & DS1307_HI_SEC)>>4))+(num & DS1307_LO_BCD);
}
