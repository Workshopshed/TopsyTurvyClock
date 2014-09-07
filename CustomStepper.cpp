//Copyright 2012 Igor Campos Origional Custom Steper Class
//Copyright 2014 Andy Clark Modifications to support OptoSensor, Homing and Rotate to Position
//
//This file is part of CustomStepper.
//
//CustomStepper is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//CustomStepper is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with CustomStepper.  If not, see <http://www.gnu.org/licenses/>.

#include "CustomStepper.h"

// My motors seem to be capable of handling upto about 16RPM but have good torque around 10rpm
//Full constructor, just the first 4 parameters are necessary, they are the pins connected to the motor,
//the others are optional, and default to the following below
// 5 and 6 refer to the LED and Opto transistor pins
//the 7th paramater is the steps sequence, where the 1st element of the array is the number of steps
//it can have a maximum of 8 steps
//the 8th parameter is the SPR (Steps Per Rotation)
//the 9th parameter is the RPM
//the 10th parameter is the rotation orientation
CustomStepper::CustomStepper(byte pin1, byte pin2, byte pin3, byte pin4, byte pinOptoLED, byte pinOptoTran, byte steps[], float spr, float rpm, motordir direction)
{
  this->setRPM(rpm);
  this->setSPR(spr);
  this->setDirection(direction);
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;
  this->pinOptoLED = pinOptoLED; 
  this->pinOptoTran = pinOptoTran;
  this->setupMotor();
  this->setupOpto();
  this->noSteps = steps[0];
  for (int i = 0; i < this->noSteps; i++)
    this->steps[i] = steps[i+1];
  this->nextStep = 0;
  this->stepCorrection = 0;
  this->done = true;
  this->absolutePosition = -1; //Unknown
  this->setTimer();
}

void CustomStepper::setupMotor()
{
  pinMode(this->pin1, OUTPUT);
  pinMode(this->pin2, OUTPUT);
  pinMode(this->pin3, OUTPUT);
  pinMode(this->pin4, OUTPUT);
}

void CustomStepper::setupOpto()
{
  pinMode(this->pinOptoLED, OUTPUT);
  digitalWrite(this->pinOptoLED, HIGH);
  pinMode(this->pinOptoTran, INPUT_PULLUP);
}

void CustomStepper::dostep()
{
      digitalWrite(this->pin1, (this->steps[this->nextStep] & B1000));
      digitalWrite(this->pin2, (this->steps[this->nextStep] & B0100));
      digitalWrite(this->pin3, (this->steps[this->nextStep] & B0010));
      digitalWrite(this->pin4, (this->steps[this->nextStep] & B0001));
      this->setTimer();
      if (this->direction == CW)
        this->nextStep == this->noSteps-1 ? this->nextStep = 0 : this->nextStep++;
      else if (this->direction == CCW)
        this->nextStep == 0 ? this->nextStep = this->noSteps-1 : this->nextStep--;
      this->stepsToGo--;
      this->calcAbsolutePosition();
}

void CustomStepper::step()
{
  if (this->mode == HOME && this->readOpto()){ 
         this->absolutePosition = 0;
         this->disable();
       }
  if (this->mode == UNHOME && !this->readOpto())
         this->mode = HOME;         
  if (this->done) return;
  
  if (this->stepsToGo > 0)
  {
    if (this->direction != STOP)
      this->dostep();
    else
      this->disable();
  }
  else if (this->mode == CONTINUOUS || this->mode == UNHOME || this->mode == HOME)
  {
    this->stepsToGo = this->spr;
    this->step();
  }
  else if (this->mode == STEPS)
    this->disable();
}

void CustomStepper::home()
{
  this->rotateDegrees(360);
  if (this->readOpto() == 1){
    this->unhome();
    return;
  }
  this->mode = HOME;
}

void CustomStepper::unhome()
{
  this->mode = UNHOME;
}

void CustomStepper::disable()
{
  this->time = 0;
  digitalWrite(this->pin1, LOW);
  digitalWrite(this->pin2, LOW);
  digitalWrite(this->pin3, LOW);
  digitalWrite(this->pin4, LOW);
  this->done = true;
}

void CustomStepper::setRPM(float rpm)
{
  if (rpm > 0)
    this->rpm = rpm;
  this->calcTime();
}

void CustomStepper::setSPR(float spr)
{
  if (spr > 0)
    this->spr = spr;
  this->calcTime();
}

void CustomStepper::calcTime()
{
  if (this->spr > 0 && this->rpm > 0)
    this->time = 60000000.0/this->spr/this->rpm;
}

void CustomStepper::setDirection(motordir direction)
{
  if (direction == CW || direction == CCW || direction == STOP)
    this->direction = direction;
}

void CustomStepper::rotateDegrees(float degrees)
{
  if (this->spr > 0 && this->rpm > 0 && degrees > 0)
  {
    this->mode = STEPS;
    this->stepsToGo = this->degreesToSteps(degrees) + this->stepCorrection + 0.5;
    this->stepCorrection = this->degreesToSteps(degrees) + this->stepCorrection - this->stepsToGo;
    this->done = false;
    this->calcTime();
    this->step();
  }
}

unsigned int CustomStepper::degreesToSteps(float degrees){
  return ((degrees/360.0)*this->spr);
}

void CustomStepper::rotateToDegrees(float degrees)
{ 
  float target = degrees-this->positionDegrees();
  if (target < 0) {
    target = 360 + target;
  }
  if (target > 180) {
    this->direction == CCW;
    target = 360 - target;
  }
  else {
    this->direction == CW;
  }
  this->rotateDegrees(target);
}

float CustomStepper::positionDegrees()
{
  return ((this->absolutePosition * 360.0) / this->spr);
}

void CustomStepper::calcAbsolutePosition()
{
   // just done a step, what is the new position 
   if (this->mode == HOME || this->mode == UNHOME){
     this->absolutePosition = -1;
     return;
   }
   if (this->direction == CW)
        this->absolutePosition >= this->spr-1 ? this->absolutePosition = 0 : this->absolutePosition++;
      else if (this->direction == CCW)
        this->absolutePosition == 0 ? this->absolutePosition = this->spr-1 : this->absolutePosition--;
}

void CustomStepper::setTimer()
{
  this->timer = micros() + this->time;
}

void CustomStepper::run()
{
  if (this->timer < micros() && !this->done) /* Why?? && micros() < this->timer + this->time) */
    this->step();
}

boolean CustomStepper::isDone()
{
  return this->done;
}

boolean CustomStepper::readOpto()
{
  return digitalRead(this->pinOptoTran);
}

//Return state of class
void CustomStepper::dump()
{
    Serial.println("------------------");
    Serial.print("Pin1: ");
        Serial.println(this->pin1);  
    Serial.print(" stepsToGo: ");
	Serial.println(this->stepsToGo);
    Serial.print(" mode: ");
	Serial.println(this->mode);
    Serial.print(" direction: ");
	Serial.println(this->direction);
    Serial.print(" stepCorrection: ");
	Serial.println(this->stepCorrection);
    Serial.print(" angle: ");
	Serial.println(this->positionDegrees());
    Serial.print(" Time:");
        Serial.println(this->time);
    Serial.print(" Timer:");
        Serial.println(this->timer);   
    Serial.print(" Opto:");
        Serial.print(this->readOpto());
    Serial.print(" Done:");
        Serial.print(this->isDone());        
    Serial.println();
}
