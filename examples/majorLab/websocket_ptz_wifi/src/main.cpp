/*
 ===============================================================================
 Name        : main.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/7/4
 Copyright   : 2013 (C) Embeda Technology Inc. (http://www.embeda.com.tw)
 License	 : MIT
 Description : WiFi Shield Demo with WebSocket+PTZ
 Hardware	 : P/N:SHIELD-WIFI-GS1000
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------

 ===============================================================================
 */
#include "uCXpresso.h"
#include "class/pin.h"
#include "debug.h"
#include "probe/probe.h"

CDebug dbg(DBG_USB);

/* ==============================================
 user adjustable pool memory
 ============================================== */
static uint8_t pool[DEFAULT_POOL_SIZE];

//
// TODO: insert other include files here
//
#include "servo/servo.h"
#include "websocket_ptz.cpp"

#define WS_INDEX_PTZ	0
#define WS_INDEX_SYS	1

class CMyWebSocketServer: public CWebSocketServer {
protected:
	virtual bool onAccept(int index, CWebSocket *ws) {
		CWebSocketService *wss;
dbg.println("onAccept:");
		switch (index) {
		case WS_INDEX_PTZ:
			wss = new CPtzWebSocketService;
			wss->bind(ws);
			return true;
		}
		return false;
	}
};

//
// TODO: insert other definitions and declarations here
//
CServo servoX(PWM5, &SERVO_RB_421);
CServo servoY(PWM6, &SERVO_RB_421);

//
// for WiFi
//
#define WIFI	1

#if WIFI
#include "class/netif.h"
#include "wifi/gainspan/wifi_gainspan.h"

#define WIFI_SSID0	"MY-HOME"
#define WIFI_KEY0	"26922464"

#define WIFI_SSID1	"uCXpresso"
#define WIFI_KEY1	"26957969"

#define WIFI_SSID2	"LTC1"
#define WIFI_KEY2	"0226275966"

#define WIFI_SSID4	"Golden-IC"
#define WIFI_KEY4	"gicrouterkeys"

#define WIFI_SSID5	"Wendel Neihu"
#define WIFI_KEY5	"wendel@8751"


#define USE_GS_SPI 	1
#endif

/* ==============================================
 main task routine
 ============================================== */
int main(void) {
	pool_memadd((uint32_t) pool, sizeof(pool));

#ifdef DEBUG
	dbg.start();
	probe.start();
#endif
	// Simple demo Code (removable)
	CPin led(LED1);
	led = LED_ON;

	// TODO: insert setup code here
	servoX.begin();
	servoY.begin();

	servoX = 0;
	servoY = 0;

#if WIFI
	//
	// for WiFi
	//
	CPin IF(D8);
	IF.output();

#if USE_GS_SPI
	IF = HIGH;	// Use SPI, IF set to high
	spiGS s(SPI0, D10, D9, WIFI_MTU);
	s.begin();

#else
	IF = LOW;	// Use UART, IF set to low
	CSerial s(UART2, WIFI_MTU);
//	s.handshaking(D2, D3);
	s.begin(DEF_GS_BAUDRATE);

#endif
	//
	// Create GainSpan WiFi Adaptor
	// SSID = MY-HOME
	// Reset Pin = P20 (A5)
	//
	wifiGS gs(s, P20);

	CNetIf wifi(gs);		// wifi object use the gs (NetStream) adaptor
	wifi.mtu(WIFI_MTU);
	wifi.name("nanoWiFi");	// set host name

	if (gs.start()) {

		gs.txPower(7);
		gs.txRetry(3);

		//
		// Connect to AP Server
		//
		if (gs.connect(WIFI_SSID1, WIFI_KEY1)) {

			gs.led(LED_ON);

			//
			// Use gs Adaptor for new NETIF
			//
			wifi.start("WiFi"); // Start the WiFi Network Interface
			wifi.setDefault();
		} // */
	} // */
#endif  // WIFI

	// Register a WebSocket Server
	CMyWebSocketServer server;
	server.add("/ptz", WS_INDEX_PTZ);

	CString cmd;
	int ch;
	// Enter an endless loop
	while (1) {
#if WIFI
		//
		// input AT command on debug mode
		//
		if ((ch = dbg.isAnyKey())) {
			switch (ch) {
			case '\r':
				cmd.makeUpper();
				gs.command(cmd, "OK", 10000);
				cmd.clear();	// clear string without free memory
				break;
			default:
				if (ch >= 0x20) {
					cmd += (TCHAR) ch;
				}
			} // end of switch
		}
#endif
		led = !led;
		sleep(100);
	}
	return 0;
}
