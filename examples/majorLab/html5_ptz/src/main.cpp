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
#include "servo/servo.h"
#include "ajax_ptz.cpp"
#include "ajax_sys.cpp"

//
// TODO: insert other definitions and declarations here
//
CServo servoX(PWM5, &SERVO_MG_995);
CServo servoY(PWM6, &SERVO_MG_995);


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
	servoX.begin();
	servoY.begin();

	// Register a Ajax for PTZ
	CAjaxPTZ ptz;
	CAjaxSYS sys;

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
