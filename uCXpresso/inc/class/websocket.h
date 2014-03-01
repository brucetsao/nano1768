/*
 ===============================================================================
 Name        : websocket.h
 Author      : Jason
 Version     : v1.0.6
 Date		 : 2013/7/5
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/8/27	v1.0.1	Modify the onAccept of CWebSocketServer			Jason
 2012/8/31			Add CWebSocket::uriParam member function		Jason
 2012/9/09	v1.0.2	Remove onPing and onPong member fuinctions		Jason
 	 	 	 	 	Add keepAlive (internal used)
 2012/9/12: v1.0.3	Add CWebSocketClient class						Jason
 	 	 	 	 	Add isClient() member in CWebSocket class.
 2012/10/07 v1.0.4	Add remoteIP() member in CWebSocket class.		Jason
 	 	 	 	 	Add remotePort() member in CWebSocket class.	Jason
 2013/04/28	v1.0.5  Update keep-alive 								Jason
 2013/7/5	v1.0.6	Auto destroy when onClose or onError			Jason
 	 	 	 	 	for CWebSocketService class.
  ===============================================================================
 */

#ifndef WEBSOCKETSERVER_H_
#define WEBSOCKETSERVER_H_

#include "class/webserver.h"
#include "class/string.h"
#include "class/webserver.h"
#include "class/buffer.h"


/* \enum WS_OPCODE_T
 */
typedef enum {
	OP_ERROR = 0x00,
	OP_TEXT = 0x01,
	OP_BINARY = 0x02,
	OP_CLOSE = 0x08,
	OP_PING = 0x09,
	OP_PONG = 0x0A
}WS_OPCODE_T;

/*! \cond PRIVATE */
extern "C" bool websocket_connect(void *data, size_t len, struct tcp_pcb *pcb);
class CWebSocket;
/*! \endcond */

/*!	\class CWebSocketService websocket.h "class/websocket.h"
 * \brief The CWebSocketService is a WebSocket application layer.
 * \note The CWebSocketService is an abstract class, to define the WebSocket event interface.
 */
class CWebSocketService: public CWebServer {
public:
	/**Bind to a CWebSocket Object.
	 * \param ws a pointer to CWebSocket object.
	 *
	 * \code
	 * Example:
	 *		class CMyWebSocketServer: public CWebSocketServer {
	 *		public:
	 *			virtual bool onAccept(int index, CWebSocket *ws) {
	 *				CWebSocketService *wss;
	 *				switch(index) {
	 *				case WSS_INDEX_ECHO:
	 *					wss = new CEchoWebSocketService;
	 *					wss->bind(ws);
	 *					return true;
	 *				}
	 *				return false;
	 *			}
	 *		};
	 *
	 * \endcode
	 */
	virtual void bind(CWebSocket *ws);

	/**onOpen event, callback by WebSocket protocol layer service when WebSocket connect successful.
	 *
	 * \code
	 * Example:
	 * 		CMyWebSocketService: public CWebSocketService, CThread {
	 * 		public:
	 * 			virtual void onOpen() {
	 * 				start("myWS");	// start the thread when WebSocket connected
	 * 			}
	 *			virtual void run() {
	 *				... // setup code
	 *				m_ws->send("Hello World!!");
	 *				while( m_ws->isConnected() ) {
	 *					// loop code
	 *				}
	 *			}
	 * 		};
	 * \endcode
	 * \note This is an empty virtual member function, implement by inheritor.
	 */
	virtual void onOpen() {};

	/**onText event, callback by WebSocket protocol layer service when WebSocket receive a text frame.
	 * \param msg a CString object that content the text string from remote.
	 *
	 * \code
	 * Example:
	 * 		class CEchoWebSocketService: public CWebSocketService {
	 * 		public:
	 * 			virtual void onText(CString &msg) {
	 * 				m_ws->send(msg);	// send back
	 * 			}
	 * 		};
	 * \endcode
	 * \note This is an empty virtual member function, implement by inheritor.
	 */
	virtual void onText(CString &msg) {};

	/**onBinary event, callback by WebSocket protocol layer service when WebSocket receive a binary frame.
	 * \note This is an empty virtual member function, implement by inheritor.
	 */
	virtual void onBinary(byte *payload, uint16_t length) {};

	/**onClose event, callback by WebSocket protocol layer service when WebSocket disconnect the connection.
	 *
	 * \code
	 * Example:
	 * 		class CEchoWebSocketService: public CWebSocketService {
	 * 		public:
	 * 			virtual void onText(CString &msg) {
	 * 				m_ws->send(msg); // send back
	 * 			}
	 * 			virtual void onClose() {
	 * 				delete this; 	 // if the WebSocketSerive is allocated.
	 * 			}
	 * 		};
	 * \endcode
	 * \note This is an empty virtual member function, implement by inheritor.
	 */
	virtual void onClose() {};

	/**onError event, callback by WebSocket protocol layer service when WebSocket got TCP/IP layer error.
	 *
	 * \note This is an empty virtual member function, implement by inheritor.
	 */
	virtual void onError() {};

	/**onUnknow event, callback by WebSocket protocol layer service when WebSocket receive unknown frame.
	 * \note This is an empty virtual member function, implement by inheritor.
	 */
	virtual void onUnknow(int opcode, byte *payload, size_t length) {};

	/*! \cond PRIVATE */
public:
	CWebSocketService();
	virtual ~CWebSocketService();
	bool  m_autoDestroy;

protected:
	friend class CWebSocket;
	friend class CWebSocketServer;
	/*! \endcond */

	/** The m_ws is a CWebSocket object to provide WebSocket communication functions.
	 */
	CWebSocket 	*m_ws;
};

///////////////////////////////////////////////////////////////////////////////////////////
//
// Web Socket State
//
#define WS_STATE_CONNECTED	0
#define WS_STATE_IS_CLIENT	1
#define WS_STATE_CON_ERROR	7

/*!	\class CWebSocket websocket.h "class/websocket.h"
	\brief The CWebSocket is a WebSocket protocol layer.
 */
class CWebSocket: public CWebServer {
public:
	/**Call the member function to send a message (text) to remote
	 * \param msg a const string pointer.
	 * \return how many byte to sent.
	 */
	virtual int send(LPCTSTR msg);

	/**Call the member function to send a message (test) to remote
	 * \param str a CString object.
	 * \return how many byte to sent.
	 */
	virtual int send(CString str);

	/**Call the member function to send a WebSocket frame to remote
	 * \param opcode a WS_OPCODE_T code
	 * \param payload a pointer to the payload buffer.
	 * \param length a unsigned long value to identify the length of payload in bytes.
	 * \return how many byte to sent.
	 */
	virtual int send(WS_OPCODE_T opcode, byte *payload, uint32_t length);

	/**Call the member function to close the WebSocket connection.
	 */
	virtual void close();

	/**Call the member function to check this connection is connecting or not.
	 * \return true is connected; otherwise, is disconnected.
	 */
	virtual inline bool isConnected() {
		return bit_chk(m_state, WS_STATE_CONNECTED);
	}

	/**Call the member function to identify the connection type of localhost. (client or server).
	 * \return true,if this side is client; otherwise, the side is server.
	 */
	virtual inline bool isClient() {
		return bit_chk(m_state, WS_STATE_IS_CLIENT);
	}

	/**Call the member function to check WebSocket version from browser.
	 * \return zero if olddler; v13 is current version.
	 */
	virtual inline int version() {	// check browser websocket version
		return (m_state>>16);
	}

	/**Call the member function to retrieve the parameter of URI from browser.
	 * \return a CString object content the parameter of URI.
	 *
	 * \code
	 * Example:
	 * 		URI: ws://192.168.1.25/echo?text=plain
	 *
	 * 		str = m_ws->uriParam("text");	// retriver str=plain
	 * \endcode
	 */
	virtual CString uriParam(LPCTSTR param);

	/**Call the member function to retrieve the ip address of remote.
	 * \return IP_ADDR_T is a union type.
	 */
	virtual IP_ADDR_T remoteIP();

	/**Call the member function to retrieve the port number of remote.
	 * \return port number is a integer value type.
	 */
	virtual int	remotePort();

	/*! \cond PRIVATE */
public:
	CWebSocket(xHandle pcb);
	virtual ~CWebSocket();
	virtual void onReceive(byte *raw, uint32_t len);
	virtual void setState(uint32_t state);
	virtual uint32_t getState();

	CWebSocketService  *m_wss;
	CString 			m_uri;
protected:
	friend class CWebSocketService;
	friend class CWebSocketServer;
	friend class CWebSocketClient;

	xHandle 			m_pcb;
	uint32_t			m_state;
	gcBuffer 			m_buffer;

#ifdef WS_SUPPORT_OLD_VERSION
	int sendOld(WS_OPCODE_T opcode, byte *payload, uint16_t length);
	void recvOld(byte *raw, uint32_t len);
#endif
	//
	// WebSocket Header (internal use!!)
	void parse_header(void *header, int size);
	LPCTSTR				m_version;
	LPCTSTR				m_key1;
	LPCTSTR				m_key2;
	const byte*			m_key3;
	LPCTSTR				m_host;
	LPCTSTR				m_origin;
	LPCTSTR				m_protocol;
	LPCTSTR				m_extensions;
	/*! \endcond */
};

/*!	\class CWebSocketServer websocket.h "class/websocket.h"
	\brief Use the CWebSocketServer to listen the request which WebSocket from client, and trigger the onAccept event after handshaking finished.
 */
class CWebSocketServer: public CWebServer {
public:
	/**Call the member function to add a service to WebSocket Server list.
	 * \param service is a string to point to the service name.
	 * \param index is a integer value to correspond to the service name.
	 *
	 * \code
	 * Example:
	 * 		class CMyWebSocketServer: public CWebSocketServer {
	 *		public:
	 *			virtual bool onAccept(int index, CWebSocket *ws) {
	 *				switch(index) {
	 *				case WSS_INDEX_ECHO:
	 *					...
	 *					return true;
	 *				case WSS_INDEX_ADC:
	 *					...
	 *					return true;
	 *				}
	 *				return false;
	 *			}
	 * 		};
	 *
	 * 		int main() {
	 * 			...
	 * 			CMyWebSocketServer server;
	 * 			// add a "/echo" service and correspond to WSS_INDEX_ECHO
	 * 			server.add("/echo", WSS_INDEX_ECHO);
	 * 			// add a "/adc" service and correspond to WSS_INDEX_ADC
	 * 			server.add("/adc", WSS_INDEX_ADC);
	 * 			...
	 * 		}
	 * \endcode
	 */
	bool add(LPCTSTR service, int index);

	/**The member function will callback by WebSocket Server after a request coming.
	 *
	 * \param index a integer value to identify the request service (correspond to service name).
	 * \param ws a CWebSocket object to provide the WebSocket protocol layer service, must bind to CWebSocketService.
	 * \return true to accept the connection; otherwise, to reject the connection.
	 *
	 * \code
	 * Example:
	 * 		class CMyWebSocketServer: public CWebSocketServer {
	 *		public:
	 *			virtual bool onAccept(int index, CWebSocket *ws) {
	 *				CWebSocketService *wss;
	 *				switch(index) {
	 *				case WSS_INDEX_ECHO:
	 *					wss = CEchoWebSocketService;
	 *					wss->bind(ws);
	 *					return true;
	 *				case WSS_INDEX_ADC:
	 *					...
	 *					return true;
	 *				}
	 *				return false;
	 *			}
	 * 		};
	 *
	 * 		int main() {
	 * 			...
	 * 			CMyWebSocketServer server;
	 * 			// add a "/echo" service and correspond to WSS_INDEX_ECHO
	 * 			server.add("/echo", WSS_INDEX_ECHO);
	 * 			// add a "/adc" service and correspond to WSS_INDEX_ADC
	 * 			server.add("/adc", WSS_INDEX_ADC);
	 * 			...
	 * 		}
	 * \endcode
	 * \note The onAccept is pure virtual functions, must reimplement by inheritor.
	 */
	virtual bool onAccept(int index, CWebSocket *ws) = PURE_VIRTUAL_FUNC;

	/*! \cond PRIVATE */
protected:
	CWebSocketServer();
	virtual ~CWebSocketServer();
	friend class CWebSocketService;
	friend bool websocket_connect(void *data, size_t len, struct tcp_pcb *pcb);
	bool connect(int index, void *data, int len, xHandle pcb, LPCTSTR uri);
	void accept(CWebSocket *ws);
#ifdef WS_SUPPORT_OLD_VERSION
	void acceptOld(CWebSocket *ws);
#endif
	/*! \endcond */
};

/*!	\class CWebSocketClient websocket.h "class/websocket.h"
	\brief Use the CWebSocketClient to send a request to a WebSocket Server, and trigger an onConnect event after handshaking finished.
 */
class CWebSocketClient: public CWebServer {
public:
	/**Call the member function to connect to a WebSocket Server.
	 * \param host is a url string or dotted number address such as "192.168.1.10".
	 * \param request is a service name.
	 * \param port is a tcp port number, the default is 80 for HTTP.
	 * \return true to continue the connection; otherwise, to disconnect the connection.
	 *
	 * \code
	 * Example:
	 * 		class CMyWebSocketClient: public CWebSocketClient {
	 * 		public:
	 * 			virtual bool onConnect(CWebSocket *ws) {
	 * 				CEchoWebSocketService *wss = new CEchoWebSocketService;
	 * 				wss->bind(ws);
	 * 				return true;
	 * 			}
	 * 		};
	 *
	 * 		int main() {
	 * 			...
	 * 			CMyWebSocketClient client;
	 * 			client.connect("
	 *
	 * 		}
	 * \endcode
	 */
	bool connect(LPCTSTR host, LPCTSTR request, uint16_t port=80);

	/**The member function callback by WebSocket Client after the WebSocket Server accept the client's request.
	 * \param ws a CWebSocket object to provide the WebSocket protocol layer service.
	 *
	 * \code
	 * Example:
	 * 		class CMyWebSocketClient: public CWebSocketClient {
	 * 		public:
	 * 			virtual bool onConnect(CWebSocket *ws) {
	 * 				CEchoWebSocketService *wss = new CEchoWebSocketService;
	 * 				wss->bind(ws);
	 * 				return true;
	 * 			}
	 * 		};
	 *
	 * 		int main() {
	 * 			...
	 * 			CMyWebSocketClient client;
	 * 			client.connect("
	 *
	 * 		}
	 * \endcode
	 * \note the onConnect is a "pure virtual function", must reimplement by inheritor.
	 */
	virtual bool onConnect(CWebSocket *ws) = PURE_VIRTUAL_FUNC;

	/*! \cond PRIVATE */
public:
	CWebSocketClient();
	~CWebSocketClient();
	virtual void handshaking(xHandle pcb);
protected:
	CString m_host;
	CString m_request;
	/*! \endcond */
};

#endif /* WEBSOCKETSERVER_H_ */
