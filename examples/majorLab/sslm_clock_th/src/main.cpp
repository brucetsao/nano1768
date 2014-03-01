/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/1/13
 Copyright   : Embeda Intarnational Inc.
 License	 : CC BY-SA 3.0
 Description : SSLM-K (Serial Seven-Segment LED Module) Demo
 History	 :
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
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "led/sslm-k.h"
#include "class/websocket.h"
#include "class/thread.h"
#include "class/json.h"
#include "class/bus.h"
#include "class/mailbox.h"

//
//	SSLM-K LED Module (6 digits Demo)
//
#define MAX_DIGITS	8
SSLM_K sslm(MAX_DIGITS, SPI0, UNO_10, PWM5, UNO_7);

int gnShowType;
#define SHOW_FOR_DATE	0
#define SHOW_FOR_TIME	1

int gnHourType;
#define SHOW_HOUR_24	0
#define SHOW_HOUR_12	1
#define SSLM_MailBox	"SSLM"

//
//
//
class mySSLMail: public CThread {
public:
	mySSLMail():m_mailBox(SSLM_MailBox, 8) {

	}

protected:
	CMailBox m_mailBox;

	virtual void run() {
		CString *str;
		while (1) {
			str = (CString *) m_mailBox.wait();
			if (str) {
				CJson json((LPTSTR) str->getBuffer());
				if (json["command"] == CJson::JSON_INT) {
					switch (json["command"].asInteger()) {
					case 0:	// toggle Date/Time
						gnShowType = json["value"].asInteger();
						if (gnShowType == SHOW_FOR_TIME) {
							// disable date dot
							sslm.dot(MAX_DIGITS-3, false);
							sslm.dot(MAX_DIGITS-5, false);

						} else {
							// enable date dot.
							sslm.dot(MAX_DIGITS-3, true);
							sslm.dot(MAX_DIGITS-5, true);
							// disable time dot
							sslm.dot(MAX_DIGITS-2, false);
							sslm.dot(MAX_DIGITS-4, false);
						}
						break;

					case 1:	// brightness
						sslm.brightness(json["value"].asInteger() / 100.0f);// to percentage
						break;

					case 2: // time format, hour type (24/12)
						gnHourType = json["value"].asInteger();
						break;
					}
				}
				delete str;	// free message
			}
		}
	}
};

//
//
//
class MyWebSocketService: public CWebSocketService {
protected:

	virtual void onClose() {
		delete this;
	}

	virtual void onError() {
		delete this;
	}

	virtual void onText(CString &msg) {
		CString *str = new CString(msg.getBuffer());	// alloc message
		CMailBox::post(SSLM_MailBox, str);				// post message to SSLM_MailBox
	}
};

//
//
//
#define WSS_INDEX_SSLM		0

class MyWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch (index) {
		case WSS_INDEX_SSLM:
			MyWebSocketService *wss = new MyWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
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

	//
	// SSLM Mail Receiver Thread
	//
	mySSLMail sm;
	sm.start("SSLMail", 180);

	//
	// Add WebSocket Server with service 'sslm-k-056b'
	//
	MyWebSocketServer server;
	server.add("/sslm-k-056b", WSS_INDEX_SSLM);

	//
	// set reversion LED module index
	//
	sslm.setting(MAX_DIGITS-2, sslmREVERSE);
	sslm.setting(MAX_DIGITS-4, sslmREVERSE);
	sslm.start();

	//
	// set default LED brightness
	//
	sslm.brightness(0.5);	// brightness=50%

	// LED Test
	for (int v = 0; v < 16; v++) {
		for (int i = 0; i < MAX_DIGITS; i++) {
			sslm.write(i, v);
			sslm.dot(i, (v & 0x01) ? true : false);
		}
		sslm.trigger();
		sleep(500);
	}
	sslm.clear();

	CTime tm;
	int v0, v1, v2;

	gnShowType = SHOW_FOR_TIME;
	gnHourType = SHOW_HOUR_24;

	// set dots : for time display
	sslm.dot(MAX_DIGITS-2, true);
	sslm.dot(MAX_DIGITS-3, true);
	sslm.dot(MAX_DIGITS-4, true);
	sslm.dot(MAX_DIGITS-5, true);

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(500);

		// update time
		tm = CTime::now();		// get system date/time

		if (gnShowType == SHOW_FOR_TIME) {
			// blink decimal point for Time Format
			if (led) {
				sslm.dot(MAX_DIGITS-2, true);
				sslm.dot(MAX_DIGITS-3, true);
				sslm.dot(MAX_DIGITS-4, true);
				sslm.dot(MAX_DIGITS-5, true);
			} else {
				sslm.dot(MAX_DIGITS-2, false);
				sslm.dot(MAX_DIGITS-3, false);
				sslm.dot(MAX_DIGITS-4, false);
				sslm.dot(MAX_DIGITS-5, false);
			}

			v0 = tm.getHour();
			if (gnHourType == SHOW_HOUR_12) {
				if (v0 > 12)
					v0 -= 12;
			}
			v1 = tm.getMinute();
			v2 = tm.getSecond();

		} else {
			//
			// Show Date Info
			//
			v0 = tm.getYear() % 100;
			v1 = tm.getMonth();
			v2 = tm.getDay();
		}

		// update sslm
		sslm.write(MAX_DIGITS-1, v2 % 10);
		sslm.write(MAX_DIGITS-2, v2 / 10);
		sslm.write(MAX_DIGITS-3, v1 % 10);
		sslm.write(MAX_DIGITS-4, v1 / 10);
		sslm.write(MAX_DIGITS-5, v0 % 10);
		sslm.write(MAX_DIGITS-6, v0 / 10);
		sslm.trigger();
	}
	return 0;
}
