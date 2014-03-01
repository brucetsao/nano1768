/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 : 2012/12/19
 Copyright   :
 License	 :
 Description : WebSocket + USM for Mobile Demo App
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
#include "probe/probe.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "sensor/hcsr04.h"
#include "class/websocket.h"
#include "class/bus.h"


//
// TODO: insert other definitions and declarations here
//
#define WSS_INDEX_DISTANCE	0

HCSR04 hcsr(UNO_2, UNO_3);
CBus leds(LED2, LED3, LED4, END);	// DEBUG LED

//
//
//
class UsmWebSocketService: public CWebSocketService, CThread {
protected:
	bool m_bAuto;

	virtual void run() {
		CString msg;
		m_bAuto = false;

		while(m_ws->isConnected()) {
			if (hcsr.m_distance>0 ) {
				// make JSON result string
				msg = "{\"result\":";
				msg += hcsr.m_distance;
				msg += "}";
				// sent JSON result to browser via WebSocket
				m_ws->send(msg);
			}

			if ( m_bAuto )
				 sleep(500);
			else this->suspend();		// if not auto, suspend the thread
		}
	}

	virtual void onOpen() {
		start("wsADC", 180, PRI_HIGH);	// start the thread on the open event
	}

	virtual void onText(CString &msg) {
		switch(msg[0]) {
		case 'a': // automatic
			leds[1] = !leds[1];
			m_bAuto = true;
			break;
		case 't': // trigger
			leds[2] = !leds[2];
			m_bAuto = false;
			break;
		}
		this->resume();
	}
};

//
//
//
class UsmWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch (index) {
		case WSS_INDEX_DISTANCE:
			leds[0] = !leds[0];
			UsmWebSocketService *wss = new UsmWebSocketService;
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
	probe.start();
#endif

	// Simple demo Code (removable)
	CPin led(LED1);

	// TODO: insert setup code here
	UsmWebSocketServer server;
	server.add("/distance", WSS_INDEX_DISTANCE);	// add service name = /

	hcsr.start();
	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
