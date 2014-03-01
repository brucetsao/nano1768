/*
 * dcmotor.h
 *
 *  Created on: 2012/8/3
 *      Author: Jason
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "class/pin.h"
#include "class/pwm.h"

typedef enum {
	FORWARD = HIGH,
	REVERSE = LOW
}MOTOR_ACTION_T;

class DCMotor: public CObject {
public:
	DCMotor(PWM_CH_T pwm, PIN_NAME_T dir);
	virtual ~DCMotor();

	// a short hand code for speed
	DCMotor&  operator = (float percent);

	// a short hand code for action
	DCMotor& operator = (MOTOR_ACTION_T action);

protected:
	CPwm	m_pwm;
	CPin	m_dir;
};

#endif /* DCMOTOR_H_ */
