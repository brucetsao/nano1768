/*
===============================================================================
 Name        : ajax_ptz.cpp
 Author      : Jason
 Version     : 1.0.0
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License 	 : CC BY-SA 3.0
 Description : Ajxa Demo
 History	 :
===============================================================================
*/
#include "class/ajax.h"
#include "class/string.h"
#include "servo/servo.h"
#include "debug.h"

#define CGI_INDEX_PTZ		0

extern CServo servoX;
extern CServo servoY;


// Ajax for Temperature & Humidity
class CAjaxPTZ: public CAjax {
public:
	CAjaxPTZ() {
		// CGI Demo
		add("/ajax/ptz", CGI_INDEX_PTZ);
	}

	//
	// On Ajax XHTML request Event
	//
	virtual void onRequest(const int index, CString &result) {

		int x,y;

		switch(index) {
		case CGI_INDEX_PTZ:
			if ( getValue("X", x) ) {
				servoX = (float)(x/100.0f);
				dbg.println("X=%0.2f\n", (float)(x/100.0f));
			}
			if ( getValue("Y", y) ) {
				servoY = (float)(y/100.0f);
				dbg.println("Y=%0.2f\n", (float)(y/100.0f));
			}
			result = ajaxOK;
			break;

		default:
			result = ajaxNG;
		}
	}
};

