/*
 ===============================================================================
 Name        : weather.h
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

#ifndef WEATHER_H_
#define WEATHER_H_

#include "class/thread.h"

class Weather: public CThread {
public:
	Weather(LPCTSTR id="2306179");	// default WOEID=Taipei
	virtual ~Weather();

	bool request(LPCTSTR id);
	bool start(LPCTSTR name);
	void woeid(LPCTSTR id);

	inline bool isValid() { return m_valid; }
	inline int temp() { return m_temp; }
	inline int humi() { return m_humi; }
protected:
	int 	m_temp;
	int 	m_humi;
	LPCTSTR m_id;
	bool 	m_valid;
	virtual void run();
};

#endif /* WEATHER_H_ */
