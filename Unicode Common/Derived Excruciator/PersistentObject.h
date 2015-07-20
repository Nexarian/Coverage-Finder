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

class PersistantObject  
{
public:
	PersistantObject(const char* DClassName = NULL);
	PersistantObject(PersistantObject& CopyObject);
	virtual ~PersistantObject();

	friend class FileStream; //FileStream is allowed to access anything in PersistantObject

	virtual void ReadData(FileStream& File) = 0;
	virtual void WriteData(FileStream& File) const = 0;

	void SetDerivedClassName(const char* ClassName);

	virtual PersistantObject& operator=(PersistantObject& CopyObject);
	virtual bool operator==(PersistantObject& TestObject);
	virtual bool operator!=(PersistantObject& TestObject) {return !((*this) == TestObject);}

	friend FileStream& operator<<(FileStream& os, const PersistantObject& obj);
	friend FileStream& operator>>(FileStream& is, PersistantObject& obj);

protected:

	char* DerivedClassName;

};

FileStream& operator<<(FileStream& os, const PersistantObject& obj);
FileStream& operator>>(FileStream& is, PersistantObject& obj);

#endif // !defined(AFX_PERSISTANTOBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_)
