// PersistentString.h: interface for the PersistentString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTENTSTRING_H__3FDCEA0C_DC65_4C95_89F1_EE238ED4636B__INCLUDED_)
#define AFX_PERSISTENTSTRING_H__3FDCEA0C_DC65_4C95_89F1_EE238ED4636B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileStream.h"

class PersistentString : public PersistentObject
{
public:
	PersistentString(bool OverrideStrSize = false, unsigned long OSize = 0);
	virtual ~PersistentString();
	//Copy constructors
	PersistentString(PersistentString& NewStr);
	PersistentString(const char* NewStr);

	const unsigned long GetStringLength() const;
	void SetStringLength(unsigned long NewSize); //Override must be enabled to use this

	void SetStringOverride(bool NewStatus) {SizeOverride = NewStatus;}
	bool GetStringOverride() {return SizeOverride;}

	operator const char*() {return GetContents();}
	const char* GetContents() {return Str;}

	PersistentString& operator=(PersistentString& StrEqual);
	const char* operator=(const char* StrEqual);
	//Function used by both operators, allows string length to be specified
	void SetStrsEqual(const char* Str, unsigned long LengthOfNewString);

	PersistentString operator+(const char StrPlus);
	PersistentString operator+(const char* StrPlus);
	PersistentString operator+(const PersistentString& StrPlus);

	//Overloaded overloaded operators
	PersistentString operator+=(const char StrPlusEqual);
	PersistentString operator+=(const char* StrPlusEqual);
	PersistentString operator+=(const PersistentString& StrPlusEqual);

	const bool operator==(PersistentString& StrCompare) const;
	const bool operator==(const char* StrCompare) const;
	const bool operator!=(PersistentString& StrCompare) const {return !(*this == StrCompare);}

	virtual void ReadData(FileStream& File);
	virtual void WriteData(FileStream& File) const;

private:
	//Pointer to the character array
	char* Str;

	bool SizeOverride;
	unsigned long OverriddenSize;

	void BasicConstruct(bool OverrideStrSize = false, unsigned long OSize = 0); //Call this before ANYTHING else
};

#endif // !defined(AFX_PERSISTENTSTRING_H__3FDCEA0C_DC65_4C95_89F1_EE238ED4636B__INCLUDED_)
