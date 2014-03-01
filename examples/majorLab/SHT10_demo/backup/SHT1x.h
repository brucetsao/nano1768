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
#ifndef SHT1x_h
#define SHT1x_h

#include "class/pin.h"

class SHT1x: public CPeripheral
{
  public:
    SHT1x(PIN_NAME_T dataPin, PIN_NAME_T clockPin);
    float readHumidity();
    float readTemperatureC();
    float readTemperatureF();

  private:
    CPin _dataPin;
    CPin _clockPin;
    float readTemperatureRaw();
//    int shiftIn(int _dataPin, int _clockPin, int _numBits);
    void sendCommandSHT(int _command);
    void waitForResultSHT();
    int getData16SHT();
    void skipCrcSHT();
};

#endif
