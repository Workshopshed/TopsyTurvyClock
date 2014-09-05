#include "Controller.h"

Controller::Controller(CustomStepper *stepperH,CustomStepper *stepperM,Clock *clock)
{
  //Todo: Merge this details from my other controller code
  _clock = clock;
  _stepperH = stepperH;
  _stepperM = stepperM;
}

void Controller::init()
{
  _clock->init();
  
  
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
      if (this->_stepperM->isDone() && this->_stepperH->isDone()) {
        this->_stepperH->rotateToDegrees(CalcHourPos(this->_clock->hour(),this->_clock->minute()));
        this->_stepperM->rotateToDegrees(CalcMinutePos(this->_clock->minute()));
      }
    }
    
    this->_stepperH->run();
    this->_stepperM->run();
  }
  
void Controller::dump()
{
   this->_stepperH->dump();
   this->_stepperM->dump();
}
