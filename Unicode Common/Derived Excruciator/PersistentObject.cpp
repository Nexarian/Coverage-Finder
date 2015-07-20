// PersistantObject.cpp: implementation of the PersistantObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PersistantObject.h"
#include "FileStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistantObject::PersistantObject(const char* DClassName)
{
	if (DClassName != 0) {
		SetDerivedClassName(DClassName);
	}
	else {
		DerivedClassName = 0;
	}
}

//Copy constructor
PersistantObject::PersistantObject(PersistantObject& CopyObject) {
	if (DerivedClassName != 0) DerivedClassName = 0;
	*this = CopyObject;
}

PersistantObject::~PersistantObject()
{
	if (DerivedClassName != NULL) delete [] DerivedClassName;
}

void PersistantObject::SetDerivedClassName(const char* ClassName) {
	if (DerivedClassName != NULL) delete [] DerivedClassName;
	unsigned int StrLength = strlen(ClassName) + 1;
	DerivedClassName = new char[StrLength];
	DerivedClassName[StrLength - 1] = '\0';
	strcpy(DerivedClassName, ClassName);
}
PersistantObject& PersistantObject::operator=(PersistantObject& CopyObject) {
	if (*this != CopyObject) SetDerivedClassName(CopyObject.DerivedClassName);
	return *this;
}
bool PersistantObject::operator==(PersistantObject& TestObject) {
	if (DerivedClassName == 0) return false;

	return strcmp(DerivedClassName, TestObject.DerivedClassName) == 0;
}

FileStream& operator<<(FileStream& os, const PersistantObject& obj) {
	os << (const char *)obj.DerivedClassName; //Add the class name to the file (this will notate that a class has been seralized
	obj.WriteData(os);
	return os;
}

FileStream& operator>>(FileStream& is, PersistantObject& obj) {
	char * ClassName = NULL;
	is >> ClassName;

	if (strcmp(ClassName, obj.DerivedClassName) == 0) {
		obj.ReadData(is);
	}
	else {
		OutputDebugString("Class Load Error - Class not found in FileStream\n");
	}

	delete [] ClassName;
	
	return is;
}
