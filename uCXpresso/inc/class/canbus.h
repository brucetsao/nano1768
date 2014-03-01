/*
 ===============================================================================
 Name        : canbus.h
 Author      : Jason
 Version     : 1.0.2
 Date	 	 : 2012/2/28
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : 
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/2/28	v1.0.0	First Edition									Jason
 2012/4/19  v1.0.1	move CAN_MSG_T to canmsg.h						Jason
 2012/9/27	v1.0.2  Remove the canmsg.h								Jason
 	 	 	 	 	Add begin, end, available inline member functions.
  ===============================================================================
 */

#ifndef CANBUS_H_
#define CANBUS_H_

#include "class/peripheral.h"

/*! \cond PRIVATE */
extern "C" void canTask(void *arg);
/*! \endcond */

/*! \enum CAN_PORT_T
 */
typedef enum {
	CAN0 = 0,	///< CAN0 (P9=RD, P10=TD)
	CAN1		///< CAN1 (P30=RD, P29=TD)
}CAN_PORT_T;


/*! \struct _can_msg_type_ canbus.h "class/canbus.h"
 * \brief CAN message object structure
 */
struct _can_msg_type_{
	uint32_t id; 			/**< 29 bit identifier, it depend on "format" value
								 - if format = STD_ID_FORMAT, id should be 11 bit identifier
								 - if format = EXT_ID_FORMAT, id should be 29 bit identifier
							 */
	uint8_t dataA[4]; 		/**< Data field A */
	uint8_t dataB[4]; 		/**< Data field B */
	uint8_t len; 			/**< Length of data field in bytes, should be:
								 - 0000b-0111b: 0-7 bytes
								 - 1xxxb: 8 bytes
							*/
	uint8_t format; 		/**< Identifier Format, should be:
								 - STD_ID_FORMAT: Standard ID - 11 bit format
								 - EXT_ID_FORMAT: Extended ID - 29 bit format
							*/
	uint8_t type; 			/**< Remote Frame transmission, should be:
								 - DATA_FRAME: the number of data bytes called out by the DLC
								 field are send from the CANxTDA and CANxTDB registers
								 - REMOTE_FRAME: Remote Frame is sent
							*/
}PACK_STRUCT;
typedef struct _can_msg_type_ CAN_MSG_T;


/** \class CANBus canbus.h "class/canbus.h"
 * \brief Use the CANBus class to transceiver the CAN-BUS message.
 */
class CANBus: public CPeripheral {
public:
	/**
	 * @brief CAN ID format definition
	 */
	typedef enum {
		STD_ID_FORMAT = 0, 	/**< Use standard ID format (11 bit ID) */
		EXT_ID_FORMAT = 1	/**< Use extended ID format (29 bit ID) */
	} CAN_ID_FORMAT_T;

	/**
	 * @brief CAN Mode Type definition
	 */
	typedef enum {
		OPERATING_MODE = 0, 	/**< Operating Mode */
		RESET_MODE, 			/**< Reset Mode */
		LISTENONLY_MODE, 		/**< Listen Only Mode */
		SELFTEST_MODE, 			/**< Seft Test Mode */
		TXPRIORITY_MODE, 		/**< Transmit Priority Mode */
		SLEEP_MODE, 			/**< Sleep Mode */
		RXPOLARITY_MODE, 		/**< Receive Polarity Mode */
		TEST_MODE				/**< Test Mode */
	} CAN_MODE_T;

	/**
	 * @brief Error values that functions can return
	 */
	typedef enum {
		OK = 1, 				/**< No error */
		OBJECTS_FULL_ERROR, 	/**< No more rx or tx objects available */
		FULL_OBJ_NOT_RCV, 		/**< Full CAN object not received */
		NO_RECEIVE_DATA, 		/**< No have receive data available */
		AF_ENTRY_ERROR, 		/**< Entry load in AFLUT is unvalid */
		CONFLICT_ID_ERROR, 		/**< Conflict ID occur */
		ENTRY_NOT_EXIT_ERROR	/**< Entry remove outo AFLUT is not exit */
	} CAN_ERROR_T;

public:
	/**Constructs a CANBus object.
	 * \param port is a CAN_PORT_T enumerations.
	 */
	CANBus(CAN_PORT_T port);

	/**Call the member function to close the CAN-BUS port.
	 * \return always return zero.
	 */
	virtual int  close();

	/**Call the member function to set the CAN-BUS mode.
	 * \param mode is a CAN_MODE_T enumerations.
	 * \param state is a boolean, true is enabled, false is disabled.
	 */
	virtual void mode(CAN_MODE_T mode, bool state);

	/**Call the member function to reset the CAN-BUS port.
	 */
	virtual void reset();

	/**Call the member function to check that receive buffer is ready to read.
	 * \return true if a message is ready for read.
	 */
	virtual bool readable();

	/**Call the member function to check that transmit buffer is ready to write.
	 * \return true if buffer is ready for write.
	 */
	virtual bool writeable();

	/**Call the member function to read a message from the CAN-BUS.
	 *	\param[in] msg is a CAN_MSG_T pointer which receive the message from the CAN-BUS.
	 *	\param[in] timeout is a time to wait for read in millisecond.
	 *	\return zero or less if failed; otherwise, number of bytes in the received message.
	 */
	virtual int	 read(CAN_MSG_T *msg, uint32_t timeout=MAX_DELAY_TIME);

	/**Call the member function to write a message to the CAN-BUS.
	 * \param[out] msg is a CAN_MSG_T pointer which transmit the message to the CAN-BUS.
	 * \return zero or less if failed; otherwise, number of bytes in the transmited message.
	 */
	virtual int	 write(CAN_MSG_T *msg);

	/**Call the member function to retrieve the current value of the Receive Error Count.
	 * \return integer value.
	 */
	virtual int  rxerror();

	/**Call the member function to retrieve the current value of the Transmit Error Count.
	 * \return integer value.
	 */
	virtual int  txerror();

	/**Call the member function to begin the serial port.
	 * \note The 'begin' is an inline code to call the exist settings() member function.
	 */
	virtual void begin(uint32_t speed);

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

	/*! \cond PRIVATE */
	CANBus();
	virtual void assign(CAN_PORT_T port, uint32_t baudrate);
	virtual ~CANBus();

protected:
	CAN_PORT_T	m_port;
	/*! \endcond */
};

#endif /* CANBUS_H_ */
