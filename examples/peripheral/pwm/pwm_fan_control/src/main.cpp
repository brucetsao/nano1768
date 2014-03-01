/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/8/4
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : main definition
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/8/4	v1.0.0	First Edition									Jason
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
#include "class/websocket.h"
#include "class/json.h"
#include "fan.h"

//
// Global Fan Control Object
//
CFan fan(PWM6, D8);

//
// Fan WebSocket Service
//
class FanWebSocketService: public CWebSocketService, public CThread {
protected:
	//
	// override the thread::run()
	//
	virtual void run() {
		CString msg;
		while(m_ws->isConnected()) {
			//
			// Make JSON string
			//
			msg = "{";
			msg += "\"rpm\":";
			msg += fan.rpm();
			msg += "}";

			//
			// send to WebSocket
			//
			m_ws->send(msg);

			sleep(500);
		}
	}

	//
	// override the CWebSocketService::onOpen() Event
	//
	virtual void onOpen() {
		start("wsFan");		// start the thread
	}

	//
	// override the CWebSocketService::onText() Event
	//
	virtual void onText(CString &str) {
		CJson msg((LPTSTR)str.getBuffer());
		if ( msg["command"]== CJson::JSON_INT ) {
			switch( msg["command"].asInteger() ) {
			case 0:	// PWM
				fan.write(msg["value"].asInteger()/100.0f);
				break;
			default:
				break;
			}
		}
	}
};

//
// Fan WebSocket Server
//
#define WSS_INDEX_FAN		0

class FanWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch (index) {
		case WSS_INDEX_FAN:
			FanWebSocketService *wss = new FanWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
	}
}; // */

//
// TODO: insert other definitions and declarations here
//
#define FAN_PWM_FREQ	KHZ(25)

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
	led = LED_ON;

	//
	// WebSocket
	//
	FanWebSocketServer server;
	server.add("/fan", WSS_INDEX_FAN);	// add service */

	//
	// start globel PWM
	//
	CPwm::frequency(FAN_PWM_FREQ);
	CPwm::start(); 					// start global PWM control

	//
	// start fan control object
	//
	fan.start("FAN");
	fan = 0.5;	// 50%

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
		dbg.printf("%d\n", fan.rpm());
	}
	return 0;
}
