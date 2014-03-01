/*
 ===============================================================================
 Name        : btHC05.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/8/12
 Copyright   : 2012 (C) www.embeda.com.tw / www.ucxpresso.net
 License	 : CC BY-SA 3.0
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 
 ===============================================================================
 */

#include "btHC05.h"
#include "class/timeout.h"

btHC05::btHC05(SERIAL_PORT_T port, PIN_NAME_T key, PIN_NAME_T link, uint32_t baudrate):
m_port(port),
m_key(key),
m_link(link)
{
	m_link.input(INTERNAL_PULL_DOWN);	// high active when link successful
	m_key.output();
	m_key = LOW;
	m_port.begin(baudrate);
}

btHC05::~btHC05() {
	m_port.end();
}

#define BUF_SIZE	   8
#define HC05_RESPONSE_OK	("OK" CRLF)

CString btHC05::ATcommand(LPCTSTR cmd) {
	CString	 tmp;
	int len;

	m_key = HIGH;
	tmp.printf("AT+%s\r\n", cmd);
	m_port.write(tmp.getBuffer(), tmp.length());

	char	 buf[BUF_SIZE+1];
	CTimeout tm;
	tmp.clear();
	while( tm.isExpired(500)==false  ) {
		if ( m_port.readable() ) {
			len = m_port.read(buf, BUF_SIZE);
			if ( len ) {
				buf[len] = '\0';
				tmp += buf;
				if ( tmp.find(HC05_RESPONSE_OK)>=0 ) break;
				if ( tmp.find("ERROR")>=0 ) {
					sleep(100);
					break;
				}
			}
		}
	}
	m_key = LOW;
	return tmp;
}

bool btHC05::isConnected() {
	if ( m_link==HIGH ) {
		return true;
	} else {
		return false;
	}
}
