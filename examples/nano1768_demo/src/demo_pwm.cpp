/*
 ===============================================================================
 Name        : demo_pwm.cpp
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/2/1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : 
 History	 :
 ===============================================================================
 */

#include "class/thread.h"
#include "class/pwm.h"
#include "class/pin.h"

class CPwmDemo: public CThread {
protected:
	virtual void run() {
		int rp;
		CPwm RLED;
		CPwm BLED;

		CPin GLED(P18);
		GLED.output(NOT_OPEN);
		GLED = LED_OFF;

		RLED.frequency(KHZ(500));
		RLED.assign(PWM1);
		BLED.assign(PWM2);
		RLED.begin();
		BLED.begin();
		RLED.start();
		while(1) {
			for (rp=0; rp<100; rp++){
				RLED.dutyCycle(rp);
				BLED.dutyCycle(100-rp);
				sleep(10);
			}
		}
	}
};

