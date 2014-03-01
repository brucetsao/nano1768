/*
 ===============================================================================
 Name        : main.cpp
 Author      :
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
 History	 :
 ===============================================================================
 */
#include "uCXpresso.h"
#include "class/pin.h"

#include "debug.h"
CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "arduino.h"
#include "sensor/SHT1x.h"

//
// TODO: insert other definitions and declarations here
//
SHT1x sht1x(8, 9);

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif
	// TODO: insert setup code here
	float temp_c;
	float temp_f;
	float humidity;

	// Enter an endless loop
	while (1) {
		// Read values from the sensor
		sht1x.update();
		temp_c = sht1x.readTemperatureC();
		temp_f = sht1x.readTemperatureF();
		humidity = sht1x.readHumidity();

		// Print the values to the serial port
		dbg.println("Temperature:%0.2fC (%0.2f) / Humidity:%0.2f", temp_c, temp_f, humidity);
		// */
	}
	return 0;
}
