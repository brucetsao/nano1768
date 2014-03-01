/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/9/16
 License	 : CC BY-SA 3.0
 Description : Localhost loop-test for CSocket
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
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
#include "class/socket.h"
#include "class/thread.h"
#include "class/timeout.h"
#include "class/bus.h"

//
// TODO: insert other definitions and declarations here
//
#define REMOTE_HOST		"localhost"
#define REMOTE_PORT		1024

CBus leds(LED2, LED3, LED4, END);
const char msg[] = "Hello World.";

//
// Server
//
class Server: public CThread {
protected:
	virtual void run() {
		CSocket *client;
		CSocket sock(SOCK_STREAM);
		sock.bind(REMOTE_PORT);
		sock.listen();
		while(1) {
			client = sock.accept();								// wait for client connecting
			if ( client ) {
				dbg.println("server: connected");
				byte buf[sizeof(msg)];
				int  len;
				while( client->isConnected() ) {
					if ( client->readable() ) {
						len = client->read(buf, sizeof(buf));	// read message from client
						client->write(buf, len);				// write back message to client
						leds[0] = !leds[0];
						dbg.println("Client > %s", buf);
					}
				}
				delete client;									// free the client socket after disconnected
				dbg.println("server: disconnected");
			}
		}
	}
};

//
// Client
//
class Client: public CThread {
protected:
	virtual void run () {
		CSocket sock(SOCK_STREAM);
		dbg.println("client: connecting...");
		sleep(3000); // wait for local server ready
		if ( sock.connect(REMOTE_HOST, REMOTE_PORT)==ERR_OK ) {	// connect to server
			char buf[sizeof(msg)];
			int	 len;
			dbg.println("client: connected");
			sock.write(msg, sizeof(msg));						// send first message to server
			while( sock.isConnected() ) {
				if ( sock.readable() ) {						// is data from server
					len = sock.read(buf, sizeof(buf));			// read message from server
					sock.write(buf, len);						// write back message to server
					leds[1] = !leds[1];
					dbg.println("Server > %s", buf);
				}
			}
			dbg.println("client: disconnected");
		}
		leds[2] = LED_ON; // error LED
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

//#ifdef DEBUG
	dbg.start();
//#endif
	// Simple demo Code (removable)
	CPin led(LED1);

	// TODO: insert setup code here
	dbg.waitToDebugMode();
	Server s;
	s.start("server", 180);

	Client c;
	c.start("client", 180);

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
