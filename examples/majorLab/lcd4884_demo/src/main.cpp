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
#include "class/string.h"
#include "lcd/lcd4884.h"

//
// TODO: insert other definitions and declarations here
//
LCD4884 lcd;

#define MENU_X	10		// 0-83
#define MENU_Y	1		// 0-5
void init_MENU(void) {
	lcd.LCD_clear();
	lcd.LCD_write_string(MENU_X, MENU_Y, "test screen", LCD_HIGHLIGHT);
}

void setup() {
	lcd.LCD_init();
	lcd.LCD_clear();

	//menu initialization
	init_MENU();
}

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
	setup();

	// Test
	int counter = 0;
	CString str;

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;

		// do something (your loop code here)
		if (++counter < 1000) {
			str.printf("%d", counter);
			lcd.LCD_write_string(MENU_X, MENU_Y + 2, *str);
		} else {
			counter = 0;
			init_MENU();
		}
		sleep(10);
	}
	return 0;
}
