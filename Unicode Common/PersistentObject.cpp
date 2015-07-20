// PersistentObject.cpp: implementation of the PersistentObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PersistentObject.h"
#include "FileStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistentObject::PersistentObject()
{

}
//Copy constructor
PersistentObject::PersistentObject(PersistentObject& CopyObject) {
	*this = CopyObject;
}
PersistentObject::~PersistentObject()
{

}
PersistentObject& PersistentObject::operator=(PersistentObject& CopyObject) {
	return *this;
}
bool PersistentObject::operator==(PersistentObject& TestObject) {
	if (&PersistentObject::DerivedClassName == 0) return false;

	return strcmp(DerivedClassName(), TestObject.DerivedClassName()) == 0;
}

const char* PersistentObject::DerivedClassName() const {
	return typeid(*this).name();
}

FileStream& operator<<(FileStream& os, const PersistentObject& obj) {
	os << obj.DerivedClassName(); //Add the class name to the file (this will notate that a class has been seralized
	obj.WriteData(os);
	return os;
}

FileStream& operator>>(FileStream& is, PersistentObject& obj) {
	char* ClassName = NULL;
	is >> ClassName;

	if (strcmp(ClassName, obj.DerivedClassName()) == 0) {
		obj.ReadData(is);
	}
	else {
		OutputDebugString(L"Class Load Error - Class not found in FileStream\n");
	}

	delete [] ClassName;
	
	return is;
}
