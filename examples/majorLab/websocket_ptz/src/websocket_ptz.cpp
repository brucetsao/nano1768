/*
 ===============================================================================
 Name        : websocket_ptz.cpp
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2013/1/20
 Copyright   :
 License 	 : CC BY-SA 3.0
 Description : WebSocket Demo (PTZ)
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ===============================================================================
 2013/1/20	 : Use CJson to parse the WebSocket Message
 ===============================================================================
*/
#include "class/string.h"
#include "servo/servo.h"
#include "debug.h"

extern CServo servoX;
extern CServo servoY;

#include "class/websocket.h"
#include "class/json.h"

class CPtzWebSocketService: public CWebSocketService {
protected:
	virtual void onText(CString &msg) {
		CJson json(msg.getBuffer());	// parse the message by CJson
		servoX = json["x"].asFloat();	// set X angle to servoX
		servoY = json["y"].asFloat();   // set Y angle to servoY
		dbg.printf("%0.2f, %0.2f\n", json["x"].asFloat(), json["y"].asFloat());
	}

	virtual void onOpen() {
		dbg.println("ptz::onOpen");
	}

	virtual void onClose() {
		dbg.println("ptz::onClose");
		delete this;
	}

	virtual void onError() {
		dbg.println("ptz::onError");
		delete this;
	}
};
