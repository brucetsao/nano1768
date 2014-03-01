/*
 ===============================================================================
 Name        : adc.h
 Author      : Jason
 Version     : v1.0.5
 Date		 : 2013/1/7
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : The CAdc Interface is used to read the voltage applied to
 	 	 	   an analog input pin.
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/12/18 v1.0.0	First Edition									Jason
 2012/9/10	v1.0.1	Use DMA											Jason
 2012/9/20	v1.0.2	Add Median Filter								Jason
 2012/10/3	v1.0.3	Add ADCBus for mulit ADC pins.					Jason
 2012/10/13 v1.0.4	Remove DMA mode...								Jason
 2013/01/07 v1.0.5	Change float operator to 0.0~3.3V				Jason
 ===============================================================================
 */

#ifndef ADC_H_
#define ADC_H_

#include <cstdarg>
#include "class/peripheral.h"
#include "class/mutex.h"
#include "class/timer.h"

/*! \def MAX_ADC_VALUE
	Indicates the ADC maximum value
 */
#define MAX_ADC_VALUE		4095.0

/*! \enum ADC_CH_T
 */
typedef enum {
	AD0 = 0,	///< pin name to AD0 (NANO1768.P15)
	AD1,		///< pin name to AD1 (NANO1768.P16)
	AD2,		///< pin name to AD2 (NANO1768.P17)
	AD3,		///< pin name to AD3 (NANO1768.P18)
	AD4,		///< pin name to AD4 (NANO1768.P19)
	AD5			///< pin name to AD5 (NANO1768.P20)
}ADC_CH_T;
#define MAX_ADC_PIN	((int)AD5)

/*!	\class CAdc adc.h "class/adc.h"
	\brief an Analog-to-Digital converter class.
 */
class CAdc: public CPeripheral {
public:

	/**Constructs a CAdc object with an AD pin.
	 * \param ch is a ADC_CH_T to specified an AD pin to connected to the object.
	 *
	 * \code
	 * Example:
	 *		CAdc ad(AD5);
	 *		ad.begin();
	 * \endcode
	 */
	CAdc(ADC_CH_T ch);

	/** Enable the ADC function on the object
	 */
	virtual void enable();

	/** Disable the ADC function on the object
	 */
	virtual void disable();

	/** Read the ADC value from the AD pin
	 * \return 12 bits resolution (0~4095) ADC value
	 */
	virtual int read();

	/** Read the ADC value from the AD pin with Compare-Filter
	 * \param[in] filter a float value to identify the offset range with last value. (0.1 = 10%, 0.25=25%)
	 * \param[in] count a integer value to identify to retry count if ADC value over the filter range. (default value is 3)
	 * \return 12 bits resolution (0~4095) ADC value
	 *
	 * \code
	 * 	Example:
	 * 		CAdc ad(AD0);
	 * 		ad.begin();
	 * 		int value = ad.read(0.25, 3);	// if AD value over 25% with last value, try to read the AD value 3 times.
	 * \endcode
	 */
	virtual int read(float filter, int count=3);

	/** Read the ADC value from the AD pin with Median-Filter
	 * \param[in] samples a integer to identify how many ADC value read in sample buffer.
	 * \param[in] interval a CTimer object to provide the interval timer service.
	 * \return 12 bits resolution (0~4095) ADC value
	 *
	 * \code
	 * Example:
	 * 		CAdc ad(AD0);
	 * 		ad.begin();
	 *
	 * 		CTime t(TIMER0);
	 * 		t.setting(10, 50);			// 10x50 = 500us interval
	 * 		t.begin();
	 *
	 * 		int value = ad.read(5, t);	// ADC value with Median-Filter, total 5 samples, interval 500us
	 *
	 * \endcode
	 * <a href="http://en.wikipedia.org/wiki/Median_filter">WiKi for Median Filter</a>
	 */
	virtual int read(int samples, CTimer &interval);

	/** A shorthand operator code to read ADC value
	 * \return a float value, divide by MAX_ADC_VALUE
	 *
	 * \code
	 * 	Example:
	 * 		CAdc ad(AD5);
	 * 		ad.begin();
	 *
	 * 		float value = ad;
	 * \endcode
	 */
	virtual inline operator float() { return map( (float)read(0.2, 3), 0.0f, MAX_ADC_VALUE, 0.0f, 3.3f); }

	inline void begin() { enable(); }
	inline void end()	{ disable(); }

	/** A global ADC power control
	 * \param flag a boolean value, true=power down, false=power up.
	 * \return none
	 */
	static void	 setAllPowerDown(bool flag);

	/*! \cond PRIVATE */
	CAdc();
	virtual ~CAdc();
	virtual void assign(ADC_CH_T ch);

protected:
	int m_nAdcCH;
private:
	uint32_t 	m_lastVal;
	uint32_t	m_value;
	/*! \endcond */
};

/*! \class ADCBus adc.h "class/adc.h"
 * \brief The ADCBus class provides the multi channels ADC service.
 */
class ADCBus: public CPeripheral {
public:
	/**Constructs a ADCBus object.
	 * \param pin are ADC pin name (ADC_CH_T).
	 */
	ADCBus(ADC_CH_T pin, ...);

	/**Call the member function to enable all the ADC channels of object.
	 */
	void enable();

	/**Call the member function to disable all the ADC channels of object.
	 */
	void disable();

	/**Call the member function to read the specified channel value from m_value buffer.
	 * \return integer value (0~4095)
	 */
	virtual int value(int index);

	/**use the array operator to retrieve the CAdc object.
	 */
	inline CAdc* operator [](int index) { return &m_adcs[index]; }


	/**Call the member function to retrieve the number of channels in the object.
	 */
	inline int count()		{ return m_count; }

	/*! \cond PRIVATE */
	inline void begin() 	{ enable(); }
	inline void end()   	{ disable(); }
	~ADCBus();
protected:
	CAdc *m_adcs;
	int	 m_count;
	/*! \endcond */
};

#endif /* ADC_H_ */
