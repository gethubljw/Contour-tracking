#if !defined(AFX_COLORPALETTE_H__5CFB7A00_0F3F_4978_981D_971743E31974__INCLUDED_)
#define AFX_COLORPALETTE_H__5CFB7A00_0F3F_4978_981D_971743E31974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorPalette.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorPalette dialog

class CColorPalette : public CDialogBar
{
// Construction
public:
	CColorPalette(CWnd* pParent = NULL);   // standard constructor
	~CColorPalette();

// Dialog Data
	//{{AFX_DATA(CColorPalette)
	enum { IDD = IDD_COLOR_PALETTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPalette)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorPalette)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg bool OnIdleUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_ForeColor;
	COLORREF m_BackColor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPALETTE_H__5CFB7A00_0F3F_4978_981D_971743E31974__INCLUDED_)
