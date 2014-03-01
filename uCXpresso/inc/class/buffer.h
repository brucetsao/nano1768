/*
 ===============================================================================
 Name        : buffer.h
 Author      : Jason
 Version     : 1.0.7
 Created on	 : 2013/7/14
 Copyright   : Copyright (C) 2013 Embeda Technology Inc.
 Description : A buffer class (support Garbage Collection)
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/8/31	v1.0.1	auto realloc the buffer memory if need.			Jason
 2012/9/12	v1.0.2	add append(byte) member function.				Jason
 2012/9/25	v1.0.3	add incRef and decRef for garbage collection.	Jaosn
 2012/10/17 v1.0.4	add clear() member function.					Jason
 2013/7/4	v1.0.5  add autoDestroy in construction. 				Jason
 2013/7/14	v1.0.6	rename autoDestroy to autoCollect				Jason
 	 	    		add attach & discard
 	 	    		add operator = (short hand operator for attach)
 	 	    		add PBUF_T structure for buffer of gcBuffer class
 2013/7/17	v1.0.7	add operator LPCTSTR, LPTSTR					Jason
 ===============================================================================
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "class/object.h"
#include "class/mutex.h"

typedef struct {
	int     length;
	void 	*buf;
	int		ref;
	bool	autoCollect;
}PBUF_T;


#define DEF_BUFFER_SIZE	64

//
// gcBuffer Class
//
class gcBuffer: public CObject {
public:
	gcBuffer(int size=0, bool autoCollect=true);
	gcBuffer(gcBuffer &gc);

	virtual int append(const void *data, int len);
	virtual int append(const byte c);

	virtual size_t size();

	//
	// inline member functions
	//
	inline void* getBuffer() {
		return (m_buf) ? m_buf->buf : NULL;
	}

	inline size_t length() {
		return (m_buf) ? m_buf->length : 0;
	}

	inline void clear() {
		if ( m_buf ) {
			m_buf->length = 0;
		}
	}

	inline int getRemainingSize() {
		return (m_buf) ? size() - m_buf->length : 0;
	}

	inline operator void* () {
		return getBuffer();
	}

	inline operator byte* () {
		return (byte *)getBuffer();
	}

	inline operator LPTSTR() {
		return (LPTSTR)getBuffer();
	}

	inline operator LPCTSTR() {
		return (LPCTSTR)getBuffer();
	}

	inline byte& operator [] (int index) {
		return ((byte *)getBuffer())[index];
	}

	//
	// short hand operators
	//
	virtual gcBuffer& operator = (gcBuffer *buf);
	virtual gcBuffer& operator = (gcBuffer &buf);

	virtual inline bool operator == (gcBuffer &p) {
		return (m_buf==p.m_buf) ? true : false;
	}
	virtual inline bool operator != (gcBuffer &p) {
		return (m_buf!=p.m_buf) ? true : false;
	}

	/*! \cond PRIVATE */
	~gcBuffer();
	void attach(gcBuffer *buf);
	void discard();

protected:
	PBUF_T	*m_buf;
	int	realloc(int more);
	/*! \endcond */
};

#endif /* GC_H_ */
