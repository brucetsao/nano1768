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
#ifndef SHT1x_h
#define SHT1x_h

/*
 #if (ARDUINO >= 100)
 #include <Arduino.h>
 #else
 #include <WProgram.h>
 #endif // */
#include "class/pin.h"

class SHT1x {
public:
	SHT1x(int dataPin, int clockPin);
	void update();
	float readHumidity();
	float readTemperatureC();
	float readTemperatureF();

protected:
	int _dataPin;
	int _clockPin;

	int	 tempRaw;
	int	 humiRaw;

	int readTemperatureRaw();
	int readHumidityRaw();

//	int shiftIn(int _numBits);
	void sendCommandSHT(int _commandn);
	void waitForResultSHT();
	int getData16SHT();
	void skipCrcSHT();
};

#endif
