// StdFont.cpp: implementation of the StdFont class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "StdFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StdFont::StdFont(int Size, bool Italic, bool Underline, bool Strikeout, const char *Name, FontFamily Family, FontWeight Boldness)
{
	FontCreated = false;
	CreateNewFont(Size, Italic, Underline, Strikeout, Name, Family, Boldness);
}
StdFont::StdFont(StdFont &NewFont) {
	*this = NewFont;
}
StdFont::~StdFont()
{
	if (FontCreated == true) DeleteObject(RealFont);
}
void StdFont::CreateNewFont(int Size, bool Italic, bool Underline, bool Strikeout, const char *Name, FontFamily Family, FontWeight Boldness) {
	FontProperties.lfHeight = Size;
	FontProperties.lfWidth = 0;
	FontProperties.lfEscapement = 0;
	FontProperties.lfOrientation = 0;
	FontProperties.lfWeight = Boldness;
	FontProperties.lfItalic = Italic;
	FontProperties.lfUnderline = Underline;
	FontProperties.lfStrikeOut = Strikeout;
	FontProperties.lfCharSet = DEFAULT_CHARSET;
	FontProperties.lfOutPrecision = OUT_DEFAULT_PRECIS;
	FontProperties.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	FontProperties.lfQuality = DEFAULT_QUALITY;
	FontProperties.lfPitchAndFamily = DEFAULT_PITCH | Family;
	strncpy_s(FontProperties.lfFaceName, Name, strlen(Name));
	CreateHFont();
}
void StdFont::CreateHFont() {
	if (FontCreated == true) DeleteObject(RealFont); //Delete the old font before creating a new one
	RealFont = CreateFontIndirect(&FontProperties);
	FontCreated = true;
}
//Set functions
void StdFont::SetFontSize(int NewSize) {
	if (FontProperties.lfHeight != NewSize) {
		FontProperties.lfHeight = NewSize;
		CreateHFont();
	}
}
void StdFont::SetFontName(char *NewName) {
	if (strcmp(FontProperties.lfFaceName, NewName) != 0) {
		strcpy_s(FontProperties.lfFaceName, NewName);
		CreateHFont();
	}
}
void StdFont::SetBold(FontWeight NewBold) {
	if (FontProperties.lfWeight != NewBold) {
		FontProperties.lfWeight = NewBold;
		CreateHFont();
	}
}
void StdFont::SetItalic(bool NewItalic) {
	if (ConvertByteToBool(FontProperties.lfItalic) != NewItalic) {
		FontProperties.lfItalic = NewItalic;
		CreateHFont();
	}
}
void StdFont::SetStrikeOut(bool NewStrikeOut) {
	if (ConvertByteToBool(FontProperties.lfStrikeOut) != NewStrikeOut) {
		FontProperties.lfStrikeOut = NewStrikeOut;
		CreateHFont();
	}
}
void StdFont::SetUnderline(bool NewUnderline) {
	if (ConvertByteToBool(FontProperties.lfUnderline) != NewUnderline) {
		FontProperties.lfUnderline = NewUnderline;
		CreateHFont();
	}
}
void StdFont::SetFamily(FontFamily NewFamily) {
	if (FF != NewFamily) {
		FF = NewFamily;
		CreateHFont();
	}
}
const StdFont &StdFont::operator=(StdFont &Object) {
	if (*this != Object) FontProperties = Object.FontProperties;
	Object.CreateHFont();
	return *this; //allows cascading
}