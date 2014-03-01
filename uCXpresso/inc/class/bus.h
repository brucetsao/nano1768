/*
 ===============================================================================
 Name        : bus.h
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2012/1/2
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/1/2	v1.0.0	first edition									Jason
 2012/9/13	v1.0.1	rename pinCount() to count()					Jason
 2012/10/15 v1.0.2	change output() default to NOT_OPEN				Jason
 ===============================================================================
 */

#ifndef BUS_H_
#define BUS_H_

#include <cstdarg>
#include "class/object.h"
#include "class/pin.h"

/*! \class CBus bus.h "class/bus.h"
 * \brief A digital input/output bus, used for reading/writing the state of a collection of pins.
 */
class CBus: public CObject {
public:
	/**Constructs a CBus object to connect to the specified pins.
	 * \param pin ... are PIN_NAME_T to specified one or more pins to the CBus.
	 *
	 * \code
	 * Example:
	 * 		CBus bus(P21, P22, P23, P24, END);	// Collect P21~P24 in the CBus object.
	 * 		bus.output();						// Set the bus as output pins.
	 * \endcode
	 * \remark to 'END' of the pin arguments is MUST!!
	 */
	CBus(PIN_NAME_T pin,...);

	/**Call the member function to set the bus as output pins.
	 * \param mode is a PIN_OUTPUT_MODE_T, default is OPEN_DRAIN (provide current).
	 */
	void output(PIN_OUTPUT_MODE_T mode=NOT_OPEN);		// set as an output pin

	/**Call the member function to set the bus as input pins
	 * \param mode is a PIN_INPUT_MODE_T, default is INTERNAL_PUTT_UP.
	 */
	void input(PIN_INPUT_MODE_T mode=INTERNAL_PULL_UP);	// set as an input pin

	/**Call the member function to write a value to the bus.
	 * \param val is a unsigned integer to map the bit wide value to pins.
	 */
	virtual void write(uint32_t val);

	/**Call the member function to retrieve a value from pins.
	 * \return a unsigned integer value that map the bit-wide of pins.
	 */
	virtual uint32_t read();

	/**Call the member function to retrieve a identity pin from bus.
	 * \return CPin
	 */
	CPin&	pin(int index);

	/**A shorthand for read
	 *
	 * \code
	 * Example:
	 * 		CBus bus(P21, P22, P23, P24, END);	// Collect P21~P24 in the CBus object.
	 * 		bus.input();						// Set the bus as input pins.
	 * 		uint32_t val = bus;					// Read a value from bus.
	 * \endcode
	 */
	inline operator uint32_t() { return read(); }

	/**A shorthand for write
	 *
	 * \code
	 * Example:
	 * 		CBus bus(P21, P22, P23, P24, END);	// Collect P21~P24 in the CBus object.
	 * 		bus.output();						// Set the bus as output pins.
	 * 		bus = 0x05;							// Write 0x05 to bus.
	 * \endcode
	 */
	inline void operator = (uint32_t val) { write(val); }

	/**A shorthand for equal to...
	 * \return true if the bus==val, otherwise, failed.
	 */
	inline bool operator == (uint32_t val) { return (read()== val ? true : false); }

	/**A shorthand for not equal to...
	 * \return true if the bus not equal the val; otherwise, failed.
	 */
	inline bool operator != (uint32_t val) { return (read()!= val ? true : false); }

	/**A shorthand for array
	 * \return CPin
	 *
	 * \code
	 * Example:
	 * 		CBus bus(P21, P22, P23, P24, END);	// Collect P21~P24 in the CBus object.
	 * 		bus.output();						// Set the bus as output pins.
	 *		bus[2] = LOW;						// Set P23 is LOW.
	 * \endcode
	 */
	inline CPin& operator [] (int index)   { return pin(index); }

	/**Call the member function to retrieve the number of pins in the bus.
	 * \return integer value.
	 */
	inline int	count() { return m_nPinCount; }

	/*! \cond PRIVATE */
	CBus();
	virtual ~CBus();
	void assign(PIN_NAME_T pin, ...);
	void assign(PIN_NAME_T p, va_list pins);


protected:
	CPin    *m_pins;
	int		m_nPinCount;

	void alloc(va_list pins);
	void free();
	/*! \endcond */
};
#endif /* BUS_H_ */
