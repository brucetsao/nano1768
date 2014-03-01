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
#include "class/console.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
	user adjustable pool memory
   ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

#include "class/socket.h"
#include "class/thread.h"
#include "class/bus.h"

static int conn_count = 0;
CBus leds(LED1, LED2, LED3, LED4, END);

//
// Thread for Remote Connection
//
class CRemoteLED: public CThread {
protected:
	CSocket *m_sock;

	virtual void run() {
		char val;
		Console con(*m_sock);
		con << "Hello Client #" << conn_count++ << endl;
		while( m_sock->isConnected() ) {			// check connection
			if ( m_sock->read(&val, sizeof(val)) ) {
				if ( val>='1' && val<='4' ) {		// only '1' ~ '4' (for LED index)
					leds[val-'1'] = !leds[val-'1'];	// blink specified LED
				}
			}
		}
		delete m_sock;								// free disconnected socket
	}

public:
	void assign(CSocket *sock) {
		m_sock = sock;
	}
};

/* ==============================================
	main task routine
   ============================================== */
int main(void) {
	pool_memadd((uint32_t)pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	CSocket *client;
	CSocket server;

	server.create(SOCK_STREAM);
	server.bind(23);	// bind to port 23
	server.listen();	// enter listen mode

	// Enter an endless loop
	while(1){
		client = server.accept();				// accept and create a connected socket
		if ( client!=NULL ) {

			CRemoteLED *remote = new CRemoteLED;
			if ( remote!=NULL ) {
				remote->assign(client);			// assign the accepted connection socket
												// for the remote thread

				remote->start("remote");		// start remote thread (name="tcp_echo")

			} else delete client;				// if remote==NULL...
		}
	}
	return 0 ;
}
