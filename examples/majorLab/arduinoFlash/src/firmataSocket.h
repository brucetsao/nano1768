/*
===============================================================================
 Name        : firmataSocket.h
 Author      : Jason
 Version     : 1.0.0
 Copyright   : Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description :
 History	 :
===============================================================================
*/

#ifndef FLASHSOCKET_H_
#define FLASHSOCKET_H_

#include "class/thread.h"
#include "class/socket.h"

/* ==============================================
	firmata thread
   ============================================== */
class CFirmataSocket: public CThread {
protected:
	CSocket *m_sock;
	virtual void run();
public:
	inline virtual void assign(CSocket *client) {
		m_sock = client;
	}
};

extern void board_setup();

#endif /* FLASHSOCKET_H_ */
