/*
 ===============================================================================
 Name        : porttimer.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/4
 License     : CC BY-SA 3.0
 Description : timer port for modbus
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/4	v1.0.0	First Edition									Jason
*/

#include "class/thread.h"
#include "class/timer.h"
#include "mb.h"
#include "mbport.h"

extern "C" {
typedef  BOOL (*TIMER_EXPIRED_FUNC)(void);
}

//
// defines
//

class mbTimer:public CThread {
public:
	mbTimer () {
		m_timer = NULL;
		m_expired_func = NULL;
	}

	~mbTimer() {
		if ( m_timer ) delete m_timer;
	}

	bool setting(uint32_t us, TIMER_EXPIRED_FUNC func) {
		TIMER_T t = CTimer::getUnused();
		if ( t!=TIMER_BUSY ) {
			m_timer = new CTimer(t);
			m_expired_func = func;
			m_timer->setting(1, us);
			m_timer->begin();
			return true;
		}
		return false;
	}

	void enable() {
		m_timer->begin();
	}

	void disable() {
		m_timer->end();
	}

	bool start() {
		// TODO: check stack size
		return CThread::start("mbTim", 96, PRI_HIGH);
	}

protected:
	CTimer *m_timer;
	TIMER_EXPIRED_FUNC m_expired_func;
	virtual void run() {
		while(1) {
			m_timer->wait();
			m_expired_func();
		}
	}
};

#define TIMER(x) (static_cast<mbTimer*>(x))		// Use C++ Object in C functions

//
// Static variables
//
static mbTimer	*xTimer = NULL;

//
// Start implementation
//
PR_BEGIN_EXTERN_C

BOOL xMBPortTimersInit( USHORT usTim1Timerout50us ) {
	xTimer = new mbTimer;
	if ( xTimer ) {
		if ( TIMER(xTimer)->setting(usTim1Timerout50us, pxMBPortCBTimerExpired) ) {
			return TIMER(xTimer)->start();
		}
	}
	return false;
}

void vMBPortTimersEnable() {
	TIMER(xTimer)->enable();
}

void vMBPortTimersDisable() {
	TIMER(xTimer)->disable();
}

PR_END_EXTERN_C
