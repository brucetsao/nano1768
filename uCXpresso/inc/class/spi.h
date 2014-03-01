/*
 ===============================================================================
 Name        : spi.h
 Author      : Jason
 Version     : v1.0.4
 Date		 : 2013/12/20
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2011/12/18	v1.0.0	First Edition									Jason
 2012/11/02 v1.0.1	Add Master/Slave options in CSPI class.			Jason
 2013/06/19 v1.0.2	Add readwrite member for byte					Jason
 2013/09/15 v1.0.3	Add bitOrder member for bit sequence of byte	Jason
 2013/12/20 v1.0.4	Add write() member in CSPI class				Jason
 2014/1/1	v1.0.5
 ===============================================================================
 */

#ifndef SPI_H_
#define SPI_H_

#include "class/peripheral.h"
#include "class/pin.h"

/**********************************************************
 SPI Data Bits between the 8 and 16
 **********************************************************/
typedef enum {
	SPI0 = 0,
	SPI1 = 1
}SPI_PORT_T;

/**********************************************************
 SPI Data Bits between the 8 and 16
 **********************************************************/
typedef enum {
	SPI_DATABIT_8	= 8,	// Default
	SPI_DATABIT_9 	= 9,
	SPI_DATABIT_10 	= 10,
	SPI_DATABIT_11 	= 11,
	SPI_DATABIT_12 	= 12,
	SPI_DATABIT_13 	= 13,
	SPI_DATABIT_14 	= 14,
	SPI_DATABIT_15 	= 15,
	SPI_DATABIT_16 	= 0
}SPI_DATABIT_T;

/**********************************************************
 SPI Data To Clock Phase Relationship
	mode | POL PHA
	-----+---------
	  0  |  0   0  (Default)
	  1  |  0   1
	  2  |  1   0
	  3  |  1   1
 **********************************************************/
typedef enum {
	SPI_MODE_0 = 0,
	SPI_MODE_1,
	SPI_MODE_2,
	SPI_MODE_3
}SPI_MODE_T;

typedef enum {
	SPI_MSBFIRST = 0,	// default
	SPI_LSBFIRST = 1
}SPI_BIT_ORDER_T;

typedef enum {
	SPI_MASTER = 1,
	SPI_SLAVE = 0
}SPI_ROLE_T;


// *****************************************************************************
// Class Name: CSPI
// Description:
// The SPI base class to define hardware interface
// Default frequency = 2MHz
// *****************************************************************************
class CSPI: public CPeripheral {
public:
	CSPI();
	virtual ~CSPI();

	/*
	 * CSPI::assign(SPI_PORT port)
	 * 		Descriptions:
	 * 			To assign a SPI port for the class
	 * 		Parameters:
	 * 			port: SPI0 or SPI1
	 * 		Return:
	 * 			bool : true = successful.
	 */
	void assign(SPI_PORT_T port, SPI_ROLE_T role=SPI_MASTER);

	/*
	 * CSPI::format(SPI_DATABIT_T bits, SPI_MODE_T mode)
	 *		Descriptions:
	 *			To set the SPI interface format.
	 *		Parameters:
	 *			bits: SPI_DATABIT_8~SPI_DATABIT_16 (Default 8)
	 *			mode: SPI Data To Clock Phase Relationship:
	 *					mode | POL PHA
	 *					-----+---------
	 * 	  	  	  	  	  0  |  0   0  (Default)
	 * 	  	  	  	  	  1  |  0   1
	 * 	  	  	  	  	  2  |  1   0
	 *					  3  |  1   1
	 *		Return:
	 *			None (void)
	 */
	void format(SPI_DATABIT_T bits, SPI_MODE_T mode=SPI_MODE_0);

	/*
	 * CSPI::bitOrder(SPI_BIT_ORDER value)
	 * 		Descriptions:
	 * 			To set the SPI bit sequence of data-byte.
	 *
	 * 		Parameter:
	 * 			SPI_MSBFIRST (Default)
	 * 			SPI_LSBFIRST
	 *
	 * 		Return:
	 * 			None.
	 */
	void bitOrder(SPI_BIT_ORDER_T value);

	/*
	 * CSPI::frequency(hz)
	 * 		Descriptions:
	 * 			Set the SPI data clock rate
	 * 		Parameters:
	 * 			hz = a clock rate, (Default 2Mhz, Max 25Mhz)
	 * 		Return:
	 * 			None (void)
	 */
	void frequency(uint32_t hz);

	virtual void enable(); 	// enable SPI
	virtual void disable();		// disable SPI

	inline void begin() { enable(); }
	inline void end() { disable(); }

public:
	virtual int readwrite(void *txbuf, void *rxbuf, uint32_t length);

	//
	// inline functions
	//
	inline int readwrite(uint8_t tx, uint8_t &rx) {
		return readwrite(&tx, &rx, 1);
	}

	inline int write(uint8_t tx) {
		uint8_t rx=0;
		readwrite(&tx, &rx, 1);
		return rx;
	}

	//
	// shorthand operator
	//
	inline void operator >> (uint8_t &val) { readwrite(NULL, &val, 1); }
	inline void operator << (uint8_t val)  { readwrite(&val, NULL, 1); }
	inline void operator >> (uint16_t &val) { readwrite(NULL, &val, 2); }
	inline void operator << (uint16_t val)  { readwrite(&val, NULL, 2); }
	inline void operator >> (uint32_t &val) { readwrite(NULL, &val, 4); }
	inline void operator << (uint32_t val)  { readwrite(&val, NULL, 4); }

protected:
	xHandle 	m_hSPI;
	uint32_t	m_nFlag;
};

// *****************************************************************************
// Class Name: CSPI
// Description:
// The SPI base class to define hardware interface
// Default frequency = 2MHz
// *****************************************************************************
class CSpiMaster: public CSPI {
protected:
	CPin		m_cs;	// chip select pin

public:
	CSpiMaster() {}
	/*
	 * CSpiMaster Construct
	 * 		Descriptions:
	 * 			To assign a SPI port and chip select pin for the class
	 * 		Parameters:
	 * 			port: SPI0 or SPI1
	 * 			csPin: chip select pin
	 * 		Return:
	 * 			None
	 */
	CSpiMaster(SPI_PORT_T port, PIN_NAME_T selPin);
	virtual void assign(SPI_PORT_T port, PIN_NAME_T selPin);

	/*
	 * CSpiMaster::readwrite(void *txbuf, void *rxbuf, uint32_t length)
	 * 		Descriptions:
	 *			SPI read/write data function.
	 * 		Parameters:
	 *			txbuf : Pointer to transmit data
	 *			rxbuf : Pointer to receive data
	 *			length : Length of transfer data
	 * 		Return:
	 *			Actual Data length has been transferred (int).
	 */
	virtual int readwrite(void *txbuf, void *rxbuf, uint32_t length);
};

#endif /* SPI_H_ */
