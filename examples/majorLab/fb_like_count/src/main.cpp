/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/10/17
 Copyright   : 2012 (C) Embeda International Inc.
 License	 : MIT
 Description : retrieve the Facebook post status
 History	 :
 2012/10/17	v1.0.0 First Edition.
 2012/10/23 v1.0.1 Simplify
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
#include "net/http/httpclient.h"
#include "led/seven_seg_ledx4.h"

//
// TODO: insert other definitions and declarations here
//
#define fb_api_host	"api.facebook.com"
#define fb_like_api	"/restserver.php?method=links.getStats&urls="
#define like_url 	"http://www.embeda.com.tw/tw/?p=4314"

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
	CSocket sock;
	HttpClient fb(sock, fb_api_host);

	SevenLEDx4 leds(UNO_0, UNO_1, UNO_2, UNO_3, UNO_4, UNO_5, UNO_6, UNO_7, UNO_8, UNO_9, UNO_10, UNO_11);
	leds.clear();
	leds.start();

	int i,f;
	// Enter an endless loop
	while (1) {
		if ( fb.request(fb_like_api like_url) ) {
			led = !led;
			f = fb.response().find("</total_count>");
			if ( f>4 ) {
				leds.clear();
				for (i=1; i<=4; i++) {
					if ( fb.response()[f-i]>='0' && fb.response()[f-i]<='9' ) {
						leds[i-1] = fb.response()[f-i] - '0';
					} else break;
				}
			}
		}

		// Simple demo Code (removable)
		sleep(3000);
	}
	return 0;
}
