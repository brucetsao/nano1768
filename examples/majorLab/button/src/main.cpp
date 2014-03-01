/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/pin.h"
#include "probe/probe.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "key/button.h"

//
// TODO: insert other definitions and declarations here
//
class MyButton: public CButton {
public:
	MyButton(PIN_NAME_T key, PIN_NAME_T led) : CButton(key), m_led(led) {
	}

protected:
	CPin m_led;
	virtual void onKeyDown() {
		m_led = LED_ON;
	}

	virtual void onKeyUp() {
		m_led = LED_OFF;
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

	// Simple demo Code (removable)
	CPin led(LED1);

	// TODO: insert setup code here
	MyButton btn1(UNO_2, LED2);
	MyButton btn2(UNO_3, LED3);

	btn1.start("btn1");
	btn2.start("btn2");

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here

	}
	return 0;
}
