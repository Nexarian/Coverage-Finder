// Coverage FinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Coverage Finder.h"
#include "Coverage FinderDlg.h"
#include "DisplayDLG.h"
#include <PersistentString.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCoverageFinderDlg dialog

CCoverageFinderDlg::CCoverageFinderDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCoverageFinderDlg::IDD, pParent), IdleMessage("Idle."), DrawMessage("Drawing Probe."), ProbeMessage("Probing Points.")
{	
}

CCoverageFinderDlg::~CCoverageFinderDlg() {
}

void CCoverageFinderDlg::ShowMyself(DisplayDLG* Parent) {
	DrawWindow = Parent;
	CreateIndirect(LoadResource(NULL, FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDD_COVERAGEFINDER_DIALOG), RT_DIALOG)), NULL);
}

void CCoverageFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINTBUTTON, m_PointButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_DRAWPROBE, m_DrawPoints);
	DDX_Control(pDX, IDC_ERASE, m_Erase);
	DDX_Control(pDX, IDC_COVER, m_Cover);
	DDX_Control(pDX, IDC_VERTCHECK, m_VertCheck);
	DDX_Control(pDX, IDC_HORZCHECK, m_HorzCheck);
	DDX_Control(pDX, IDC_SCANTYPE, m_ScanType);

	DDX_Control(pDX, IDC_OBRECTSCHK, m_ObRectChk);
	DDX_Control(pDX, IDC_VERTRECTSCHK, m_VertRectsChk);
	DDX_Control(pDX, IDC_PATHFINDLINECHK, m_PathFindLineChk);
	DDX_Control(pDX, IDC_VERTDIVCHK, m_VertDivChk);
	DDX_Control(pDX, IDC_HORZDIVCHK, m_HorzDivChk);
	DDX_Control(pDX, IDC_RECTDIVCHK, m_RectDivChk);
	DDX_Control(pDX, IDC_SCANCHK, m_ScanChk);

	DDX_Control(pDX, IDC_SCANVERT, m_VertScanDirection);
	DDX_Control(pDX, IDC_SCANHORZ, m_HorzScanDirection);
	DDX_Control(pDX, IDC_WIDTHBOX, m_WidthBox);
	DDX_Control(pDX, IDC_HEIGHTBOX, m_HeightBox);
	DDX_Control(pDX, IDC_DRAWSPEED, m_DrawSpeed);
	DDX_Control(pDX, IDC_SUBVAL, m_SubVal);
	DDX_Control(pDX, IDC_WHITEVAL, m_WhiteVal);
	DDX_Control(pDX, IDC_TOTALVAL, m_TotalAreaVal);
	DDX_Control(pDX, IDC_COVERVAL, m_CoverVal);
	DDX_Control(pDX, IDC_CONNECTVAL, m_ConnectVal);
	DDX_Control(pDX, IDC_SCANDIRECT, m_ScanDirection);
	DDX_Control(pDX, IDC_SCANHEUR, m_ScanHeuristic);
	DDX_Control(pDX, IDC_FILENAME, m_PathFileBox);
}

BEGIN_MESSAGE_MAP(CCoverageFinderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_POINTBUTTON, OnBnClickedPointButton)
	ON_BN_CLICKED(IDC_DRAWPROBE, OnBnClickedDrawProbe)
	ON_BN_CLICKED(IDC_COVER, OnBnClickedCover)
	ON_BN_CLICKED(IDC_ERASE, OnBnClickedErase)
	ON_BN_CLICKED(IDC_RECTBTN, OnBnClickedRectBtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_OBRECTBTN, OnBnClickedObRectBtn)
	ON_CBN_SELCHANGE(IDC_SCANTYPE, OnCbnSelChangeScanType)
	ON_BN_CLICKED(IDC_APPLYWINDOW, OnBnClickedApplyWindow)
	ON_BN_CLICKED(IDC_RESTOREWINDOW, OnBnClickedRestoreWindow)
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_CBN_SELCHANGE(IDC_SCANDIRECT, OnCbnSelchangeScandirect)
	ON_BN_CLICKED(IDC_SALL, OnBnClickedSall)
	ON_BN_CLICKED(IDC_SNONE, OnBnClickedSnone)
	ON_BN_CLICKED(IDC_SAVEPATHBUTTON, &CCoverageFinderDlg::OnBnClickedSavepathbutton)
	ON_BN_CLICKED(IDC_BROWSEBUTTON, &CCoverageFinderDlg::OnBnClickedBrowsebutton)
END_MESSAGE_MAP()

// CCoverageFinderDlg message handlers

BOOL CCoverageFinderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	VERIFY(m_StatBar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM, CRect(0,0,0,0), this, IDC_STATBAR));

	CRect WinRect;

	DrawWindow->GetClientRect(&WinRect);

	m_ScanDirection.AddString(L"Vertical Scan");
	m_ScanDirection.AddString(L"Horizontal Scan");
	m_ScanDirection.AddString(L"Diagonal Upward Scan");
	m_ScanDirection.AddString(L"Diagonal Downward Scan");

	m_ScanType.AddString(L"Recursive Optimal");
	m_ScanType.AddString(L"Recursive Semi-Optimal");
	m_ScanType.AddString(L"Heuristic Scan");
	m_ScanType.AddString(L"Simple Blind Scan");

	m_ScanHeuristic.AddString(L"Order-Based");
	m_ScanHeuristic.AddString(L"Vertex-Based");
	m_ScanHeuristic.AddString(L"Center-Based");

	m_VertScanDirection.AddString(L"Top-Down Scan");
	m_VertScanDirection.AddString(L"Bottom-Up Scan");

	m_HorzScanDirection.AddString(L"Left-Right Scan");
	m_HorzScanDirection.AddString(L"Right-Left Scan");

	m_WidthBox.SetWindowText(L"500");
	m_HeightBox.SetWindowText(L"500");

	Initializing = true;

	OnBnClickedApplyWindow();

	Initializing = false;

	OnBnClickedRestoreWindow();

	MessageBuffers.push(IdleMessage);

	GetClientRect(&WinRect);
	
	int IntArray[2];
	IntArray[0] = WinRect.Width() / 2;
	IntArray[1] = -1;
	m_StatBar.SetParts(2, IntArray);
	m_StatBar.SetText(IdleMessage, 0, 0);
	m_StatBar.SetText(L"( , )", 1, 0);

	ZeroMemory(&FileDialogInfo, sizeof(FileDialogInfo));

	FileDialogInfo.lStructSize = OPENFILENAME_SIZE_VERSION_400;
	FileDialogInfo.hwndOwner = this->m_hWnd;
	FileDialogInfo.lpstrFilter = NULL;
	FileDialogInfo.lpstrInitialDir = GetCurrentWorkingDirectory();
	FileDialogInfo.lpstrFileTitle = NULL;
	FileDialogInfo.lpfnHook = NULL;
	FileDialogInfo.lCustData = NULL;
	FileDialogInfo.Flags = NULL;
	FileDialogInfo.hInstance = AfxGetInstanceHandle();
	FileDialogInfo.lpstrCustomFilter = NULL;
	FileDialogInfo.lpTemplateName = NULL;
	FileDialogInfo.nMaxFileTitle = 0;
	FileDialogInfo.lpstrFile = FileName;
	FileDialogInfo.nMaxFile = sizeof(FileName);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCoverageFinderDlg::OnPaint() 
{
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
		OnBnClickedDrawProbe();

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCoverageFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCoverageFinderDlg::OnBnClickedPointButton()
{
	if (MessageBox("Do you want to find a path?", "Find Path?", MB_YESNO | MB_ICONQUESTION) == IDYES) {
		DrawWindow->DoPathLine();
	}
}

void CCoverageFinderDlg::DoPathFind(POINT StartPoint, POINT EndPoint) {

	UpdateUserMessage("Finding an optimized path...");

	SetConnectingPathLength((unsigned long)PathFinder.GeneratePath(StartPoint, EndPoint).size());

	RestoreUserMessage();

	OnBnClickedDrawProbe();
}

void CCoverageFinderDlg::UpdateMouseDisplay(POINT MousePoint) {

	char* NumberBuffer;
	int StrLength;
	CString DisplayString;

	StrLength = NumberDigits(MousePoint.x);
	NumberBuffer = new char[StrLength + 1];
	NumberBuffer[StrLength] = '\0';
	ltoa(MousePoint.x, NumberBuffer, 10);

	DisplayString += "(";
	DisplayString += NumberBuffer;
	DisplayString += ", ";
	delete [] NumberBuffer;
	
	StrLength = NumberDigits(MousePoint.y);
	NumberBuffer = new char[StrLength + 1];
	NumberBuffer[StrLength] = '\0';
	ltoa(MousePoint.y, NumberBuffer, 10);

	DisplayString += NumberBuffer;
	DisplayString += ")";
	delete [] NumberBuffer;
	
	m_StatBar.SetText(DisplayString, 1, 0);
}

void CCoverageFinderDlg::OnBnClickedRectBtn()
{
	if (MessageBox(L"Beginning Probe...", L"Probe starting...", MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	UpdateUserMessage(ProbeMessage);

	//This is the line that actually probes. The returned result is the whitespace area
	SetWhiteSpaceValue(GetArea()->ReProbe((m_VertCheck.GetCheck() == 0 ? false : true), (m_HorzCheck.GetCheck() == 0 ? false : true)));
	
	//Set the total area
	SetTotalAreaValue(GetArea()->GetTotalArea());
	
	RestoreUserMessage();

	UpdateUserMessage(DrawMessage);

	OnBnClickedDrawProbe();

	RestoreUserMessage();

	SetSubRectValue((long)GetArea()->Rectangles.size());

	MessageBox(L"Probe Done!", L"Probe Finished!", MB_OK | MB_ICONINFORMATION);
}

void CCoverageFinderDlg::OnBnClickedDrawProbe()
{	
	UpdateUserMessage(DrawMessage);

	HDC DCBuf = ::GetDC(DrawWindow->m_hWnd);

	CString SpeedBuffer;
	m_DrawSpeed.GetWindowText(SpeedBuffer);
	GeometricObject::SetDrawSpeed(_wtol(SpeedBuffer));

	PathFinder.GetArea()->Draw(
		DCBuf, 
		true,
		(m_ObRectChk.GetCheck() == 1 ? true : false), //obstacle rectangles
		(m_VertRectsChk.GetCheck() == 1 ? true : false), //vertices (not vertical)
		(m_VertDivChk.GetCheck() == 1 ? true : false), //vertical division lines
		(m_HorzDivChk.GetCheck() == 1 ? true : false), //horizontal division lines
		(m_RectDivChk.GetCheck() == 1 ? true : false)); //division rectangle definitions

	if (m_ScanChk.GetCheck() == 1 && Scanner.GeneratedCoverages.size()) {
		Scanner.Draw(DCBuf, --Scanner.GeneratedCoverages.end(), false, false, m_PathFindLineChk.GetCheck() == 1, false);
	}

	PathFinder.GetArea()->Draw(
		DCBuf, 
		false,
		(m_ObRectChk.GetCheck() == 1 ? true : false), //obstacle rectangles
		(m_VertRectsChk.GetCheck() == 1 ? true : false), //vertices (not vertical)
		(m_VertDivChk.GetCheck() == 1 ? true : false), //vertical division lines
		(m_HorzDivChk.GetCheck() == 1 ? true : false), //horizontal division lines
		(m_RectDivChk.GetCheck() == 1 ? true : false)); //division rectangle definitions

	if (m_PathFindLineChk.GetCheck() == 1) {
		PathFinder.Draw(DCBuf);
	}

	if (m_ScanChk.GetCheck() == 1 && Scanner.GeneratedCoverages.size()) {
		Scanner.DrawRectangleNumbers(DCBuf, --Scanner.GeneratedCoverages.end());
	}

	::ReleaseDC(DrawWindow->m_hWnd, DCBuf);

	RestoreUserMessage();
}

void CCoverageFinderDlg::OnBnClickedCover()
{
	if (MessageBox(L"Commence with covering?", L"Start?", MB_ICONQUESTION | MB_YESNO) == IDNO) return;

	if (m_ScanType.GetCurSel() == CB_ERR) {
		MessageBox(L"You didn't select a scan type... aborting operation.", L"Fatal Error", MB_ICONSTOP | MB_OK);
		return;
	}
	if (m_ScanDirection.IsWindowEnabled() && m_ScanDirection.GetCurSel() == CB_ERR) {
		MessageBox(L"You didn't select a scan direction... aborting operation.", L"Fatal Error", MB_ICONSTOP | MB_OK);
		return;
	}
	if (m_VertScanDirection.IsWindowEnabled() && m_VertScanDirection.GetCurSel() == CB_ERR) {
		MessageBox(L"You didn't select a vertical starting position... aborting operation.", L"Fatal Error", MB_ICONSTOP | MB_OK);
		return;
	}
	if (m_HorzScanDirection.IsWindowEnabled() && m_HorzScanDirection.GetCurSel() == CB_ERR) {
		MessageBox(L"You didn't select a horizontal starting position... aborting operation.", L"Fatal Error", MB_ICONSTOP | MB_OK);
		return;
	}
	if ((long)GetArea()->Rectangles.size() == 0) {
		MessageBox(L"No rectangles have been defined - please probe the points before doing a scan.", L"Nothing to do!", MB_OK | MB_ICONINFORMATION);
		return;
	}

	SetSubRectValue((long)GetArea()->Rectangles.size());
	UpdateUserMessage("Covering Area.");

	VerticalStart VertStart = (VerticalStart)m_VertScanDirection.GetCurSel();
	HorizontalStart HorzStart = (HorizontalStart)m_HorzScanDirection.GetCurSel();

	Scanner.GeneratedCoverages.clear();
	PathFinder.ClearData();
	Scanner.SetPathCreator(&PathFinder);

	switch (m_ScanType.GetCurSel()) {
		case 0: //Recursive Optimal
			Scanner.PerformCoverage(Complete, (MovementHeuristic)m_ScanHeuristic.GetCurSel());
			break;
		case 1: //Recursive Semi-Optimal
			Scanner.PerformCoverage(Semi, (MovementHeuristic)m_ScanHeuristic.GetCurSel());
			break;
		case 2: //Heurstic
			Scanner.PerformBasicCoverage((ScanType)m_ScanDirection.GetCurSel(), (MovementHeuristic)m_ScanHeuristic.GetCurSel());
			break;
		case 3: //Simple Blind Scan
			Scanner.PerformBlindCoverage((ScanType)m_ScanDirection.GetCurSel(), VertStart, HorzStart);
			break;
	}

	Scanner.RectanglesToPoints(--Scanner.GeneratedCoverages.end(), false);

	unsigned long ConnectingLength = 0;

	SetCoverageLength((unsigned long)Scanner.GeneratedCoverages.back().EquivalentPoints.size());

	for (std::list<PointAr>::const_iterator PathLoop = PathFinder.PathsGenerated.begin(); PathLoop != PathFinder.PathsGenerated.end(); ++PathLoop) {
		ConnectingLength += (unsigned long)PathLoop->size();
	}

	SetConnectingPathLength(ConnectingLength);

	OnBnClickedDrawProbe();

	RestoreUserMessage();
}

void CCoverageFinderDlg::OnBnClickedErase()
{
	UpdateUserMessage(L"Erasing data...");

	if (m_ScanChk.GetCheck() == 1) {
		Scanner.GeneratedCoverages.clear();
	}

	PathFinder.GetArea()->ClearData(
		(m_ObRectChk.GetCheck() == 1 ? true : false), //obstacle rectangles
		(m_VertRectsChk.GetCheck() == 1 ? true : false), //vertices (not vertical)
		(m_VertDivChk.GetCheck() == 1 ? true : false), //vertical division lines
		(m_HorzDivChk.GetCheck() == 1 ? true : false), //horizontal division lines
		(m_RectDivChk.GetCheck() == 1 ? true : false)); //division rectangle definitions

	if (m_PathFindLineChk.GetCheck() == 1) {
		PathFinder.ClearData();
	}

	OnBnClickedDrawProbe();

	RestoreUserMessage();
}

void CCoverageFinderDlg::OnBnClickedOk()
{
	DrawWindow->ExitApp();
}

void CCoverageFinderDlg::OnBnClickedObRectBtn()
{
	//Control;Centered Rectangle;Edge Rectangles;Centered with Edges;Random Rectangles 1;Random Rectangles 2;

	CRect WinRect;
	DrawWindow->GetClientRect(&WinRect);
	WinRect.bottom--;
	WinRect.right--;

	if (false) {
		switch (0) {
			case 0: //control - nothing - delete everything
				OnBnClickedApplyWindow();
				break;
			case 1: //centered rectangle with 1/2 height width of the area
				PathFinder.GetArea()->AddObstacleRect((WinRect.Width() / 2) - (WinRect.Width() / 4), (WinRect.Height() / 2) - (WinRect.Height() / 4), WinRect.Width() / 2, WinRect.Height() / 2);
				break;
			case 2: //edge rectangles with 1/4 width height of area
				PathFinder.GetArea()->AddObstacleRect(0, 0, WinRect.Width() / 4, WinRect.Height() / 4);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 4), 0, WinRect.Width() / 4, WinRect.Height() / 4);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 4), WinRect.Height() - (WinRect.Height() / 4), WinRect.Width() / 4, WinRect.Height() / 4);
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() - (WinRect.Height() / 4), WinRect.Width() / 4, WinRect.Height() / 4);
				break;
			case 3: //special configuration

				//centered rectangle
				PathFinder.GetArea()->AddObstacleRect((WinRect.Width() / 2) - (WinRect.Width() / 4), (WinRect.Height() / 2) - (WinRect.Height() / 4), WinRect.Width() / 2, WinRect.Height() / 2);

				PathFinder.GetArea()->AddObstacleRect(0, 0, WinRect.Width() / 6 - 1, WinRect.Height() / 6 - 1); //edge rectangle 1
				//3 horizontal expansions
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() / 6, 0, WinRect.Width() / 12 - 1, WinRect.Height() / 12 - 1);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() / 6 + WinRect.Width() / 12, 0, WinRect.Width() / 24 - 1, WinRect.Height() / 24 - 1);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() / 6 + WinRect.Width() / 12 + WinRect.Width() / 24, 0, WinRect.Width() / 48 - 1, WinRect.Height() / 48 - 1);
				//3 vertical expansions
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() / 6, WinRect.Width() / 12 - 1, WinRect.Height() / 12 - 1);
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() / 6 + WinRect.Height() / 12, WinRect.Width() / 24 - 1, WinRect.Height() / 24 - 1);
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() / 6 + WinRect.Height() / 12 + WinRect.Height() / 24, WinRect.Width() / 48 - 1, WinRect.Height() / 48 - 1);

				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6), 0, WinRect.Width() / 6, WinRect.Height() / 6 - 1); //edge rectangle 2
				//3 horizontal expansions
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6) - (WinRect.Width() / 12) + 1, 0, WinRect.Width() / 12 + 1, WinRect.Height() / 12 - 1);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6) - (WinRect.Width() / 12) - (WinRect.Width() / 24) + 1, 0, WinRect.Width() / 24 + 1, WinRect.Height() / 24 - 1);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6) - (WinRect.Width() / 12) - (WinRect.Width() / 24) - (WinRect.Width() / 48) + 1, 0, WinRect.Width() / 48 - 1, WinRect.Height() / 48 - 1);
				//3 vertical expansions
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 12) + 1, WinRect.Height() / 6, WinRect.Width() / 12 - 1, WinRect.Height() / 12 - 1);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 24) + 1, WinRect.Height() / 6 + WinRect.Height() / 12, WinRect.Width() / 24 - 1, WinRect.Height() / 24 - 1);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 48) + 1, WinRect.Height() / 6 + WinRect.Height() / 12 + WinRect.Height() / 24, WinRect.Width() / 48 - 1, WinRect.Height() / 48 - 1);

				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6), WinRect.Height() - (WinRect.Height() / 6), WinRect.Width() / 6, WinRect.Height() / 6); //edge rectangle 3
				//3 horizontal expansions
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6) - (WinRect.Width() / 12) + 1, WinRect.Height() - (WinRect.Height() / 12), WinRect.Width() / 12 - 1, WinRect.Height() / 12);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6) - (WinRect.Width() / 12) - (WinRect.Width() / 24) + 1, WinRect.Height() - (WinRect.Height() / 24), WinRect.Width() / 24 - 1, WinRect.Height() / 24);
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 6) - (WinRect.Width() / 12) - (WinRect.Width() / 24) - (WinRect.Width() / 48) + 1, WinRect.Height() - (WinRect.Height() / 48), WinRect.Width() / 48 - 1, WinRect.Height() / 48);
				//3 vertical expansions
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 12) + 1, WinRect.Height() - (WinRect.Height() / 6) - (WinRect.Height() / 12) + 1, (WinRect.Width() / 12) + 1, (WinRect.Height() / 12));
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 24) + 1, WinRect.Height() - (WinRect.Height() / 6) - (WinRect.Height() / 12) - (WinRect.Height() / 24) + 1, (WinRect.Width() / 24) + 1, (WinRect.Height() / 24));
				PathFinder.GetArea()->AddObstacleRect(WinRect.Width() - (WinRect.Width() / 48) + 1, WinRect.Height() - (WinRect.Height() / 6) - (WinRect.Height() / 12) - (WinRect.Height() / 24) - (WinRect.Height() / 48) + 1, (WinRect.Width() / 48) + 1, (WinRect.Height() / 48));

				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() - (WinRect.Height() / 6), WinRect.Width() / 6 - 1, WinRect.Height() / 6); //edge rectangle 4
				//3 horizontal expansions
				PathFinder.GetArea()->AddObstacleRect( (WinRect.Width() / 6), WinRect.Height() - (WinRect.Height() / 12), WinRect.Width() / 12 - 1, WinRect.Height() / 12);
				PathFinder.GetArea()->AddObstacleRect( (WinRect.Width() / 6) + (WinRect.Width() / 12), WinRect.Height() - (WinRect.Height() / 24), WinRect.Width() / 24 - 1, WinRect.Height() / 24);
				PathFinder.GetArea()->AddObstacleRect( (WinRect.Width() / 6) + (WinRect.Width() / 12) + (WinRect.Width() / 24), WinRect.Height() - (WinRect.Height() / 48), WinRect.Width() / 48 - 1, WinRect.Height() / 48);
				//3 vertical expansions
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() - (WinRect.Height() / 6) - (WinRect.Height() / 12) + 1, (WinRect.Width() / 12) - 1, (WinRect.Height() / 12));
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() - (WinRect.Height() / 6) - (WinRect.Height() / 12) - (WinRect.Height() / 24) + 1, (WinRect.Width() / 24) - 1, (WinRect.Height() / 24));
				PathFinder.GetArea()->AddObstacleRect(0, WinRect.Height() - (WinRect.Height() / 6) - (WinRect.Height() / 12) - (WinRect.Height() / 24) - (WinRect.Height() / 48) + 1, (WinRect.Width() / 48) - 1, (WinRect.Height() / 48));

				break;
			case 4: //another special configuration

				PathFinder.GetArea()->AddObstacleRect(25, 25, 200, 200);
				PathFinder.GetArea()->AddObstacleRect(300, 200, 150, 100);
				PathFinder.GetArea()->AddObstacleRect(200, 150, 150, 150);
				PathFinder.GetArea()->AddObstacleRect(250, 350, 200, 100);

				break;
			case 5: //another special configuration

				PathFinder.GetArea()->AddObstacleRect(25, 25, 100, 200);
				PathFinder.GetArea()->AddObstacleRect(225, 425, 175, 50);
				PathFinder.GetArea()->AddObstacleRect(200, 25, 25, 25);
				PathFinder.GetArea()->AddObstacleRect(300, 50, 50, 50);

				break;
			default:

				break;
		}
	}
	else {
		DrawWindow->DoAddRect();
	}

	OnBnClickedDrawProbe();
}
void CCoverageFinderDlg::UpdateUserMessage(const wchar_t* UsrMsg) {
	MessageBuffers.push(m_StatBar.GetText(0));
	m_StatBar.SetText(UsrMsg, 0, 0);
}
void CCoverageFinderDlg::RestoreUserMessage() {
	CString Buf = MessageBuffers.top();
	m_StatBar.SetText(Buf, 0, 0);
	//Never delete the idle message
	if (MessageBuffers.size() > 1) MessageBuffers.pop();
}

void CCoverageFinderDlg::OnCbnSelChangeScanType()
{
	switch (m_ScanType.GetCurSel()) {
		case 0: //Recursive Optimal
			m_ScanDirection.EnableWindow(FALSE);
			m_ScanHeuristic.EnableWindow(FALSE);
			m_VertScanDirection.EnableWindow(FALSE);
			m_HorzScanDirection.EnableWindow(FALSE);
			break;
		case 1: //Recurive Semi-Optimal
			m_ScanDirection.EnableWindow(FALSE);
			m_ScanHeuristic.EnableWindow(TRUE);
			m_VertScanDirection.EnableWindow(FALSE);
			m_HorzScanDirection.EnableWindow(FALSE);
			break;
		case 2: //Heuristic Scan
			m_ScanDirection.EnableWindow(TRUE);
			m_ScanHeuristic.EnableWindow(TRUE);
			m_VertScanDirection.EnableWindow(FALSE);
			m_HorzScanDirection.EnableWindow(FALSE);
			break;
		case 3: //Simple Blind Scan
			m_ScanDirection.EnableWindow(TRUE);
			m_ScanHeuristic.EnableWindow(FALSE);
			m_VertScanDirection.EnableWindow(TRUE);
			m_HorzScanDirection.EnableWindow(TRUE);
			break;
		default:
			break;
	}
	OnCbnSelchangeScandirect();
}
void CCoverageFinderDlg::OnCbnSelchangeScandirect()
{
	if (m_ScanDirection.IsWindowEnabled() && m_ScanDirection.GetCurSel() != CB_ERR && m_VertScanDirection.IsWindowEnabled()) m_HorzScanDirection.EnableWindow(m_ScanDirection.GetCurSel() < 2);
}
void CCoverageFinderDlg::OnBnClickedApplyWindow()
{
	//WinWidth and WinHeight are the dimensions of the INTERNAL area, not the entire window
	if (!Initializing) {
		if (MessageBox(L"Changing the window size clears all previously stored data - do you want to continue?", "Continue?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
			return;
		}
	}

	//clear everything
	Scanner.GeneratedCoverages.clear();
	PathFinder.GetArea()->ClearData(true, true, true, true, true); //division rectangle definitions
	PathFinder.ClearData();
	OnBnClickedDrawProbe(); //redraw the nothingness

	//now do the resize
	CString WinWidth;
	m_WidthBox.GetWindowText(WinWidth); //retrieve value from screen

	CString WinHeight;
	m_HeightBox.GetWindowText(WinHeight); //retrieve value from screen

	//send the resize message to the draw window
	DrawWindow->DoResize(_wtol(WinWidth) + (GetSystemMetrics(SM_CXFIXEDFRAME) * 2), _wtol(WinHeight) + GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFIXEDFRAME) * 2));
	//resize the internal memory map
	PathFinder.GetArea()->SetAreaHeight(_wtol(WinHeight));
	PathFinder.GetArea()->SetAreaWidth(_wtol(WinWidth));

}

void CCoverageFinderDlg::OnBnClickedRestoreWindow()
{
	wchar_t StringBuffer[20];

	//Dimensions of the INTERNAL area, not the entire window

	_ltow(PathFinder.GetArea()->GetWidth() + 1, StringBuffer, 10);
	m_WidthBox.SetWindowText(StringBuffer);

	_ltow(PathFinder.GetArea()->GetHeight() + 1, StringBuffer, 10);
	m_HeightBox.SetWindowText(StringBuffer);
}

void CCoverageFinderDlg::OnTimer(UINT_PTR nIDEvent) {
	//drawing...
}

void CCoverageFinderDlg::OnFileLoad()
{
	FileDialogInfo.Flags = OFN_CREATEPROMPT | OFN_LONGNAMES | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;
	FileDialogInfo.lpstrFilter = L"Area Description (*.ard)\0*.ard\0All Files (*.*)\0*.*\0\0";
	FileDialogInfo.lpstrDefExt = L"ard";
	
	FileName[0] = 0;

	if (GetOpenFileName(&FileDialogInfo) != 0) {
	
		FileStream LoadStream(Load);

		if (LoadStream.OpenArchiveFile(FileDialogInfo.lpstrFile)) {

			UpdateUserMessage(L"Loading data from file...");

			LoadStream >> *GetArea();
			LoadStream >> PathFinder;

			LoadStream >> Scanner;
			Scanner.GeneratedCoverages.clear();

			SetSubRectValue((long)GetArea()->Rectangles.size());
			SetWhiteSpaceValue(GetArea()->GetWhiteArea());
			SetTotalAreaValue(GetArea()->GetTotalArea());

			LoadStream.CloseArchiveFile();

			RestoreUserMessage();

			OnBnClickedDrawProbe();
		}
		else {
			MessageBox(L"There was an error opening the file you selected.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
		}
	}
	else {

		char ErrorValue[100];

		ltoa(CommDlgExtendedError(), ErrorValue, 10);

		if (ErrorValue[0] != '0') {
			CString ErrorMessage;
			ErrorMessage += "The following error occured while trying to load the broswer:\n";
			ErrorMessage += ErrorValue;
			ErrorMessage += "\nPlease report this to the software's creator.";
		}
	}
}

void CCoverageFinderDlg::OnFileSave()
{
	FileDialogInfo.Flags = OFN_CREATEPROMPT | OFN_LONGNAMES | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;
	FileDialogInfo.lpstrFilter = L"Area Description (*.ard)\0*.ard\0All Files (*.*)\0*.*\0\0";
	FileDialogInfo.lpstrDefExt = L"ard";
	
	FileName[0] = 0;

	if (GetSaveFileName(&FileDialogInfo) != 0) {
		
		FileStream SaveStream(Save);

		if (SaveStream.OpenArchiveFile(FileDialogInfo.lpstrFile)) {

			UpdateUserMessage(L"Saving data to file...");

			SaveStream << *GetArea();
			SaveStream << PathFinder;
			SaveStream << Scanner;

			SaveStream.CloseArchiveFile();

			RestoreUserMessage();
		}
		else {
			MessageBox(L"There was an error opening the file you selected.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
		}
	}
	else {
		char ErrorValue[100];

		ltoa(CommDlgExtendedError(), ErrorValue, 10);

		if (ErrorValue[0] != '0') {
			CString ErrorMessage;
			ErrorMessage += "The following error occured while trying to load the broswer:\n";
			ErrorMessage += ErrorValue;
			ErrorMessage += "\nPlease report this to the software's creator.";
		}
	}
}

void CCoverageFinderDlg::OnFileExit() {
	OnBnClickedOk();
}
void CCoverageFinderDlg::SetSubRectValue(const unsigned long Value) {
	unsigned long Digits = NumberDigits(Value);
	wchar_t* CharBuffer = new wchar_t[Digits + 1];
	CharBuffer[Digits] = '\0';

	_ltow(Value, CharBuffer, 10);
	m_SubVal.SetWindowText(CharBuffer);

	delete [] CharBuffer;
}
void CCoverageFinderDlg::SetWhiteSpaceValue(const unsigned long Value) {
	unsigned long Digits = NumberDigits(Value);
	wchar_t* CharBuffer = new wchar_t[Digits + 1];
	CharBuffer[Digits] = '\0';

	_ltow(Value, CharBuffer, 10);
	m_WhiteVal.SetWindowText(CharBuffer); //Set the whitespace area

	delete [] CharBuffer;
}
void CCoverageFinderDlg::SetTotalAreaValue(const unsigned long Value) {
	unsigned long Digits = NumberDigits(Value);
	wchar_t* CharBuffer = new wchar_t[Digits + 1];
	CharBuffer[Digits] = '\0';

	_ltow(Value, CharBuffer, 10);
	m_TotalAreaVal.SetWindowText(CharBuffer);

	delete [] CharBuffer;
}
void CCoverageFinderDlg::SetCoverageLength(const unsigned long Value) {
	unsigned long Digits = NumberDigits(Value);
	char* CharBuffer = new char[Digits + 1];
	CharBuffer[Digits] = '\0';

	ltoa(Value, CharBuffer, 10);
	m_CoverVal.SetWindowText(CharBuffer);

	delete [] CharBuffer;
}
void CCoverageFinderDlg::SetConnectingPathLength(const unsigned long Value) {
	unsigned long Digits = NumberDigits(Value);
	char* CharBuffer = new char[Digits + 1];
	CharBuffer[Digits] = '\0';

	ltoa(Value, CharBuffer, 10);
	m_ConnectVal.SetWindowText(CharBuffer);

	delete CharBuffer;
}

void CCoverageFinderDlg::ToggleCheckState(int CheckStatus) {
	m_ObRectChk.SetCheck(CheckStatus); //Obstacle Rectangles
	m_VertRectsChk.SetCheck(CheckStatus); //Vertices
	m_PathFindLineChk.SetCheck(CheckStatus); //Pathfinding lines
	m_VertDivChk.SetCheck(CheckStatus); //Vertical Division Lines
	m_HorzDivChk.SetCheck(CheckStatus); //Horizontal Division Lines
	m_RectDivChk.SetCheck(CheckStatus); //Division Rectangle Definitions
	m_ScanChk.SetCheck(CheckStatus); //Last scan done
}

void CCoverageFinderDlg::OnBnClickedSall()
{
	ToggleCheckState(BST_CHECKED);
}
void CCoverageFinderDlg::OnBnClickedSnone()
{
	ToggleCheckState(BST_UNCHECKED);
}
void CCoverageFinderDlg::OnBnClickedSavepathbutton()
{
	FileStream SaveStream(Save);

	CString TextBuffer;
	m_PathFileBox.GetWindowText(TextBuffer);

	UpdateUserMessage(L"Saving data to file...");

	PathFinder.SavePathVectors(TextBuffer);

	RestoreUserMessage();
}

void CCoverageFinderDlg::OnBnClickedBrowsebutton()
{
	FileDialogInfo.Flags = OFN_CREATEPROMPT | OFN_LONGNAMES | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY;
	FileDialogInfo.lpstrFilter = L"Area Description (*.ard)\0*.ard\0All Files (*.*)\0*.*\0\0";
	FileDialogInfo.lpstrDefExt = L"ard";
	
	FileName[0] = 0;

	if (GetSaveFileName(&FileDialogInfo) != 0) {
		m_PathFileBox.SetWindowText(FileDialogInfo.lpstrFile);
	}
}
