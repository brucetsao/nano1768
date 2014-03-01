/*
 ===============================================================================
 Name        : timer.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/8/31
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/1/31	v1.0.0	First Edition									Jason
 2012/9/20	v1.0.1	Modify getUnused return for TIMER_T				Jason
 ===============================================================================
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "class/peripheral.h"

typedef enum {
	TIMER_BUSY = -1,
	TIMER0 = 0,
	TIMER1 = 1,
	TIMER2 = 2,
	TIMER3 = 3
}TIMER_T;

class CTimer: public CPeripheral {
public:
	CTimer(TIMER_T t);
	virtual ~CTimer();

	void setting(uint32_t prescale, uint32_t match, bool autoLoad=true);

	void enable();	// start
	void disable();	// stop

	inline void begin() {
		enable();
	}

	inline void end() {
		disable();
	}

	void reset();	// restart & recount
	bool wait(uint32_t timeout=MAX_DELAY_TIME);
	void update(uint32_t match);

	static TIMER_T getUnused();	// get Unused timer, if all in used, return -1

private:
	TIMER_T m_timer;
};

#endif /* TIMER_H_ */
