/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
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

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include <string.h>
#include "network/cgi.h"
#include "network/ssi.h"

//
// TODO: insert other definitions and declarations here
//

//
// CGI Functions
//

extern "C" int hello(int iIndex, int iNumParams, char *pcParam[], char *pcValue[], const char **response) {
	*response = "Hello World!!";
	return strlen(*response);
}

extern "C" int setLED(int iIndex, int iNumParams, char *pcParam[], char *pcValue[], const char **response) {
	CPin led(LED2);
	int  index;
	index = FindCGIParameter("led", pcParam, iNumParams);
	if ( index>=0 ) {
		if ( strcmp(pcValue[index], "ON")==0 ) {
			led = LED_ON;
		} else {
			led = LED_OFF;
		}
		*response = "OK";
	} else *response = "NG";

	return strlen(*response);
}

static const tCGI myCGI[] = {
	{"/hello", hello},
	{"/setled", setLED}
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
	http_set_cgi_handlers(myCGI, 2);

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here

	}
	return 0;
}
