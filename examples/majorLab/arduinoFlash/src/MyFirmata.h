/*
===============================================================================
 Name        : MyFirmata.h
 Author      : Jason
 Date		 : 2012/5/13
 Version     : 1.0.0
 Copyright   : Embeda Technology Inc.
 License	 : CC BY-SA
 Description : FirmataClass Extension
 History	 :
===============================================================================
*/

#ifndef MYFIRMATA_H_
#define MYFIRMATA_H_

#include "arduino\Firmata.h"

#define TOTAL_DIGITAL_PINS	14	// for UNO.Net board
#define TOTAL_ANALOG_PINS	6
#define TOTAL_PWM_PINS		4
#define TOTAL_PORTS			3
#define ANALOG_PORT			2
#define PWM_PEIROD			0.02f	// 20ms

#define PWM_PIN_START		2
#define PWM_PIN_END			5
/* ==============================================
	FirmataClass
   ============================================== */
class CMyFirmata: public FirmataClass {
public:
	// construct (board initialize)
	CMyFirmata();
	virtual void chkLoop();
	virtual void outLoop();
	virtual void checkDigitalInputs();

protected:
	int  analogInputsToReport;
	byte reportPINs[TOTAL_PORTS];
	byte previousPINs[TOTAL_PORTS]; // PIN == input port
	byte pinStatus[TOTAL_DIGITAL_PINS]; // store pin status, default OUTPUT
	byte portStatus[TOTAL_PORTS];

	void outputPort(byte portNumber, uint32_t portValue);

	/* override virtual callback functions */
	virtual void currentAnalog(byte pin, int val);
	virtual void currentDigital(byte port, int val);
	virtual void currentReportAnalog(byte pin, int val);
	virtual void currentReportDigital(byte port, int val);
	virtual void currentPinMode(byte pin, int val);

	virtual void currentSystemReset();
	virtual void currentString(LPTSTR str);
	virtual void currentSysex(byte command, byte argc, byte*argv);
};

#endif /* MYFIRMATA_H_ */
