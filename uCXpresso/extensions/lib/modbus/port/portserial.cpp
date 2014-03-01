/*
 ===============================================================================
 Name        : portserial.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/4
 License     : CC BY-SA 3.0
 Description : serial port for modbus
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/4	v1.0.0	First Edition.									Jason
 */

#include "class/thread.h"
#include "class/serial.h"
#include "mb.h"

//
// defines
//

class mbSerial: public CThread {
public:
	mbSerial() {
		m_serial = NULL;
	}

	~mbSerial() {
		if (m_serial)
			delete m_serial;
	}

	void settings(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
			eMBParity eParity) {
		if (ucPORT <= 3) {
			m_serial = new CSerial((SERIAL_PORT_T) ucPORT);
			if (m_serial) {
				CSerial::UART_DATABIT_T d =
						(CSerial::UART_DATABIT_T) (ucDataBits - 5);
				m_serial->settings(ulBaudRate, (CSerial::UART_PARITY_T) eParity,
						d, CSerial::UART_STOPBIT_1);
			}
		}
	}

	bool start() {
		// TODO: check stack size
		return CThread::start("mbSer", 96);
	}

	inline bool putByte(CHAR ucByte) {
		if (m_serial->write(&ucByte, 1) == 1) {
			return true;
		}
		return false;
	}

	inline bool getByte(CHAR *pucByte) {
		if (m_serial->read(pucByte, 1) == 1) {
			return true;
		}
		return false;
	}

protected:
	CSerial *m_serial;
	virtual void run() {
		while (1) {
			if (m_serial) {
				if (m_serial->readable()) {
					pxMBFrameCBByteReceived();
				}

				if (m_serial->writeable()) {
					pxMBFrameCBTransmitterEmpty();
				}
			}
		}
	}
};

#define SERIAL(x) (static_cast<mbSerial*>(x))

//
// Static variables
//
static mbSerial *xSerial = NULL;

//
// Start implementation
//
PR_BEGIN_EXTERN_C

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity) {
	xSerial = new mbSerial;
	// start port
	if (xSerial) {
		SERIAL(xSerial)->settings( ucPORT, ulBaudRate, ucDataBits, eParity);
		return SERIAL(xSerial)->start();
	}
	return false;
}

void vMBPortClose(void) {
	if (xSerial) {
		delete xSerial;
	}
}

BOOL xMBPortSerialPutByte(CHAR ucByte) {
	return SERIAL(xSerial)->putByte( ucByte );
}

BOOL xMBPortSerialGetByte(CHAR * pucByte) {
	return SERIAL(xSerial)->getByte( pucByte );
}

PR_END_EXTERN_C
