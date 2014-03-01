/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.2
 Date		 : 2012/10/28
 Copyright   : 2012 (C) Embeda International Inc.
 License	 : MIT
 Description : retrieve the Facebook information by sslSocket
 History	 :
 2012/11/12	v1.0.0 First Edition.
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
#include "class/json.h"
#include "class/sslSocket.h"
#include "net/http/httpclient.h"
#include "led/seven_seg_ledx4.h"

//
// TODO: insert other definitions and declarations here
//
#define fb_host			"graph.facebook.com"
#define fb_port			443		// SSL (HTTPS)

#ifdef _WESTIN_TAIPEI_
#define fb_query		"/140101032725916"		// 六褔皇宮
#define fb_field		"were_here_count"		// 打卡數
#endif

#ifdef _EMBEDA_INC_
#define fb_query		"/301127589948171"		// 英倍達
#define fb_field		"likes"					// 讚數
#endif

class fbQuery: public CThread {
public:
	int m_value;

protected:
	virtual void run() {
		int i;
		CPin led(LED2);
		sslSocket sock;
		HttpClient fb(sock, fb_host, fb_port);
		while(1) {
			led = LED_ON;
			if ( fb.request(fb_query) ) {
				led = LED_OFF;
				i = fb.response().find(CRLF CRLF);

				if ( i>0 ) {
					CJson json(fb.response().getBuffer()+i+4); // parse by CJson class
					if ( json.error()==ERR_OK ) {
						m_value = json[fb_field].asInteger();
						dbg.println(m_value);
					} else {
						dbg.println(json.errorDesc());
					}
				}
			}
			sleep(2000);
		}
	}
};

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

	SevenLEDx4 leds(UNO_0, UNO_1, UNO_2, UNO_3, UNO_4, UNO_5, UNO_6, UNO_7, UNO_8, UNO_9, UNO_10, UNO_11);
	leds.clear();
	leds.start();

	for (int i=0; i<16; i++) {
		leds[0] = i;
		leds[1] = i;
		leds[2] = i;
		leds[3] = i;
		sleep(500);
	} // */

	// TODO: insert setup code here
	fbQuery q;
	q.start("ssl", 1024);
	q.m_value = 0;

	int 	i, exp, lastValue = 0xFFFFFFFF;
	uint8_t v[8];

	while (1) {
		led = !led;

		if ( q.m_value!=lastValue ) {
			lastValue = q.m_value;

			for (i=8, exp=0; i>0; i--) {
				v[i-1] = ((int)(lastValue / pow(10.0f, i-1))) % 10;		// 計算個字節的值
				if ( exp==0 && v[i-1]>0 ) exp = i;						// 記錄最大位數
			}
			leds.clear();
			for (i=1; i<=exp; i++) {									// 滾動字幕
				leds[0] = v[exp-i];
				if (i>1) leds[1] = v[exp-i+1];
				if (i>2) leds[2] = v[exp-i+2];
				if (i>3) leds[3] = v[exp-i+3];
				sleep(500);
			}
		} // */
		sleep(500);
	}
	return 0;
}
