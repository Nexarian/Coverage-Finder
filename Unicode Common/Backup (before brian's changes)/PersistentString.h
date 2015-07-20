// PersistantString.h: interface for the PersistantString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTANTSTRING_H__3FDCEA0C_DC65_4C95_89F1_EE238ED4636B__INCLUDED_)
#define AFX_PERSISTANTSTRING_H__3FDCEA0C_DC65_4C95_89F1_EE238ED4636B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileStream.h"

class PersistantString : public PersistantObject
{
public:
	PersistantString(bool OverrideStrSize = false, unsigned long OSize = 0);
	virtual ~PersistantString();
	//Copy constructors
	PersistantString(PersistantString& NewStr);
	PersistantString(const char* NewStr);

	const unsigned long GetStringLength() const;
	void SetStringLength(unsigned long NewSize); //Override must be enabled to use this

	void SetStringOverride(bool NewStatus) {SizeOverride = NewStatus;}
	bool GetStringOverride() {return SizeOverride;}


	operator const char*() {return GetContents();}
	const char* GetContents() {return Str;}

	PersistantString& operator=(PersistantString& StrEqual);
	const char* operator=(const char* StrEqual);
	//Function used by both operators, allows string length to be specified
	void SetStrsEqual(const char* Str, unsigned long LengthOfNewString);

	PersistantString operator+(const char StrPlus);
	PersistantString operator+(const char* StrPlus);
	PersistantString operator+(const PersistantString& StrPlus);

	//Overloaded overloaded operators
	PersistantString operator+=(const char StrPlusEqual);
	PersistantString operator+=(const char* StrPlusEqual);
	PersistantString operator+=(const PersistantString& StrPlusEqual);

	const bool operator==(PersistantString& StrCompare) const;
	const bool operator==(const char* StrCompare) const;
	const bool operator!=(PersistantString& StrCompare) const {return !(*this == StrCompare);}

	void DataTransaction(FileStream& File);

private:
	//Pointer to the character array
	char* Str;

	bool SizeOverride;
	unsigned long OverriddenSize;

	void BasicConstruct(bool OverrideStrSize = false, unsigned long OSize = 0); //Call this before ANYTHING else
};

#endif // !defined(AFX_PERSISTANTSTRING_H__3FDCEA0C_DC65_4C95_89F1_EE238ED4636B__INCLUDED_)
