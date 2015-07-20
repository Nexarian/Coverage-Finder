// Coverage FinderDlg.h : header file
//

#pragma once

#include "Path.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "AreaCover.h"
#include "direct.h"
#include <stack>

class DisplayDLG;

// CCoverageFinderDlg dialog
class CCoverageFinderDlg : public CDialog
{
// Construction
public:
	CCoverageFinderDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CCoverageFinderDlg();

	void ShowMyself(DisplayDLG* Parent);

// Dialog Data
	enum { IDD = IDD_COVERAGEFINDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	Area* GetArea() {return PathFinder.GetArea();}

	void UpdateUserMessage(const wchar_t* UsrMsg);
	void RestoreUserMessage();

	void DoPathFind(POINT StartPoint, POINT EndPoint);
	void UpdateMouseDisplay(POINT MousePoint);

	afx_msg void OnBnClickedPointButton();

private:
	Path PathFinder;

	DisplayDLG* DrawWindow;

	AreaCover Scanner;

	std::stack<CString> MessageBuffers;

	const char* IdleMessage;
	const char* DrawMessage;
	const char* ProbeMessage;

	bool Initializing;

	OPENFILENAME FileDialogInfo;
	char FileName[MAX_PATH];

	void SetSubRectValue(const unsigned long Value);
	void SetWhiteSpaceValue(const unsigned long Value);
	void SetTotalAreaValue(const unsigned long Value);
	void SetCoverageLength(const unsigned long Value);
	void SetConnectingPathLength(const unsigned long Value);
	void ToggleCheckState(int CheckStatus);

public:
	// Finds the path
	CButton m_PointButton;
	// The ok button
	CButton m_OkButton;
	afx_msg void OnBnClickedDrawProbe();
	// Draw Button
	CButton m_DrawPoints;
	afx_msg void OnBnClickedCover();
	afx_msg void OnBnClickedErase();
	// Erases the paths found
	CButton m_Erase;
	// Covers the area
	CButton m_Cover;
	// Checkbox to determine whether to divide vertically
	CButton m_VertCheck;
	// Checkbox to determine whether or not 
	CButton m_HorzCheck;
	afx_msg void OnBnClickedRectBtn();
	afx_msg void OnBnClickedOk();
	// Status Bar
	CStatusBarCtrl m_StatBar;
	afx_msg void OnBnClickedObRectBtn();
	// The type of scan to be done
	CComboBox m_ScanType;
	
	CButton m_ObRectChk; //Obstacle Rectangles
	CButton m_VertRectsChk; //Vertices
	CButton m_PathFindLineChk; //Pathfinding lines
	CButton m_VertDivChk; //Vertical Division Lines
	CButton m_HorzDivChk; //Horizontal Division Lines
	CButton m_RectDivChk; //Division Rectangle Definitions
	CButton m_ScanChk; //Last scan done
	// The scan direction
	CComboBox m_VertScanDirection;
	CComboBox m_HorzScanDirection;
	afx_msg void OnCbnSelChangeScanVert();
	afx_msg void OnCbnSelChangeScanType();
	afx_msg void OnBnClickedApplyWindow();
	// The Width
	CEdit m_WidthBox;
	// The height
	CEdit m_HeightBox;
	afx_msg void OnBnClickedRestoreWindow();
	CEdit m_DrawSpeed;
	CStatic m_SubVal;
	CStatic m_WhiteVal;
	CStatic m_TotalAreaVal;
	CStatic m_CoverVal;
	CStatic m_ConnectVal;
	afx_msg void OnFileLoad();
	afx_msg void OnFileSave();
	afx_msg void OnFileExit();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// Variable for scanner direction
	CComboBox m_ScanDirection;
	afx_msg void OnCbnSelchangeScandirect();
	CComboBox m_ScanHeuristic;
	afx_msg void OnBnClickedSall();
	afx_msg void OnBnClickedSnone();
	afx_msg void OnBnClickedSavepathbutton();
	afx_msg void OnBnClickedBrowsebutton();
private:
	CEdit m_PathFileBox;
};