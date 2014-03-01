/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
#include "class/pin.h"
#include "debug.h"

CDebug dbg(DBG_NET);
/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "arduino.h"
#include "class/thread.h"
#include "class/timer.h"
#include "class/timeout.h"
//
// CTimeDemo Class
//
class CTimerDemo: public CThread {
protected:
	virtual void run() {
		CTick tick;
		CTimer t(TIMER3);	// use TIMER1
		t.setting(10, 8);	// prescale=2us, match=10, 2x20=40us
		t.begin();			// start timer & interrupt
		while(1) {
			tick.reset();
			t.wait();		// wait for Interrupt Semaphore
			elapsed = tick.elapsed();
		}
	}
public:
	uint32_t elapsed;
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	// Simple demo Code (removable)
	CPin led(LED1);

	// Define CTimerDemo object
	CTimerDemo t;
	t.start("timer", 120, PRI_SUPER);	// start timer service (thread) with high priorities

	dbg.waitToDebugMode();
	dbg.breakpoint();
	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);
		if ( dbg.isAnyKey() ) {
			dbg.println(t.elapsed);
		}
	}
	return 0;
}
