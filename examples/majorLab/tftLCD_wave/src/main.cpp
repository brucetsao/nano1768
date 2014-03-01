/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/5/26
 Copyright   : www.embeda.com.tw
 License	 : CC BY-SA 3.0
 Description : main definition
 History	 :
 	 2012/5/27, add PWM output for wave show
 ===============================================================================
 */
#include "uCXpresso.h"
#include "class/pin.h"

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

#include "class/adc.h"
#include "class/pwm.h"
#include "class/thread.h"
#include "class/timeout.h"

#include "debug.h"
CDebug dbg(DBG_USB);

//
// TODO: insert other definitions and declarations here
//
#define WAVE_START_X		5
#define WAVE_START_Y		5
#define WAVE_WINDOW_WIDTH	120
#define WAVE_WINDOW_HEIGHT	120
#define WAVE_DOT_COLOR		0xFFFF	//16 bit color
#define WAVE_PERIOD_TIME	5

class CShowWave: public CThread {
protected:
	virtual void run() {
		//
		// Setup TFT-LCD
		//
		TFT_ILI9325 tft;
		tft.assign(SPI0, UNO_10, UNO_9);
		tft.begin();

		//
		// TFT-LCD basic test
		//
		tft.backlight(LED_OFF);
		tft.clear(); // clear
		tft.bitmap(0, 0, 320, 240, embeda_bmp); // show embeda logo bmp
		tft.backlight(LED_ON);

		//
		// Show ADC wave
		//
		CAdc adc(UNO_A0); // using the A5 of UNO.Net
		adc.begin();
		uint16_t *waveLine = new uint16_t[WAVE_WINDOW_HEIGHT];
		int i, V, lastV = 0, x = 0;
		CTimeout to;

		while (1) {
			to.reset();

			// read AD value in WAVE_WINDOW_HEIGHT range
			V = (adc.read() * WAVE_WINDOW_HEIGHT) / (MAX_ADC_VALUE+1);
			if ( V>=WAVE_WINDOW_HEIGHT ) V = WAVE_WINDOW_HEIGHT-1;

			// clear line (fill black color)
			memset(waveLine, 0, WAVE_WINDOW_HEIGHT * 2);

			// draw a line in waveLine bitmap buffer
			if (lastV >= V) {
				for (i = V; i <= lastV; i++) {
					waveLine[i] = WAVE_DOT_COLOR;
				}
			} else {
				for (i = lastV; i <= V; i++) {
					waveLine[i] = WAVE_DOT_COLOR;
				}
			}

			// send the wave line bitmap
			tft.bitmap(WAVE_START_X + x, WAVE_START_Y, 1, WAVE_WINDOW_HEIGHT,
					waveLine);

			// to next position
			x = (x + 1) < WAVE_WINDOW_WIDTH ? x + 1 : 0;
			lastV = V;

			to.wait(WAVE_PERIOD_TIME);
		}
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

	// Simple demo Code (removable)
	CPin led(LED1);

	//
	// TFT Demo
	//
	CShowWave wave;
	wave.start("wave", 128, PRI_HIGH); // task name="wave", default stack size=128

	//
	// PWM output for wave show
	//
	CPwm::period(0.05); // 50ms
	CPwm::start();

	CPwm pwm(PWM1);
	pwm.dutyCycle(0.3); // 10%
	pwm.begin();

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// do something (your loop code here)
	}
	return 0;
}
