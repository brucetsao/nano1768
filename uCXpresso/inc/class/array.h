/*
 ===============================================================================
 Name        : array.h
 Author      : Jason
 Version     : 1.0.1
 Created on	 : 2012/2/18
 Copyright   : Copyright (C) 2012 Embeda Technology Inc.
 Description : Array buffer template
 ===============================================================================
  	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2012/2/18	v1.0.1	first edition									Jason
 ===============================================================================
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include "class/buffer.h"

/*! \class CArray buffer.h "class/buffer.h"
 * \brief CArray is a template class to provide a array in any data type.
 */
template <class CType>
class CArray: public gcBuffer {
public:

	CArray(int count):gcBuffer(count) {
	}

	/**Call the member function to retrieve the number of elements.
	 * \return integer
	 */
	virtual inline size_t size() {
		return (gcBuffer::size()/sizeof(CType));
	}

	/**A shorthand for retrieve the pointer of array.
	 */
	virtual inline CType* operator & () {
		return (CType*) m_buf->buf;
	}

	/**A shorthand for retrieve the element of array by index.
	 *
	 * \code
	 * Example:
	 * 		CArray<int> a(100);		// create an array which 100 integer elements.
	 * 		a[55] = 128;
	 * \endcode
	 */
	inline CType& operator [] (int index) {
		return ((CType*)m_buf->buf)[index];
	}

	/*! \cond PRIVATE */
	CArray() {}
	virtual ~CArray() {
		delete (CType *) m_buf->buf;
	}
	/*! \endcond */
};

#endif /* BUFFER_H_ */
