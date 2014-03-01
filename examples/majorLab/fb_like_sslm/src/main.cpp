/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/1/2
 Copyright   : 2012 (C) Embeda International Inc.
 License	 : MIT
 Description : retrieve the Facebook information by sslSocket,
			   and show on the SSLM-K-056B Display Module.
 History	 :
 2013/1/2	v1.0.0 First Edition.
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
#include <cctype>
#include "class/string.h"
#include "class/json.h"
#include "net/http/httpclient.h"
#include "class/sslSocket.h"
#include "led/sslm-k.h"


//
//	SSLM-K LED Module (6 digits Demo)
//
#define MAX_DIGITS	8
SSLM_K sslm(MAX_DIGITS, SPI0, UNO_10, PWM5, UNO_7);

//
//
//
void animation() {
	for (int d=0; d<6; d++) {
		for (int i=0; i<MAX_DIGITS; i++) {
			sslm.symbol(i, 1<<d);
		}
		sslm.trigger();
		sleep(80);
	}
}

//
// TODO: insert other definitions and declarations here
//
#define fb_host			"graph.facebook.com"
#define fb_port			443		// SSL (HTTPS)

#ifdef _EMBEDA_INC_
#define fb_id			"/301127589948171"		// Embeda INC.
#define fb_field		"likes"					// Field for 'likes'
#endif

#ifdef _LA_LAKERS_
#define fb_id			"/losangeleslakers"		// LA Lakers Facebook ID
#define fb_field		"likes"					// Field for 'likes'
#endif


class fbQuery: public CThread {
public:
//	int m_value;

protected:
	virtual void run() {
		int i, pos, m_lastValue = 0;
		LPCTSTR	raw;

		CPin led(LED2);	// for Debug
		sslSocket sock;
		HttpClient fb(sock, fb_host, fb_port);

		while(1) {
			led = LED_ON;
			if ( fb.request(fb_id) ) {
				led = LED_OFF;
				i = fb.response().find(CRLF CRLF);				// skip HTML Header
				if ( i>0 ) {
					CJson json(fb.response().getBuffer()+i+4);	// parse by CJson class
					if ( json.error()==ERR_OK && json[fb_field].asInteger()!=m_lastValue ) {
						//m_value = json[fb_field].asInteger();
						raw = json[fb_field].raw();
						pos = MAX_DIGITS - json[fb_field].raw_length();

						animation();
						sslm.clear();

						while( isdigit(*raw) ) {
							sslm.write(pos++, (*raw++)-0x30);
						}
						sslm.trigger();
						m_lastValue = json[fb_field].asInteger();
					}
				}
			}
			sleep(1000);
		}
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

//#ifdef DEBUG
	dbg.start();
//#endif

	// Simple demo Code (removable)
	CPin led(LED1);

	//
	// set reversion LED module index
	//
	sslm.setting(MAX_DIGITS-2, sslmREVERSE);
	sslm.setting(MAX_DIGITS-4, sslmREVERSE);
	sslm.start();

	//
	// set default LED brightness
	//
	sslm.brightness(0.1);	// brightness=10%

	// LED Test
	animation();
	sslm.clear();

	//
	// Facebook query Task
	//
	fbQuery fb;
	fb.start("ssl", 650);

	while (1) {
		led = !led;
		sleep(500);
	}
	return 0;
}
