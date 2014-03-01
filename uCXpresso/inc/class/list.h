/*
 ===============================================================================
 Name        : list.h
 Author      : jason
 Version     :
 Date		 : 2011/12/28
 Copyright   : Copyright (C) www.embeda.com.tw
 Description : 

 History     :
 ===============================================================================
 */

#ifndef LIST_H_
#define LIST_H_

#include "class/object.h"
#include "class/mutex.h"

typedef void* LIST_POS;
typedef void* ELEM_PTR;

class CList: public CObject {
protected:
	CMutex m_mutex;
	LIST_POS m_ndHead;
	LIST_POS m_ndTail;
	int length;

	virtual LIST_POS alloc();
	virtual void free(LIST_POS pos);

public:
	CList();
	virtual ~CList();

	virtual int  count();
	virtual bool isEmpty();

	virtual LIST_POS addHead(ELEM_PTR elem);
	virtual LIST_POS addTail(ELEM_PTR elem);
	virtual ELEM_PTR getHead();
	virtual ELEM_PTR getTail();
	virtual ELEM_PTR getAt(LIST_POS pos);
	virtual ELEM_PTR getAt(int index);
	virtual ELEM_PTR removeHead();
	virtual ELEM_PTR removeTail();
	virtual ELEM_PTR removeAt(LIST_POS pos);
	virtual void  removeAll(bool free_elem=false, bool isObj=false);

	virtual LIST_POS getHeadPos();
	virtual LIST_POS getTailPos();
	virtual LIST_POS getNext(LIST_POS pos);
	virtual LIST_POS getPrev(LIST_POS pos);
	virtual LIST_POS find(ELEM_PTR elem);
	virtual LIST_POS insertBefore(LIST_POS pos, ELEM_PTR elem);
	virtual LIST_POS insertAfter(LIST_POS pos, ELEM_PTR elem);

	virtual inline ELEM_PTR operator [](int index) {
		return getAt(index);
	}
};


template <class CType>
class CListT: public CList {
public:
	virtual inline LIST_POS addHeadT(CType* elem) {
		return CList::addHead(elem);
	}

	virtual inline LIST_POS addTailT(CType* elem) {
		return CList::addTail(elem);
	}

	virtual inline CType* getHeadT() {
		return (CType *)CList::getHead();
	}

	virtual inline CType* getTailT() {
		return (CType*)CList::getTail();
	}

	virtual inline CType* getAtT(LIST_POS pos) {
		return (CType*)CList::getAt(pos);
	}

	virtual inline CType* removeHeadT() {
		return (CType*)CList::removeHead();
	}

	virtual inline CType* removeTailT() {
		return (CType*)CList::removeTail();
	}

	virtual inline void removeAtT(CType* elem) {
		LIST_POS pos;
		pos = find(elem);
		if ( pos!=NULL ) {
			CList::removeAt(pos);
		}
	}

	virtual inline void removeAllT(bool free_elem=false) {
		CType* obj;
		while( (obj=removeHeadT())!=NULL ) {
			if ( free_elem ) {
				delete obj;
			}
		}
	}

	virtual inline LIST_POS insertBeforeT(LIST_POS pos, CType* elem) {
		return CList::insertBefore(pos, elem);
	}

	virtual inline LIST_POS insertAfterT(LIST_POS pos, CType* elem) {
		return CList::insertAfter(pos, elem);
	}
};

#endif /* LIST_H_ */
