#ifndef COMMON_MAIN_INCLUDED
#define COMMON_MAIN_INCLUDED

#include "StdFont.h"
class PersistantString;
#include <Math.h>
#include <string>
#include <direct.h>

#define IntegerPart(Number) (__int64)((double)Number + 0.5)
#define DecimalPart(Number) (double)Number - (__int64)Number


// Font creator
LPSTDFONT CreateStdFont(int Size, bool Italic, bool Underline, bool Strikeout, char *Name, FontFamily Family, FontWeight Boldness = WEIGHT_NORMAL);
//Custom String routines
const char *UCase(char *String); //Convert all of string to upper-case
const char *LCase(char *String); //Convert all of string to lower-case
char* StrMid(const char* String, int Start, int End, int StringLength);

PersistantString NumberToString(double Number); //returns the number of characters in any double number to the tenth decimal place

double StringToNumber(const char* String);

int RandomNumber(int MinNumber, int MaxNumber);

void DebugAssert(bool Expression);

PersistantString& GetCurrentWorkingDirectory();

#endif