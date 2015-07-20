// DataObject.h: interface for the DataObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAOBJECT_H__D70D29E0_13A8_4AFA_91BE_F18616421961__INCLUDED_)
#define AFX_DATAOBJECT_H__D70D29E0_13A8_4AFA_91BE_F18616421961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Main.h"

enum TypeList {
	Int = 0,
	UnInt,
	Long,
	UnLong,
	Short,
	UnShort,
	Float,
	Double,
	Boolean,
	Character
};


class DataObject  
{
public:
	DataObject();
	virtual ~DataObject();

	void SetActiveType(TypeList NewType) {ActiveType = NewType;}
	TypeList GetActiveType() {return ActiveType;}

	DataObject& operator=(DataObject& RightObject);
	bool operator==(DataObject& RightObject);
	bool operator!=(DataObject& RightObject) {return !((*this) == RightObject);}


	//The sets
	void SetInt(int NewInt);
	void SetUnsignedInt(unsigned int NewUInt);

	void SetLong(long NewLong);
	void SetUnsignedLong(unsigned long NewULong);

	void SetShort(short NewShort);
	void SetUnsignedShort(unsigned short NewUShort);

	void SetFloat(float NewFloat);
	void SetDouble(double NewDouble);
	void SetBool(bool NewBool);
	void SetChar(char NewChar);

	//The gets
	int GetInt();
	unsigned int GetUnsignedInt();

	long GetLong();
	unsigned long GetUnsignedLong();

	short GetShort();
	unsigned short GetUnsignedShort();

	float GetFloat();
	double GetDouble();
	bool GetBool();
	char GetChar();

	//The get addresses
	int& GetIntAddr() {return IntType;}
	unsigned int& GetUnsignedIntAddr() {return UnsignedIntType;}

	long& GetLongAddr() {return LongType;}
	unsigned long& GetUnsignedLongAddr() {return UnsignedLongType;}

	short& GetShortAddr() {return ShortType;}
	unsigned short& GetUnsignedShortAddr() {return UnsignedShortType;}

	float& GetFloatAddr() {return FloatType;}
	double& GetDoubleAddr() {return DoubleType;}
	bool& GetBoolAddr() {return BoolType;}
	char& GetCharAddr() {return CharType;}*/



private:
	TypeList ActiveType;

	int IntType;
	unsigned int UnsignedIntType;

	long LongType;
	unsigned long UnsignedLongType;

	short ShortType;
	unsigned short UnsignedShortType;

	float FloatType;
	double DoubleType;
	bool BoolType;
	char CharType;
};

#endif // !defined(AFX_DATAOBJECT_H__D70D29E0_13A8_4AFA_91BE_F18616421961__INCLUDED_)
