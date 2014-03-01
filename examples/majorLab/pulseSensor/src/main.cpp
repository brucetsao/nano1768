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
#include "arduino.h"
#include "pulseSensor.h"
#include "class/pwm.h"

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
	CPulseSensor pulse(AD5, P21);
	pulse.start();

	CPwm beatLED(PWM1);
	CPwm::period(0.02);	// 20

	beatLED.begin();
	CPwm::start();

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(20);

		// Your loop code here
#ifdef PulseB
		if ( pulse.B ) {
			dbg.println("HRV=%d", pulse.HRV);
			pulse.B = false;
		}

		if ( pulse.QS ) {
			dbg.println("BPM=%d", pulse.BPM);
			pulse.QS = false;
		}
#endif
		beatLED.dutyCycle(pulse.Fade / 255.0f);
		pulse.Fade -= 10;
		pulse.Fade = constrain(pulse.Fade, 0, 255);
	}
	return 0;
}
