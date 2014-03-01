/*
 ===============================================================================
 Name        : shell.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/2/11
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : 
 History	 :
 	 2012/8/29: modify m_console to public for internal use.
 	 2012/9/05: add DEBUG mode
 	 2013/5/03: add "renew"
 ===============================================================================
 */

#ifndef SHELL_H_
#define SHELL_H_

#include "class/thread.h"
#include "class/console.h"
#include "class/string.h"
#include "class/serial.h"
#include "class/socket.h"
#include "class/semaphore.h"


class CShell: public CThread {
public:
	CShell();
	CShell(CSerial &s);
	CShell(CSocket *s);
	virtual ~CShell();
	virtual void assign(CSerial &s);
	virtual void assign(CSocket *s);
	virtual bool start();

	virtual void onQuery(CString &str);
	virtual void show_welcome();
	virtual void show_prompt();
	virtual void show_menu();
	virtual void show_task();
	virtual void show_heap();
	virtual void show_date();
	virtual void show_time();
	virtual void show_clear();
	virtual void show_network();
	virtual void renew();
	virtual void show_version();
	virtual void show_core();
	virtual void show_diskfree();
	virtual void show_dir();
	virtual void show_file(LPCTSTR arg);
	virtual void change_dir(LPCTSTR arg);

	bool isDebugMode();
	int  dbgWaitKey(uint32_t t=MAX_DELAY_TIME);

// internal use
public:
	virtual void run();
	virtual void onDebug();
	virtual void onClose() {};
	Console 	m_console;

protected:
//	uint32_t	m_flag;
	CStream 	*m_sock;
	CString		m_path;
	CString 	cmd;
	CSemaphore	m_semInput;
	int			m_dbgKey;
};

#endif /* SHELL_H_ */
