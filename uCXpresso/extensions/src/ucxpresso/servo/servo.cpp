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
#include "servo/servo.h"
#include "debug.h"
#include "arduino.h"

const SERVO_SPEC_T SERVO_S03_STD = { 120.0f, // SERVO_TOTAL_DEGREES
		60.0f, // SERVO_MAX_DEGREES
		-60.0f, // SERVO_MIN_DEGREES
		0.0021f, // SERVO_MAX_PWM
		0.0009f, // SERVO_MIN_PWM
		0.0015f, // SERVO_CENTER_PWM
		4.0 // SERVO_SPEED (ms)
		};

const SERVO_SPEC_T SERVO_RB_421 = { 180.0f, // SERVO_TOTAL_DEGREES
		90.0f, // SERVO_MAX_DEGREES
		-90.0f, // SERVO_MIN_DEGREES
		0.0024f, // SERVO_MAX_PWM
		0.0006f, // SERVO_MIN_PWM
		0.0015f, // SERVO_CENTER_PWM
		2.8 // SERVO_SPEED (ms)
		};

const SERVO_SPEC_T SERVO_MG_995 = { 120.0f, // SERVO_TOTAL_DEGREES
		60.0f, // SERVO_MAX_DEGREES
		-60.0f, // SERVO_MIN_DEGREES
		0.0021f, // SERVO_MAX_PWM
		0.0009f, // SERVO_MIN_PWM
		0.0015f, // SERVO_CENTER_PWM
		3.0 // SERVO_SPEED (ms)
		};

#define SERVO_FLAG_RUNNING	0
#define SERVO_FLAG_BUSY		1
#define SERVO_FLAG_SUSPEND	2

#define SERVO_D2P(d)			((d *(m_spec->max_pwm - m_spec->min_pwm))/m_spec->total_degrees)
#define	SERVO_PWM_PERIOD		0.02f	// servo motor period time = 20ms
CServo::CServo() {
	sem.binary();
	lastDeg = -1;
	newDeg = 0;
	flag = 0;
}

CServo::CServo(PWM_CH_T ch, const SERVO_SPEC_T *spec) {
	sem.binary();
	lastDeg = 0;
	newDeg = 0;
	flag = 0;
	assign(ch, spec);
}

CServo::~CServo() {
}

void CServo::assign(PWM_CH_T ch, const SERVO_SPEC_T *spec) {
	m_spec = (SERVO_SPEC_T *) spec;
	servo.assign(ch);
	CPwm::period(SERVO_PWM_PERIOD);
	CPwm::start(); // start global PWM control
}

void CServo::run() {
	float pw;
	uint32_t s;

	bit_set(flag, SERVO_FLAG_RUNNING);
	while (1) {
		sem.wait(); // wait for new degrees input.

		bit_set(flag, SERVO_FLAG_BUSY);

		// cal. new pulse width
		pw = m_spec->center_pwm + SERVO_D2P(newDeg);

		// output new pulse width
		servo = pw;

		// wait for new angle finish
		s = ABS(newDeg - lastDeg) * m_spec->speed;
		lastDeg = newDeg;

		sleep(s);
		bit_clr(flag, SERVO_FLAG_BUSY);
	}
}

void CServo::begin() {
	servo.begin();
	if (bit_chk(flag, SERVO_FLAG_RUNNING)) {
		resume();
		bit_clr(flag, SERVO_FLAG_SUSPEND);
	} else {
		start("Servo"); // start the Servo Task (stack=64)
	}
}

void CServo::end() {
	bit_set(flag, SERVO_FLAG_SUSPEND);
	suspend();
	servo.end();
}

void CServo::position(float degrees) {
	degrees = constrain(degrees, m_spec->min_degrees, m_spec->max_degrees);
	if ( degrees!=lastDeg ) {
		newDeg = degrees;
		sem.release(); // release new degrees
	}
}

bool CServo::isRunning() {
	return bit_chk(flag, SERVO_FLAG_RUNNING);
}

bool CServo::isBusy() {
	return bit_chk(flag, SERVO_FLAG_BUSY);
}
