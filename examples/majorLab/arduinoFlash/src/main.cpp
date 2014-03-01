/*
===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Copyright   :
 Description : main definition
 History	 :
===============================================================================
*/
#include "uCXpresso.h"
#include "debug.h"

/* ==============================================
	user adjustable pool memory
   ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
#include "firmataSocket.h"
#include "class/bus.h"

#include "debug.h"
CDebug dbg(DBG_USB);

//
// TODO: insert other definitions and declarations here
#define DEF_SERVER_PORT		5331

class CFirmataServer: public CThread {
public:
	virtual void run() {
		CSocket server(SOCK_STREAM);
		CSocket *client;

		server.bind(DEF_SERVER_PORT);
		server.listen();

		// Enter an endless loop
		while(1){
			client = server.accept();
			if ( client ) {
				CFirmataSocket *fs = new CFirmataSocket;
				if ( fs ) {
					fs->assign(client);
					fs->start("firmata", 256);
				} else {
					delete client;
				}
			}
		}
	}
};


/* ==============================================
	main task routine
   ============================================== */
uint32_t *RSID = (uint32_t *) 0x400FC180;
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	CBus leds(LED1, LED2, END);

	CFirmataServer svr;
	svr.start("FServer", 256);

	// Enter an endless loop
	while(1){
		//
		// Simple demo Code (removable)
		//
		leds[0] = !leds[0];				// active, LED1 blink

		if ( CSocket::getLocalIp() ) {	// network ready, LED3 blink
			leds[1] = !leds[1];
		} else {
			leds[1] = LED_OFF;
		}

		sleep(200);
	}
	return 0 ;
}
