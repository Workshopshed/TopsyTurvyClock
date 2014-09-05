#include "Controller.h"

Controller::Controller(CustomStepper *stepper1,CustomStepper *stepper2,Clock *clock)
{
  //Todo: Merge this details from my other controller code
  _clock = clock;
}

void Controller::init()
{
  //Setup Time provider
  //todo: Move to clock?
  setSyncProvider(RTC.get); 
  setSyncInterval(59);
  if(timeStatus()!= timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time"); 
  
  
  //Setup Motors
  //Todo: Merge this details from my other controller code  
  
  //Mark clock as running
  this->running = true;
}

void Controller::readinput()
{
  if (Serial.available() > 0)
  {
    this->commandbuffer = Serial.readString();
    if (this->commandbuffer.length() > 0)
    {
      switch(this->commandbuffer.charAt(0)){
      case '?':
        Serial.println("Todo:Instructions here");
        break;
      case 'T':
        if (this->_clock->parseTime(this->commandbuffer.substring(1))){
          Serial.println("Setting Time");
        }
        else {
          Serial.println("Invalid Time");
        }
        break;
      case 'D':
        if (this->_clock->parseDate(this->commandbuffer.substring(1))){
          Serial.println("Setting Date");
        }
        else {
          Serial.println("Invalid Date");
        }
        break;
      case 'R':
        this->running = true;
        break;
      case 'S':
        this->running = false;
        break;
      case 'H':
        //Todo: Home motors
        break;
      default:
        this->_clock->display();
      }
    }
  }
 }
 
 float Controller::CalcMinutePos(int m)
 {
   //Todo: What degrees is M at?
 }
 
 float Controller::CalcHourPos(int h,int m)
 {
   //Todo: What degrees is h at?
 }
 
 void Controller::run()
  {
    if (this->running) {
      if (this->stepperM->isDone() && this->stepperH->isDone()) {
        this->stepperH->rotateToDegrees(CalcHourPos(this->_clock->hour(),this->_clock->minute()));
        this->stepperM->rotateToDegrees(CalcMinutePos(this->_clock->minute()));
      }
    }
    
    this->stepperH->run();
    this->stepperM->run();
  }
  

