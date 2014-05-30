// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "myBMP.h"
#include "myBMPDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_COMMAND_EX(IDM_HISTOGRAM,OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDM_HISTOGRAM,OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDM_COLOR_PALETTE,OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDM_COLOR_PALETTE,OnUpdateControlBarMenu)
	ON_COMMAND_EX(IDM_DATA_LIST,OnBarCheck)
	ON_UPDATE_COMMAND_UI(IDM_DATA_LIST,OnUpdateControlBarMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CreateHistogram();
	CreateColorPalette();
	CreateDataList();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::CreateHistogram()
{
	m_wndHistogram.Create(this,IDD_HISTOGRAM,CBRS_TOP,IDM_HISTOGRAM);
	CMyBMPDoc *pDoc=(CMyBMPDoc*)GetActiveDocument();
	CString strText;
	strText.Format("Histogram");

	m_wndHistogram.SetWindowText(strText);//设置直方图控件标题为"Histogram"
	m_wndHistogram.EnableDocking(0);//该参数为0表示该控件不可停驻

	CRect Fram;
	GetWindowRect(&Fram);
	int x=Fram.right-350;
	int y=Fram.top+60;
	FloatControlBar(&m_wndHistogram,CPoint(x,y),CBRS_ALIGN_TOP);

	return true;
}

BOOL CMainFrame::CreateColorPalette()
{
	m_wndColorPalette.Create(this,IDD_COLOR_PALETTE,CBRS_TOP,IDM_COLOR_PALETTE);
	CMyBMPDoc *pDoc=(CMyBMPDoc*)GetActiveDocument();
	CString strText;
	strText.Format("ColorPalette");

	m_wndColorPalette.SetWindowText(strText);
	m_wndColorPalette.EnableDocking(0);

	CRect Fram;
	GetWindowRect(&Fram);
	int x=Fram.right-100;
	int y=Fram.top+60;
	FloatControlBar(&m_wndColorPalette,CPoint(x,y),CBRS_ALIGN_TOP);

	return true;

}

BOOL CMainFrame::CreateDataList()
{
	m_wndDataList.Create(this,IDD_DIALOG_DATA,CBRS_BOTTOM,IDM_DATA_LIST);	
	m_wndDataList.SetWindowText(_T("Data List"));
	m_wndDataList.EnableDocking(0);
	m_wndDataList.ShowWindow(SW_HIDE);//刚开始不显示该窗口

	CRect Fram;
	GetWindowRect(&Fram);
	int x=Fram.right-200;
	int y=Fram.top+300;
	FloatControlBar(&m_wndDataList,CPoint(x,y),CBRS_ALIGN_TOP);

	return true;
}
