/*
 ===============================================================================
 Name        : pulseSensor.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/6/29
 Copyright   :
 License	 : CC BY-SA 3.0
 Description : Hartbeat detect sensor (refer from PulseSensor.com)
 History	 :
 ===============================================================================
 */

#include "pulseSensor.h"
#include "class/timeout.h"
#include "debug.h"
#include "class/time.h"

CPulseSensor::CPulseSensor(ADC_CH_T ch, PIN_NAME_T led) :
	adPulse(ch), ledPulse(led) {

	Fade = 0;
	Senvitive = 0;
	Peek = 0;

	B = false;
	QS = false;
	HRVs = 0;
	hrvCount = 0;

	adPulse.begin();
	ledPulse.output(NOT_OPEN);
}

CPulseSensor::~CPulseSensor() {
}

void CPulseSensor::start(LPCTSTR name) {
	CThread::start(name, 128, PRI_HIGH);
}

#define PULSE_SENVITIVE 	15

void CPulseSensor::run() {
	CTick to;
	while (1) {
		to.reset();

		Signal = adPulse.read(0.1);				// read AD with low level filter

		if (Signal >= 2000 && Signal <= 2090) {
			if ( Fade==0 ) ledPulse = LOW;		// no finger detected
			goto takeBreak;
		}
//		DBG("Signal=%d\n", Signal);

		ledPulse = HIGH;							// finger present
		if (Pulse) {
			//
			// UP
			//
			if (Signal <= Peek) { // will go to down
				Senvitive++;
				if (Senvitive > PULSE_SENVITIVE) {
					Pulse = false; // going down
					Peek = MAX_ADC_VALUE;

					HRV = CTime::getTickCount() - lastTime;
					lastTime = CTime::getTickCount();
					B = true;

					HRVs += HRV;
					hrvCount++;
					if (hrvCount == 10) {
						BPM = 600000 / HRVs;
						HRVs = 0;
						hrvCount = 0;
						QS = true;
					}
				}
			} else {
				Peek = Signal;
				Senvitive = 0;
			}
		} else { //if ( Fade==0 ) {
			//
			// DOWN
			//
			if (Signal >= Peek) { // will go to up
				Senvitive++;
				if (Senvitive > PULSE_SENVITIVE) {
					Pulse = true; // going up
					Peek = 0;
					Fade = 255;		// set Pulse LED PWM value
				}
			} else {
				Peek = Signal;
				Senvitive = 0;
			}
		}

		takeBreak: to.delay(10);
	}
}
