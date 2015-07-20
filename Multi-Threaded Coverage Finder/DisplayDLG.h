#pragma once

#include "Coverage FinderDlg.h"

// DisplayDLG dialog

class DisplayDLG : public CDialog
{
	DECLARE_DYNAMIC(DisplayDLG)

public:
	DisplayDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~DisplayDLG();

	void ExitApp();
	void DoAddRect();
	void DoPathLine();
	void DoResize(long Width, long Height);

// Dialog Data
	enum { IDD = IDD_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	CCoverageFinderDlg ControlWindow;

	//Selection variables
	POINT FirstPoint;
	POINT SecondPoint;
	bool FirstSelect;
	bool SecondSelect;

	//Flags to indicate what's going on
	bool AddRect;
	bool PathLine;
};
