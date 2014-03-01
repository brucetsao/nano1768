/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Copyright   :
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
static uint8_t pool[24000];

//
// TODO: insert other include files here
//
#include "lcd/txtLcd.h"
#include "arduino.h"

#include "class/console.h"
#include "class/timeout.h"
#include "class/time.h"

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

	CTxtLcd lcd(UNO_8, UNO_9, UNO_10, UNO_0, UNO_1, UNO_2, UNO_3, UNO_4, UNO_5, UNO_6, UNO_7);
	lcd.begin();

//	lcd.cursor();
//	lcd.blink();

	Console con(lcd);
	con << "Hello World" << endl;

	CTick to;
	CTime tm;

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		if ( to.isExpired(1000) ) {
			to.reset();

			tm = CTime::now();
			lcd.setCursor(0,1);
			con.printf("%02d/%02d %02d:%02d:%02d",
					tm.getMonth(),
					tm.getDay(),
					tm.getHour(),
					tm.getMinute(),
					tm.getSecond()
			);
		}
	}
	return 0;
}
