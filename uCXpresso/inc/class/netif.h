/*
 ===============================================================================
 Name        : netif.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/6/21
 Copyright   : Copyright (C)  Embede International Inc.
 Description : Network I/F for lwIP
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2013/06/21 v1.0.0	First Edition									Jason
 ===============================================================================
 */

#ifndef NETIF_H_
#define NETIF_H_

#include "class/stream.h"
#include "class/thread.h"
#include "class/mutex.h"
#include "class/socket.h"

//
// NetStream class (Adaptor for CNetIf)
//
class CNetStream: public CStream {
public:
	enum netif_status {
		NETIF_UP,
		NETIF_DOWN
	};
public:
	//
	// for low level Input
	//
	virtual inline int readyToRead() { return readable(); }
	virtual void endOfRead();

	//
	// for low level Output
	//
	virtual bool requestToSend(uint16_t size);
	virtual void endOfSend();

	//
	// NetIf Status Change Event
	//
	virtual void onStatus(enum netif_status s) {}

	//
	// No Used virtual functions
	//
	virtual inline int	 writeable() {return 0;}
};

//
// Netif Class
//
class CNetIf: public CThread {
public:
	CNetIf(CNetStream &If);

	virtual ~CNetIf();

	void address(uint32_t ip, uint32_t netmask, uint32_t gateway);
	void mtu(int value);
	void name(LPCTSTR name);

	virtual bool start(LPCTSTR name, int stack=90, PRIORITIES_T priority=PRI_SUPER);

	IP_ADDR_T getLocalIp();
	IP_ADDR_T getNetMask();
	IP_ADDR_T getGateWay();

	//
	// Control
	//
	virtual void setDefault();
	virtual void set_up();		// bring the interface up
	virtual void set_down();	// bring the interface down

protected:
	xHandle m_netif;
	virtual void run();
	uint32_t	m_ip, m_netmask, m_gateway;
	uint32_t	m_mtu;
	LPCTSTR		m_name;

public:
	CMutex		m_mutex;
	CNetStream 	*m_if;
	virtual void setup();
};

#endif /* NETIF_H_ */
