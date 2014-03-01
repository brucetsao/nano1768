/*
 ===============================================================================
 Name        : btHC05.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2012/8/12
 Copyright   : 2012 (C) www.embeda.com.tw / www.ucxpresso.net
 License	 : CC BY-SA 3.0
 Description : A class for bluetooth module (HC-05)
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 
 ===============================================================================
 */

#ifndef BTHC05_H_
#define BTHC05_H_

#include "class/pin.h"
#include "class/serial.h"
#include "class/string.h"


class btHC05: public CStream {
public:
	btHC05(SERIAL_PORT_T port, PIN_NAME_T key, PIN_NAME_T link, uint32_t baudrate=38400);

	inline virtual int readable() {
		return m_port.readable();
	}

	inline virtual int writeable() {
		return m_port.writeable();
	}

	inline virtual int read(void *buf, int len, bool block=true) {
		return m_port.read(buf, len, block);
	}

	inline virtual int write(const void *buf, int len, bool block=true) {
		return m_port.write(buf, len, block);
	}

	inline virtual int close() {
		return m_port.close();
	}

	virtual bool isConnected();

	inline void  flush() {
		free();
	}

	virtual ~btHC05();
	virtual CString ATcommand(LPCTSTR cmd);

protected:
	CSerial	m_port;
	CPin	m_key, m_link;
};

#endif /* BTHC05_H_ */
