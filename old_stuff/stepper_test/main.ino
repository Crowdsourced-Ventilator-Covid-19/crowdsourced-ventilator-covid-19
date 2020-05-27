#include <Arduino.h>
#define ARDUINO 10812
#include "AccelStepper.h"

#define STEP_DIR 32
#define STEP_STEP 13
#define STEP_EN 2


int Distance = 0;

void setup() {               
  pinMode(STEP_DIR, OUTPUT);    
  pinMode(STEP_STEP, OUTPUT);
  pinMode(STEP_EN, OUTPUT);
  digitalWrite(STEP_DIR, LOW);
  digitalWrite(STEP_STEP, LOW);
  digitalWrite(STEP_EN, LOW);
}

void loop() {
  digitalWrite(STEP_STEP, HIGH);
  delayMicroseconds(2000);         
  digitalWrite(STEP_STEP, LOW);
  // delayMicroseconds(1000);
  Distance = Distance + 1;   // record this step
 
  // Check to see if we are at the end of our move
  if (Distance == 120)
  {
    // We are! Reverse direction (invert DIR signal)
    if (digitalRead(STEP_DIR) == LOW)
    {
      digitalWrite(STEP_DIR, HIGH);
    }
    else
    {
      digitalWrite(STEP_DIR, LOW);
    }
    // Reset our distance back to zero since we're
    // starting a new move
    Distance = 0;
    // Now pause for half a second
    delay(500);
  }
}

/*
AccelStepper stepper(AccelStepper::DRIVER, STEP_STEP, STEP_DIR);
int pos = 65;

void setup()
{ 
    stepper.setEnablePin(STEP_EN);
    stepper.setPinsInverted(false,false,true);
    stepper.setMaxSpeed(9000);
    stepper.setAcceleration(7000);
}

void loop()
{
    stepper.enableOutputs();
  if (stepper.distanceToGo() == 0)
  {
    delay(500);
    pos = -pos;
    stepper.moveTo(pos);
  }
  stepper.run();
}
*/