/*
 ===============================================================================
 Name        : debug.h @ extensions
 Author      : Jason
 Date		 : 2013/5/15
 Version     : 1.0.3
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : debugged shell
 History	 :
 	 2012/9/6: add isAnyKey()
 	 2012/9/12: add println(CString&)
 	 2012/9/12: rename waitEnterToDebugMode() to waitToDebugMode()
 	 2013/5/15: add dbg_printf() function
 ===============================================================================
 */
#ifndef _CDEBUG_H
#define _CDEBUG_H

#include <cstdarg>
#include "class/thread.h"
#include "class/shell.h"
#include "class/mutex.h"
#include "class/string.h"

typedef enum {
	DBG_USB,	// usb CDC com port
	DBG_NET		// telnet (tcpip port 23)
}DEBUG_PORT_T;

class CDebug: public CThread {
public:
	CDebug(DEBUG_PORT_T port);
	virtual bool start();

	//
	// for DEBUG
	//
#ifdef DEBUG
	virtual void waitToDebugMode();
	virtual void printf(LPCTSTR format, va_list varg);
	virtual void printf(LPCTSTR format, ...);
	virtual void println(LPCTSTR format, ...);
	virtual void println(int value);
	virtual void println(uint32_t value);
	virtual void println(float value);

	virtual int  breakpoint(LPCTSTR desc=NULL);

	virtual inline void println(CString &str){
		println((LPCTSTR)str);
	}
	virtual inline bool isDebugMode() {
		return m_shell.isDebugMode();
	}
	virtual inline int isAnyKey() {
		int k;
		return  ((k=m_shell.dbgWaitKey(0))!=0x1b? k : 0);
	}
	virtual void putc(int c);
#else
	virtual void waitToDebugMode() NO_EFFECT
	virtual void println(LPCTSTR format, ...) NO_EFFECT
	virtual void println(int value) NO_EFFECT
	virtual void println(uint32_t value) NO_EFFECT
	virtual void println(float value) NO_EFFECT
	virtual void putc(int c) NO_EFFECT
	virtual int breakpoint(LPCTSTR desc=NULL) { return 0; }
	virtual bool isDebugMode() { return false; }
	virtual int isAnyKey() { return 0xffff; }
#endif

	inline int bp() {
		return breakpoint();
	}

	inline operator bool () {
		return isDebugMode();
	}

	//
	// internal use
	//
protected:
	virtual void run();

	DEBUG_PORT_T	m_port;
	CShell  		m_shell;
#ifdef DEBUG
	CMutex  m_mutex;
#endif
};

extern CDebug dbg;

#ifdef DEBUG
extern void dbg_printf(LPCTSTR format, ...);
#endif

#endif	// _CDEBUG_H
