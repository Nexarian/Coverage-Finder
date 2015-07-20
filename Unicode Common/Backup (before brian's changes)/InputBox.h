// InputBox.h: interface for the InputBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTBOX_H__560FC96E_8969_40B3_A73A_C52967CDE6C3__INCLUDED_)
#define AFX_INPUTBOX_H__560FC96E_8969_40B3_A73A_C52967CDE6C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class InputBox  
{
public:
	InputBox();
	virtual ~InputBox();

private:
	char* Prompt;

};

#endif // !defined(AFX_INPUTBOX_H__560FC96E_8969_40B3_A73A_C52967CDE6C3__INCLUDED_)
