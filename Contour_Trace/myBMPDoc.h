// myBMPDoc.h : interface of the CMyBMPDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBMPDOC_H__02E2A46C_1731_4905_BDF9_1971C3D073E1__INCLUDED_)
#define AFX_MYBMPDOC_H__02E2A46C_1731_4905_BDF9_1971C3D073E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DECLARE_HANDLE(HDIB);//声明HDIB为句柄

typedef struct MyPoint
{
	int xPos;
	int yPos;
	MyPoint *pNext;
}SPoint;

typedef struct MyList
{
	SPoint *ptrSPoint;
	int Length;
	MyList *pNext;
}SList;

class CMyBMPDoc : public CDocument
{
protected: // create from serialization only
	CMyBMPDoc();
	DECLARE_DYNCREATE(CMyBMPDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBMPDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBMPDoc();
	void UpdateHistogram();
//	BOOL GetHistogramValue(int *RedArray,int *GreenArray,int *BlueArray,int Length);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	BOOL m_bFlag;
	BOOL m_bThreshold;
	BOOL ShowDataList(BOOL bTrace);
	BOOL m_bTrace;
	int GetListLength();
	BOOL GetAllCurvesArea();
	LPDWORD m_lpArea;
	SList *m_pCurveListHdr;
	BOOL PtInRegion(int x,int y,SPoint *ptrHeader,int nLen);
	BOOL GetHistoGramValue(int *RedArray,int *GreenArray,int *BlueArray,int Length);
	HDIB m_hDib;
    int m_nWidth;
	int m_nHeight;
	CSize size;
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyBMPDoc)
	afx_msg void OnTrace();
	afx_msg void OnThreshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:	
	CRgn m_cRgn;//该变量用于PtInRegion当中
	BOOL m_bOK;//该变量用于PtInRegion当中
	int m_nScanWidth;
	int m_nBitCount;
	
	DWORD m_dwFileLength;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBMPDOC_H__02E2A46C_1731_4905_BDF9_1971C3D073E1__INCLUDED_)
