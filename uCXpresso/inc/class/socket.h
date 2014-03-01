/*
 ===============================================================================
 Name        : socket.h
 Author      : Jason
 Version     : v1.0.1
 Date		 : 2013/05/03
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : TCP/IP BSD Socket Stream
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2011/12/18	v1.0.0	First Edition										Jason
 2012/09/15	v1.0.1	Add shorthand TCP/UDP enumerations in SOCK_MODE_T	Jason
 2012/10/12 v1.0.2	Add block option in read() and write() member.		Jason
 2012/10/19 v1.0.3  Add more valid socket checked.						Jason
 2013/04/21 v1.0.4	Add Static CSocket::getLinkStatus() member.			Jason
 2013/05/02 v1.0.5  Add SO_RCVTIMEO socket option						Jason
 2013/05/03 v1.0.6	Add CSocket::renew() static function				Jason
 ===============================================================================
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "class/stream.h"
#include "class/thread.h"

#define ANY_URL_ADDRESS	NULL


/* \enum SOCK_MODE_T
 */
typedef enum {
	SOCK_STREAM	= 1,	///< TCP SOCKET
	SOCK_DGRAM  = 2,	///< UDP SOCKET
	TCP = SOCK_STREAM,	///< shorthand SOCK_STREAM
	UDP = SOCK_DGRAM	///< shorthand SOCK_DGRAM
}SOCK_MODE_T;

/* \enum OPTION_NAMN_T
 */
typedef enum {
	SO_DEBUG 		= 0x0001,  ///< Unimplemented: turn on debugging info recording
	SO_ACCEPTCONN   = 0x0002,  ///< socket has had listen()
	SO_REUSEADDR   	= 0x0004,  ///< Allow local address reuse
	SO_KEEPALIVE    = 0x0008,  ///< keep connections alive
	SO_DONTROUTE    = 0x0010,  ///< Unimplemented: just use interface addresses
	SO_BROADCAST    = 0x0020,  ///< permit to send and to receive broadcast messages (see IP_SOF_BROADCAST option)
	SO_USELOOPBACK  = 0x0040,  ///< Unimplemented: bypass hardware when possible
	SO_LINGER		= 0x0080,  ///< linger on close if data present
	SO_RCVTIMEO  	= 0x1006,  ///< receive timeout
	SO_DONTLINGER   = ((int)(~SO_LINGER)),
}OPTION_NAMN_T;

/*!	\class CSocket socket.h "class/socket.h"
 *	\brief CSocket provide the BSD socket interface.
 */
class CSocket: public CStream {
public:
	/**Constructs a CSocket object.
	 *
	 * \code
	 * Exmple:
	 * 		CSocket client;
	 * 		if ( client.create(SOCK_STREAM)>=0 ) {
	 * 			// create socket successful
	 * 			...
	 * 		}
	 * 		....
	 * 	\endcode
	 * \remark Must use the create member function to create the socket object.
	 */
	CSocket();

	/**Constructs a CSocket object with SOCK_MODE_T.
	 * \param mode to assign a SOCKET_MODE_T to create the socket object.
	 *
	 * \code
	 * Example:
	 * 		CSocket client(SOCK_STREAM);
	 * 		...
	 * \endcode
	 */
	CSocket(SOCK_MODE_T mode);

	/**Call the create member function after constructing a socket object.
	 * \param mode to assign a SOCK_MODE_T to create the socket object.
	 * \return -1 if the function fail (ERR_MEM)
	 *
	 * \code
	 * Exmple:
	 * 		CSocket client;
	 * 		if ( client.create(SOCK_STREAM)>=0 ) {
	 * 			// create socket successful
	 * 			...
	 * 		}
	 * 		....
	 * 	\endcode
	 */
	virtual int 	 create(SOCK_MODE_T mode=SOCK_STREAM);

	/**Call this member function to associate a local address with the socket.
	 * \param port to identify the socket application.
	 * \param remote address, a url string or dotted number address.
	 * \return ERR_OK if successful; otherwise, check ERR_T error code.
	 *
	 * \code
	 * Example:
	 * 		CSocket server(SOCK_STREAM);
	 * 		server.bind(23);	// TELNET application (port=23)
	 *		...
	 * \endcode
	 * \remarks for Server side only.
	 */
	virtual ERR_T	 bind(int port, LPCTSTR remote=ANY_URL_ADDRESS);

	/**Call this member function to establish a connection to an unconnected stream or datagram socket.
	 * \param remote a url string or dotted number address such as "192.168.1.10".
	 * \param port to connect the remove (server) application (port).
	 * \return ERR_OK if successful; otherwise, check ERR_T error code.
	 *
	 * \code
	 * Example:
	 * 		CSocket client(SOCK_STREAM);
	 * 		if ( client.connect("localhost", 23)==ERR_OK ) {
	 * 			// connect successful
	 * 			while( client.isConnected() ) {
	 * 				...
	 * 			}
	 * 		}
	 * \endcode
	 * \remark for Client side only.
	 */
	virtual ERR_T  	 connect(LPCTSTR remote, int port);

	/**Call this member function to listen for incoming connection requests.
	 * \return ERR_OK if successful; otherwise, check ERR_T error code.
	 *
	 * \code
	 * Example:
	 * 		CSocket server(SOCK_STREAM);
	 * 		server.bind(23);
	 * 		server.listen();
	 * 		while(1) {
	 * 			CSocket *client = server.accept();
	 * 			...
	 * 		}
	 * \endcode
	 * \remark for Server side only
	 */
	virtual ERR_T 	 listen();

	/**Call this member function to accept a connection on a socket.
	 * \return CSocket a connected socket object point.
	 *
	 * \code
	 * Example:
	 * 		CSocket server(SOCK_STREAM);
	 * 		server.bind(23);
	 * 		server.listen();
	 * 		while(1) {
	 * 			CSocket *client = server.accept();
	 * 			...
	 * 		}
	 * \endcode
	 * \remark Must delete the client socket resource if not in used.
	 */
	virtual CSocket* accept();

	/**Close and free the socket resource
	 * \return ERR_OK if successful; otherwise, check ERR_T error code.
	 */
	virtual ERR_T 	 close();

	/**Call this member function to retrieve a socket option.
	 * \param[in] optname an OPTION_NAME_T value to select the socket options.
	 * \param[out] optval a point to the buffer in which the value for the requested option.
	 * \param[out] optlen a point the size of the optval buffer in bytes.
	 * \return ERR_OK if successful; otherwise, check ERR_T error code.
	 */
	virtual ERR_T 	 getOption(OPTION_NAMN_T optname, void *optval, int *optlen);

	/**Call this member function to set a socket option.
	 * \param[in] optname an OPTION_NAME_T value to select the socket options.
	 * \param[in] optval a pointer to the buffer in which the value for the requested option is supplied.
	 * \param[in] optlen a size of the optval buffer in bytes.
	 * \return ERR_OK if successful; otherwise, check ERR_T error code.
	 */
	virtual ERR_T 	 setOption(OPTION_NAMN_T optname, const void *optval, int optlen);

	virtual int	readable();
	virtual int	writeable();
	virtual int read(void *buf, int len, bool block=true);
	virtual int write(const void *buf, int len, bool block=true);

	/**Call the member function to check the present connection is connected or not.
	 * \return true if connection is connected.
	 */
	virtual bool	 isConnected();

	/**Set socket in keep alive. (unit:second)
	 */
	virtual void	 keepAlive(int idle, int interval, int count);
public:
	int getLastError();

	//
	// global static founctions
	//
	static uint32_t 	getLocalIp();
	static LPCTSTR  	addr2url(IP_ADDR_T *addr);
	static IP_ADDR_T	url2addr(LPCTSTR url);
	static uint32_t		getRxPackageCount();
	static uint32_t		getTxPackageCount();
	static bool			getLinkStatus();
	static bool			renew(uint32_t timeout=MAX_DELAY_TIME);

	/*! \cond PRIVATE */
public:
	virtual ~CSocket();

	virtual int sendTo(void *buf, int len, int remote_port, LPCTSTR remote_url, int flag=0);
	virtual int recvFrom(void *buf, int len, int remote_port, LPCTSTR remote_url, int flag=0);

private:
	virtual ERR_T connect(IP_ADDR_T remote_addr, int remote_port);

protected:
	int		  m_sock;
	IP_ADDR_T m_Remote;
	bool	  m_bConnect;
	/*! \endcond */
};

#endif /* SOCKET_H_ */
