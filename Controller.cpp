#include "Controller.h"

Controller::Controller(CustomStepper *stepperH,CustomStepper *stepperM,Clock *clock)
{
  _clock = clock;
  _stepperH = stepperH;
  _stepperM = stepperM;
  //initialise positions to match those on the clock face 0 = 12
  _positions[0] = 300;
  _positions[1] = 120;
  _positions[2] = 270;
  _positions[3] = 180;
  _positions[4] = 330;
  _positions[5] = 60;
  _positions[6] = 0;
  _positions[7] = 90;
  _positions[8] = 150;
  _positions[9] = 240;
  _positions[10] = 30;
  _positions[11] = 210;
}

void Controller::init()
{
  _clock->init();
  _stepperH->setRPM(10);
  _stepperM->setRPM(10);
  _stepperH->home();
  _stepperM->home();
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
        this->_stepperH->home();
        this->_stepperM->home();
        this->running = false;
        break;
      default:
        this->_clock->display();
      }
    }
  }
 }
 
 float Controller::CalcMinutePos(int m,int s)
 {
   return _positions[(int)(m / 5.0)-1] + ((m % 5) * 6.0) + (s * 0.1);
 }
 
 float Controller::CalcHourPos(int h,int m)
 {
   return _positions[h-1] + (m * 0.5);
 }
 
 void Controller::run()
  {
    if (this->running) {
      if (this->_stepperM->isDone() && this->_stepperH->isDone()) {
        this->_stepperH->rotateToDegrees(CalcHourPos(this->_clock->hour(),this->_clock->minute()));
        this->_stepperM->rotateToDegrees(CalcMinutePos(this->_clock->minute(),this->_clock->second()));
      }
    }
    
    this->_stepperH->run();
    this->_stepperM->run();
  }
  
void Controller::dump()
{
   Serial.println("=================");
   Serial.print(" Micros:");
   Serial.println(micros());
   Serial.print(" Hours:");
   Serial.println(this->_clock->hour());   
   Serial.print(" Target Angle:");
   Serial.println(CalcHourPos(this->_clock->hour(),this->_clock->minute()));
   Serial.print(" Minutes:");
   Serial.println(this->_clock->minute());   
   Serial.print(" Target Angle:");
   Serial.println(CalcMinutePos(this->_clock->minute(),this->_clock->second()));
   this->_stepperH->dump();
   this->_stepperM->dump();
}
