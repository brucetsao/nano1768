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
#include "arduino.h"
#include "class/pin.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "pwm/swpwm.h"

//
// TODO: insert other definitions and declarations here
//


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

	// TODO: insert setup code here
	SWPwm pwm(TIMER0, 240);
	pwm.period(0.02);		// 20ms
	pwm.add(UNO_0, 0.75);	// ch 0, dutycycle = 75%
	pwm.add(UNO_1, 0.25);	// ch 1, dutycycle = 25%
	pwm.add(UNO_2, 0.90);	// ch 2, dutycycle = 90%
	pwm.start();

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
