/*
 ===============================================================================
 Name        : time.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/1/4
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : provide Date & Time information
 History	 :
 2013/3/28	v1.0.2	Add CTime::update() member function.	Jason
 ===============================================================================
 */

#ifndef TIME_H_
#define TIME_H_

#include "object.h"

typedef enum {
	MS = 0,
	US = 1
}TICK_T;

typedef long	TIME_T;

class CTime: public CObject {
protected:
	TIME_T m_time;

public:
	CTime();
	CTime(TIME_T t);
	CTime(int year, int month, int day, int hour, int minute, int second);
	virtual ~CTime();

	virtual void setYear(int year);
	virtual void setMonth(int mon);
	virtual void setDay(int day);
	virtual void setHour(int hour);
	virtual void setMinute(int min);
	virtual void setSecond(int sec);

	virtual int	getYear();
	virtual int getMonth();
	virtual int getDay();
	virtual int getDayOfWeek();
	virtual int getHour();
	virtual int getMinute();
	virtual int getSecond();
	virtual TIME_T getTime();

	virtual void	 operator = (long t);
	virtual void	 operator = (CTime &t);
	virtual double	 operator - (CTime &t);
	virtual void 	 operator += (CTime &t);
	virtual void 	 operator += (TIME_T t);
	virtual void 	 operator -= (CTime &t);
	virtual void 	 operator -= (TIME_T t);
	virtual bool	 operator == (CTime &t);
	virtual TIME_T	 operator * ();

	static TIME_T now();
	static inline TIME_T getCurrentTime() { return now(); }
	static uint32_t getTickCount(TICK_T t=MS);

	//
	// update system date/time (by UTC time)
	//
	static void UTC(int year, int month, int day, int hour, int minute, int second);
};

#endif /* TIME_H_ */
