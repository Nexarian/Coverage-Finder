#ifndef COMMON_MAIN_INCLUDED
#define COMMON_MAIN_INCLUDED

#include "StdFont.h"
class PersistentString;

#define _USE_MATH_DEFINES
#include <Math.h>

#include <string>
#include <exception>
#include <direct.h>
#include <Mmsystem.h>


#define IntegerPartUnRound(Number) (__int64)((double)Number)
#define IntegerPartRound(Number) (__int64)((double)Number + 0.5)
#define DecimalPart(Number) (double)Number - (__int64)Number

double DegreesToRadians(double Degree);
double RadiansToDegrees(double Radians);

// Font creator
LPSTDFONT CreateStdFont(int Size, bool Italic, bool Underline, bool Strikeout, char *Name, FontFamily Family, FontWeight Boldness = WEIGHT_NORMAL);
//Custom String routines
const char *UCase(char *String); //Convert all of string to upper-case
const char *LCase(char *String); //Convert all of string to lower-case
char* StrMid(const char* String, int Start, int End, int StringLength);

PersistentString NumberToString(double Number); //returns the number of characters in any double number to the tenth decimal place

double StringToNumber(const char* String);

const unsigned long NumberDigits(const long Number);

int RandomNumber(int MinNumber, int MaxNumber);

void DebugAssert(bool Expression);

template< typename TestType >
const bool IsPtrValid(const TestType*& Buffer) {
	try {
		TestType Test = *Buffer; //attempt to dereference the pointer
	}
	catch (...) {
		return false;
	}
	return true;
}

PersistentString& GetCurrentWorkingDirectory();

#endif