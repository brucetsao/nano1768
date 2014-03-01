/*
 ===============================================================================
 Name        : main.cpp
 Author      : Arik & Jason
 Version     : 1.0.0
 Date		 : 2013/4/26
 Copyright   : 
 License	 : MIT
 Description : main definition
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
#include "probe/probe.h"
#include "debug.h"
#include "class/websocket.h"
#include "class/mailbox.h"
#include "class/timeout.h"
#include "class/json.h"
#include "motor/dcmotor.h"

#define WSS_INDEX_DOOR 0
#define DOOR_STATUS		"STATUS"
#define DOOR_CONTROL	"CONTROL"

#define DI_SENSOR			D2
#define DO_CONTROL			D13

#define STATUS_INVALID		0
#define STATUS_LOCK			1
#define STATUS_UNLOCK		2
#define STATUS_LOCKING		3
#define STATUS_UNLOCKING	4

CDebug dbg(DBG_NET);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//

//
// Door Sensor
//
class CDoorSensor: public CThread {
public:
	CDoorSensor() :
			sensor(DI_SENSOR) {
		sensor.input();
	}

	bool isLock() {
		return (sensor == LOW);
	}

protected:
	CPin sensor;
	virtual void run() {
		CPin door_led(LED3);
		PIN_LEVEL_T last = sensor;
		int status;
		while (1) {
			if (sensor != last) {	// 與之前狀態不同 (有變化)
				door_led = !door_led;
				last = sensor.read();
				if (sensor == LOW) { // close
					status = STATUS_LOCK;
				} else {
					status = STATUS_UNLOCK;
				}
				CMailBox::post(DOOR_STATUS, &status);	// 派送 status 給所有信箱
			}
			sleep(100);
		}
	}
};
CDoorSensor sensor;

//
// Door Control
//
class CDoorControl: public CThread {
public:
	CDoorControl() :
			control(DO_CONTROL) {
		control.output();
	}

protected:
	CPin control;
	virtual void run() {
		CPin led(LED4);
		CMailBox ctrl(DOOR_CONTROL);
		CMailBox status(DOOR_STATUS);
		DCMotor m1(PWM5, UNO_7);
		m1 = FORWARD;
		m1 = 0.0f; // OFF
		int *cmd, msg;
		while (1) {
			cmd = (int *) ctrl.wait();
			if (cmd != NULL) {
				led = LED_ON;
				msg = *cmd;
				switch (msg) {
				case STATUS_LOCKING:
					control = HIGH;
					CMailBox::post(DOOR_STATUS, &msg);
					break;
				case STATUS_UNLOCKING:
					control = LOW;
					CMailBox::post(DOOR_STATUS, &msg);
					m1 = FORWARD;
					m1 = 1.0f;
					sleep(500);
					m1 = 0;
					break;
				}
				status.reset();	// reset current status

				//
				// D/O Period time
				//
				status.wait(5000);	// within 5 seconds
				control = LOW;
				led = LED_OFF;

				// door lock
				m1 = REVERSE;
				m1 = 1.0f;
				sleep(500);
				m1 = 0;

				// check status
				msg = (sensor.isLock()) ? STATUS_LOCK : STATUS_UNLOCK;
				CMailBox::post(DOOR_STATUS, &msg);
			}
		}
	}
};
CDoorControl control;

//
// door websocket
//
class CDoorWebSocketService: public CWebSocketService, CThread {
protected:
	int mbMessages;

	virtual void run() {
		CString status;
		CMailBox mail(DOOR_STATUS, 4);
		int *i;
		CString jsonMessages;
		CPin led(LED2);

		while (m_ws->isConnected()) {
			i = (int *) mail.wait(100);	// 等待事件
			if ( i!=NULL ) {
				led = !led;
				dbg.println("ws run()");
				dbg.println(*i);
				switch (*i) {

				case STATUS_INVALID:
					status = "invalid";
					break;
				case STATUS_LOCK:
					status = "lock";
					break;
				case STATUS_UNLOCK:
					status = "unlock";
					break;
				case STATUS_LOCKING:
					status = "locking";
					break;
				case STATUS_UNLOCKING:
					status = "unlocking";
					break;
				}

				jsonMessages = "{";
				jsonMessages += "\"id\":";
				jsonMessages += 001;
				jsonMessages += ", \"status\":\"";
				jsonMessages += status;
				jsonMessages += "\"}";
				dbg.println(jsonMessages);
				m_ws->send(jsonMessages);
			}
		}
	}

	virtual void onOpen() {
		int i;
		CString jsonMessages;
		CString status;

		start("wsDoor", 160);	// start the thread on the open event
		dbg.println("doorWS open");

		i = (sensor.isLock()) ? STATUS_LOCK : STATUS_UNLOCK;
		switch (i) {
		case STATUS_LOCK:
			status = "lock";
			break;
		case STATUS_UNLOCK:
			status = "unlock";
			break;
		}

		jsonMessages = "{";
		jsonMessages += "\"id\":";
		jsonMessages += 001;
		jsonMessages += ", \"status\":\"";
		jsonMessages += status;
		jsonMessages += "\"}";
		dbg.println(jsonMessages);
		m_ws->send(jsonMessages);
	}

	virtual void onText(CString &msg) {
		//Cstring to Char
		char* charSource; //宣告char*
		charSource = (char*) msg.getBuffer(0);

		CJson jsonMessages(charSource);
		dbg.println("ws onText()");
		dbg.println(jsonMessages["id"].asString());
		dbg.println(jsonMessages["action"].asString());

		CString action = jsonMessages["action"].asString();

		if (action == "locking") {
			mbMessages = STATUS_LOCKING;
			CMailBox::post(DOOR_CONTROL, &mbMessages);
		} else if (action == "unlocking") {
			mbMessages = STATUS_UNLOCKING;
			CMailBox::post(DOOR_CONTROL, &mbMessages);
		} else if (action == "enroll") {
			// TODO: Enroll
		}
	}
};

class CDoorWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		dbg.println("onAccept");
		switch (index) {
		case WSS_INDEX_DOOR:
			CDoorWebSocketService *wss = new CDoorWebSocketService;
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
	CPin heart_beet(LED1);

	sensor.start("sensor");
	control.start("control");

	CDoorWebSocketServer server;
	server.add("/door", WSS_INDEX_DOOR);	// add service name = /

	// TODO: insert setup code here
//	int i;
	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		heart_beet = !heart_beet;
		sleep(200);

		// Your loop code here
		switch (dbg.isAnyKey()) {
		case 114:	//enter
		case 13:	//'r'
			dbg.println("manual lock/unlock");
			/*				door_led = !door_led;
			 i = ( door_led ) ? STATUS_LOCK : STATUS_UNLOCK;
			 CMailBox::post(DOOR_STATUS, &i); // */
			break;
		}
	}
	return 0;
}
