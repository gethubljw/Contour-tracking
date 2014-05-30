#if !defined(AFX_HISTOGRAM_H__F719EBC0_600E_4C9E_9431_ED6D9AB04732__INCLUDED_)
#define AFX_HISTOGRAM_H__F719EBC0_600E_4C9E_9431_ED6D9AB04732__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Histogram.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistogram dialog

class CHistogram : public CDialogBar
{
// Construction
public:
	BOOL SetAllChecked();
	CHistogram(CWnd* pParent = NULL);   // standard constructor
	~CHistogram();
	virtual BOOL Create(CWnd *pParentWnd,UINT nIDTemplate,UINT nStyle,UINT nID);
	virtual BOOL Create(CWnd *pParentWnd,LPCSTR lpszTemplateName,UINT nStyle,UINT nID);
	BOOL GetMax(int&);


// Dialog Data
	//{{AFX_DATA(CHistogram)
	enum { IDD = IDD_HISTOGRAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistogram)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistogram)
	afx_msg void OnPaint();
	afx_msg void OnCheckRed();
	afx_msg void OnCheckGreen();
	afx_msg void OnCheckBlue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bJudge;
	int m_nLength;
	int *m_pnBlue;
	int *m_pnGreen;
	int *m_pnRed;
	BOOL m_bBlue;
	BOOL m_bGreen;
	BOOL m_bRed;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAM_H__F719EBC0_600E_4C9E_9431_ED6D9AB04732__INCLUDED_)
