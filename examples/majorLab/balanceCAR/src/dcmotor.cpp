/*
 * dcmotor.cpp
 *
 *  Created on: 2012/8/3
 *      Author: 廖正清
 */

#include "dcmotor.h"

DCMotor::DCMotor(PWM_CH_T pwm, PIN_NAME_T dir) : m_pwm(pwm), m_dir(dir)
{
	CPwm::period(0.01);
	CPwm::start();
	m_pwm = 0;
	m_pwm.begin();

	m_dir.output(NOT_OPEN);
	m_dir = HIGH;
}

DCMotor::~DCMotor() {
	// TODO Auto-generated destructor stub
}

DCMotor& DCMotor::operator = (float percent) {
	m_pwm.write(percent);
	return *this;
}

DCMotor& DCMotor::operator = (MOTOR_ACTION_T action) {
	m_dir = (PIN_LEVEL_T)action;
	return *this;
}
