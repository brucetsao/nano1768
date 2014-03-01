/*
===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.1
 Copyright   :
 Description : main definition
 History	 :
===============================================================================
*/
#include "uCXpresso.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
	user adjustable pool memory
   ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

#include <class/socket.h>

/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif
	char	ch;
	CSocket *client;
	CSocket server;

	server.create(SOCK_STREAM);
	server.bind(23);	// bind to port 23
	server.listen();	// enter listen mode

	// Enter an endless loop
	while(1){
		client = server.accept();						// accept and create a connected socket
		if ( client!=NULL ) {
			dbg.println("Client connected\n");
			while( client->isConnected() ) {			// check connection
				if ( client->read(&ch, sizeof(ch)) ) {	// read from client
					client->write(&ch, sizeof(ch));		// echo to client
				}
			}
			dbg.println("Client disconnected\n");
			delete client;								// free client after disconnected
		}
	}
	return 0 ;
}
