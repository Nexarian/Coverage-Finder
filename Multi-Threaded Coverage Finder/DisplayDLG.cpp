// DisplayDLG.cpp : implementation file
//

#include "stdafx.h"
#include "Coverage Finder.h"
#include "DisplayDLG.h"

// DisplayDLG dialog

IMPLEMENT_DYNAMIC(DisplayDLG, CDialog)
DisplayDLG::DisplayDLG(CWnd* pParent /*=NULL*/)
	: CDialog(DisplayDLG::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	FirstSelect = false;
	SecondSelect = false;
	AddRect = false;
	PathLine = false;
}
DisplayDLG::~DisplayDLG()
{
}
void DisplayDLG::ExitApp() {
	OnOK();
}
BOOL DisplayDLG::OnInitDialog() {

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ControlWindow.ShowMyself(this);

	return TRUE;
}
void DisplayDLG::OnPaint() {

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		ControlWindow.OnBnClickedDrawProbe();

		CDialog::OnPaint();
	}
}
HCURSOR DisplayDLG::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}
void DisplayDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void DisplayDLG::DoAddRect() {
	FirstSelect = true;
	SecondSelect = false;
	AddRect = true;
	PathLine = false;
	ControlWindow.UpdateUserMessage("Please select the upper-left corner.");
}
void DisplayDLG::DoPathLine() {
	FirstSelect = true;
	SecondSelect = false;
	AddRect = false;
	PathLine = true;
	ControlWindow.UpdateUserMessage("Please select the first point in the line.");
}
void DisplayDLG::DoResize(long Width, long Height) {

	RECT WinRect;
	GetWindowRect(&WinRect);

	SetWindowPos(&wndTop, WinRect.left, WinRect.top, Width, Height, SWP_NOMOVE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}
void DisplayDLG::OnLButtonUp(UINT nFlags, CPoint point) {

	if (FirstSelect || SecondSelect) ControlWindow.RestoreUserMessage();

	if (FirstSelect) {
		FirstPoint = point;

		FirstSelect = false;
		SecondSelect = true;

		if(AddRect) {
			ControlWindow.UpdateUserMessage("Please select the bottom-right corner.");
		}
		else if (PathLine) {
			ControlWindow.UpdateUserMessage("Please select the second point in the line.");
		}
	}
	else if (SecondSelect) {

		SecondPoint = point;

		FirstSelect = SecondSelect = false;

		if (AddRect) {

			int Swapper;

			if (SecondPoint.y < FirstPoint.y) {
				Swapper = FirstPoint.y;
				FirstPoint.y = SecondPoint.y;
				SecondPoint.y = Swapper;
			}
			if (SecondPoint.x < FirstPoint.x) {
				Swapper = FirstPoint.x;
				FirstPoint.x = SecondPoint.x;
				SecondPoint.x = Swapper;
			}

			ControlWindow.GetArea()->AddObstacleRect(FirstPoint.x, FirstPoint.y, abs(SecondPoint.x - FirstPoint.x), abs(SecondPoint.y - FirstPoint.y));
			ControlWindow.OnBnClickedDrawProbe();
						
		}
		else if (PathLine) {

			ControlWindow.DoPathFind(FirstPoint, SecondPoint);

		}
	}
}
void DisplayDLG::OnMouseMove(UINT nFlags, CPoint point) {

	ControlWindow.UpdateMouseDisplay(point);

	if (SecondSelect) {

		ControlWindow.OnBnClickedDrawProbe();

		HDC DrawDC = ::GetDC(m_hWnd);

		HPEN DrawPen = ::CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
		HPEN OldPen = SelectPen(DrawDC, DrawPen);

		MoveToEx(DrawDC, FirstPoint.x, FirstPoint.y, NULL);
		LineTo(DrawDC, point.x, point.y);
		SetPixel(DrawDC, point.x, point.y, RGB(0, 0, 255));

		SelectPen(DrawDC, OldPen);
		DeletePen(DrawPen);
	}
}

BEGIN_MESSAGE_MAP(DisplayDLG, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()