/*
 ===============================================================================
 Name        : portevent.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/4
 License     : CC BY-SA 3.0
 Description : event port for modbus
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/4	v1.0.0	port event handle for Modbus					Jason
 */

#include "class/mailbox.h"
#include "mb.h"

//
//	defines
//
#define MB_POLL_CYCLETIME       100     						// Poll cycle time is 100ms
#define MAIL(x)					(static_cast<CMailBox*>(x))		// Use C++ Object in C functions
#define MAIL_BOX_NAME			"mbEvent"


//
// Static variables
//
static CMailBox *xMailBox = NULL;
static eMBEventType eMailBoxEvent;

//
// Start implementation
//
PR_BEGIN_EXTERN_C

BOOL xMBPortEventInit(void) {
	eMailBoxEvent = EV_READY;
	if ( xMailBox==NULL ) {
		xMailBox = new CMailBox(MAIL_BOX_NAME);
	}
	return (xMailBox != NULL ? TRUE : FALSE);
}

void vMBPortEventClose(void) {
	if (xMailBox != NULL) {
		delete xMailBox;
		xMailBox = NULL;
	}
}

BOOL xMBPortEventPost(eMBEventType eEvent) {
	eMailBoxEvent = eEvent;
	MAIL(xMailBox)->post("mbEvent", &eMailBoxEvent);
	return TRUE;
}

BOOL xMBPortEventGet(eMBEventType * eEvent) {
	void *peMailBoxEvent;
	BOOL xEventHappend = FALSE;

	peMailBoxEvent = MAIL(xMailBox)->wait(MB_POLL_CYCLETIME);
	if ( peMailBoxEvent!=NULL ) {
		*eEvent = *(eMBEventType *) peMailBoxEvent;
		eMailBoxEvent = EV_READY;
		xEventHappend = TRUE;
	}

	return xEventHappend;
}

PR_END_EXTERN_C

