/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2012/9/10
 Copyright   : 2012 (C) ucxpresso.net
 License	 : CC BY-SA 3.0
 Description : WebSocket Loop Test
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/9/10	v1.0.0	first edition									Jason
 2012/9/13	v1.0.1	Fixed CLoopWebSocketService statck overflow		Jason
 	 	 	 	 	problem.
 	 	 	 	 	Add leds for debug.
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
#include "class/websocket.h"
#include "class/thread.h"
#include "class/timeout.h"
#include "class/bus.h"

//
// TODO: insert other definitions and declarations here
//
#define WS_SERVICE_INDEX_LOOP	1
//#define WS_HOST		"ws://echo.websocket.org"
#define WS_HOST			"ws://localhost"
#define WS_SERVICE		"/echo"

CBus leds(LED2, LED3, LED4, END);
//
// Loop Test Service
//
class CLoopWebSocketService: public CWebSocketService, CThread {
protected:
	virtual void run() {
		CTimeout to;
		dbg.println("connected");
		while(m_ws->isConnected() ) {
			to.reset();
			if ( m_ws->isClient() )
				 m_ws->send("Hi, I'm Client.");
			else m_ws->send("Hi, I'm Server.");
			to.wait(50);
		}
		dbg.println("disconnected");
	}

	virtual void onOpen() {
		dbg.println("onOpen");
		if ( m_ws->isClient() )
			 start("client", 160);	// start the 'client' websocket service thread
		else start("server", 160);	// start the 'server' websocket service thread
	}

	virtual void onText(CString &msg) {
		dbg.println(msg);
		if ( m_ws->isClient() ) {
			leds[1].invert();
		} else {
			leds[0].invert();
		}
	}
};

//
// WebSocket Server
//
class CMyWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		CWebSocketService *wss;
		dbg.println("onAccept");
		switch(index) {
		case WS_SERVICE_INDEX_LOOP:
			wss = new CLoopWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
	}
};

//
// WebSocket Client
//
class CMyWebSocketClient: public CWebSocketClient {
protected:
	virtual bool onConnect(CWebSocket *ws) {
		dbg.println("onConnect");
		CLoopWebSocketService *wss = new CLoopWebSocketService;
		wss->bind(ws);
		return true;
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
	dbg.waitToDebugMode();

	CMyWebSocketServer server;
	server.add(WS_SERVICE, WS_SERVICE_INDEX_LOOP);

	CMyWebSocketClient client;
	if ( client.connect(WS_HOST, WS_SERVICE)==false ) {
		dbg.println("connect fail");
	} // */

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
