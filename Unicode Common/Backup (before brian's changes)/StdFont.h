// StdFont.h: interface for the StdFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDFONT_H__E3B6FC41_40B7_11D4_A81C_0050BAA28E02__INCLUDED_)
#define AFX_STDFONT_H__E3B6FC41_40B7_11D4_A81C_0050BAA28E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

enum FontWeight {
	WEIGHT_DONTCARE   = 0,
	WEIGHT_THIN       = 100,
	WEIGHT_ULTRALIGHT = 200, 
	WEIGHT_LIGHT      = 300, 
	WEIGHT_NORMAL     = 400,  
	WEIGHT_MEDIUM     = 500,
	WEIGHT_SEMIBOLD   = 600,
	WEIGHT_BOLD       = 700,
	WEIGHT_ULTRABOLD  = 800,
	WEIGHT_HEAVY      = 900,
};
enum FontFamily {
	FAMILY_DECORATIVE = FF_DECORATIVE,
	FAMILY_DONTCARE   = FF_DONTCARE,
	FAMILY_MODERN     = FF_MODERN,
	FAMILY_ROMAN      = FF_ROMAN,
	FAMILY_SCRIPT     = FF_SCRIPT,
	FAMILY_SWISS      = FF_SWISS
};
class StdFont  
{
public:
	StdFont(int Size = 0, bool Italic = false, bool Underline = false, bool Strikeout = false, const char *Name = "MS Sans Serif", FontFamily Family = FAMILY_SWISS, FontWeight Boldness = WEIGHT_NORMAL);
	StdFont(StdFont &NewFont);
	virtual ~StdFont();
	void CreateNewFont(int Size, bool Italic, bool Underline, bool Strikeout, const char *Name, FontFamily Family, FontWeight Boldness = WEIGHT_NORMAL);
	//Get functions
	int GetFontSize() {return FontProperties.lfHeight;}
	char *GetFontName() {return FontProperties.lfFaceName;}
	FontWeight Bold() {return static_cast<FontWeight>(FontProperties.lfWeight);}
	bool GetItalic() {return ConvertByteToBool(FontProperties.lfItalic);}
	bool GetStrikeOut() {return ConvertByteToBool(FontProperties.lfStrikeOut);}
	bool GetUnderline() {return ConvertByteToBool(FontProperties.lfUnderline);}
	FontFamily GetFamily() {return FF;} 
	HFONT GetHFont() {return RealFont;}
	//Set functions
	void SetFontSize(int NewSize);
	void SetFontName(char NewName[]);
	void SetBold(FontWeight NewBold);
	void SetItalic(bool NewItalic);
	void SetStrikeOut(bool NewStrikeOut);
	void SetUnderline(bool NewUnderline);
	void SetFamily(FontFamily NewFamily);

	LOGFONT GetFontProperties() {return FontProperties;}

	//Overloaded operators
	bool operator==(const StdFont &Object) const {return (memcmp(&FontProperties, &Object.FontProperties,sizeof(FontProperties)) == 0);}
	bool operator!=(const StdFont &Object) const {return !(*this == Object);}
	const StdFont &operator=(StdFont &);
private:
	HFONT RealFont; //This class is devoted to just manipulating this variable :)
	LOGFONT FontProperties; //Font structure that describes the properties of the font
	void CreateHFont();
	inline bool ConvertByteToBool(BYTE Value) {return Value == 1;}
	bool FontCreated;
	FontFamily FF;
};


typedef StdFont* LPSTDFONT;

#endif // !defined(AFX_STDFONT_H__E3B6FC41_40B7_11D4_A81C_0050BAA28E02__INCLUDED_)
