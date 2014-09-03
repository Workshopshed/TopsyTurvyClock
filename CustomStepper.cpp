//Copyright 2012 Based on the work of Igor Campos
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

  //when a command is finished it the isDone will return true, it is important to notice that
  //you can't give one command just after another since they don't block the execution, 
  //which allows the program to control multiple motors
  
  //Run is very important and must be placed in your loop, it is this that makes the motor steps
  //when necessary

#include "CustomStepper.h"

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
      this->stepsGone++;
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
  
  if (this->stepsToGo - this->stepsGone > 0)
  {
    if (this->direction != STOP)
      this->dostep();
    else
      this->disable();
  }
  else if (this->mode == CONTINUOUS || this->mode == UNHOME || this->mode == HOME)
  {
    this->stepsGone = 0;
    this->step();
  }
  else if (this->mode == ROTATIONS)
  {
    this->stepsGone = 0;
    this->stepsToGo = this->spr + this->stepCorrection + 0.5; //Steps per rotation + the step correction + 0.5 to force round
    this->stepCorrection = this->spr + this->stepCorrection - this->stepsToGo; //adjust the step correction for the next steps
    if (++this->rotationsGone >= this->rotationsToGo)
      this->disable();
    else
      this->step();
  }
  else if (this->mode == STEPS)
    this->disable();
 
    
}

void CustomStepper::home()
{
  this->rotate();
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

void CustomStepper::rotate(unsigned int rotations)
{
  if (this->spr > 0 && this->rpm > 0)
  {
    if (rotations > 0)
      this->mode = ROTATIONS;
    else
      this->mode = CONTINUOUS;
      
    this->rotationsToGo = rotations;
    this->rotationsGone = 0;
    this->stepsToGo = this->spr + this->stepCorrection + 0.5; //Steps per rotation + the step correction + 0.5 to force round
    this->stepCorrection = this->spr + this->stepCorrection - this->stepsToGo; //adjust the step correction for the next steps
    this->stepsGone = 0;
    this->done = false;
    this->calcTime();
    this->step();
  }
}

void CustomStepper::rotateDegrees(float degrees)
{
  if (this->spr > 0 && this->rpm > 0 && degrees > 0)
  {
    this->mode = STEPS;
    this->stepsToGo = degrees/360.0*this->spr + this->stepCorrection + 0.5;
    this->stepCorrection = degrees/360.0*this->spr + this->stepCorrection - this->stepsToGo;
    this->stepsGone = 0;
    this->done = false;
    this->calcTime();
    this->step();
  }
}

void CustomStepper::rotateToDegrees(float degrees)
{
  //Workout current position and then call rotate
  //Note might need to change direction
  //Calculation might be exact opposite of that below.
}

float CustomStepper::positionDegrees()
{
  //Calculate the absolute angle based on the absolute position
  return this->absolutePosition;
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
    Serial.print(" stepsGone: ");
	Serial.println(this->stepsGone);
    Serial.print(" rotationsToGo: ");
	Serial.println(this->rotationsToGo);
    Serial.print(" rotationsGone: ");
	Serial.println(this->rotationsGone);
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
    Serial.print(" Micros:");
        Serial.println(micros());
    Serial.print(" Opto:");
        Serial.print(this->readOpto());
    Serial.print(" Done:");
        Serial.print(this->isDone());        
    Serial.println();
}
