// PersistentObject.cpp: implementation of the PersistentObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PersistentObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistentObject::PersistentObject(const char* DClassName)
{
	if (DClassName != NULL) {
		SetDerivedClassName(DClassName);
	}
	else {
		DerivedClassName = NULL;
	}
}

//Copy constructor
PersistentObject::PersistentObject(PersistentObject& CopyObject) {
	*this = CopyObject;
}

PersistentObject::~PersistentObject()
{
	if (DerivedClassName != NULL) delete [] DerivedClassName;
}

void PersistentObject::SetDerivedClassName(const char* ClassName) {
	if (DerivedClassName != NULL) delete [] DerivedClassName;
	unsigned int StrLength = strlen(ClassName) + 1;
	DerivedClassName = new char[StrLength];
	DerivedClassName[StrLength - 1] = '\0';
	strcpy(DerivedClassName, ClassName);
}
PersistentObject& PersistentObject::operator=(PersistentObject& CopyObject) {
	if (*this != CopyObject) SetDerivedClassName(CopyObject.DerivedClassName);
	return *this;
}
bool PersistentObject::operator==(PersistentObject& TestObject) {
	return strcmp(DerivedClassName, TestObject.DerivedClassName) == 0;
}