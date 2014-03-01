/*
 ===============================================================================
 Name        : ssl.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/11/11
 Copyright   : Copyright (C) 
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/11/11	v1.0.0	First Edition.									Jason
 ===============================================================================
 */

#ifndef SSL_SOCKET_H_
#define SSL_SOCKET_H_

#include "class/socket.h"

class sslSocket: public CSocket {
public:
	sslSocket();
	virtual ERR_T connect(LPCTSTR server_name, int server_port);
	virtual ERR_T close();

	virtual int ss_read(void *buf, int len);
	virtual int ss_write(const void *buf, int len);

	virtual inline int read(void *buf, int len, bool block=true) {
		return ss_read(buf, len);
	}

	virtual inline int  write(const void *buf, int len, bool block=true) {
		return ss_write(buf, len);
	}

public:
	virtual ~sslSocket();

protected:
	xHandle	m_ssl;
	xHandle m_ssn;
	virtual void client();

};

#endif /* SSL_H_ */
