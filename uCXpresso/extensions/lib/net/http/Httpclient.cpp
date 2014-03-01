/*
 ===============================================================================
 Name        : httpclient.cpp
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
#include "httpclient.h"
#include "class/socket.h"
#include "class/buffer.h"
#include "debug.h"
#include <string.h>

#define HTTP_BUF_SIZE	64

HttpClient::HttpClient(CSocket &sock, LPCTSTR host, int port) {
	m_sock = &sock;
	m_host = host;
	m_port = port;
}

HttpClient::~HttpClient() {

}

bool HttpClient::request(LPCTSTR uri) {
	bool result = false;
	CString m_header;
	byte *buf;
	int	len, f, content_length, total_length;
	ERR_T err;

	m_response.clear();

	//
	// connect to server if need.
	//
	if ( !m_sock->isConnected() ) {
		m_sock->create(SOCK_STREAM);	// important! create socket after disconnected.
		err = m_sock->connect(m_host, m_port);
		if ( err==ERR_OK ) {
			m_sock->keepAlive(10000, 3000, 3);
		}
	}

	if ( m_sock->isConnected() ) {
		//
		// build Http request
		//
		CString request = buildGET(uri);

		//
		// send Http "GET" request
		//
		if ( m_sock->write(request.getBuffer(), request.length())>0 ) {

			buf = new byte[HTTP_BUF_SIZE+1];
			if ( buf ) {
				total_length = 0;
				content_length = 0;
				//
				// is socket is connect?
				//
				while( m_sock->isConnected() &&
					  (total_length==0 || m_response.length()<total_length) ) {
					//
					// read data from socket
					//
					len = m_sock->read(buf, HTTP_BUF_SIZE);
					//
					// add to response string
					//
					if ( len>0 ) {
						buf[len] = 0x00;
						m_response += (LPCTSTR) buf;
					}

					//
					// get content length
					//
					if ( content_length==0 ) {
						if ( (f = m_response.find("Content-Length: ")) &&
							 m_response.find(CRLF, f+16) ) {
							LPCTSTR s = m_response.getBuffer()+f+16;
							while( *s>='0' && *s<='9' ) {	// is digit?
								content_length = (content_length * 10) + (*s-'0');
								s++;
							}
						}
					}

					//
					// check total length
					//
					if ( total_length==0 && content_length ) {
						if ( (f=m_response.find(CRLF CRLF)) ) {
							total_length = content_length + f + 4;
						}
					}
				}
				//
				// parse response
				//
				if ( m_response.find("HTTP/1.1 200 OK")>=0 ) {
					result = true;
				}
				delete buf;
			}
		}
	}

	if ( m_sock->getLastError()!=ERR_OK ) {
		m_sock->close();	// free all SSL resource & memory!!
	}
	return result;
}

CString HttpClient::buildGET(LPCTSTR uri) {
	CString m_header;
	m_header.printf(
			"GET %s HTTP/1.1" CRLF
			"Host: %s" CRLF
			"Connection: keep-alive" CRLF
			"Cache-Control: max-age=0" CRLF
			"User-Agent: uCXpresso/1.0.1" CRLF
			CRLF,
			uri,
			m_host
			);
	return m_header;
}
