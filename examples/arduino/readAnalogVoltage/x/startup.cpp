/*
 ===============================================================================
 Name        : startup.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : arduino startup
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/pin.h"

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
#endif

	// Start arduino object
	Arduino arduino;
	arduino.start("arduino", 250);

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
