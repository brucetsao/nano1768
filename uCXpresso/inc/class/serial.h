/*
 ===============================================================================
 Name        : serial.h
 Author      : Jason
 Version     :
 Date		 : 2013/05/24
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2011/12/18	v1.0.0	First Edition									Jason
 2012/09/27 v1.0.1	Add begin, end, available inline functions.		Jason
 2012/10/06 v1.0.2	Add FIFO Trigger-Level in settings() member.	Jason
 2012/10/12	v1.0.3	Add block option in read() and write() member	Jason
 2013/05/25 v1.0.4  Improve read block control						Jason
 	 	 	 	 	Add h/w handshaking								Jason
  ===============================================================================
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "class/stream.h"
#include "class/pin.h"
#include "utils/fifo.h"

/*! \cond PRIVATE */
extern "C" void uartTask(void *arg);
/*! \endcond */

#define DEF_UART_BAUDRATE	9600

/*! \enum SERIAL_PORT_T
 */
typedef enum {
	USB = 0,	///< port to USB (CDC Class)
	UART1,		///< port to UART1
	UART2,		///< port to UART2
	UART3,		///< port to UART3
}SERIAL_PORT_T;


/* \class CSerial serial.h "class/serial.h"
 * \brief Use the CSerial class to transceiver the serial stream.
 */
class CSerial: public CStream {
public:
	/**
	 * @brief UART Parity type definitions
	 */
	typedef enum {
		UART_PARITY_NONE 	= 0,			/*!< No parity */
		UART_PARITY_ODD,	 				/*!< Odd parity */
		UART_PARITY_EVEN, 					/*!< Even parity */
		UART_PARITY_SP_1, 					/*!< Forced "1" stick parity */
		UART_PARITY_SP_0 					/*!< Forced "0" stick parity */
	} UART_PARITY_T;

	/**
	 * @brief UART Databit type definitions
	 */
	typedef enum {
		UART_DATABIT_5		= 0,     		/*!< UART 5 bit data mode */
		UART_DATABIT_6,		     			/*!< UART 6 bit data mode */
		UART_DATABIT_7,		     			/*!< UART 7 bit data mode */
		UART_DATABIT_8		     			/*!< UART 8 bit data mode */
	} UART_DATABIT_T;

	/**
	 * @brief UART Stop bit type definitions
	 */
	typedef enum {
		UART_STOPBIT_1		= (0),   		/*!< UART 1 Stop Bits Select */
		UART_STOPBIT_2		 				/*!< UART Two Stop Bits Select */
	} UART_STOPBIT_T;


	/**
	 * @brief FIFO Level type definitions
	 */
	typedef enum {
		UART_FIFO_TRGLEV0 = 0,	/*!< UART FIFO trigger level 0: 1 character */
		UART_FIFO_TRGLEV1, 		/*!< UART FIFO trigger level 1: 4 character */
		UART_FIFO_TRGLEV2,		/*!< UART FIFO trigger level 2: 8 character */
		UART_FIFO_TRGLEV3		/*!< UART FIFO trigger level 3: 14 character */
	} UART_FITO_LEVEL_T;

public:
	/**Constructs a CSerial object.
	 * \param port is a SERIAL_PORT_T enumerations.
	 * \param fifo
	 */
	CSerial(SERIAL_PORT_T port, size_t fifo=32);

	/**Call the member function to set the serial parameters.
	 * \param baudrate is a unsigned long integer to specified the data baud-rate.
	 * \param parity is an UART_PARITY_T enumerations.
	 * \param databits is an UART_DATABIT_T enumerations.
	 * \param stopbits is an UART_STOPBIT_T enumerations.
	 * \param level is UART_FITO_LEVEL_T to trigger how many receiver UARTn FIFO characters must be written before an interrupt or DMA request is activated.
	 */
	virtual void settings(uint32_t baudrate,
						  UART_PARITY_T parity = UART_PARITY_NONE,
						  UART_DATABIT_T databits = UART_DATABIT_8,
						  UART_STOPBIT_T stopbits = UART_STOPBIT_1,
						  UART_FITO_LEVEL_T level=UART_FIFO_TRGLEV0);

	/**Call the member function to check that receive buffer is ready to read.
	 * \return greater than zero if receive data from serial port.
	 */
	virtual int	readable();

	/**Call the member function to check that transmit buffer is ready to write.
	 * \return how many bytes can be write to transmit buffer.
	 */
	virtual int	writeable();

	/**Call the member function to read a data block from serial port.
	 * \param[out] buf is a pointer which contain the data block from serial port.
	 * \param[in] len is a integer to specified the buffer length.
	 * \param[in] block is a boolean value to specified to wait for reading.
	 */
	virtual int	read(void *buf, int len, bool block=true);

	/**Call the member function to write a data block to serial port
	 * \param[in] buf is a pointer which data block want to send to serial port.
	 * \param[in] len is a integer value to specified the buffer length.
	 * \param[in] block is a boolean value to specified to wait for writing.
	 */
	virtual int	write(const void *buf, int len, bool block=true);

	/**Call the member function to wait for transmission  of outgoing serial data to complete.
	 */
	virtual void flush();

	/**Call the member function to close the serial port.
	 * \return always return zero (ERR_OK).
	 */
	virtual int close();

	/**Call the member function to begin the serial port.
	 * \note The 'begin' is an inline code to call the exist settings() member function.
	 */
	inline virtual void begin(uint32_t speed) {
		settings(speed);
	}

	/**Call the member function to end the serial port.
	 * \note The 'end' is an inline code to call the exist close() member function.
	 */
	inline virtual void end() {
		close();
	}

	/**Call the member function to check that receive buffer is ready.
	 * \note The 'available' is an inline code to call the exist readable() member function.
	 */
	inline virtual int available() {
		return readable();
	}

	/**Call the member function to check that USB serial port is ready.
	 * \note The 'isConnected' is for USB port only in the CSerial class.
	 */
	virtual bool isConnected();

	/**Enable hardware handshaking
	 * \node The handshaking is for UART1~3 port only
	 */
	void handshaking(PIN_NAME_T rts, PIN_NAME_T cts);

	/*! \cond PRIVATE */
public:
	CSerial();
	virtual ~CSerial();
	virtual void assign(SERIAL_PORT_T port, size_t fifo_size);
	virtual void alloc(int bufsize=256) {};
	inline CPin& rts() { return m_rts; }
	inline CPin& cts() { return m_cts; }
	inline bool flag(uint32_t val) { return bit_chk(m_nFlag, val); }

protected:
	int 	 m_port;
	FIFO_T 	 m_txFifo;
	FIFO_T 	 m_rxFifo;
	uint32_t m_nFlag;
	CPin	 m_rts, m_cts;

private:
	xHandle  m_handle;
	friend void uartTask(void *arg);
	/*! \endcond */
};


class CIrDA: public CSerial {
public:
	/**
	 * UART IrDA Control type Definition
	 */
	typedef enum {
		UART_IrDA_PULSEDIV2		= 0,		/**< Pulse width = 2 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV4,				/**< Pulse width = 4 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV8,				/**< Pulse width = 8 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV16,				/**< Pulse width = 16 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV32,				/**< Pulse width = 32 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV64,				/**< Pulse width = 64 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV128,				/**< Pulse width = 128 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
		UART_IrDA_PULSEDIV256				/**< Pulse width = 256 * Tpclk
											- Configures the pulse when FixPulseEn = 1 */
	} UART_IrDA_PULSE_T;

public:
	CIrDA(){}
	CIrDA(SERIAL_PORT_T port, size_t fifo_size=32);

	// for UART
	virtual void settings(uint32_t baudrate,
						  UART_PARITY_T parity = UART_PARITY_NONE,
						  UART_DATABIT_T databits = UART_DATABIT_8,
						  UART_STOPBIT_T stopbits = UART_STOPBIT_1);

	virtual void invertInput(bool NewState);
	virtual void pulseDiv(UART_IrDA_PULSE_T PulseDiv);
};



#endif /* STREAM_H_ */
