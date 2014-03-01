/*
 ===============================================================================
 Name        : at_command.h
 Author      : Jason
 Version     : 1.0.1
 Date		 : 2013/10/06
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : AT Command via stream class
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/6/21	v1.0.0	First Edition									Jason
 2013/10/6	v1.0.1	Add debug enable/disable for AT command message	Jason
 ===============================================================================
 */

#ifndef AT_COMMAND_H_
#define AT_COMMAND_H_

#include "class/thread.h"
#include "class/stream.h"
#include "class/semaphore.h"
#include "class/string.h"
#include "class/mutex.h"

typedef enum {
	AT_DATA = 0,
	AT_COMMAND
}AT_MODE_T;

class ATcommand: public CThread {
public:
	ATcommand(CStream &s);
	virtual ~ATcommand();

	//
	// command
	//
	virtual CString& command(LPCTSTR cmd, LPCTSTR wait="OK", uint32_t timeout=3000);

	//
	// control
	//
	virtual void mode(AT_MODE_T m);
	virtual bool isCommandMode();

	//
	// override the CThread::start
	//
	virtual bool start(const char *name, int stack=96, PRIORITIES_T priority=PRI_HARDWARE);

	//
	// debug
	//
	void debug(bool enable);

protected:
	//
	// virtual functions
	//
	virtual void onUnknown(uint8_t ch);	// in Command Mode, receive a unknown character
	virtual void onReceive(uint8_t ch); // in Data Mode, receive a character

	CStream     *m_serial;
	CSemaphore  m_receive;
	CString		m_result;
	uint32_t	m_flag;

	virtual void run();
	virtual void send(LPCTSTR cmd);
	virtual CString& response(LPCTSTR wait, uint32_t timeout);
};

#endif /* AT_COMMAND_H_ */
