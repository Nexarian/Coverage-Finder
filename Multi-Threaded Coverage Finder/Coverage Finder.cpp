// Coverage Finder.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Coverage Finder.h"
#include "Coverage FinderDlg.h"
#include "DisplayDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCoverageFinderApp

BEGIN_MESSAGE_MAP(CCoverageFinderApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// CCoverageFinderApp construction

CCoverageFinderApp::CCoverageFinderApp()
{
}

// The one and only CCoverageFinderApp object

CCoverageFinderApp theApp;

// CCoverageFinderApp initialization

BOOL CCoverageFinderApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	/*Factory<PersistentObject, MultiArray<MappedPoint> >* f1 = new Factory<PersistentObject, MultiArray<MappedPoint> >;
	Factory<PersistentObject, MappedPoint >* f2 = new Factory<PersistentObject, MappedPoint >;
	Factory<PersistentObject, MappedLine >* f3 = new Factory<PersistentObject, MappedLine >;
	Factory<PersistentObject, MappedRect >* f4 = new Factory<PersistentObject, MappedRect >;
	Factory<PersistentObject, Area >* f5 = new Factory<PersistentObject, Area >;
	Factory<PersistentObject, Path >* f6 = new Factory<PersistentObject, Path >;
	Factory<PersistentObject, AreaCover >* f7 = new Factory<PersistentObject, AreaCover >;


	FactoryBase<PersistentObject>* ff1 = f1;
	FactoryBase<PersistentObject>* ff2 = f2;
	FactoryBase<PersistentObject>* ff3 = f3;
	FactoryBase<PersistentObject>* ff4 = f4;
	FactoryBase<PersistentObject>* ff5 = f5;
	FactoryBase<PersistentObject>* ff6 = f6;
	FactoryBase<PersistentObject>* ff7 = f7;


	/*FileStream::Classes().AddFactory(f1);
	FileStream::Classes().AddFactory(f2);
	FileStream::Classes().AddFactory(f3);
	FileStream::Classes().AddFactory(f4);
	FileStream::Classes().AddFactory(f5);
	FileStream::Classes().AddFactory(f6);
	FileStream::Classes().AddFactory(f7);*/

	/*delete ff1;
	delete ff2;
	delete ff3;
	delete ff4;
	delete ff5;
	delete ff6;
	delete ff7;*/

	FileStream::Classes().AddFactory(new Factory<PersistentObject, MultiArray<MappedPoint> >);
	FileStream::Classes().AddFactory(new Factory<PersistentObject, MappedPoint>);
	FileStream::Classes().AddFactory(new Factory<PersistentObject, MappedLine>);
	FileStream::Classes().AddFactory(new Factory<PersistentObject, MappedRect>);
	FileStream::Classes().AddFactory(new Factory<PersistentObject, Path>);
	FileStream::Classes().AddFactory(new Factory<PersistentObject, Area>);
	FileStream::Classes().AddFactory(new Factory<PersistentObject, AreaCover>);



	DisplayDLG dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
