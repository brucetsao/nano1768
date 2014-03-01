/*
 ===============================================================================
 Name        : wifi_gainspan.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/6/27
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT  (No Commercial)
 Description : WiFi Stream Class for GS1000 series
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/6/27	v1.0.0	First Edition									Jason
 ===============================================================================
 */

#ifndef WIFI_GAINSPAN_H_
#define WIFI_GAINSPAN_H_

#include "class/stream/at_command.h"
#include "class/pin.h"
#include "class/mailbox.h"
#include "class/serial.h"
#include "class/timeout.h"
#include "class/netif.h"
#include "spi_gainspan.h"


#define DEF_GS_BAUDRATE		115200
#define MAX_GS_BAUDRATE		230400
#define WIFI_MTU			1500

typedef enum {
	W_NONE = 0,
	W_OPEN,
	WEP,
	WPA_PSK,
	WPA2_PSK,
	WPS_BUTTON
}GS_WIFI_SECURITY_T;

typedef struct _gs_rawdata {
	int 	length;
	uint8_t *buf;
}GS_RAWDATA_T;

class wifiGS: public ATcommand, public CNetStream {
protected:
	enum gs_state {
		GS_STATE_COMMAND = 0,
		GS_STATE_ESCAPE,
		GS_STATE_RAWDATA
	};

	enum gs_if {
		UART =0,
		SPI
	};

public:
	//
	// Construct for UART stream
	//
	wifiGS(CSerial &s, PIN_NAME_T reset);

	//
	// Construct for SPI stream
	//
	wifiGS(spiGS &s, PIN_NAME_T reset);

	//
	// Destruct
	//
	virtual ~wifiGS();

	//
	// override CThread class
	//
	virtual bool start(LPCTSTR name="GS");

	//
	// H/W & interface
	//
	virtual void reset();

	virtual bool baudrate(uint32_t val);
	virtual void led(PIN_LEVEL_T val);

	//
	// WiFi AP Client
	//
	bool connect(LPCTSTR ssid, LPCTSTR key, GS_WIFI_SECURITY_T mode=WPA2_PSK);
	bool disconnect();

	//
	// TODO: WiFi AP Server
	//

	//
	// Stream virtual functions for CNetStream class
	//
	virtual int	 readable();
	virtual int  read(void *payload, int len, bool block=true);
	virtual int  write(const void *payload, int len, bool block=true);
	virtual int  close();
	virtual bool isConnected();

	// Implement the virtual functions for CNetStream class
	//
	virtual bool requestToSend(uint16_t size);
	virtual void endOfSend();

	//
	// Ethernet
	//
	bool	MAC(uint32_t *addr);

	//
	// WiFi Tramsint & Control
	//
	bool	txPower(int val);
	bool	txRetry(int val);	// Max. 7

protected:
	enum gs_state m_state;
	enum gs_if	  m_interface;
	CPin 		  m_reset;
	LPCTSTR		  m_ssid;
	uint32_t	  m_step;
	int	  		  m_length;

	CMailBox	  m_rxMail;
	GS_RAWDATA_T  *m_rawdata;
	CTimeout	  m_timeout;

	//
	// override ATcommand's virtual functions
	//
	virtual void mode(AT_MODE_T m);
	virtual void onUnknown(uint8_t ch);
	virtual void onReceive(uint8_t ch);

	//
	// Miscellaneous
	//
	virtual bool setup();

};

#endif /* WIFI_GAINSPAN_H_ */
