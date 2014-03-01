/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Copyright   : www.embeda.com.tw
 License	 : CC BY-SA 3.0
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
#include "lcd/tft_ili9325.h"

#include "embeda_bmp.h"

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
	TFT_ILI9325 tft;
	tft.assign(SPI0, UNO_10, UNO_9);
	tft.begin();

	// TFT-LCD basic test
	tft.clear();								// clear
	tft.bitmap( 0, 0, 320,  240, embeda_bmp);	// show embeda logo bmp

	while (1) {
		led = !led;
		sleep(200);
	}
	return 0;
}
