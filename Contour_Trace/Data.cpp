// Data.cpp : implementation file
//

#include "stdafx.h"
#include "myBMP.h"
#include "Data.h"

#include "MainFrm.h"
#include "myBMPView.h"
//#include "ChildFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CData dialog


CData::CData(CWnd* pParent /*=NULL*/)
//	: CDialog(CData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSelectCurve=-1;
	m_nSelectItem=-1;	
	m_lpArea=NULL;
	m_lstCurPos=NULL;
	m_lstHdr=NULL;
	m_nHeight=0;
	m_nCurPos=0;
}

CData::~CData()
{
	if(m_lpArea)
		delete [] m_lpArea;
}


void CData::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CData)
	DDX_Control(pDX, IDC_LIST_POS, m_listPos);
	DDX_Control(pDX, IDC_LIST_CURVE, m_listCurve);
	DDX_Control(pDX, IDC_EDIT_CURRENT_CURVE_LENGTH, m_edtCurCurveLen);
	DDX_Control(pDX, IDC_EDIT_CURRENT_CURVE, m_edtCurCurve);
	DDX_Control(pDX, IDC_TOTAL_CURVES, m_edtTotalCurves);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CData, CDialogBar)
	//{{AFX_MSG_MAP(CData)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CURVE, OnClickListCurve)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PREINITDIALOGBAR,OnPreInitDialogBar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CData message handlers

BOOL CData::OnPreInitDialogBar()
{
	UpdateData(FALSE);
	OnInitListCtrl();
	OnInitListCurve();
	return (ShowItem()&&ShowAllCurves());

}

BOOL CData::OnInitListCurve()//为左边的List控件插入column
{
	LPSTR lpText;
	int i=0;

	lpText="Curve Index";
	m_listCurve.InsertColumn(i,lpText,LVCFMT_CENTER,m_listCurve.GetStringWidth(lpText)+20,i++);

	lpText="Curve Area(Pixels)";
	m_listCurve.InsertColumn(i,lpText,LVCFMT_CENTER,m_listCurve.GetStringWidth(lpText)+25,i++);

	m_listCurve.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return TRUE;

}

BOOL CData::OnInitListCtrl()//为右边的List控件插入column
{
	LPSTR lpText;
	int i=0;

	lpText="Index";
	m_listPos.InsertColumn(i,lpText,LVCFMT_CENTER,m_listPos.GetStringWidth(lpText)+20,i++);

	lpText="Value of X";
	m_listPos.InsertColumn(i,lpText,LVCFMT_CENTER,m_listPos.GetStringWidth(lpText)+25,i++);

	lpText="Value of Y";
	m_listPos.InsertColumn(i,lpText,LVCFMT_CENTER,m_listPos.GetStringWidth(lpText)+25,i++);

	m_listPos.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return TRUE;
}

BOOL CData::ShowItem()//显示Data List右边的List控件中的内容
{
	if(!m_lstCurPos)
		return FALSE;
	m_listPos.DeleteAllItems();

	SPoint *pPoint=m_lstCurPos->ptrSPoint;
	CString strMsg;

	for(int i=0;i<m_nLength&&pPoint;i++)
	{
		strMsg.Format("%d",i+1);
		m_listPos.InsertItem(i,strMsg);

		strMsg.Format("%d",pPoint->xPos);
		m_listPos.SetItemText(i,1,strMsg);

		strMsg.Format("%d",pPoint->yPos);
		m_listPos.SetItemText(i,2,strMsg);

		pPoint=pPoint->pNext;

	}

	return TRUE;

}

BOOL CData::ShowAllCurves()//显示Data List左边List控件中的内容
{
	if(!m_lstHdr)
		return false;

	m_listCurve.DeleteAllItems();
	CString strMsg;

	for(int i=0;i<m_nListLength;i++)
	{
		strMsg.Format("%d",i+1);
		m_listCurve.InsertItem(i,strMsg);

		strMsg.Format("%d",m_lpArea[i]);
		m_listCurve.SetItemText(i,1,strMsg);
	}
	return TRUE;

}

BOOL CData::OnInitOtherCtrl()
{
	if(!m_lstHdr)
		return FALSE;

	CString strText;
	strText.Format("%d",m_nListLength);
	m_edtTotalCurves.SetWindowText(strText);

	strText.Format("%d",m_lstHdr->Length);
	m_edtCurCurveLen.SetWindowText(strText);

	m_edtCurCurve.SetWindowText(_T("1"));

	return TRUE;


}

BOOL CData::SetCurveList(CMyBMPDoc *pDoc, int nlistLength)//获取线条链表的头指针以及相关的成员变量的数据初始化
{
	if(!pDoc)
		return FALSE;

	m_lstCurPos=m_lstHdr=pDoc->m_pCurveListHdr;
	m_nListLength=nlistLength;
	m_nHeight=pDoc->m_nHeight;

	if(m_lpArea)
		delete [] m_lpArea;

	m_lpArea=new DWORD[m_nListLength];
	for(int i=0;i<m_nListLength;i++)
		m_lpArea[i]=pDoc->m_lpArea[i];

	

	return TRUE;
}

void CData::OnClickListCurve(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex=m_listCurve.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if(nIndex==-1)
		return;

	m_nSelectCurve=nIndex;//第一项Item的Index的返回值是0
	SetCurvePos(m_nSelectCurve);
	ShowItem();
	OnUpdateEdits();

	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrame);
	CMyBMPView *pView=(CMyBMPView*)pMainFrame->GetActiveFrame()->GetActiveView();
	if(!pView)
		return;
	pView->DrawCurrentCurve(m_nHeight);
	
	*pResult = 0;
}

BOOL CData::SetCurvePos(int nCurPos)////该函数的作用是获得第nCurPos个轮廓线的像素的个数（即组成线条的点像素个数）
{
	if(nCurPos<0||nCurPos>m_nListLength)
		return FALSE;
	if(!m_lstHdr||!m_lstCurPos)
		return FALSE;

	m_nCurPos=nCurPos;//获取当前选中的线条
	m_lstCurPos=m_lstHdr;
	for(int i=0;i<m_nCurPos;i++)
		m_lstCurPos=m_lstCurPos->pNext;
	m_nLength=m_lstCurPos->Length;
	return TRUE;

}

BOOL CData::OnUpdateEdits()
{
	if(!m_lstHdr)
		return FALSE;

	CString strText;
	strText.Format("%d",m_nListLength);
	m_edtTotalCurves.SetWindowText(_T(strText));

	strText.Format("%d",m_lstCurPos->Length);
	m_edtCurCurveLen.SetWindowText(_T(strText));

	strText.Format("%d",m_nCurPos+1);
	m_edtCurCurve.SetWindowText(_T(strText));

	return TRUE;


}

int CData::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	PostMessage(WM_PREINITDIALOGBAR);
	
	return 0;
}
