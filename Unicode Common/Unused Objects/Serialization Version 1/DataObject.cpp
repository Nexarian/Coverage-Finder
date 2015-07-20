// DataObject.cpp: implementation of the DataObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataObject::DataObject()
{
	//Make sure the data is empty (initialize everything)
	ActiveType = Int;

	IntType = 0;
	UnsignedIntType = 0;

	LongType = 0;
	UnsignedLongType = 0;

	ShortType = 0;
	UnsignedShortType = 0;

	FloatType = 0;
	DoubleType = 0;
	BoolType = false;
	CharType = '\0';
}

DataObject::~DataObject()
{

}

//******* The SETS implementation *******

//The ints
void DataObject::SetInt(int NewInt) {
	if (ActiveType == Int) {
		IntType = NewInt;
	}
}
void DataObject::SetUnsignedInt(unsigned int NewUInt) {
	if (ActiveType == UnInt) {
		UnsignedIntType = NewUInt;
	}
}
//The longs
void DataObject::SetLong(long NewLong) {
	if (ActiveType == Long) {
		LongType = NewLong;
	}
}
void DataObject::SetUnsignedLong(unsigned long NewULong) {
	if (ActiveType == UnLong) {
		UnsignedLongType = NewULong;
	}
}
//The shorts
void DataObject::SetShort(short NewShort) {
	if (ActiveType == Short) {
		ShortType = NewShort;
	}
}
void DataObject::SetUnsignedShort(unsigned short NewUShort) {
	if (ActiveType == UnShort) {
		UnsignedShortType = NewUShort;
	}
}

void DataObject::SetFloat(float NewFloat) {
	if (ActiveType == Float) {
		FloatType = NewFloat;
	}
}
void DataObject::SetDouble(double NewDouble) {
	if (ActiveType == Double) {
		DoubleType = NewDouble;
	}
}
void DataObject::SetBool(bool NewBool) {
	if (ActiveType == Boolean) {
		BoolType = NewBool;
	}
}
void DataObject::SetChar(char NewChar) {
	if (ActiveType == Character) {
		CharType = NewChar;
	}
}

//******* The GETS implementation *******

//The ints
int DataObject::GetInt() {
	if (ActiveType == Int) {
		return IntType;
	}
	else {
		return NULL;
	}
}
unsigned int DataObject::GetUnsignedInt() {
	if (ActiveType == UnInt) {
		return UnsignedIntType;
	}
	else {
		return NULL;
	}
}

//The longs
long DataObject::GetLong() {
	if (ActiveType == Long) {
		return LongType;	
	}
	else {
		return NULL;
	}
}
unsigned long DataObject::GetUnsignedLong() {
	if (ActiveType == UnLong) {
		return UnsignedLongType;
	}
	else {
		return NULL;
	}
}

//The shorts
short DataObject::GetShort() {
	if (ActiveType == Short) {
		return ShortType;
	}
	else {
		return NULL;
	}
}
unsigned short DataObject::GetUnsignedShort() {
	if (ActiveType == UnShort) {
		return UnsignedShortType;
	}
	else {
		return NULL;
	}
}

float DataObject::GetFloat() {
	if (ActiveType == Float) {
		return FloatType;
	}
	else {
		return NULL;
	}
}
double DataObject::GetDouble() {
	if (ActiveType == Double) {
		return DoubleType;
	}
	else {
		return NULL;
	}
}
bool DataObject::GetBool() {
	if (ActiveType == Boolean) {
		return BoolType;
	}
	else {
		return NULL;
	}
}
char DataObject::GetChar() {
	if (ActiveType == Character) {
		return CharType;
	}
	else {
		return NULL;
	}
}

//Overloaded operators
DataObject& DataObject::operator=(DataObject& RightObject) {
	if (*this != RightObject) {
		SetActiveType(RightObject.GetActiveType());
		switch (GetActiveType()) {
			case Int:
				SetInt(RightObject.GetInt());
				break;
			case UnInt:
				SetUnsignedInt(RightObject.GetUnsignedInt());
				break;
			case Long:
				SetLong(RightObject.GetLong());
				break;
			case UnLong:
				SetUnsignedLong(RightObject.GetUnsignedLong());
				break;
			case Short:
				SetShort(RightObject.GetShort());
				break;
			case UnShort:
				SetUnsignedShort(RightObject.GetUnsignedShort());
				break;
			case Float:
				SetFloat(RightObject.GetFloat());
				break;
			case Double:
				SetDouble(RightObject.GetDouble());
				break;
			case Boolean:
				SetBool(RightObject.GetBool());
				break;
			case Character:
				SetChar(RightObject.GetChar());
				break;
		}
	}
	return *this;
}

bool DataObject::operator==(DataObject& RightObject) {
	if (GetActiveType() == RightObject.GetActiveType()) {
		switch (GetActiveType()) {
			case Int:
				return GetInt() == RightObject.GetInt();
				break;
			case UnInt:
				return GetUnsignedInt() == RightObject.GetUnsignedInt();
				break;
			case Long:
				return GetLong() == RightObject.GetLong();
				break;
			case UnLong:
				return GetUnsignedLong() == RightObject.GetUnsignedLong();
				break;
			case Short:
				return GetShort() == RightObject.GetShort();
				break;
			case UnShort:
				return GetUnsignedShort() == RightObject.GetUnsignedShort();
				break;
			case Float:
				return GetFloat() == RightObject.GetFloat();
				break;
			case Double:
				return GetDouble() == RightObject.GetDouble();
				break;
			case Boolean:
				return GetBool() == RightObject.GetBool();
				break;
			case Character:
				return GetChar() == RightObject.GetChar();
				break;
		}
	}
	return false;
}