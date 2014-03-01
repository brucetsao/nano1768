/*
 ===============================================================================
 Name        : main.cpp
 Author      : Arik Yang
 Version     : 1.0.0
 Date		 :
 Copyright   :
 License	 :
 Description : main definition
 History	 :
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
#include "class/timeout.h"
#include "class/pwm.h"
#include "class/websocket.h"
#include "class/mailbox.h"

#include "debug.h"

#define MAXPULSES 100
#define WSS_INDEX_IR 0

#define TOPLAY			0
#define TORECORD		1
#define READYTORECORD	2
#define RECORDING		3
#define RECORDDONE		4
#define READYTOPLAY		5

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];
uint32_t pulse[MAXPULSES];
int index_rx;
CSemaphore s_rx;
CSemaphore s_tx;
CSemaphore s_udpate_status;

int IR_status = READYTOPLAY;

//CString IR_status = "play";
//
// TODO: insert other include files here
//

//
// TODO: insert other definitions and declarations here
//

class CRecord: public CThread {
protected:
	virtual void run(){
		CPin p_rx(P24);
		p_rx.input();

		CTick tick;
		CTimeout to;
//		int value;

		while(1)
		{
			s_rx.wait();
			dbg.println("go record");

			to.reset();
			index_rx = 0;
			while(1)
			{
				tick.reset();
				// wait rx HIGH->LOW
				while (p_rx == HIGH) {
					if (to.isExpired(5000)||index_rx >= MAXPULSES)
						break;
				}
				pulse[index_rx] = tick.elapsed();
				index_rx++;
				if (to.isExpired(5000)||index_rx >= MAXPULSES)
				{
					if(pulse[0]>10000)
						pulse[0] = 1000;

					if ((index_rx!=0) && (pulse[index_rx-1]>10000))
						pulse[index_rx-1] = 1000;

					for(int i=0; i< index_rx; i++)
					{
						if(i%2)
						{
							dbg.println("hi");
						}
						else
						{
							dbg.println("lo");
						}
						dbg.println(pulse[i]);
					}

					dbg.println("record end");
					IR_status = RECORDDONE;
//					value = RECORDDONE;
//					CMailBox::post("IR", &value);	// post message to the LED-demo object
					break;

				}


				tick.reset();
				// wait rx LOW->HIGH
				while (p_rx == LOW) {
				}
				pulse[index_rx] = tick.elapsed();
				index_rx++;

			}
		}
	}
};


class CTransmit: public CThread {
protected:
	virtual void run(){


		CPwm tx_pin(PWM5);
		// IR transmit is using 38KHz PWM, the same as IR receiver used.
		tx_pin.frequency(KHZ(38));
		tx_pin.start();
		tx_pin.dutyCycle(0);
		tx_pin.begin();

		CTick tick;

		while(1)
		{
			s_tx.wait();
			dbg.println("go play");

			for(int i=0; i< index_rx; i++)
			{
				if(i%2)
				{
//					dbg.println("hi");
					//hi
					tx_pin.dutyCycle(0.5);
					tick.delay(pulse[i]);
					tx_pin.dutyCycle(0);
				}
				else
				{
//					dbg.println("lo");
					//low
					tick.delay(pulse[i]);
				}
			}
			tx_pin.dutyCycle(0);

			dbg.println("transmit end");


		}
	}
};

class CIRWebSocketService: public CWebSocketService, CThread {
protected:
	virtual void run(){
			CString msg;
			CPin led(LED4);
			led = LED_ON;
			int previous_status = IR_status;
//			CMailBox mail("IR", sizeof(int));
//			int *pMailVal, remoteVal=0;

			while(m_ws->isConnected()) {

//				s_udpate_status.wait();
//				dbg.println("s_rx_done release");



				if(previous_status != IR_status)
				{
					switch(IR_status)
					{
						case READYTORECORD:
							m_ws->send("ready_to_record");
							break;
						case TOPLAY:
							m_ws->send("to_play");
							break;
						case TORECORD:
							m_ws->send("to_record");
							break;
						case RECORDDONE:
							m_ws->send("record_done");
							break;
						case READYTOPLAY:
							m_ws->send("ready_to_play");
							break;

					}
					previous_status = IR_status;
				}
				sleep(200);
			}
			led = LED_OFF;
//		}
	}

	virtual void onOpen() {
//		int value;
		start("wsIR", 180, PRI_HIGH);	// start the thread on the open event
//		value = IR_status;
//		CMailBox::post("IR", &value);	// post message to the LED-demo object
//
		switch(IR_status)
		{
			case READYTORECORD:
				m_ws->send("ready_to_record");
				break;
			case TOPLAY:
				m_ws->send("to_play");
				break;
			case TORECORD:
				m_ws->send("to_record");
				break;
			case RECORDDONE:
				m_ws->send("record_done");
				break;
			case READYTOPLAY:
				m_ws->send("ready_to_play");
				break;

		}
	}

	virtual void onText(CString &msg) {
//		int value;
		dbg.println(msg);

//		dbg.println(IR_status);
		if(msg == "ready_to_record")
		{
			IR_status = READYTORECORD;
		}
		if(msg == "to_record")
		{
			s_rx.release();
			IR_status = TORECORD;
		}
		if(msg == "to_play")
		{
			s_tx.release();
			IR_status = TOPLAY;
		}
		if(msg == "ready_to_play")
		{
			IR_status = READYTOPLAY;

		}
//		value = IR_status;
//		CMailBox::post("IR", &value);	// post message to the LED-demo object

	}
};

class CIRWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		dbg.println("onAccept");
		switch (index) {
		case WSS_INDEX_IR:
			CIRWebSocketService *wss = new CIRWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
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
	CPin led(LED3);

	// TODO: insert setup code here

//	CPinInt pinInt;
//	pinInt.start("pinIRQ");

	//IR receive trigger
	s_rx.binary();
	//IR transmit trigger
	s_tx.binary();

	s_udpate_status.binary();

	CRecord crx;
	crx.start("IR_RX", 128, PRI_HIGH);

	CTransmit ctx;
	ctx.start("IR_TX", 128, PRI_HIGH);

	CIRWebSocketServer server;
	server.add("/IR", WSS_INDEX_IR);	// add service name = /

	// Enter an endless loop
	while (1) {
		// Simple demo Code (removable)
		led = !led;
		sleep(200);


		// Your loop code here
		switch(dbg.isAnyKey())
		{
//			key = dbg.isAnyKey();
//			dbg.println(key);
			case 114:
			case 13:
				dbg.println("go record");

				led = HIGH;
				s_rx.release();
				break;
			case 112:
				dbg.println("go play");

				led = HIGH;
				s_tx.release();
				break;

		}

	}
	return 0;
}
