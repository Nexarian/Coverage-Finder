// PersistantObject.h: interface for the PersistantObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTANTOBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_)
#define AFX_PERSISTANTOBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FileStream;
#include <typeinfo.h>

class PersistentObject  
{
public:
	PersistentObject(const char* DClassName = NULL);
	PersistentObject(PersistentObject& CopyObject);
	virtual ~PersistentObject();

	friend class FileStream; //FileStream is allowed to access anything in PersistentObject

	virtual void DataTransaction(FileStream& File) = 0;

	void SetDerivedClassName(const char* ClassName);

	virtual PersistentObject& operator=(PersistentObject& CopyObject);
	virtual bool operator==(PersistentObject& TestObject);
	virtual bool operator!=(PersistentObject& TestObject) {return !((*this) == TestObject);}

private:


protected:

	char* DerivedClassName;

};

#endif // !defined(AFX_PERSISTENT_OBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_)
