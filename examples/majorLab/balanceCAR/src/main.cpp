/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2012/8/7
 Copyright   :
 License	 : CC BY_SA 3.0
 Description : main definition
 History	 : Beta
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
#include "dcmotor.h"
#include "arduino.h"

#include "sensor/adxl345.h"
#include "class/timeout.h"
//
// TODO: insert other definitions and declarations here
//
#define MAGIC_CODE			1.4
#define ACCEL_BALANCE_MIN	-0.1
#define ACCEL_BALANCE_MAX	0.1

typedef struct g2speed {
	float	g;
	float	speed;
}G2SPEED_T;

#define G2S_LEVEL	5
static const G2SPEED_T G2S[G2S_LEVEL] = {
		{0.40, 1.0},
		{0.35, 0.9},
		{0.30, 0.8},
		{0.20, 0.3},
		{0.10, 0.05}
};

float g2speed(float g) {
	for (int i=0; i<G2S_LEVEL; i++) {
		if ( ABS(g)>=G2S[i].g ) {
			return G2S[i].speed;
		}
	}
	return 0.0;
}

#define BUFFER_LENGTH 5
#include <cstdlib>

int compare(const void *a, const void *b) {
	int _a = *(int *)a;
	int _b = *(int *)b;

	if ( _a < _b ) {
		return -1;
	} else if ( _a > _b ) {
		return 1;
	} else {
		return 0;
	}
}

float smoothByMedianFilter(float *buf, int count) {
	qsort(buf, count, sizeof(float), compare);
	return buf[count/2];
}

class SelfBalance: public CThread {
protected:
	virtual void run() {
		float g[BUFFER_LENGTH];


		// TODO: insert setup code here
		DCMotor m1(PWM5, UNO_7);
		DCMotor m2(PWM4, UNO_4);

		CAdxl345 gs;
		gs.assign(I2C0, KHZ(400));
		gs.powerOn();

		MOTOR_ACTION_T dir = REVERSE;
		int x, y, z, count = 0;
		float gx = 0, last_gx = 0;
		float speed = 0;
		CTimeout to;
		while (1) {
			to.reset();
			gs.readAccel(&x, &y, &z);
			g[count++] = x / 256.0f;

			if ( count<BUFFER_LENGTH ) continue;

			count = 0;
			gx = smoothByMedianFilter(g, BUFFER_LENGTH);

			dbg.println("gx=%0.2f", gx);

			if (gx > ACCEL_BALANCE_MAX ) { // to back
				dir = FORWARD;
				speed = g2speed(gx);
			} else if (gx < ACCEL_BALANCE_MIN ) { // to forward
				dir = REVERSE;
				speed = g2speed(gx);
			} else {
				speed = 0;
			}

			m1 = dir;
			m2 = dir;
			m1 = speed;
			m2 = speed;
			last_gx = gx;
			to.wait(2);
		}
	}
};

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

	SelfBalance sb;
	sb.start("balance", 256, PRI_HIGH);

	// Enter an endless loop
	while (1) {
		led = !led;
		sleep(200);
	}
	return 0;
}
