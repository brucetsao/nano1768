/*
 ===============================================================================
 Name        : stream_spi.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/6/25
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : SPI stream with full duplex fifo
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/6/25	v1.0.0	First Edition									Jason
 ===============================================================================
 */

#ifndef SPI_STREAM_H_
#define SPI_STREAM_H_

#include "class/stream.h"
#include "class/thread.h"
#include "class/spi.h"
#include "class/pinint.h"
#include "utils/fifo.h"
#include "class/mutex.h"
#include "class/semaphore.h"

class spiStream: public CStream, public CSpiMaster, public CThread {
public:

public:
	spiStream(SPI_PORT_T port, PIN_NAME_T ssl, PIN_NAME_T weakup, int fifo_size);
	virtual ~spiStream();

	//
	// Implement CStream pure virtual functions
	//
	virtual int	 readable();
	virtual int	 writeable();
	virtual int  read(void *buf, int len, bool block=true);
	virtual int  write(const void *buf, int len, bool block=true);
	virtual int  close();
	virtual bool isConnected();

	//
	// override CSPI
	//
	virtual void enable(); 	// enable SPI
	virtual void disable();	// disable SPI

	//
	// begin to transmit tx fifo
	//

protected:
	CPinINT  	m_weakup;
	uint32_t 	m_flag;
	FIFO_T	 	m_tx_fifo;
	FIFO_T	 	m_rx_fifo;
	CMutex	 	m_txMutex;
	CSemaphore 	m_rxReady;
	CSemaphore	m_txReady;

	//
	// override member run() from CThread class
	//
	virtual void run();

	//
	// virutal function for data encode/decode
	//
	virtual void 	txEncode(uint8_t val);
	virtual bool	rxDecode(uint8_t &val);

	// tx control
	virtual void	txTrigger();	// call by End Of Write

};

#endif /* STREAM_SPI_H_ */
