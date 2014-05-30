// Histogram.cpp : implementation file
//

#include "stdafx.h"
#include "myBMP.h"
#include "Histogram.h"


#include "MainFrm.h"
//#include "myBMPDoc.h"
#include "myBMPView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogram dialog


CHistogram::CHistogram(CWnd* pParent /*=NULL*/)
//	: CDialog(CHistogram::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogram)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bRed=m_bGreen=m_bBlue=true;
	m_nLength=256;
	m_pnRed=new int[m_nLength];
	m_pnGreen=new int[m_nLength];
	m_pnBlue=new int[m_nLength];
}

CHistogram::~CHistogram()
{
	if(!m_pnRed)
		delete [] m_pnRed;
	if(!m_pnGreen)
		delete [] m_pnGreen;
	if(!m_pnBlue)
		delete [] m_pnBlue;
}

BOOL CHistogram::Create(CWnd *pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID)
{
	BOOL bSuccess=CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID);
	BOOL bOK=SetAllChecked();
	return (bSuccess&&bOK);
}

BOOL CHistogram::Create(CWnd *pParentWnd,LPCSTR lpszTemplateName,UINT nStyle,UINT nID)
{
	BOOL bSuccess=CDialogBar::Create(pParentWnd,lpszTemplateName,nStyle,nID);
	BOOL bOK=SetAllChecked();
	return (bSuccess&&bOK);
}


void CHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogram)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CHistogram::GetMax(int& nTotalPixel)
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	CMyBMPView *pView=(CMyBMPView*)pMainFrame->GetActiveFrame()->GetActiveView();
	CMyBMPDoc *pDoc=(CMyBMPDoc*)pView->GetDocument();

	BOOL bSuccess=false;
	if(m_bRed||m_bGreen||m_bBlue)
	{
		bSuccess=pDoc->GetHistoGramValue(m_pnRed,m_pnGreen,m_pnBlue,m_nLength);

	}
	if(!bSuccess)
		return false;

	int nRMax=m_pnRed[0];
	int nGMax=m_pnGreen[0];
	int nBMax=m_pnBlue[0];
	int nflag=0;

	if(m_bRed)
	{
		for(int i=1;i<m_nLength;i++)
		{
			if(nRMax<m_pnRed[i])
				nRMax=m_pnRed[i];
		}
		nflag=nflag+1;
	}

	if(m_bGreen)
	{
		for(int i=1;i<m_nLength;i++)
		{
			if(nGMax<m_pnGreen[i])
				nGMax=m_pnGreen[i];
		}
		nflag=nflag+2;
	}

	if(m_bBlue)
	{
		for(int i=1;i<m_nLength;i++)
		{
			if(nBMax<m_pnBlue[i])
				nBMax=m_pnBlue[i];
		}
		nflag=nflag+4;
	}

	switch(nflag)
	{
	case 1:
		nTotalPixel=nRMax;
		break;
	case 2:
		nTotalPixel=nGMax;
		break;
	case 3:
		nTotalPixel=(nRMax>nGMax?nRMax:nGMax);
		break;
	case 4:
		nTotalPixel=nBMax;
		break;
	case 5:
		nTotalPixel=nRMax>nBMax?nRMax:nBMax;
		break;
	case 6:
		nTotalPixel=nGMax>nBMax?nGMax:nBMax;
		break;
	case 7:
		nTotalPixel=nGMax>nBMax?nGMax:nBMax;
		nTotalPixel=(nTotalPixel>nRMax?nTotalPixel:nRMax);
		break;
	default:
		return false;

	}

	return true;
}


BEGIN_MESSAGE_MAP(CHistogram, CDialogBar)
	//{{AFX_MSG_MAP(CHistogram)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_RED, OnCheckRed)
	ON_BN_CLICKED(IDC_CHECK_GREEN, OnCheckGreen)
	ON_BN_CLICKED(IDC_CHECK_BLUE, OnCheckBlue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogram message handlers

void CHistogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialogBar::OnPaint() for painting messages    why?
	

	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	CMyBMPView *pView=(CMyBMPView*)pMainFrame->GetActiveFrame()->GetActiveView();

	if(pView)
	{
		CRect rect;
		GetClientRect(&rect);

		CPen newPen(PS_SOLID,1,RGB(0,0,0));
		CPen *pOldPen=dc.SelectObject(&newPen);
		ASSERT(pOldPen);

		int Height=rect.Height();
		int Width=rect.right;
		dc.MoveTo(15,30);
		dc.LineTo(15,Height-40);
		dc.LineTo(Width-20,Height-40);//画出坐标轴

		int nTotalPixel;
		BOOL bSuccess=GetMax(nTotalPixel);
		if(!bSuccess)
			return;
		CString strText;
		strText.Format("%d",nTotalPixel);
		dc.TextOut(9,10,strText);//输出数字

		int MaxHeight=Height-70;
		int y=MaxHeight;

		if(m_bRed)
		{
			CPen RPen(PS_SOLID,1,RGB(255,0,0));
			pOldPen=dc.SelectObject(&RPen);
			ASSERT(pOldPen);

			y=y-m_pnRed[0]*MaxHeight/nTotalPixel;
			dc.MoveTo(16,y);
			for(int i=1;i<m_nLength;i++)
			{
				y=MaxHeight;
				y=y-m_pnRed[i]*MaxHeight/nTotalPixel;
				dc.LineTo(16+i,y);
			}
		}

		if(m_bGreen)
		{
			CPen GPen(PS_SOLID,1,RGB(0,255,0));
			pOldPen=dc.SelectObject(&GPen);
			ASSERT(pOldPen);

			y=MaxHeight;
			y=y-m_pnGreen[0]*MaxHeight/nTotalPixel;
			dc.MoveTo(16,y);

			for(int i=1;i<m_nLength;i++)
			{
				y=MaxHeight;
				y=y-m_pnGreen[i]*MaxHeight/nTotalPixel;
				dc.LineTo(16+i,y);
			}
		}

		if(m_bBlue)
		{
			CPen BPen(PS_SOLID,1,RGB(0,0,255));
			pOldPen=dc.SelectObject(&BPen);
			ASSERT(pOldPen);

			y=MaxHeight;
			y=y-m_pnBlue[0]*MaxHeight/nTotalPixel;
			dc.MoveTo(16,y);

			for(int i=1;i<m_nLength;i++)
			{
				y=MaxHeight;
				y=y-m_pnBlue[i]*MaxHeight/nTotalPixel;
				dc.LineTo(16+i,y);
			}
		}

	}
	
}

void CHistogram::OnCheckRed() 
{
	// TODO: Add your control notification handler code here
	CButton *pRed=(CButton*)CWnd::GetDlgItem(IDC_CHECK_RED);
	ASSERT(pRed);

	if(pRed->GetCheck()==1)
		m_bRed=true;
	else
		m_bRed=false;

	Invalidate();
	
}

void CHistogram::OnCheckGreen() 
{
	// TODO: Add your control notification handler code here
	CButton *pGreen=(CButton*)CWnd::GetDlgItem(IDC_CHECK_GREEN);
	ASSERT(pGreen);

	if(pGreen->GetCheck()==1)
		m_bGreen=true;
	else
		m_bGreen=false;

	Invalidate();
	
}

void CHistogram::OnCheckBlue() 
{
	// TODO: Add your control notification handler code here
	CButton *pBlue=(CButton*) CWnd::GetDlgItem(IDC_CHECK_BLUE);
	ASSERT(pBlue);

	if(pBlue->GetCheck()==1)
		m_bBlue=true;
	else
		m_bBlue=false;

	Invalidate();
	
}

BOOL CHistogram::SetAllChecked()
{
	CButton *pRed=(CButton*)CWnd::GetDlgItem(IDC_CHECK_RED);
	ASSERT(pRed);
	pRed->SetCheck(1);

	CButton *pGreen=(CButton*)CWnd::GetDlgItem(IDC_CHECK_GREEN);
	ASSERT(pGreen);
	pGreen->SetCheck(1);

	CButton *pBlue=(CButton*)CWnd::GetDlgItem(IDC_CHECK_BLUE);
	pBlue->SetCheck(1);

	return true;

}
