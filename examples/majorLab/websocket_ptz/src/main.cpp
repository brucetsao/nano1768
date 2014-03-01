/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2013/1/20
 Copyright   :
 License	 : CC BY-SA 30.
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
#include "servo/servo.h"
#include "websocket_ptz.cpp"

#define WS_INDEX_PTZ	0
#define WS_INDEX_SYS	1

class CMyWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		CWebSocketService *wss;
dbg.println("onAccept:");
		switch (index) {
		case WS_INDEX_PTZ:
			wss = new CPtzWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
	}
};

//
// TODO: insert other definitions and declarations here
//
CServo servoX(PWM5, &SERVO_MG_995);
CServo servoY(PWM6, &SERVO_MG_995);

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
	servoX.begin();
	servoY.begin();

	// Register a WebSocket Server
	CMyWebSocketServer server;
	server.add("/ptz", WS_INDEX_PTZ);

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
