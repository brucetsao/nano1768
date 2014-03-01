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

#include "class/semaphore.h"
#include "class/thread.h"
#include "class/pwm.h"

typedef struct _servo_cb {
	float total_degrees;	// SERVO_TOTAL_DEGREES
	float max_degrees;		// SERVO_MAX_DEGREES
	float min_degrees;		// SERVO_MIN_DEGREES
	float max_pwm;			// SERVO_MAX_PWM
	float min_pwm;			// SERVO_MIN_PWM
	float center_pwm;		// SERVO_CENTER_PWM
	float speed;			// SERVO_SPEED (ms)
} SERVO_SPEC_T;

extern const SERVO_SPEC_T SERVO_RB_421;
extern const SERVO_SPEC_T SERVO_S03_STD;
extern const SERVO_SPEC_T SERVO_MG_995;

//
// CServo Class
//
class CServo: public CThread {
public:
	CServo();
	CServo(PWM_CH_T ch, const SERVO_SPEC_T *spec);
	virtual ~CServo();
	virtual void assign(PWM_CH_T ch, const SERVO_SPEC_T *spec);
	virtual void begin();
	virtual void end();

	virtual void position(float degrees);

	inline virtual void operator =(float degrees) {
		position(degrees);
	}

	// short-hand to get Servo Contents (specifications)
	inline SERVO_SPEC_T* operator ->() {
		return m_spec;
	}

	bool isRunning();
	bool isBusy();

protected:
	CPwm servo;
	SERVO_SPEC_T *m_spec; // Servo Motor Specifications
	float newDeg, lastDeg;
	uint32_t flag;
	CSemaphore sem;
	virtual void run();
};

#endif /* SERVO_H_ */
