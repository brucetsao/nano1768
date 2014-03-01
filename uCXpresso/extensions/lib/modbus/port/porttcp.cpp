/*
 ===============================================================================
 Name        : porttcp.cpp
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2013/1/4
 License     : CC BY-SA 3.0
 Description : tcp port for modbus
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/1/4	v1.0.0	First Edition.									Jason
 */

#include "class/thread.h"
#include "class/socket.h"
#include "mb.h"

#include "debug.h"
//
//  MBAP Header
//
#define MB_TCP_UID          6
#define MB_TCP_LEN          4
#define MB_TCP_FUNC         7

//
// defines
//
#define MB_TCP_DEFAULT_PORT 	502 		/* TCP listening port. */
#define MB_TCP_BUF_SIZE     	( 256 + 7 ) /* Must hold a complete Modbus TCP frame. */

class mbServer: public CThread {
public:
	mbServer(USHORT usTCPPort) {
		m_server = new CSocket(SOCK_STREAM);
		m_port = (usTCPPort == 0 ? MB_TCP_DEFAULT_PORT : usTCPPort);

		m_aucTCPBuf = new UCHAR[MB_TCP_BUF_SIZE];
		m_usTCPBufPos = 0;

		m_client = NULL;
	}

	~mbServer() {
		if (m_server)
			delete m_server;
		if ( m_aucTCPBuf )
			delete m_aucTCPBuf;
	}

	bool start() {
		return CThread::start("mbSvr", 280);
	}

	/**Drop connected client socket
	 */
	void drop() {
		if (m_client) {
			m_client->close();
			while (m_client) {	// wait for client free
				sleep(100);
			}
		}
	}

	bool getRequest(UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength) {
		if (m_client) {
			*ppucMBTCPFrame = &m_aucTCPBuf[0];
			*usTCPLength = m_usTCPBufPos;
			/* Reset the buffer. */
			m_usTCPBufPos = 0;
			return true;
		}
		return false;
	}

	bool sendResponse(const UCHAR * pucMBTCPFrame, USHORT usTCPLength) {
		if (m_client) {
			if (m_client->write(pucMBTCPFrame, usTCPLength) == usTCPLength) {
				return true;
			}
		}
		return false;
	}

protected:
	CSocket *m_server;
	CSocket *m_client;
	UCHAR *m_aucTCPBuf;
	int m_port, m_usTCPBufPos;

	virtual void run() {
		int len;
		USHORT usLength;

		m_server->bind(m_port);
		m_server->listen();
		while (1) {
			m_client = m_server->accept();
			if (m_client) {
#ifdef MB_TCP_DEBUG
				vMBPortLog(MB_LOG_DEBUG, "MBTCP", "Connected.\r\n");
#endif
				m_client->keepAlive(3000, 1000, 3);
				while (m_client->isConnected()) {
					len = m_client->read(m_aucTCPBuf + m_usTCPBufPos,
							MB_TCP_BUF_SIZE - m_usTCPBufPos);

					if (len) {
						m_usTCPBufPos += len;
						/* If we have received the MBAP header we can analyze it and calculate
						 * the number of bytes left to complete the current request. If complete
						 * notify the protocol stack.
						 */
						if (m_usTCPBufPos >= MB_TCP_FUNC) {
							/* Length is a byte count of Modbus PDU (function code + data) and the
							 * unit identifier. */
							usLength = m_aucTCPBuf[MB_TCP_LEN] << 8U;
							usLength |= m_aucTCPBuf[MB_TCP_LEN + 1];

							/* Is the frame already complete. */
							if (m_usTCPBufPos < (MB_TCP_UID + usLength)) {
								// not finish
							} else if (m_usTCPBufPos
									== (MB_TCP_UID + usLength)) {
#ifdef MB_TCP_DEBUG
								prvvMBTCPLogFrame("MBTCP-RECV", &m_aucTCPBuf[0],
										m_usTCPBufPos);
#endif
								(void) xMBPortEventPost(EV_FRAME_RECEIVED);
							} else {
#ifdef MB_TCP_DEBUG
								vMBPortLog(MB_LOG_DEBUG, "MBTCP-ERROR",
										"Received to many bytes! Droping client.\r\n");
#endif
								/* This should not happen. We can't deal with such a client and
								 * drop the connection for security reasons.
								 */
								break; // exit while and drop client;
							}
						} // end of if (m_usTCPBufPos >= MB_TCP_FUNC)
					} // end of if ( len )
				} // end of while(isconnected)

				// free client socket
				delete m_client;
				m_client = NULL;

#ifdef MB_TCP_DEBUG
				vMBPortLog(MB_LOG_DEBUG, "MBTCP", "Disconnected.\r\n");
#endif
			}
		}
	}
};

#define SERVER(x) (static_cast<mbServer*>(x))

//
// Static variables
//
static mbServer *xServer = NULL;

//
// Start implementation
//
PR_BEGIN_EXTERN_C

BOOL xMBTCPPortInit(USHORT usTCPPort) {
	xServer = new mbServer(usTCPPort);
	return SERVER(xServer)->start();
}

void vMBTCPPortClose() {
	if ( xServer ) {
		SERVER(xServer)->drop();
		delete xServer;
		xServer = NULL;
	}
}

void vMBTCPPortDisable() {
	SERVER(xServer)->drop();
}

BOOL xMBTCPPortGetRequest(UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength) {
	return SERVER(xServer)->getRequest(ppucMBTCPFrame, usTCPLength);
}

BOOL xMBTCPPortSendResponse(const UCHAR * pucMBTCPFrame, USHORT usTCPLength) {
#ifdef MB_TCP_DEBUG
	prvvMBTCPLogFrame( "MBTCP-SENT", pucMBTCPFrame, usTCPLength );
#endif
	return SERVER(xServer)->sendResponse(pucMBTCPFrame, usTCPLength);
}

PR_END_EXTERN_C
