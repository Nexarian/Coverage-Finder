// PersistentObject.h: interface for the PersistentObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTENTOBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_)
#define AFX_PERSISTENTOBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FileStream;
#include <typeinfo.h>

class PersistentObject  
{
public:
	PersistentObject();
	PersistentObject(PersistentObject& CopyObject);
	virtual ~PersistentObject();

	friend class FileStream; //FileStream is allowed to access anything in PersistentObject

	virtual void ReadData(FileStream& File) = 0;
	virtual void WriteData(FileStream& File) const = 0;

	virtual PersistentObject& operator=(PersistentObject& CopyObject);
	virtual bool operator==(PersistentObject& TestObject);
	virtual bool operator!=(PersistentObject& TestObject) {return !((*this) == TestObject);}

	friend FileStream& operator<<(FileStream& os, const PersistentObject& obj);
	friend FileStream& operator>>(FileStream& is, PersistentObject& obj);

protected:

	const char* DerivedClassName() const;

};

FileStream& operator<<(FileStream& os, const PersistentObject& obj);
FileStream& operator>>(FileStream& is, PersistentObject& obj);

#endif // !defined(AFX_PERSISTENTOBJECT_H__6CBD68B9_A7F2_49D5_86B5_1BD24033BEBD__INCLUDED_)
