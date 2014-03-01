/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/1/20
 Copyright   :
 License	 :
 Description : WebSocket ADC Monitor (use CMailBox)

 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/pin.h"
#include "probe/probe.h"
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
#include "class/mailbox.h"

//
// TODO: insert other definitions and declarations here
//
#define WSS_INDEX_ADC	  0
#define MAX_AD_SAMPLES	100
#define xMAIL_AD		"AD"
#define MAX_WSS_COUNT	  5

CBus leds(LED2, LED3, LED4, END);
int	 gnWssCount;

//
// ADC Service
//
class CAdcService: public CThread {
protected:
	virtual void run() {
		CAdc ad(AD0);
		CTimer tm(TIMER0);
		tm.setting(10, 100);	// TIMER interval 100us
		tm.begin();
		ad.begin();
		CString json;
		while (1) {
			//
			// make JSON value (array) string
			//
			json = "{\"value\":[";
			for (int i = 0; i < MAX_AD_SAMPLES; i++) {
				tm.reset();
				json += ad.read();		// add A/D value to string
				if (i < (MAX_AD_SAMPLES - 1))
					json += ",";
				tm.wait();
			}
			json += "]}";

			//
			// sent JSON to All WebSocket Service
			//
			CMailBox::post(xMAIL_AD, json.getBuffer());
		}
	}
};

//
// WebSocket Service
//
class CAdWebSocketService: public CWebSocketService,public CThread {
public:
	CAdWebSocketService() : m_mail(xMAIL_AD) {	// create a receive mailbox
	}

protected:
	CMailBox m_mail;
	virtual void run() {
		LPCTSTR msg;
		while (m_ws->isConnected()) {
			msg = (LPCTSTR) m_mail.wait();		// wait for a JSON AD data from mailbox
			if ( msg ) {
				m_ws->send(msg);					// send JSON to browser via WebSocket
			}
		}
		gnWssCount--;
	}

	virtual void onOpen() {
		start("wsADC", 160);	// start the thread on the open event
		gnWssCount++;
	}

	virtual void onText(CString &msg) {
		m_ws->send(msg); 				// send back (echo)
	}
};

//
// WebSocket Server
//
class CAdWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch (index) {
		case WSS_INDEX_ADC:
			if ( gnWssCount<MAX_WSS_COUNT ) {
				CAdWebSocketService *wss = new CAdWebSocketService;
				wss->bind(ws);
				return true;
			}
			break;
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
	CAdcService ad;
	ad.start("AD", 120, PRI_HIGH);		// start ADC service

	gnWssCount = 0;
	CAdWebSocketServer server;
	server.add("/adc", WSS_INDEX_ADC);	// add service name = /

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
		leds = gnWssCount;
	}
	return 0;
}
