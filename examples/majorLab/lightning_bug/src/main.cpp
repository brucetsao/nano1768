/*
 ===============================================================================
 Name        : main.cpp
 Author      :
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

#include <math.h>
#include "interface/pwm/swpwm.h"


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

	// TODO: insert setup code here
	SWPwm pwm(TIMER0, 180);			// accuracy = 180 = 360 degree / 2 (step)
	pwm.period(0.005);				// set period time = 5ms for all channels
	pwm.add(LED1, 0.0);				// add PIN LED1 to channel 0
	pwm.add(LED2, 0.0);				// add PIN LED2 to channel 1
	pwm.add(LED3, 0.0);				// add PIN LED3	to channel 2
	pwm.add(LED4, 0.0);				// add PIN LED4 to channel 3
	pwm.start();

	float y;
	int	x[4] = {0, 15, 30, 45};		// initialize all channels x degree

	// Enter an endless loop
	while (1) {
		for (int i=0; i<4; i++) {
			x[i] = (x[i]+2) % 360;							// degree 0~360, step by 2
			y = sin((x[i] * M_PI)/180.0);					// y = sine @x
			pwm.update(i, map(y, -1.0f, 1.0f, 0.0f, 1.0f));	// update the duty-cycle of channel
		}
		sleep(8);	// speed
	}
	return 0;
}
