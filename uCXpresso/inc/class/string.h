/*
 ===============================================================================
 Name        : string.h
 Author      : Jason
 Version     : v1.0.5
 Date		 : 2013/5/21
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : 

 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/2/1	v1.0.0	First Edition									Jason
 2012/8/15	v1.0.1	Add two operators								Jason
 	 	 	 	 		+= (TCHAR)
 	 	 	 	 		+= (LPCTSTR)
 2012/8/30	v1.0.2	Add lock flag in getBuffer						Jason
 	 	 	 	 	 	 to keep string memory, don't auto destroy.
 2012/9/19	v1.0.3	Add 3 operators									Jason
  	 	 	 	 	 	+= (int)
  	 	 	 	 	 	+= (double)
  	 	 	 	 	 	+= (uint32_t)
 2013/4/5	v1.0.4	Rename member function params to field			Jason
 	 	 	 	 	Rename member function paramsInt to fieldInt
 	 	 	 	 	Rename member function paramsFloat to fieldFloat
  	 	 	 	 	Add copy string with length
 2013/5/21	v1.0.5  Fixed bugs:										Jason
 	 	 	 	 	 makeUpper()
 	 	 	 	 	 makeLower()
 	 	 	 	 	 copy()
  ===============================================================================
 */

#ifndef STRING_HPP_
#define STRING_HPP_

#include "class/smallprintf.h"

#define DEF_STRBUF_SIZE	64

/*!	\class CString string.h "class/string.h"
	\brief a string class inherit from CSmallPrintf.
 */
class CString: public CSmallPrintf {
public:
	CString(int size=DEF_STRBUF_SIZE);
	CString(const CString &str);
	CString(LPCTSTR str, int length=0);
	CString(LPTSTR buf);
	virtual ~CString();

	virtual int		getBufSize();
	virtual LPTSTR 	getBuffer(bool lock=false);
	virtual int		chkLength();
	virtual void 	freeExtra();
	virtual int 	length();

	virtual void 	clear();
	virtual TCHAR 	getAt(int index);
	virtual void	setAt(int index, TCHAR ch);
	virtual int		copy(CString &str);
	virtual int		copy(LPCTSTR str, int length);
	virtual CString clone();

	virtual LPCTSTR makeUpper();
	virtual LPCTSTR makeLower();
	virtual LPCTSTR makeReverse();
	virtual CString mid(int first, int count);

	virtual int find(LPCTSTR str, int start=0);
	virtual int find(TCHAR ch, int start=0);
	virtual int findIC(LPCTSTR str, int start=0);

	virtual int replace(LPCTSTR oldstr, LPCTSTR newstr);
	virtual int uriDecode(LPCTSTR uri);

	virtual CString field(int index, TCHAR separate=0x20);
	virtual int		fieldInt(int index, TCHAR separate=0x20);
	virtual float	fieldFloat(int index, TCHAR separate=0x20);

	virtual TCHAR 	operator [](int index);
	virtual void 	operator = (CString &str);
	virtual void	operator = (LPCTSTR str);

	virtual CString& operator + (CString &str);
	virtual CString& operator + (TCHAR ch);
	virtual CString& operator + (LPCTSTR str);

	virtual void operator += (CString &str);
	virtual void operator += (TCHAR ch);
	virtual void operator += (LPCTSTR str);

	virtual inline void operator += (int val) {
		putv(val);
	}
	virtual inline void operator += (double val) {
		putv(val);
	}
	virtual inline void operator +=  (uint32_t val) {
		putv((unsigned)val);
	}

	virtual bool	 operator == (CString &str);
	virtual bool	 operator == (LPCTSTR str);

	virtual inline LPCTSTR operator *() {
			return getBuffer();
	}
	virtual inline operator LPCTSTR () {
		return getBuffer();
	}

	/*! \cond PRIVATE */
protected:
	LPTSTR	m_buffer;
	int		m_length;
	int		m_size;

	// virtual functions for class smallprintf
	virtual void alloc(int bufsize=DEF_STRBUF_SIZE);
	virtual int realloc(int chkStrLen);
	virtual void free();
	virtual int putc(int ch);
	virtual int getc(){return 0;}
	inline virtual LPCTSTR gets() {
		return (LPCTSTR) getBuffer();
	}
	void moveL(int first, int count);
	void moveR(int first, int count);
	/*! \endcond */
};
#endif /* STRING_H_ */
