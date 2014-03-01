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
#include "arduino.h"
#include "class/pin.h"
#include "debug.h"

CDebug dbg(DBG_NET);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "class/adc.h"
#include "class/websocket.h"
#include "class/timer.h"
#include "class/string.h"
#include "class/bus.h"

//
// TODO: insert other definitions and declarations here
//
#define WSS_INDEX_ADC	  0
#define MAX_AD_SAMPLES	100

CBus leds(LED1, LED2, LED3, LED4, END);

class CAdWebSocketService: public CWebSocketService, CThread {
protected:
	virtual void run() {
		TIMER_T t = (TIMER_T) CTimer::getUnused();
		if ( t!=TIMER_BUSY ) {
			leds[t] = LED_ON;
			CAdc   ad(AD0);
			CTimer to(t);
			CString msg;
			to.setting(10, 100); 	// interval 1ms
			to.begin();
			ad.begin();
			while(m_ws->isConnected()) {
				// make JSON value (array) string
				msg = "{\"value\":[";
				for (int i=0; i<MAX_AD_SAMPLES; i++) {
					to.reset();
					msg += ad.read();		// add A/D value to string
					if ( i<(MAX_AD_SAMPLES-1) ) msg += ",";
					to.wait();
				}
				msg += "]}";
				// sent JSON to browser via WebSocket
				m_ws->send(msg);
			}
			leds[t] = LED_OFF;
		}
	}

	virtual void onOpen() {
		start("wsADC", 180, PRI_HIGH);	// start the thread on the open event
	}

	virtual void onText(CString &msg) {
		m_ws->send(msg); 				// echo
	}
};

class CAdWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch (index) {
		case WSS_INDEX_ADC:
			CAdWebSocketService *wss = new CAdWebSocketService;
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

	// TODO: insert setup code here
	CAdWebSocketServer server;
	server.add("/adc", WSS_INDEX_ADC);	// add service name = /

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
