#include "StdAfx.h"
#include "PersistentString.h"
#include "Main.h"
#include <assert.h>
#include <vector>

std::vector<LPSTDFONT> Fonts;
typedef int (__cdecl *CasePtr)(int);

LPSTDFONT CreateStdFont(int Size, bool Italic, bool Underline, bool Strikeout, char *Name, FontFamily Family, FontWeight Boldness) {
	LPSTDFONT ReturnFont;
	ReturnFont = new StdFont(Size, Italic, Underline, Strikeout, Name, Family, Boldness);
	Fonts.push_back(ReturnFont);
	return ReturnFont;
}
char *ConvertString(wchar_t *String, CasePtr CaseFunction) {
	wchar_t *ReturnString = new wchar_t[wcslen(String)+1];
	ReturnString[wcslen(String) + 1] = '\0';
	strcpy(ReturnString, String);
	char CharBuffer;
	for (int StringLoop = 0; StringLoop <= (int)strlen(String); StringLoop++) {
		CharBuffer = (wchar_t)((*CaseFunction)((int)String[StringLoop]));
		ReturnString[StringLoop] = CharBuffer;
	}
	return ReturnString;
}
const char *UCase(char *String) {
	return ConvertString(String, toupper);
}
const char *LCase(char *String) {
	return ConvertString(String, tolower);
}

double DegreesToRadians(double Degree) {
	return Degree * (M_PI / 180.0);
}
double RadiansToDegrees(double Radians) {
	return Radians * (180.0 / M_PI);
}

void DebugAssert(bool Expression) {
	if (Expression == false) {
		OutputDebugString(L"Debug Assertion Failed!!!\n");
		assert(false);
	}
}

//Implementations of VB functions that really should be in C++
char* StrMid(const char* String, int Start, int End, int StringLength) {
	if (End < Start || (End - Start) > StringLength) return 0; //end cannot be less than start
	
	int ReturnWidth = (End - Start) + 1;
	char* NewText;
	NewText = new char[ReturnWidth + 1];
	NewText[ReturnWidth] = '\0';
	for (int StringLoop = 0; StringLoop < ReturnWidth; StringLoop++) {
		NewText[StringLoop] = String[StringLoop + Start];
	}
	return NewText;
}

const unsigned long NumberDigits(const long Number) {
	long EffectiveNumber = (Number < 0) ? labs(Number) : Number;

	long Length;
	
	if (EffectiveNumber != 1 && EffectiveNumber != 0) {
		Length = (int)(log10((double)EffectiveNumber) + 1);
	}
	else {
		//Log10(1) = 0 which we don't want - 1 is a 1-digit number, not a zero digit one
		Length = 1;
	}

	if (Number < 0) ++Length; //needed for the negative sign

	return Length;
}

PersistentString NumberToString(double Number) {
	//Convert the integer part only to a string
	//Here's where it gets fancy

	__int64 IntPartBuffer = (__int64)fabs(Number);

	int IntPartRealLength = NumberDigits((long)IntPartBuffer);

	PersistentString IntPartStrBuffer;

	for (int FillLoop = 0; FillLoop < IntPartRealLength; ++FillLoop) {
		/*
		The % operator is the remainder operator.
		Our number system is base ten, and the first number ascii character is 48
		When you divide by 10, the remainder has to be from 0 to 9 (or one of the digits in the number)
		*/

		IntPartStrBuffer += (char)((IntPartBuffer % (__int64)10) + 48);
		IntPartBuffer /= 10; //Now divide it by 10 to erase the last number. Since it's integer division any decimal is dropped
	}

	//This produced the number in reverse, now reverse it so it goes back to normal
	//Copy the number
	int RevLen = strlen(IntPartStrBuffer) + 1;

	wchar_t* RevBuf = new wchar_t[RevLen];
	RevBuf[RevLen - 1] = '\0';

	strcpy(RevBuf, IntPartStrBuffer);
	
	IntPartStrBuffer = _strrev(RevBuf);

	delete [] RevBuf;

	//Now do the decimals (by turning the decimal part into an integer and converting it, and then append the decimal part as a decimal)
	double DecBuffer = DecimalPart(Number);

	//Now convert the decimal to an integer
	if (DecBuffer != 0) { //If there isn't any decimal just don't do anything

		int DecPlaces = 0;

		int NumZeros = 0; //The number of zero's preceding

		do {
			//Move the decimal place to the left until the whole thing is now an integer, instead of a decimal
			DecBuffer *= 10;

			if (IntegerPartRound(DecBuffer) == 0) NumZeros++;
			
		} while (DecimalPart(DecBuffer) != 0 && (++DecPlaces) < 14);

		PersistentString DecPartStrBuffer = NumberToString((double)(__int64)DecBuffer); //Use recursion to figure out the rest

		//Since the Decimal has been converted to an integer, leading zero's may be lost
		//Add those zeros in now if any existed
		if (NumZeros != 0) {
			PersistentString ZeroString;

			for (int ZeroLoop = 0; ZeroLoop < NumZeros; ZeroLoop++) {
				ZeroString += "0";
			}

			DecPartStrBuffer = ZeroString + DecPartStrBuffer;
		}

		IntPartStrBuffer += ".";
		IntPartStrBuffer += DecPartStrBuffer;
	}

	if (fabs(Number) != Number) { //The Number is negative
		PersistentString Buffer;
		Buffer = IntPartStrBuffer;

		IntPartStrBuffer = "-";
		IntPartStrBuffer += Buffer;
	}

	return IntPartStrBuffer;
}

double StringToNumber(const wchar_t* String) { //String is a null-terminated c-style string
	//Each character in the string MUST be a digit (0 to 9, a decimal point or a negative sign, otherwise the operation is undefined)
	double ReturnNumber = 0;

	int LoopMax = wcslen(String);
	int StringLoop = 0;

	double NumberToAdd = 0;

	bool Negative = String[0] == '-';


	int DecimalPointPosition = 0;
	
	for (DecimalPointPosition; (String[DecimalPointPosition] != '.') && (DecimalPointPosition < LoopMax); DecimalPointPosition++) {
	}

	int DistanceFromDecimalPoint;

	if (DecimalPointPosition == LoopMax) {
		DistanceFromDecimalPoint = 0;
	}
	else {
		DistanceFromDecimalPoint = DecimalPointPosition - (LoopMax - (int)Negative);
	}

	int CurrentStrPosition = LoopMax - 1;

	for (StringLoop = Negative; StringLoop < LoopMax; ++StringLoop) {

		if (String[CurrentStrPosition] != '0' && String[CurrentStrPosition] != '.') {

			NumberToAdd = (((int)String[CurrentStrPosition]) - 48) * pow(10.0, DistanceFromDecimalPoint);
			ReturnNumber += NumberToAdd;
		}

		if (String[CurrentStrPosition] != '.') {
			DistanceFromDecimalPoint++;
		}

		CurrentStrPosition--;
	}

	if (String[0] == '-') ReturnNumber *= -1;

	return ReturnNumber;
}

int RandomNumber(int MinNumber, int MaxNumber) {
	/*
	rand_s() returns a number from 0 to UINT_MAX
	So divide the return value of rand by UINT_MAX + 1 (b/c we never want it to be 100%, if it is then it would return a number from 0 to MaxNumber (or possibly MaxNumber + 1)
	That returns the percentage. The percentage is mutiplied by MaxNumber - MinNumber + 1 (which is the number of numbers in between the two). Then add the min
	*/

	unsigned int ReturnValue;
	rand_s(&ReturnValue);

	return (int)((((double)ReturnValue / (double)(UINT_MAX + (double)1)) * (double)(MaxNumber - MinNumber + 1))) + MinNumber;
}

PersistentString& GetCurrentWorkingDirectory() {
	static PersistentString ReturnString;
	char* Buffer = _getcwd(NULL, _MAX_PATH);
	ReturnString = Buffer;
	free(Buffer);
	return ReturnString;
}