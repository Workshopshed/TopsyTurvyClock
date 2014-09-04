#include "Controller.h"

Controller::Controller(stepper1,stepper2,clock)
{
  //Todo: Merge this details from my other controller code
}

void Controller::init()
{
  //Setup Time provider
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
        if (this->clock.parseTime(this->commandbuffer.substring(1))){
          Serial.println("Setting Time");
        }
        else {
          Serial.println("Invalid Time");
        }
        break;
      case 'D':
        if (this->clock.parseDate(this->commandbuffer.substring(1))){
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
        //Todo: Home motoros
        break;
      default:
        clock.display();
      }
    }
  }
 }
 
 void Controller::run()
  {
  }
  

