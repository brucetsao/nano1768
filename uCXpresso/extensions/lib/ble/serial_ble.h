/*
 ===============================================================================
 Name        : serial_ble.h
 Author      : Jason
 Version     : 1.0.2
 Date		 : 2014/1/4
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : Bluetooth Low Energy (for Nodic nRF8001)
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/1/1	v1.0.0	Initialize										Jason
 2014/1/3	v1.0.1  for nano1768 (extensions)						Jason
 2014/1/4	v1.0.2	add onAckTimeout() event for ACK timeout		Jason
 ===============================================================================
 */

#ifndef SERIAL_BLE_H_
#define SERIAL_BLE_H_

#include "class/stream.h"
#include "class/thread.h"
#include "class/spi.h"
#include "class/pin.h"
#include "class/semaphore.h"
#include "class/timeout.h"

typedef enum {
	BLE_TERMINATED =1,
	BLE_UNACCEPTABLE = 2
} BLE_DISCONNECT_REASON_T;

typedef enum {
	BLE_TX_m18dBm 	= 0,	// -18dBm  	(Low)
	BLE_TX_m12dBm 	= 1,	// -12dBm
	BLE_TX_m6dBm	= 2,	// -6dBm
	BLE_TX_0dBm 	= 3		//  0dBm	(High)
}BLE_TX_POWER_T;

#define BLE_DRIVERVERSION	0x20140104
#define DEF_BLE_VENDORNAME	"uCXpresso"
#define DEF_BLE_DEVICENAME	"uCXpresso.BLE"

//
// CSerialBLE stream class
//
class CSerialBLE: public CStream, public CThread {
public:
	CSerialBLE(LPCTSTR vendorName=DEF_BLE_VENDORNAME,
			   LPCTSTR deviceName=DEF_BLE_DEVICENAME,
			   uint32_t deviceVersion=BLE_DRIVERVERSION);

	void setPins(SPI_PORT_T spi, PIN_NAME_T nReq, PIN_NAME_T nRdy, PIN_NAME_T nAct=END, PIN_NAME_T nRst=END);

	//
	// Controls
	//
	void enable();
	void disable();

	//
	// PHY functions
	//
	bool	sleep();
	bool	wakeup();
	bool	isActived();	// RF front end activity indicator

	bool	disconnect(BLE_DISCONNECT_REASON_T reason=BLE_TERMINATED);
	bool	setRadioTxPower(BLE_TX_POWER_T power);

	uint8_t getPhyVersion();
	float 	getTemperature();
	float	getBatteryLevel();

	/**SetApplLatency subrates the slave latency.
	 * nRF8001 will listen on each subrated interval and only acknowledge the received packet from the central device
	 * if it has its MD (More Data) bit set to 1 or contains data.
	 */
	bool	setAppLatency(bool enable, uint16_t latency=0);

	/**Broadcast enables a pipe to start sending advertisement data
	 * on non-connectable advertisement packets.
	 *
	 * \param timeout : Time, in seconds, to advertise before exiting to Standby mode
	 * \param advInterval: Advertising interval timing to be used
	 * \return true/false
	 */
	bool	broadcast(uint16_t timeout, uint16_t advInterval);

	//
	// Events
	//
	virtual void onConnected();
	virtual void onDisconnected();

	//
	// Implement the virtual functions of CStream class
	//
	virtual int	 readable();
	virtual int	 writeable();
	virtual int  read(void *buf, int len, bool block=true);
	virtual int  write(const void *buf, int len, bool block=true);
	virtual bool isConnected();
	virtual void flush();
	virtual int close();

	//
	// PRIVATE, internal use
	//
	virtual ~CSerialBLE();
	virtual void onResponseCommandHook(void *data);
	virtual void onBleRecv(uint8_t *buf, int len);
	virtual void onBleSend(uint8_t ack);
	virtual void onAckTimeout();

	LPCTSTR	 m_deviceName;
	uint32_t m_deviceVersion;
	LPCTSTR	 m_vendorName;
protected:
	//
	// Implement the virtual function of CThread class
	//
	virtual void run();
			void reset();

private:
	CTimeout	m_tmACK;
	CSemaphore	m_semResponse;
	xHandle		m_data;
	PIN_NAME_T	m_active;
	PIN_NAME_T	m_reset;
};

//
// PRIVATE (Internal Use)
//
extern CSerialBLE *objBLE;

#endif /* SERIAL_BLE_H_ */
