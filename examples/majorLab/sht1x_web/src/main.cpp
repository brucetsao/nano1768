/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/8/27
 Copyright   :
 License	 :
 Description : main definition
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
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
#include "sht1x_task.h"
#include "demo_ajax.cpp"
#include "arduino.h"
#include "lcd/menu.h"

//
// TODO: insert other definitions and declarations here
//
CSHT1xTask sht1x(8, 9);

#include "class/websocket.h"
#include "class/adc.h"
#include "class/timeout.h"
#include "class/time.h"
#include <math.h>
#include "class/flash.h"

//
// TODO: insert other definitions and declarations here
//
#define WSS_INDEX_ECHO 	1
#define WSS_MAX_COUNT	10
static int service_count = 0;
static CFlash	flash;

//
// CEchoWebSocketService Class
//
class CSineWebSocketService: public CWebSocketService, CThread {
protected:

	virtual void run() {
		if ( service_count<10 ) {
			service_count++;
			CTimeout to;
			CString msg;
			double x = 0.0f;
			while(m_ws->isConnected()) {
				to.reset();
				if ( x> (2*M_PI) ) {
					x = 0.0f;
				} else {
					x += 0.05f;
				}
				// make JSON data
				msg.printf("{\"time\":%d, \"value\":%0.8f, \"temp\":%0.2f}", CTime::now(), sin(x), sht1x.temp);
				m_ws->send(msg);
				to.wait(500);
			}
			service_count--;
		} else {
			m_ws->send("too many online users!!\r\n");
			m_ws->send("bye byte\r\n");
		}
	}

	virtual void onOpen() {
		start("wsSine", 180, PRI_HIGH);	// start the thread on the open event
	}

	virtual void onText(CString &msg) {
		m_ws->send(msg); 				// echo
	}
};

//
// CMyWebSocketServer Class
//
class CMyWebSocketServer: public CWebSocketServer {
public:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch (index) {
		case WSS_INDEX_ECHO:
			CSineWebSocketService *wss = new CSineWebSocketService;
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
	CPin led(LED1), online(LED4);

	// TODO: insert setup code here
	CAjaxDemo ajax;
	sht1x.start("sht1x", 320);

	CMenuDemo menu;
	menu.setup();

	//
	// WebSocket Server
	// add websocket service in the server
	//
	CMyWebSocketServer server;
	server.add("/sin", WSS_INDEX_ECHO);	// add service name = /

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(100);

		// Your loop code here
		menu.loop();

		if ( service_count ) {
			online = LED_ON;
		} else {
			online = LED_OFF;
		}
	}
	return 0;
}
