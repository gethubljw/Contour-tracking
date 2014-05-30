// ColorPalette.cpp : implementation file
//

#include "stdafx.h"
#include "myBMP.h"
#include "ColorPalette.h"

#include "MainFrm.h"
//#include "myBMPDoc.h"
#include  "myBMPView.h"

#include  "afxpriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPalette dialog


CColorPalette::CColorPalette(CWnd* pParent /*=NULL*/)
//	: CDialog(CColorPalette::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorPalette)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_ForeColor=RGB(0,0,0);
	m_BackColor=RGB(0,255,0);//设置初始颜色
}

CColorPalette::~CColorPalette()
{
	//Now there is nothing to do here
}


void CColorPalette::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorPalette)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorPalette, CDialogBar)
	//{{AFX_MSG_MAP(CColorPalette)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPalette message handlers

void CColorPalette::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CDialogBar::OnLButtonDown(nFlags, point);

	CWnd *pFore=GetDlgItem(IDC_FOREWARD_COLOR);
//	ASSERT(!pFore);
	if(!pFore)
		return;
	CWnd *pBack=GetDlgItem(IDC_BACKWARD_COLOR);
//	ASSERT(!pBack);
	if(!pBack)
		return;

	CRect rFore,rBack;

	pFore->GetClientRect(&rFore);
	pBack->GetClientRect(&rBack);

	pFore->ClientToScreen(&rFore);
	pBack->ClientToScreen(&rBack);

	ScreenToClient(&rFore);
	ScreenToClient(&rBack);

	CColorDialog clrDlg;
	clrDlg.m_cc.Flags|=CC_ANYCOLOR;
	clrDlg.m_cc.Flags|=CC_RGBINIT;

	if(rFore.PtInRect(point))
	{
		clrDlg.m_cc.rgbResult=m_ForeColor;
		if(clrDlg.DoModal()==IDOK)
		{
			m_ForeColor=clrDlg.GetColor();
			Invalidate();
		}
	}
	if(rBack.PtInRect(point))
	{
		clrDlg.m_cc.rgbResult=m_BackColor;
		if(clrDlg.DoModal()==IDOK)
		{
			m_BackColor=clrDlg.GetColor();
			Invalidate();
		}
	}
}

void CColorPalette::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialogBar::OnPaint() for painting messages

	CWnd *pFore=GetDlgItem(IDC_FOREWARD_COLOR);
	ASSERT(pFore);
	CWnd *pBack=GetDlgItem(IDC_BACKWARD_COLOR);
	ASSERT(pBack);

	CRect rFore,rBack;
	pFore->GetClientRect(&rFore);
	pBack->GetClientRect(&rBack);

	pFore->ClientToScreen(&rFore);
	pBack->ClientToScreen(&rBack);

	ScreenToClient(&rFore);
	ScreenToClient(&rBack);

	CBrush bFore,bBack;
	bFore.CreateSolidBrush(m_ForeColor);
	bBack.CreateSolidBrush(m_BackColor);

	dc.FillRect(&rFore,&bFore);
	dc.FillRect(&rBack,&bBack);
}


bool CColorPalette::OnIdleUpdate() 
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(!pMainFrame)
		return false;
	CMyBMPView *pView=(CMyBMPView*)(pMainFrame->GetActiveFrame()->GetActiveView());
	if(!pView)
		return false;
	CWnd *pRed=GetDlgItem(IDC_RED);
	if(!pRed)
		return false;
	CWnd *pGreen=GetDlgItem(IDC_GREEN);
	if(!pGreen)
		return false;
	CWnd *pBlue=GetDlgItem(IDC_BLUE);
	if(!pBlue)
		return false;
	CWnd *pIntensity=GetDlgItem(IDC_INTENSITY);
	if(!pIntensity)
		return false;
	CWnd *pXpos=GetDlgItem(IDC_XPOS_SHOW);
	if(!pXpos)
		return false;
	CWnd *pYpos=GetDlgItem(IDC_YPOS_SHOW);
	if(!pYpos)
		return false;
	CWnd *pColor=GetDlgItem(IDC_CURRENT_COLOR);
	if(!pColor)
		return false;
	CRect rect;
	pColor->GetClientRect(&rect);
	pColor->ClientToScreen(&rect);
	ScreenToClient(&rect);
//	rect.InflateRect(3,4,2,1);


	POINT curPoint;
	if(!::GetCursorPos(&curPoint))
		return false;
	pView->ScreenToClient(&curPoint);

	CSize size=pView->GetTotalSize();
	CRect rectImage(0,0,size.cx,size.cy);
	CClientDC dcDlg(this);
	CBrush brush;

	if(!rectImage.PtInRect(curPoint))
	{
		brush.CreateHatchBrush(HS_DIAGCROSS,RGB(0,0,0));//显示网格
		dcDlg.FillRect(&rect,&brush);
		return false;
	}

	CString strInit(_T("..."));
	CString strRed,strGreen,strBlue,strIntensity,strX,strY;

	strRed=strInit;
	strGreen=strInit;
	strBlue=strInit;
	strIntensity=strInit;
	strX=strInit;
	strY=strInit;

	COLORREF curColor;
	CClientDC dc(pView);
	curColor=dc.GetPixel(curPoint);


	brush.CreateSolidBrush(curColor);
	dcDlg.FillRect(&rect,&brush);//以鼠标所在点的颜色填充控件

	strRed.Format(_T("%d"),GetRValue(curColor));
	strGreen.Format(_T("%d"),GetGValue(curColor));
	strBlue.Format(_T("%d"),GetBValue(curColor));
	strIntensity.Format(_T("%d"),(GetRValue(curColor)+GetGValue(curColor)+GetBValue(curColor))/3);
	strX.Format(_T("%d"),curPoint.x);
	strY.Format(_T("%d"),curPoint.y);

	pRed->SetWindowText(strRed);
	pGreen->SetWindowText(strGreen);
	pBlue->SetWindowText(strBlue);

	pIntensity->SetWindowText(strIntensity);
	pXpos->SetWindowText(strX);
	pYpos->SetWindowText(strY);

	return true;
	
}
