/*
===============================================================================
 Name        : servo.cpp
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY 3.0
 Description : servo control class
 History	 :
===============================================================================
*/

#include "servo.h"

CServo::CServo() {
	// nothing
}

CServo::~CServo() {
	// nothing
}

void CServo::assign(PWM_CH_T ch) {
	CPwm::assign(ch);
	period(SERVO_PWM_PERIOD);
	start();	// start global PWM control
}

void CServo::position(float degrees) {
	if (degrees >= SERVO_MIN_DEG && degrees <= SERVO_MAX_DEG) {
		float pwm = SERVO_CENTER_PWM + (SERVO_PPD * degrees);
		pulseWidth(pwm);
	}
}
