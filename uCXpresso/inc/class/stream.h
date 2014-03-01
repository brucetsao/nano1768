/*
 ===============================================================================
 Name        : stream.h
 Author      : Jason
 Version     : v1.0.0
 Date		 : 2011/12/18
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2011/12/18 v1.0.0	 Initialize Edition								Jason
 ===============================================================================
 */

#ifndef STREAM_H_
#define STREAM_H_

#include "class/object.h"

/*!	\class CStream stream.h "class/stream.h"
	\brief An stream abstract class.<br />
	To define the stream input and output interface.
 */
class CStream: public CObject {
public:
	CStream();
	virtual ~CStream();

	virtual int	 readable() = PURE_VIRTUAL_FUNC;
	virtual int	 writeable() = PURE_VIRTUAL_FUNC;
	virtual int  read(void *buf, int len, bool block=true) = PURE_VIRTUAL_FUNC;
	virtual int  write(const void *buf, int len, bool block=true) = PURE_VIRTUAL_FUNC;
	virtual int  close() = PURE_VIRTUAL_FUNC;
	virtual bool isConnected() = PURE_VIRTUAL_FUNC;
	inline void  flush() { free(); }

	/*! \cond PRIVATE */
public:
	virtual int write(int c);
	virtual int read();
	virtual void alloc(int bufsize=256);
	virtual void free();
	virtual LPCTSTR gets();

protected:
	uint8_t	*m_buffer;
	int		m_index;
	int		m_bufsize;
	/*! \endcond */
};

#endif /* STREAM_H_ */
