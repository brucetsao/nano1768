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
#include "arduino.h"
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
#include "class/json.h"

//
// TODO: insert other definitions and declarations here
//
static char jsonTest[] = "{\"myCats\":["
	"{\"name\":\"Hero\", \"age\":5, \"color\":\"silver\"},"
	"{\"name\":\"Eero\", \"age\":2, \"color\":[\"brown\", \"white\", \"black\"]}"
	"]}";


void json_test() {
	dbg.waitToDebugMode();
	CJson json(jsonTest);
	if ( json.error()==ERR_OK ) {
		dbg.println( json["myCats"][0]["age"].asInteger() );
		dbg.println( json["myCats"][1]["color"][2].asString());
	} else {
		dbg.println(json.errorDesc());
	}
}

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
#endif

	// Simple demo Code (removable)
	CPin led(LED1);

	// TODO: insert setup code here
	json_test();

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);

		// Your loop code here
	}
	return 0;
}
