/*
 ===============================================================================
 Name        : json.h
 Author      : Jason
 Version     : v1.0.2
 Date		 : 2013/01/08
 Description :
 ===============================================================================
 History
 ---------+---------+--------------------------------------------+--------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+--------------
 2012/10/01	v1.0.0	First Edition.									Jason
 2013/01/02 v1.0.1	Add raw() member function						Jason
 2013/01/08 v1.0.2  Add string compare operators 					Jason
 2013/01/11 v1.0.3  Add type compare operators						Jason
 ===============================================================================
 */

#ifndef JSON_H_
#define JSON_H_

#include "class/webserver.h"
#include "class/string.h"
#include "class/list.h"

class CJson: public CWebServer {
public:
	enum JSON_TYPE{
        JSON_NULL = 0,
        JSON_OBJECT,
        JSON_ARRAY,
        JSON_STRING,
        JSON_INT,
        JSON_FLOAT,
        JSON_BOOL,
	};

public:
	CJson(LPTSTR parse=NULL);
	virtual ~CJson();

	CJson operator [] (LPCTSTR key);
	CJson operator [] (int index);

	virtual LPCTSTR asString();
	virtual bool	asBoolean();
	virtual int 	asInteger();
	virtual float	asFloat();

	virtual void operator = (CJson &json);

	/**Call the member function to retrieve the current JSON Value type
	 * \return the JSON_TYPE emulations.
	 */
	virtual JSON_TYPE type();

	/**Call the member function to retrieve the current JSON raw data.
	 * \return the LPCTSTR string to point to raw data.
	 */
	LPCTSTR	  raw();
	int		  raw_length();

	/**Shorthand compare operator for string value
	 */
	virtual bool operator == (LPCTSTR str);
	virtual bool operator != (LPCTSTR str);

	virtual inline bool operator == (JSON_TYPE t) {
		return (type()==t?true:false);
	}

	virtual inline bool operator != (JSON_TYPE t) {
		return (type()!=t?true:false);
	}

	/**Error
	 */
	inline ERR_T error() {
		if ( m_root )
			return ERR_OK;
		return ERR_VAL;
	}

	inline CString errorDesc() {
		CString err;
		err.printf("%d: %s <-%s", m_errorLine, m_errorPos, m_errorDesc);
		return err;
	}


protected:
	CList	m_lstMem;
	xHandle	m_root;

	LPCTSTR m_errorDesc;
	LPCTSTR m_errorPos;
	int 	m_errorLine;
};

#endif /* JSON_H_ */
