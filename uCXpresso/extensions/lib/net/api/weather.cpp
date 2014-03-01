/*
 ===============================================================================
 Name        : weather.cpp
 Author      : Jason
 Version     : 1.0.0
 Date		 : 2013/5/30
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : Provide Yahoo's Weather API
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2013/05/30 v1.0.0	First Edition									Jason
 ===============================================================================
 */

#include <cctype>
#include "weather.h"
#include "net/http/httpclient.h"
#include "debug.h"

#define host "weather.yahooapis.com"
#define url  "/forecastrss?u=c&w=%s"


Weather::Weather(LPCTSTR id) {
	m_temp = 0;
	m_humi = 0;
	m_valid = false;
	woeid(id);
}

Weather::~Weather() {
}

bool Weather::start(LPCTSTR name) {
	return CThread::start(name, 160);
}

void Weather::woeid(LPCTSTR id) {
	m_id = id;
}

int getValue(LPCTSTR source) {
	int val=0;
	LPCTSTR p = source;

	if ( source[0]=='-' ) {
		p++;
	}

	for (int i=0; isdigit(p[i]); i++) {
		val = val*10 + (p[i]-'0');
	}

	if ( source[0]=='-' ) {
		val = val * -1;
	}

	return val;
}

bool Weather::request(LPCTSTR id) {
	CString api;
	int f;
	CSocket sock;
	HttpClient weather(sock, host);

	api.printf(url, id);
	if ( weather.request(api) ) {
		// check temperature
		f = weather.response().find("temp=");
		if ( f>0 ) {
			m_temp = getValue(weather.response().getBuffer()+f+6);
		}

		// check humidity
		f = weather.response().find("humidity=");
		if ( f>0 ) {
			m_humi = getValue(weather.response().getBuffer()+f+10);
		}
		m_valid = true;
	} else {
		m_valid = false;
	}
	return m_valid;
}

void Weather::run() {
	while(1) {
		if ( CSocket::getLinkStatus() ) {
			request(m_id);
			sleep(60000);	// update within minute
		} else {
			sleep(30000);	// check again within 30 seconds
		}
	}
}
