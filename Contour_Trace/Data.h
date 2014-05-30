#if !defined(AFX_DATA_H__50EAB12C_2A31_4C4D_AA68_323B634009C6__INCLUDED_)
#define AFX_DATA_H__50EAB12C_2A31_4C4D_AA68_323B634009C6__INCLUDED_

#include "myBMPDoc.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Data.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CData dialog

class CData : public CDialogBar
{
// Construction
public:
	int m_nCurPos;
	BOOL OnUpdateEdits();
	BOOL SetCurvePos(int nCurPos);
	int m_nSelectCurve;
	int m_nSelectItem;
	BOOL SetCurveList(CMyBMPDoc  *pDoc,int nlistLength);
	BOOL OnInitOtherCtrl();
	int m_nListLength;
	LPDWORD m_lpArea;
	int m_nHeight;
	int m_nLength;
	SList *m_lstHdr;
	SList *m_lstCurPos;
	BOOL ShowAllCurves();
	BOOL ShowItem();
	BOOL OnInitListCtrl();
	BOOL OnInitListCurve();
	BOOL OnPreInitDialogBar();
	CData(CWnd* pParent = NULL);   // standard constructor
	~CData();

// Dialog Data
	//{{AFX_DATA(CData)
	enum { IDD = IDD_DIALOG_DATA };
	CListCtrl	m_listPos;  //该变量与Data List右边的List控件相关联
	CListCtrl	m_listCurve;//该变量与Data List左边的List控件相关联
	CEdit	m_edtCurCurveLen;
	CEdit	m_edtCurCurve;
	CEdit	m_edtTotalCurves;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CData)
	afx_msg void OnClickListCurve(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATA_H__50EAB12C_2A31_4C4D_AA68_323B634009C6__INCLUDED_)
