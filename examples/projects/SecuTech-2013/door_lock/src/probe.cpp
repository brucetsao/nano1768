/*
 ===============================================================================
 Name        : probe.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/23
 License     : CC BY-SA 3.0
 Description : websocket_adc_mail
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/22	v1.0.0	First Edition.									Jason
 */

#ifdef DEBUG

#include "probe/probe.h"
#include "class/websocket.h"
#include "class/socket.h"
#include "class/mailbox.h"
#include "class/timeout.h"
#include "class/string.h"

#define WS_INDEX_PROBE		0
#define MAX_USERS_MEMORY	38000
#define MAX_IDLE_TICK_COUNT	6000

//
// probe WebSocket Service class
//
class pbWebSocketService: public CWebSocketService, CThread {
protected:
	//
	// service run-loop
	//
	virtual void run() {
		CMailBox m_mail(xMAIL_PROBE);
		LPCTSTR msg;
		while(m_ws->isConnected()) {
				msg = (LPCTSTR) m_mail.wait();	// wait an event (message) from iProbe
				if ( msg ) {
					m_ws->send(msg);			// send to websocket client
				}
		}
	}
	//
	// websocket connected
	//
	virtual void onOpen() {
		start("wsProbe", 160);						// start service thread
	}
};

//
// probe WebSocket Server class
//
class pbWebSocketServer: public CWebSocketServer {
public:
	pbWebSocketServer() {
		add("/probe", WS_INDEX_PROBE);
	}

protected:
	//
	// accept a websocket connection requested
	//
	virtual bool onAccept(int index, CWebSocket *ws){
		CWebSocketService *wss;
		switch(index) {
		case WS_INDEX_PROBE:
			wss = new pbWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
	}
};

static pbWebSocketServer probeServer;

//
// start iProbe thread
//
bool iProbe::start(int periodTime) {
	m_periodTime = periodTime;
	return CThread::start("iProbe", 120);
}

//
// iProbe run loop
//
void iProbe::run() {
	CTimeout tm;
	CString  msg;
//	uint32_t m_lstCPU = 0;
	uint32_t m_lstNET = 0;
	while(1) {
		tm.wait(1000);
		tm.reset();
		//
		// Make JSON string
		//
		msg = "{";
		msg += "\"cpu\":";
		msg += map(CThread::getIdleTickCount(), 0, 2000, 0, 100);
		msg += ", \"net\":";
		msg += constrain(OFFSET(CSocket::getRxPackageCount(), m_lstNET, MAX_UINT32), 0, 100);
		msg += ", \"mem\":";
		msg += map(heapAvailableSize(), 0, MAX_USERS_MEMORY, 100, 0);
		msg += "}";

		//
		// Send JSON message to All mail receiver
		//
		CMailBox::post(xMAIL_PROBE, msg.getBuffer());

		//
//		m_lstCPU = CThread::getIdleTickCount();
		m_lstNET = CSocket::getRxPackageCount();
	}
}

iProbe probe;

#endif // DEBUG
