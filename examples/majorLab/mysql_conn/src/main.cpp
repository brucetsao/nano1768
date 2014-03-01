/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/5/5
 Copyright   :
 License	 : MIT
 Description : mySQL connector Demo
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/5/5	v1.0.0	Initialize										Jason
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
#include "db/mysql.h"

//
// TODO: insert other definitions and declarations here
//
const char server_addr[] = "192.168.1.10";
const int	 server_port = 3306;
const char user[] = "root";
const char pswd[] = "secret";
const char sqlINSERT[] = "INSERT INTO test_arduino.hello VALUES ('Hello, MySQL!', NULL)";


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
	CPin myLED(LED2);

	//
	// Create a CSocket adaptor
	//
	CSocket sock(TCP);

	//
	// mySQL connector Demo
	//
	Connector my_conn(sock);
	if ( my_conn.mysql_connect(server_addr, server_port, user, pswd) ) {
		sleep(500);
		/* Write Hello, World to MySQL table test_arduino.hello */
		my_conn.cmd_query(sqlINSERT);
		dbg.println("Query Success!");
		myLED = LED_ON;
	} else {
		dbg.println("Connection failed.");
		myLED = LED_OFF;
	}

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
