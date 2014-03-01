/*
 ===============================================================================
 Name        : startup.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/5
 Copyright   : 2012 (C) Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : arduino startup
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/pin.h"
#include "probe/probe.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

/* ==============================================
 Arduino Class
 ============================================== */
extern void setup();
extern void loop();

class Arduino: public CThread {
protected:
	virtual void run() {
		setup();		// call user's setup code
		while(1) {
			loop();		// call user's loop code
		}
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
	probe.start();
#endif

	// Start arduino object
	Arduino arduino;
	arduino.start("arduino", 256);

	// Simple demo Code (removable)
	CPin led(LED1);	// on board LED

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);
	}
	return 0;
}
