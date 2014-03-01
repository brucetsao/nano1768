/*
 ===============================================================================
 Name        : SHT1x
 Modify      : Jason
 Version     : v1.0.0
 Date		 : 2012/6/25
 Description :
 History     :
 ===============================================================================
 */

/**
 * SHT1x Library
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au> / <www.practicalarduino.com>
 * Based on previous work by:
 *    Maurice Ribble: <www.glacialwanderer.com/hobbyrobotics/?p=5>
 *    Wayne ?: <ragingreality.blogspot.com/2008/01/ardunio-and-sht15.html>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature / humidity sensors from Sensirion (www.sensirion.com).
 */

/*
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif // */

#include "SHT1x.h"
#include "arduino.h"

SHT1x::SHT1x(int dataPin, int clockPin)
{
  _dataPin = dataPin;
  _clockPin = clockPin;
}

void SHT1x::update() {
	// Fetch raw value
	tempRaw = readTemperatureRaw();
	humiRaw = readHumidityRaw();
}

/* ================  Public methods ================ */

/**
 * Reads the current temperature in degrees Celsius
 */
float SHT1x::readTemperatureC()
{
  float _temperature;      // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;  // for 14 Bit @ 5V
  const float D2 =   0.01; // for 14 Bit DEGC

  // Convert raw value to degrees Celsius
  _temperature = (tempRaw * D2) + D1;

  return (_temperature);
}

/**
 * Reads the current temperature in degrees Fahrenheit
 */
float SHT1x::readTemperatureF()
{
  float _temperature;       // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;   // for 14 Bit @ 5V
  const float D2 =   0.018; // for 14 Bit DEGF

  // Convert raw value to degrees Fahrenheit
  _temperature = (tempRaw * D2) + D1;

  return (_temperature);
}

/**
 * Reads current temperature-corrected relative humidity
 */
float SHT1x::readHumidity()
{
  float _linearHumidity;       // Humidity with linear correction applied
  float _correctedHumidity;    // Temperature-corrected humidity
  float _temperature;          // Raw temperature value

  // Conversion coefficients from SHT15 datasheet
  const float C1 = -4.0;       // for 12 Bit
  const float C2 =  0.0405;    // for 12 Bit
  const float C3 = -0.0000028; // for 12 Bit
  const float T1 =  0.01;      // for 14 Bit @ 5V
  const float T2 =  0.00008;   // for 14 Bit @ 5V

  // Apply linear conversion to raw value
  _linearHumidity = C1 + C2 * humiRaw + C3 * humiRaw * humiRaw;

  // Get current temperature for humidity correction
  _temperature = readTemperatureC();

  // Correct humidity value for current temperature
  _correctedHumidity = (_temperature - 25.0 ) * (T1 + T2 * humiRaw) + _linearHumidity;

  return (_correctedHumidity);
}


/* ================  Private methods ================ */

/**
 * Reads the current raw temperature value
 */
int SHT1x::readHumidityRaw()
{
  int _val;

  // Command to send to the SHT1x to request Temperature
  int _gHumidCmd = 0b00000101;

  // Fetch the value from the sensor
  sendCommandSHT(_gHumidCmd);
  waitForResultSHT();
  _val = getData16SHT();
  skipCrcSHT();

  return (_val);
}

int SHT1x::readTemperatureRaw()
{
  int _val;

  // Command to send to the SHT1x to request Temperature
  int _gTempCmd  = 0b00000011;

  sendCommandSHT(_gTempCmd);
  waitForResultSHT();
  _val = getData16SHT();
  skipCrcSHT();

  return (_val);
}

#if 0
int SHT1x::shiftIn(int _numBits)
{
  int ret = 0;
  int i;

  for (i=0; i<_numBits; ++i)
  {
     digitalWrite(_clockPin, HIGH);
     sleep(10);  // I don't know why I need this, but without it I don't get my 8 lsb of temp
     ret = ret*2 + digitalRead(_dataPin);
     digitalWrite(_clockPin, LOW);
  }

  return(ret);
}
#endif

/**
 */
void SHT1x::sendCommandSHT(int _command)
{
  int ack;

  // Transmission Start
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clockPin, LOW);

  // The command (3 msb are address and must be 000, and last 5 bits are command)
  shiftOut(_dataPin, _clockPin, MSBFIRST, _command);

  // Verify we get the correct ack
  digitalWrite(_clockPin, HIGH);
  pinMode(_dataPin, INPUT);
  ack = digitalRead(_dataPin);
  if (ack != LOW) {
    //Serial.println("Ack Error 0");
  }
  digitalWrite(_clockPin, LOW);
  ack = digitalRead(_dataPin);
  if (ack != HIGH) {
    //Serial.println("Ack Error 1");
  }
}

/**
 */
void SHT1x::waitForResultSHT()
{
  int i;
  int ack;

  pinMode(_dataPin, INPUT);

  for(i= 0; i < 100; ++i)
  {
    sleep(10);
    ack = digitalRead(_dataPin);

    if (ack == LOW) {
      break;
    }
  }

  if (ack == HIGH) {
    //Serial.println("Ack Error 2"); // Can't do serial stuff here, need another way of reporting errors
  }
}

/**
 */
int SHT1x::getData16SHT()
{
  int val;

  // Get the most significant bits
  pinMode(_dataPin, INPUT);
  pinMode(_clockPin, OUTPUT);
  val = shiftIn(_dataPin, _clockPin, MSBFIRST);
  val *= 256;

  // Send the required ack
  pinMode(_dataPin, OUTPUT);
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);

  // Get the least significant bits
  pinMode(_dataPin, INPUT);
  val |= shiftIn(_dataPin, _clockPin, MSBFIRST);

  return val;
}

/**
 */
void SHT1x::skipCrcSHT()
{
  // Skip acknowledge to end trans (no CRC)
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);

  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
}
