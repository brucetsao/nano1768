/*
===============================================================================
 Name        : servo.h
 Author      : Jason
 Version     : 1.0.1
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY 3.0
 Description : servo control class
 History	 :
===============================================================================
*/

#ifndef SERVO_H_
#define SERVO_H_

#include "class/pwm.h"

//
// define the specifications of servo motor here
//
#define SERVO_MAX_DEG	 	60.0f
#define SERVO_MIN_DEG	 	-60.0f
#define SERVO_MAX_PWM		0.0021f
#define SERVO_MIN_PWM		0.0009f
#define SERVO_CENTER_PWM	0.0015f
#define SERVO_PWM_PERIOD	0.02f	// 20ms


#define SERVO_DEGREES_RANGE (SERVO_MAX_DEG-SERVO_MIN_DEG)
#define SERVO_PPD			((SERVO_MAX_PWM-SERVO_MIN_PWM)/(SERVO_MAX_DEG-SERVO_MIN_DEG))

//
// CServo Class
//
class CServo: public CPwm {
public:
	CServo();
	CServo(PWM_CH_T ch) {
		assign(ch);
	}
	virtual ~CServo();

	virtual void assign(PWM_CH_T ch);
	virtual void position(float degrees);

	inline virtual void operator = (float degrees) {
		position(degrees);
	}
};

#endif /* SERVO_H_ */
