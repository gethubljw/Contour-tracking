// myBMPView.cpp : implementation of the CMyBMPView class
//

#include "stdafx.h"
#include "myBMP.h"

#include "myBMPDoc.h"
#include "myBMPView.h"
#include  "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBMPView

IMPLEMENT_DYNCREATE(CMyBMPView, CScrollView)

BEGIN_MESSAGE_MAP(CMyBMPView, CScrollView)
	//{{AFX_MSG_MAP(CMyBMPView)
	ON_COMMAND(IDM_ZOOM_IN, OnZoomIn)
	ON_COMMAND(IDM_ZOOM_OUT, OnZoomOut)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBMPView construction/destruction

CMyBMPView::CMyBMPView()
{
	// TODO: add construction code here
	m_bZoomIn=FALSE;
	m_bZoomOut=FALSE;
	m_nZoomIn=1;
	m_nZoomout=1;

}

CMyBMPView::~CMyBMPView()
{
}

BOOL CMyBMPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyBMPView drawing

void CMyBMPView::OnDraw(CDC* pDC)
{
	CMyBMPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(!pDoc->m_hDib)
		return;
	CSize size=GetTotalSize();
	CRect rectDes(0,0,size.cx,size.cy);
	CRect rectScr(0,0,pDoc->m_nWidth,pDoc->m_nHeight);

	LPSTR pDib=(LPSTR)::GlobalLock((HGLOBAL)pDoc->m_hDib);
	LPBITMAPINFOHEADER lpmi=(LPBITMAPINFOHEADER)pDib;
	if(!pDib)
	{
		AfxMessageBox("Error: Fail to Gain the Pointer to the First Byte of Memory",0,0);
		::GlobalUnlock((HGLOBAL)pDoc->m_hDib);
		ASSERT(!::GlobalFree((HGLOBAL)pDoc->m_hDib));
		return;
	}

	LPSTR lpszDib=pDib+lpmi->biSize;

	if(rectDes.Width()==rectScr.Width()&&rectDes.Height()==rectScr.Height())
	{
		::SetDIBitsToDevice(pDC->m_hDC,rectDes.left,rectDes.top,rectDes.right-rectDes.left,rectDes.bottom-rectDes.top,
			rectScr.left,(int)((LPBITMAPINFOHEADER)pDib)->biHeight-rectScr.bottom,0,(WORD)((LPBITMAPINFOHEADER)pDib)->biHeight,
			lpszDib,(LPBITMAPINFO)pDib,DIB_RGB_COLORS);
	}
	else
	{
		::StretchDIBits(pDC->m_hDC,rectDes.left,rectDes.top,rectDes.right-rectDes.left,rectDes.bottom-rectDes.top,
			rectScr.left,rectScr.top,rectScr.right-rectScr.left,rectScr.bottom-rectScr.top,lpszDib,
			(LPBITMAPINFO)pDib,DIB_RGB_COLORS,SRCCOPY);
	}

	::GlobalUnlock((HGLOBAL)pDoc->m_hDib);

}

/////////////////////////////////////////////////////////////////////////////
// CMyBMPView printing

BOOL CMyBMPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyBMPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyBMPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyBMPView diagnostics

#ifdef _DEBUG
void CMyBMPView::AssertValid() const
{
	CView::AssertValid();
}

void CMyBMPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyBMPDoc* CMyBMPView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyBMPDoc)));
	return (CMyBMPDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyBMPView message handlers

void CMyBMPView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize size;
	CMyBMPDoc *pDoc=GetDocument();

	if(!pDoc->m_hDib)
	{
		size.cx=size.cy=100;
	}
	else
	{
		size.cx=pDoc->m_nWidth;
		size.cy=pDoc->m_nHeight;
	}

	SetScrollSizes(MM_TEXT,size);
	ResizeParentToFit(false);

}

void CMyBMPView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();

	if(bActivate)
	{
		pMainFrame->m_wndHistogram.Invalidate();
		ASSERT(pActivateView==this);
	}
}

BOOL CMyBMPView::DrawCurrentCurve(int nHeight)
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrame);

	SList *pCurList=pMainFrame->m_wndDataList.m_lstCurPos;
	if(!pCurList)
		return FALSE;

	CRect rect;
	CClientDC dc(this);
	OnDraw(&dc);//在调用DrawCurrentCurve函数时先对View进行重绘，然后再进行绘制，这样当单击Data List左边的某个线条时
                //会只绘制选中的那个线条的轮廓
	if(m_bZoomIn)
	{
		CPen rPen(PS_SOLID,m_nZoomIn,RGB(255,0,0));
		CPen *pOldPen=dc.SelectObject(&rPen);
		ASSERT(pOldPen);

		dc.MoveTo((pCurList->ptrSPoint->xPos)*m_nZoomIn,(nHeight-1-pCurList->ptrSPoint->yPos)*m_nZoomIn);
		for(SPoint *pPoint=pCurList->ptrSPoint;pPoint;pPoint=pPoint->pNext)
			dc.LineTo((pPoint->xPos)*m_nZoomIn,(nHeight-1-pPoint->yPos)*m_nZoomIn);

	}
	else
		if(m_bZoomOut)
		{
			CPen rPen(PS_SOLID,m_nZoomout,RGB(255,0,0));
			CPen *pOldPen=dc.SelectObject(&rPen);
			ASSERT(pOldPen);

			dc.MoveTo((pCurList->ptrSPoint->xPos)*m_nZoomout,(nHeight-1-pCurList->ptrSPoint->yPos)*m_nZoomout);
			for(SPoint *pPoint=pCurList->ptrSPoint;pPoint;pPoint=pPoint->pNext)
				dc.LineTo((pPoint->xPos)*m_nZoomout,(nHeight-1-pPoint->yPos)*m_nZoomout);

		}
		else
		{
			CPen rPen(PS_SOLID,3,RGB(255,0,0));
			CPen *pOldPen=dc.SelectObject(&rPen);
			ASSERT(pOldPen);

			dc.MoveTo(pCurList->ptrSPoint->xPos,nHeight-1-pCurList->ptrSPoint->yPos);
			for(SPoint *pPoint=pCurList->ptrSPoint;pPoint;pPoint=pPoint->pNext)
				dc.LineTo(pPoint->xPos,nHeight-1-pPoint->yPos);
		}
	return TRUE;

}

void CMyBMPView::OnZoomIn() 
{
	// TODO: Add your command handler code here
	CMyBMPDoc *pDoc=GetDocument();
	if(m_bZoomOut)
		m_bZoomOut=FALSE;
	m_bZoomIn=TRUE;

	pDoc->size.cx=(pDoc->size.cx)*2;
	pDoc->size.cy=(pDoc->size.cy)*2;
	m_nZoomIn=m_nZoomIn*2;

	this->SetScrollSizes(MM_TEXT,pDoc->size);
	this->Invalidate();

	
}

void CMyBMPView::OnZoomOut() 
{
	// TODO: Add your command handler code here
	CMyBMPDoc *pDoc=GetDocument();
	if(m_bZoomIn)
		m_bZoomIn=FALSE;
	m_bZoomOut=TRUE;

	pDoc->size.cx=(pDoc->size.cx)/2;
	pDoc->size.cy=(pDoc->size.cy)/2;
	m_nZoomout=m_nZoomout/2;

	this->SetScrollSizes(MM_TEXT,pDoc->size);
	this->Invalidate();
	
}
