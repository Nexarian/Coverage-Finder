// PersistentString.cpp: implementation of the PersistentString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PersistentString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistentString::PersistentString(bool OverrideStrSize, unsigned long OSize)
{
	BasicConstruct(OverrideStrSize, OSize);
}
PersistentString::PersistentString(const char* NewStr) {
	BasicConstruct();

	*this = NewStr;
}
PersistentString::PersistentString(PersistentString& NewStr) {
	BasicConstruct();

	*this = NewStr;
}

void PersistentString::BasicConstruct(bool OverrideStrSize, unsigned long OSize) {
	Str = new char[1];
	Str[0] = '\0';

	SetDerivedClassName(typeid(*this).name());
	FileStream::Classes().AddFactory(new Factory<PersistentString>);

	SizeOverride = OverrideStrSize;
	OverriddenSize = OSize;
}

PersistentString::~PersistentString()
{
	delete [] Str;
}

const unsigned long PersistentString::GetStringLength() const {
	if (SizeOverride == true) {
		return OverriddenSize;
	}
	else {
		return strlen(Str);
	}
}

void PersistentString::SetStringLength(unsigned long NewSize) {
	if (SizeOverride == true) {
		OverriddenSize = NewSize;
	}
	else {
		OutputDebugString("Non-Fatal Error: Attempting to override PersistentString size without override enabled, override will be ignored\n");
	}
}

PersistentString& PersistentString::operator=(PersistentString& StrEqual) {
	if (*this != StrEqual) {
		SizeOverride = StrEqual.GetStringOverride();
		OverriddenSize = StrEqual.GetStringLength();
		SetStrsEqual(StrEqual, OverriddenSize);
	}
	return *this;
}

const char* PersistentString::operator=(const char* StrEqual) {
	if (*this != StrEqual) {
		SetStrsEqual(StrEqual, strlen(StrEqual));
	}
	return *this;
}

PersistentString PersistentString::operator+(const char StrPlus) {
	char CharStr[2];
	CharStr[0] = StrPlus;
	CharStr[1] = '\0';

	return operator+(CharStr);
}

PersistentString PersistentString::operator+(const char* StrPlus) {
	PersistentString AddBuffer(StrPlus);

	return operator+(AddBuffer);
}

PersistentString PersistentString::operator+(const PersistentString& StrPlus) {
	PersistentString AddBuffer;
	AddBuffer = Str;
	AddBuffer += StrPlus;
	return AddBuffer;
}

PersistentString PersistentString::operator+=(const char StrPlus) {
	//Convert the char to a string and then call the other operator +=
	char CharStr[2];
	CharStr[1] = '\0';
	CharStr[0] = StrPlus;

	return operator+=(CharStr);
}

PersistentString PersistentString::operator+=(const char* StrPlus) {
	PersistentString Buffer = StrPlus;

	return operator+=(Buffer);
}

PersistentString PersistentString::operator+=(const PersistentString& StrPlus) {

	PersistentString PreserveBuffer = *this;

	delete [] Str;

	int OriginalStringLength = PreserveBuffer.GetStringLength();
	int NewStringLength = OriginalStringLength + StrPlus.GetStringLength() + 1;

	Str = new char[NewStringLength];
	Str[NewStringLength - 1] = '\0';

	int StrLoop = 0;

	for (StrLoop = 0; StrLoop < (NewStringLength - 1); ++StrLoop) {
		if (StrLoop < OriginalStringLength) {
			Str[StrLoop] = PreserveBuffer.Str[StrLoop];
		}
		else {
			Str[StrLoop] = StrPlus.Str[StrLoop - OriginalStringLength];
		}
	}

	return *this;
}

void PersistentString::SetStrsEqual(const char* StrEqual, unsigned long LengthOfNewString) {
	delete [] Str;

	OverriddenSize = LengthOfNewString + 1;

	Str = new char[OverriddenSize];
	Str[OverriddenSize - 1] = '\0';
	memcpy(Str, StrEqual, LengthOfNewString);
}

const bool PersistentString::operator==(const char* CompareString) const {
	PersistentString CompareBuffer(CompareString);
	return (*this) == CompareBuffer;
}

const bool PersistentString::operator==(PersistentString& CompareString) const {
	if (GetStringLength() == CompareString.GetStringLength()) {

		int StrLoop = 0;
		int StrMax = GetStringLength();

		for (StrLoop; StrLoop < StrMax; ++StrLoop) {
			if (Str[StrLoop] != CompareString.Str[StrLoop]) return false;
		}
		return true;
	}
	return false;
}

void PersistentString::DataTransaction(FileStream& File) {
	if (File.IsSaving() == true) {
		File << SizeOverride;
		File << OverriddenSize;

		if (SizeOverride == true) {
			File.WriteString(Str, OverriddenSize);
		}
		else {
			File << Str;
		}
	}
	else {
		File >> SizeOverride;
		File >> OverriddenSize;

		char* Buf;
		File >> Buf;

		if (SizeOverride == true) {
			SetStrsEqual(Buf, OverriddenSize);	
		}
		else {
			SetStrsEqual(Buf, strlen(Buf));
		}

		delete [] Buf;
	}
}