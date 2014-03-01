/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : arduino main
 History	 :
 ===============================================================================*/
#include "ucxpresso.h"
#include "arduino.h"
/*===============================================================================*/
int pin = 13;
volatile int state = LOW;

void blink()
{
  state = !state;
}

void setup()
{
  pinMode(pin, OUTPUT);
  attachInterrupt(0, blink, FALLING );
}

void loop()
{
  digitalWrite(pin, state);
}
