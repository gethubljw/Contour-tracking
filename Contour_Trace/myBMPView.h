// myBMPView.h : interface of the CMyBMPView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBMPVIEW_H__BBA09528_0305_4B81_85D4_1DB671DFA8BA__INCLUDED_)
#define AFX_MYBMPVIEW_H__BBA09528_0305_4B81_85D4_1DB671DFA8BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "myBMPDoc.h"
class CMyBMPView : public CScrollView
{
protected: // create from serialization only
	CMyBMPView();
	DECLARE_DYNCREATE(CMyBMPView)

// Attributes
public:
	CMyBMPDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBMPView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_nZoomout;
	UINT m_nZoomIn;
	BOOL m_bZoomOut;
	BOOL m_bZoomIn;
	BOOL DrawCurrentCurve(int nHeight);
	virtual ~CMyBMPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyBMPView)
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in myBMPView.cpp
inline CMyBMPDoc* CMyBMPView::GetDocument()
   { return (CMyBMPDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBMPVIEW_H__BBA09528_0305_4B81_85D4_1DB671DFA8BA__INCLUDED_)
