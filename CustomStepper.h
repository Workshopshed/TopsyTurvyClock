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

#ifndef CUSTOMSTEPPER_H
#define CUSTOMSTEPPER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

enum motormode {CONTINUOUS, ROTATIONS, STEPS , HOME, UNHOME};
enum motordir  {STOP, CW, CCW};

class CustomStepper
{ 
  private:
    boolean done;
    byte pin1;
    byte pin2;
    byte pin3;
    byte pin4;
    byte pinOptoLED;
    byte pinOptoTran;
    byte noSteps;
    byte steps[8];
    byte nextStep;
    motordir direction;
    motormode mode;
    unsigned int stepsToGo;
    unsigned int stepsGone;
    unsigned int rotationsToGo;
    unsigned int rotationsGone;
    int absolutePosition;
    void calcAbsolutePosition();
    float stepCorrection;
    float rpm;
    float spr;
    float time;
    unsigned long timer;
    void setTimer();
    void calcTime();
    void disable();
    void step();
    void dostep();
    void setupOpto();
    void setupMotor();
    boolean readOpto();
    void unhome();    
  public:
    CustomStepper(byte pin1, byte pin2, byte pin3, byte pin4, byte pinOptpLED, byte pinOptoTran, byte steps[] = (byte[]){8, B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001}, float spr = 4075.7728395, float rpm = 12, motordir direction = CW);
    void setRPM(float RPM);
    void setSPR(float SPR);
    void setDirection(motordir direction = CW);
    void rotate(unsigned int rotations = 0);
    void rotateDegrees(float degrees);
    void rotateToDegrees(float degrees);
    float positionDegrees();
    void home();
    void run();
    boolean isDone();
    void dump();
};

#endif
