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

void Controller::Adjust() 
{
  //Fine tune positions
  if (this->commandbuffer.charAt(1) == 'H') {
    this->hourAdjust += this->commandbuffer.substring(2).toInt();
  }
  if (this->commandbuffer.charAt(1) == 'M') {
    this->minuteAdjust += this->commandbuffer.substring(2).toInt();
  }
  //Todo: Save these to Eeprom
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
        Serial.println("? - Instructions");
        Serial.println("H - Home and stop");
        Serial.println("Thh:mm - Set time");
        Serial.println("Ddd/mm/yyyy - Set date");
        Serial.println("R - Run");
        Serial.println("S - Stop");
        break;
      case 'A':
        Serial.println("Motor Adjustments");
        Adjust();
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
        Serial.println("Homing and stop");
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
   return _positions[(int)(m / 5.0)] + ((m % 5) * 6.0);
 }
 
 float Controller::CalcHourPos(int h,int m)
 {
   return _positions[(h % 12)] + (m * 0.5);
 }
 
 void Controller::run()
  {
    if (this->running) {
      if (this->_stepperM->isDone() && this->_stepperH->isDone()) {
        this->_stepperH->rotateToDegrees(CalcHourPos(this->_clock->chour(),this->_clock->cminute()) + this->hourAdjust);
        this->_stepperM->rotateToDegrees(CalcMinutePos(this->_clock->cminute(),this->_clock->csecond()) + this->minuteAdjust);
      }
    }
    
    this->_stepperH->run();
    this->_stepperM->run();
  }
  
void Controller::dump()
{
   Serial.println("================");
   Serial.println(this->running ? "Running" : "Stopped");
   Serial.print("Hour Adjust:");
   Serial.println(this->hourAdjust);
   Serial.print("Minute Adjust:");
   Serial.println(this->minuteAdjust);   
   this->_clock->display();
   Serial.println("------------------");
   Serial.println("Hour Motor");
   this->_stepperH->dump();
   Serial.println("------------------");
   Serial.println("Minute Motor");
   this->_stepperM->dump();
}
