/*
 ===============================================================================
 Name        : httpclient.h
 Author      : Jason
 Version	 : v1.0.1
 Date		 : 2012/10/17
 Copyright   : 2012 (C) www.embeda.com.tw / www.ucxpresso.net
 License	 : CC BY-SA 3.0
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/10/17 v1.0.0	First Editon									Jason
 2012/10/18	v1.0.1	Add Keep-Alive in request() member function		Jason
 2012/10/19 v1.0.2	Fixed to connect with fail problem.				Jason
 ===============================================================================
 */
#ifndef _HTTP_CLIENT_H
#define _HTTP_CLIENT_H

#include "class/socket.h"
#include "class/buffer.h"
#include "class/string.h"

class HttpClient: public CObject {
public:
	/** Constructs a HttpClient object
	 *
	 */
	HttpClient(CSocket &sock, LPCTSTR host, int port=80);

	/** Call the member function to send a http 'GET' request to HTTP server.
	 * \param uri a URI string to point to a page where the request page.
	 *
	 * \code
	 * Example:
	 * 		HttpClient fb("api.facebook.com");
	 * 		if ( fb.request("/restserver.php?method=links.getStats&urls=http://www.embeda.com.tw/tw/?p=4054") ) {
	 *
	 * 		}
	 * \endcode
	 */
	bool request(LPCTSTR uri);

	/** Call the member function to retrieve the response data from server.
	 */
	virtual inline CString& response() {
		return m_response;
	}

public:
	virtual ~HttpClient();

protected:
	CSocket		*m_sock;
	LPCTSTR		m_host;
	int			m_port;

	CString 	m_response;
	virtual CString buildGET(LPCTSTR uri);
};

#endif
