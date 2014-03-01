/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

 ===============================================================================
 */
#include "uCXpresso.h"
#include "arduino.h"
#include "class/pin.h"
#include "probe/probe.h"
#include "debug.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "class/bus.h"
#include "class/timeout.h"
#include "ble/serial_ble.h"
#include "MyFirmata.h"
#include "callback.h"

//
// nRF8001 Kit
//
#define nRF_NREQ		UNO_8
#define nRF_NRDY		UNO_9
#define nRF_NRST		UNO_10

//
// Debug LEDS
//
CBus leds(LED1, LED2, LED3, LED4, END);	// debug leds

#define ledACT	leds[0]		// RF active led
#define ledCON	leds[1]		// Connected led
#define ledRXD	leds[2]		// RXD Event led
#define ledTXD	leds[3]		// TXD Ack	 led

//
// MyFirmataClass Object
//
MyFirmataClass myFirmata;

//
// myBLE class
//
class myBLE: public CSerialBLE {
public:
	//
	// Construct for myBLE class
	//     set the Vendor Name, Device Name, Device Version
	//
	myBLE(): CSerialBLE("uCXpresso", "BleFirmata", 0x20140101) {
		// nothing
	}

	//
	// Override the CSerialBLE::onConnected()
	//
	virtual void onConnected() {
		CSerialBLE::onConnected();

		// your connected event code here
		ledCON = LED_ON;	// turn ON the Connected LED
	}

	//
	// Override the CSerialBLE::onDisconnected()
	//
	virtual void onDisconnected() {
		CSerialBLE::onDisconnected();

		// your disconnect event code here
		systemResetCallback();
		ledCON = LED_OFF;	// turn OFF all LEDS
		ledRXD = LED_OFF;
		ledTXD = LED_OFF;
	}

	//
	// Override the CSerialBLE::onBleRecv(...)
	//
	virtual void onBleRecv(uint8_t *buf, int len) {
		CSerialBLE::onBleRecv(buf, len);

		// your onBleRecv code here
		ledRXD.invert();	// to invert the RXD LED
	}

	//
	// Override the CSerialBLE::onBleSend()
	//
	virtual void onBleSend(uint8_t ack) {
		CSerialBLE::onBleSend(ack);

		// your onBleSend code here
		ledTXD.invert();	// to invert the TXD Ack LED
	}
};

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
	probe.start();
#endif

	//
	// your setup code here
	//
	myBLE ble;
	ble.setPins(SPI0, nRF_NREQ, nRF_NRDY);
	ble.enable();

	myFirmata.begin(ble);	// begin the Firmata Object with the ble serial stream.
	callback_init();		// initialize the callback functions for myFirmata Object.

	CTimeout t;				// a timeout counter for analog-input interval

	//
	// your loop code here
	//
	float temp;
	while (1) {
		//
		// myFirmataClass Check
		//
		if ( myFirmata.available() ) {
			do {
				myFirmata.processInput();
			} while(myFirmata.available());
		} else {

			//
			// check the Digital Input
			//
			checkDigitalInputs();

			//
			// check the Analog Input with a sampling Interval
			//
			if (t.isExpired(samplingInterval) ) {
				t.reset();
				checkAnalogInputs();
			}
		}

		ledACT = ble.isActived() ? LED_ON : LED_OFF;

#ifdef DEBUG
		//
		// ble test
		//
		switch(dbg.isAnyKey()) {
		case '0':
			ble.setRadioTxPower(BLE_TX_0dBm);
			break;
		case '1':
			ble.setRadioTxPower(BLE_TX_m6dBm);
			break;
		case '2':
			ble.setRadioTxPower(BLE_TX_m12dBm);
			break;
		case '3':
			ble.setRadioTxPower(BLE_TX_m18dBm);
			break;
		case 'w':
			ble.wakeup();
			break;
		case 'd':
			ble.disconnect(BLE_TERMINATED);
			break;

		case 't':
			temp = ble.getTemperature();
			dbg.printf("temp=%0.2f\n", temp);
			break;

		case 'v' :
			dbg.printf("nRF8001 Version=%02x\n",ble.getPhyVersion());
			break;
		}
#endif // end of DEBUG
	}
	return 0;
}
