/*
 ===============================================================================
 Name        : i2c.h
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2012/3/20
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : I2C data transceiver
 History     :
 ===============================================================================
 */

#ifndef I2C_H_
#define I2C_H_

#include "peripheral.h"

typedef enum {
	I2C0 = 0,
	I2C1
}I2C_PORT_T;

class CI2C: public CPeripheral {
protected:
	int m_port;

public:
	CI2C();
	virtual ~CI2C();
	virtual void assign(I2C_PORT_T port, uint32_t clockrate=KHZ(100));
	virtual void begin();
	virtual void end();
};

class CI2CMaster: public CI2C {
public:
	CI2CMaster();
	CI2CMaster(I2C_PORT_T port, uint32_t clockrate=KHZ(100));

	virtual int readwrite(uint8_t slaveAddr, void *txbuf, int txsize, void *rxbuf, int rxsize);
	virtual int write(uint8_t slaveAddr, void *txbuf, int txsize);
	virtual int read(uint8_t slaveAddr, void *rxbuf, int rxsize);
};

#endif /* I2C_H_ */
