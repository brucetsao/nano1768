/*
 ===============================================================================
 Name        : stream_spi.h
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/6/28
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : SPI stream for GS1000 series
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/6/28	v1.0.0	First Edition									Jason
 ===============================================================================
 */

#ifndef STREAM_SPI_H_
#define STREAM_SPI_H_

#include "class/stream/spi_stream.h"

class spiGS: public spiStream {
public:

public:
	spiGS(SPI_PORT_T port, PIN_NAME_T ssl, PIN_NAME_T weakup, int fifo_size);
	virtual ~spiGS();

protected:
	bool	 	m_escape;
	//
	// override SPI encode/decode for GS1011M
	//
	void 	txEncode(uint8_t val);
	bool	rxDecode(uint8_t &val);

	//
	// override spiStream::run()
	//
	virtual void run();
};

#endif /* STREAM_SPI_H_ */
