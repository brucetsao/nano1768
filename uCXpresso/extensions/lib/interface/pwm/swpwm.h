/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/1
 Copyright   : 2012 (C) Embeda Technology Inc. (www.embeda.com.tw)
 License	 : CC BY-SA 3.0
 Description : 2012 (C) Embeda Technology Inc.
 History	 :
 ===============================================================================
 */

#ifndef SWPWM_H_
#define SWPWM_H_

#include "class/thread.h"
#include "class/timer.h"
#include "class/list.h"
#include "class/pin.h"

class SWPwm: public CThread {
public:
	SWPwm(TIMER_T t, float accuracy=256.0f);	// default Accuracy=100 degree (for 0%~100%)
	virtual ~SWPwm();
	void period(float sec);						// Remark: minimum 0.002 = 2ms
	virtual bool start();						// start pwm
	void stop();

	int  add(PIN_NAME_T p, float dutyCycle=0.0f);
	bool update(int index, float dutyCycle);

protected:
	CTimer	 m_timer;
	CList	 m_lstCH;
	int		 m_cycle;
	float 	 m_accuracy;
	virtual void run();
};

#endif /* SWPWM_H_ */
