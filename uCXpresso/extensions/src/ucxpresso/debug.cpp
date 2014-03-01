/*
 ===============================================================================
 Name        : debug.cpp @ extensions
 Author      : Jason
 Date		 : 2013/5/15
 Version     : 1.0.3
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 License	 : CC BY-SA 3.0
 Description : debugged shell
 History	 :
 	 2012/9/6: v1.0.1 update CDebug::println(uint32_t)
 	 2013/3/1: v1.0.2 add keep alive for DBG_NET
 	 2013/5/15: add dbg_printf(...) function
 ===============================================================================
 */
#include "class/thread.h"
#include "class/serial.h"
#include "debug.h"

#define TELNET_PORT	23

CDebug::CDebug(DEBUG_PORT_T port) {
	m_port = port;
}

bool CDebug::start() {
	return CThread::start("shell", 256);
}

void CDebug::run() {
	if ( m_port==DBG_USB ) {
		//
		// USB port
		//
		CSerial cdc(USB);
		m_shell.assign(cdc);
		while(1) {
			if ( cdc.isConnected() ) {
				m_shell.run();
			}
		}

	} else {
		//
		// Telnet (TCP/IP port 23)
		//
		CSocket server;
		CSocket *client;

		server.create(SOCK_STREAM);
		server.bind(TELNET_PORT);
		server.listen();
		while (1) {
			if ((client = server.accept()) != NULL) {
				client->keepAlive(3, 3, 3);	// set keep alive in 3 seconds for idle, try in 3 seconds and 3 times
				m_shell.assign(client);
				m_shell.run();
				delete client;
			}
		}
	}
}

#ifdef DEBUG

void CDebug::waitToDebugMode() {
	while( m_shell.isDebugMode()==false );
	sleep(500);
}


void CDebug::printf(LPCTSTR format, va_list varg) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console.printf(format, varg);
		m_mutex.unlock();
	}
}

void CDebug::printf(LPCTSTR format, ...) {
	va_list varg;

	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		va_start(varg, format);
		m_shell.m_console.printf(format, varg);
		va_end(varg);
		m_mutex.unlock();
	}
}

void CDebug::println(LPCTSTR format, ...) {
	va_list varg;

	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		va_start(varg, format);
		m_shell.m_console.printf(format, varg);
		va_end(varg);
		m_shell.m_console << endl;
		m_mutex.unlock();
	}
}

void CDebug::println(int value) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console << value << endl;
		m_mutex.unlock();
	}
}

void CDebug::println(uint32_t value) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console.printf("%u\n",value);
		m_mutex.unlock();
	}
}

void CDebug::println(float value) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		m_shell.m_console << value << endl;
		m_mutex.unlock();
	}
}

void CDebug::putc(int c) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		if ( c!=0x0D ) {
			m_shell.m_console << (char)c << flush;
		} else {
			m_shell.m_console << endl;
		}
		m_mutex.unlock();
	}
}

int CDebug::breakpoint(LPCTSTR desc) {
	if ( m_shell.isDebugMode() ) {
		m_mutex.lock();
		if ( desc ) {
			m_shell.m_console << desc << flush;
		} else {
			m_shell.m_console << "\nany key to continue..." << flush;
		}
		m_mutex.unlock();
		return m_shell.dbgWaitKey();
	}
	return 0;
}

void dbg_printf(LPCTSTR format, ...) {
	va_list varg;
	if (dbg.isDebugMode() ) {
		va_start(varg, format);
		dbg.printf(format, varg);
		va_end(varg);
	}
}

#endif // DEBUG
