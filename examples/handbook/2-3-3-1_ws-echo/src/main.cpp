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

//
// TODO: insert other definitions and declarations here
//
#define WSS_INDEX_ECHO 1

//
// CEchoWebSocketService Class
//
class CEchoWebSocketService: public CWebSocketService, CThread {
protected:
	int timeout;
	virtual void run() {
		CPin led(LED4);
		m_ws->send("Hello World");
		timeout = 0;
		while(m_ws->isConnected()) {
			led = !led;

			timeout++;
			if ( timeout>=20) {
				m_ws->close();	// if timeout (no coming), force to close the websocket
			}
			sleep(500);
		}
		led = LED_OFF;
	}

	virtual void onOpen() {
		start("echo", 180);
	}

	virtual void onText(CString &msg) {
		m_ws->send(*msg); // echo
		timeout = 0;	// reset timeout
	}
};

//
// CMyWebSocketServer Class
//
class CMyWebSocketServer: public CWebSocketServer {
public:
	virtual bool onAccept(int index, CWebSocket *ws) {
		switch(index) {
		case WSS_INDEX_ECHO:
			CEchoWebSocketService *wss = new CEchoWebSocketService;
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
	// WebSocket Server
	// 		add websocket service in the server
	//
	CMyWebSocketServer server;
	server.add("/", WSS_INDEX_ECHO);	// add service name = /

	//
	// Enter an endless loop
	//
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
