#include "Controller.h"

void Controller::init()
{
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
  

