/*
 ===============================================================================
 Name        : firmataSocket.cpp
 Author      : Jason
 Version     : 1.0.0
 Copyright   : Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description :
 History	 :
 ===============================================================================
 */
#include "firmataSocket.h"
#include "myFirmata.h"
#include "debug.h"
#include "class/pin.h"

/* ==============================================
 FirmataSocket & Thread
 ============================================== */
void CFirmataSocket::run() {
	dbg.println("remote is connected\n");
	CMyFirmata myFirmata;
	myFirmata.begin(m_sock);
	m_sock->keepAlive(30, 10, 3); // keep alive timeout 30 seconds
	while (m_sock->isConnected()) {
		myFirmata.chkLoop();
		myFirmata.outLoop();
		sleep(10);
	}

	dbg.println("remote is disconnected\n");
	delete m_sock;
}
