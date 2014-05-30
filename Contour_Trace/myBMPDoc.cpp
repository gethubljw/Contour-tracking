// myBMPDoc.cpp : implementation of the CMyBMPDoc class
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
// CMyBMPDoc

IMPLEMENT_DYNCREATE(CMyBMPDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyBMPDoc, CDocument)
	//{{AFX_MSG_MAP(CMyBMPDoc)
	ON_COMMAND(IDM_TRACE, OnTrace)
	ON_COMMAND(IDM_THRESHOLD, OnThreshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBMPDoc construction/destruction

CMyBMPDoc::CMyBMPDoc()
{
	// TODO: add one-time construction code here
	m_pCurveListHdr=NULL;
	m_lpArea=NULL;
	m_bThreshold=FALSE;
	m_bFlag=FALSE;
	m_bOK=FALSE;

}

CMyBMPDoc::~CMyBMPDoc()
{
	if(m_lpArea)
		delete [] m_lpArea;
}

BOOL CMyBMPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyBMPDoc serialization

void CMyBMPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyBMPDoc diagnostics

#ifdef _DEBUG
void CMyBMPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyBMPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyBMPDoc commands

BOOL CMyBMPDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	CDocument::DeleteContents();
	CFile file;
	if(!file.Open(lpszPathName,CFile::modeRead|CFile::shareDenyWrite))//�ļ���
	{
		AfxMessageBox("Error:Fail to open the file",MB_OK,0);
		return false;
	}

	BITMAPFILEHEADER bmheader;
	if(file.Read((LPSTR)&bmheader,sizeof(BITMAPFILEHEADER))!=sizeof(BITMAPFILEHEADER))//���ļ�ͷ
	{
		AfxMessageBox("Error:Size of File Header Go Wrong",0,0);//�ú����ĵڶ�����������Ϊ0������
		file.Close();
		return false;
	}

	if(bmheader.bfType!=*(WORD*)"BM")//�ļ������ж�
	{
		AfxMessageBox("Error: Type of File don't match",0,0);
		return false;
	}

	m_dwFileLength=file.GetLength();
	HDIB hDib=(HDIB)::GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,m_dwFileLength);
	
	if(!hDib)//�ڴ����ɹ�����ж�
	{
		AfxMessageBox("Error:Fail to Allocate Global Memory ",0,0);
		file.Close();
		return false;
	}

	m_hDib=hDib;
	LPSTR pDib=(LPSTR)::GlobalLock((HGLOBAL)hDib);
	if(!pDib)//��ȡָ��ɹ�����ж�
	{
		AfxMessageBox("Error: Fail to Gain the Pointer to the First Byte of Memory",0,0);
		::GlobalUnlock((HGLOBAL)hDib);
		ASSERT(!::GlobalFree((HGLOBAL)hDib));
		file.Close();
		return false;
	}

	DWORD bitLength=m_dwFileLength-sizeof(BITMAPFILEHEADER);
	if(file.ReadHuge(pDib,bitLength)!=bitLength)//��ȡͼ������
	{
		AfxMessageBox("Error: Size of the File Go Wrong",0,0);
		file.Close();
		::GlobalUnlock((HGLOBAL)hDib);
		ASSERT(!::GlobalFree((HGLOBAL)hDib));
		return false;
	}

	m_nWidth=((LPBITMAPINFOHEADER)pDib)->biWidth;
	m_nHeight=((LPBITMAPINFOHEADER)pDib)->biHeight;
	m_nBitCount=((LPBITMAPINFOHEADER)pDib)->biBitCount;
	m_nScanWidth=(m_nWidth*m_nBitCount+31)/32*4;
	size.cx=m_nWidth;
	size.cy=m_nHeight;

	SetPathName(lpszPathName);
	SetModifiedFlag(false);

	UpdateHistogram();
	::GlobalUnlock((HGLOBAL)pDib);
	file.Close();

	return TRUE;
}

BOOL CMyBMPDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}

void CMyBMPDoc::UpdateHistogram()
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_wndHistogram.Invalidate();
}

/*BOOL CMyBMPDoc::GetHistoGramValue(int *RedArray,int *GreenArray,int *BlueArray,int Length)
{
	if(!m_hDib)
		return false;
	LPSTR lpDib=(LPSTR)::GlobalLock((HGLOBAL)m_hDib);
	ASSERT(lpDib);
	LPBITMAPHEADER lpmi=(LPBITMAPINFOHEADER)lpDib;
	LPSTR lpszDib=lpDib+lpmi->biSize;

	for(int i;i<Length;i++)//��������г�ʼ��
		RedArray[i]=GreenArray[i]=BlueArray[i]=0;

	for(int y=0;y<m_nHeight;y++)
		for(int x=0;x<m_nWidth;x++)
		{
			LPSTR lpData=lpszDib+(m_nHeight-y-1)*m_nScanWidth+x*3;
			RedArray[lpData[2]]++;
			GreenArray[lpData[1]]++;
			BlueArray[lpData[0]]++;
		}

	::GlobalUnlock((HGLOBAL)m_hDib);
	return true;
}

  */

BOOL CMyBMPDoc::GetHistoGramValue(int *RedArray, int *GreenArray, int *BlueArray, int Length)
{
	if(!m_hDib)
		return false;
	LPSTR lpDib=(LPSTR)::GlobalLock((HGLOBAL)m_hDib);
	ASSERT(lpDib);
	LPBITMAPINFOHEADER lpmi=(LPBITMAPINFOHEADER)lpDib;
	LPSTR lpszDib=lpDib+lpmi->biSize;

	for(int i=0;i<Length;i++)//��������г�ʼ��
		RedArray[i]=GreenArray[i]=BlueArray[i]=0;

	for(int y=0;y<m_nHeight;y++)
		for(int x=0;x<m_nWidth;x++)
		{
			BYTE *lpData=(BYTE*)lpszDib+(m_nHeight-y-1)*m_nScanWidth+x*3;
			RedArray[lpData[2]]++;
			GreenArray[lpData[1]]++;
			BlueArray[lpData[0]]++;
		}

	::GlobalUnlock((HGLOBAL)m_hDib);
	return true;

}

BOOL CMyBMPDoc::PtInRegion(int x, int y, SPoint *ptrHeader, int nLen)
{
	CPoint *pPoint=new CPoint[nLen];
	CPoint point(x,y);
	CRgn rgn;

	for(int i=0;i<nLen;i++)//���õ��Ƿ�������������
	{
		pPoint[i].x=ptrHeader->xPos;
		pPoint[i].y=ptrHeader->yPos;
		if(point==pPoint[i])
		{
			delete [] pPoint;
			return TRUE;
		}
		
		ptrHeader=ptrHeader->pNext;
	}



	BOOL bOK=rgn.CreatePolygonRgn(pPoint,nLen,ALTERNATE);
	BOOL bIn=FALSE;
	if(bOK)
	{
		bIn=rgn.PtInRegion(point);
		rgn.DeleteObject();
	}

	delete [] pPoint;
	return bIn;



}

void CMyBMPDoc::OnTrace()//�ú�����Ҫ�ڽ�����ֵ������ܵ��� 
{
	// TODO: Add your command handler code here
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrame);
	CStatusBar *pStatus=(CStatusBar*)pMainFrame->GetMessageBar();
	ASSERT(pStatus);

	CRect rect;
	CString str;
	CProgressCtrl progress;

	pStatus->GetItemRect(1,&rect);
	progress.Create(WS_CHILD|WS_VISIBLE,rect,pStatus,1);
	progress.SetRange(0,m_nHeight);
	progress.SetStep(1);

	str="Current Progress of Trace";
	if(progress.IsWindowVisible())
	{
		pStatus->SetPaneText(0,str);
		pStatus->UpdateWindow();
	}

///////////////////////////////////////////////////////////////////////

	if(!m_hDib)
		return;
	LPSTR lpDib=(LPSTR)::GlobalLock((HGLOBAL)m_hDib);
	ASSERT(lpDib);
	LPBITMAPINFOHEADER lpmi=(LPBITMAPINFOHEADER)lpDib;
	LPSTR lpDibStart=lpDib+lpmi->biSize;

	int x,y;//��Ϊѭ������
	int xStart=0,yStart=0;
	bool bFirst=true;
	SPoint *pHeader,*pPoint;//���ڼ�¼ɨ���

	BYTE *pData=NULL;
    int nCount=0;
	SList *plistCur,*plistHeader,*plist;

	bool bFindStart=false,bFlag=false;
	int nDir=0;

	int nDirection[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};//��������
	int nNumDirChange=0;
	int xCur=0,yCur=0;//��������⵱�б�ʾ��ǰ��

	int xMin,xMax,yMin,yMax,loop,flag=0;
	SPoint *pHunt;
	int row=0,col=0;

	for(y=0;y<m_nHeight;y++)
	{
		str.Format("Current Progress of Trace: %d%% has been Completed ",y*100/(m_nHeight-1));
		progress.StepIt();
		if(progress.IsWindowVisible())
		{
			pStatus->SetPaneText(0,str);
			pStatus->UpdateWindow();
		}
		for(x=0;x<m_nWidth;x++)
		{
			pData=(BYTE *)lpDibStart+y*m_nScanWidth+x*3;
			if(*pData==0)
			{
				xStart=x;
				yStart=y;//��¼��ʼ������
				pHeader=pPoint=new SPoint;//pHeader����ָ��һ���������еĵ�һ��SPoint
				pPoint->xPos=x;
				pPoint->yPos=y;
				nCount=1;

				if(!bFirst)
				{
					plistCur=new SList;
					plistCur->ptrSPoint=pHeader;
					plistCur->pNext=NULL;

					plist->pNext=plistCur;
					plist=plistCur;//�������������

				}
				else//���Ӿ����������trace����֮��ִֻ��һ�Σ�ֻ�ڵ�һ�ν���SList�����ʱ��ִ��
				{
					plistHeader=plist=new SList;
					plist->ptrSPoint=pHeader;
					plist->pNext=NULL;
					bFirst=false;
				}

				bFindStart=true;//��ʾ�Ѿ��ҵ����µ�������������ʼ��
				nNumDirChange=0;
				nDir=0;
				xCur=xStart;
				yCur=yStart;
				
				while(bFindStart)
				{
					if((yCur+nDirection[nDir][1])<0||(yCur+nDirection[nDir][1])>m_nHeight||
						(xCur+nDirection[nDir][0])<0||(xCur+nDirection[nDir][0])>m_nWidth)
					{
						nDir++;
						nNumDirChange++;
						if(nNumDirChange>7)
						{
							pPoint->pNext=NULL;
							plist->Length=nCount;

							bFindStart=false;
							break;
						}
						if(nDir>7)
							nDir=nDir-8;
						continue;
					}

					pData=(BYTE*)lpDibStart+(nDirection[nDir][1]+yCur)*m_nScanWidth+(nDirection[nDir][0]+xCur)*3;
					if(*pData==0)
					{
						nNumDirChange=0;
						SPoint *pNew=new SPoint;
						xCur=xCur+nDirection[nDir][0];
						yCur=yCur+nDirection[nDir][1];

						pNew->xPos=xCur;
						pNew->yPos=yCur;//����һ���µĵ㲢��¼�µĵ����Ϣ
						pNew->pNext=NULL;

						pPoint->pNext=pNew;
						pPoint=pNew;
						nCount++;

						if(xCur==xStart&&yCur==yStart)
						{
							plist->Length=nCount;							

							pHunt=plist->ptrSPoint;
							xMin=xMax=pHunt->xPos;
							yMin=yMax=pHunt->yPos;
							for(loop=1;loop<nCount;loop++)
							{
								pHunt=pHunt->pNext;
								xMin=xMin<(pHunt->xPos) ? xMin:(pHunt->xPos);
								xMax=xMax>(pHunt->xPos) ? xMax:(pHunt->xPos);
								yMin=yMin<(pHunt->yPos) ? yMin:(pHunt->yPos);
								yMax=yMax>(pHunt->yPos) ? yMax:(pHunt->yPos);
							}
////////////////////////////////////////////////////////////////////////////////////////////////
//����Ϊ�Ż����֣�ֻ����һ�ζ���Σ�Ŀ���Ǽ��ٶ�rgn.CreatePolygonRgn(pPoint,plist->Length,ALTERNATE);�ĵ��ã����Ч��							
							CPoint *pPoint=new CPoint[plist->Length];
							SPoint *ptrHeader=plist->ptrSPoint;
							
							CRgn rgn;
							for(int i=0;i<plist->Length;i++)//��������
								{
									pPoint[i].x=ptrHeader->xPos;
									pPoint[i].y=ptrHeader->yPos;									
									ptrHeader=ptrHeader->pNext;
								}																									
							rgn.CreatePolygonRgn(pPoint,plist->Length,ALTERNATE);																											pData=(BYTE*)lpDibStart+row*m_nScanWidth+col*3;									
								
						    for( row=yMin;row<=yMax;row++)
							    for(col=xMin;col<=xMax;col++)
								{
                                    CPoint point(col,row);
									m_bFlag=TRUE;								
							    	pData=(BYTE*)lpDibStart+row*m_nScanWidth+col*3;
								    for(int i=0;i<plist->Length;i++)
									{
									    if(point==pPoint[i])
										{
											if(pData[0]==0)	
												pData[0]=100;									    										
										    m_bFlag=FALSE;
									    	break;
										}
									}
								    if(m_bFlag==TRUE)
									{
									    if(rgn.PtInRegion(point))
										{
										    if(pData[0]==0)
											    pData[0]=100;
										}
									}
								
								}
                            delete [] pPoint;
					        rgn.DeleteObject();
						}
					   nDir=nDir-2;
					}
					else
					{
						nDir++;
						nNumDirChange++;
						if(nNumDirChange>7)
						{
							plist->Length=nCount;
							bFindStart=false;
							break;
						}
					}

					if(nDir<0)
						nDir=nDir+8;
					if(nDir>7)
						nDir=nDir-8;
				}
					
			}

		}
	}

	m_pCurveListHdr=plistHeader;
	::GlobalUnlock((HANDLE)m_hDib);
	SetModifiedFlag();

    GetAllCurvesArea();
	UpdateHistogram();
	UpdateAllViews(NULL);
	
	pStatus->SetPaneText(0,"");
	pStatus->UpdateWindow();

	
	m_bTrace=TRUE;
	ShowDataList(m_bTrace);
	
}

BOOL CMyBMPDoc::GetAllCurvesArea()//��ÿ��������Χ���������m_lpAreaָ����ڴ浱��
{
	CMainFrame *pMainframe=(CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainframe);
	CStatusBar *pStatus=(CStatusBar*)pMainframe->GetMessageBar();
	ASSERT(pStatus);

	CRect rect;
	CString strMsg;
	CProgressCtrl progress;

	pStatus->GetItemRect(1,&rect);
	progress.Create(WS_CHILD|WS_VISIBLE,rect,pStatus,1);
	progress.SetRange(0,m_nHeight);
	progress.SetStep(1);

	strMsg="Current Progress of Caculate Area";
	if(progress.IsWindowVisible())
	{
		pStatus->SetPaneText(0,strMsg);
		pStatus->UpdateWindow();
	}
/////////////////////////////////////////////////////////////////
	if(!m_hDib)
		return FALSE;
	LPSTR lpDib=(LPSTR)::GlobalLock((HGLOBAL)m_hDib);
	ASSERT(lpDib);
	LPBITMAPINFOHEADER lpmi=(LPBITMAPINFOHEADER)lpDib;
	LPSTR lpDibStart=lpDib+lpmi->biSize;
	BYTE *pData=NULL;
	if(!m_pCurveListHdr)
		return FALSE;

	int nListLength=0;
	nListLength=GetListLength();
	if(!nListLength)
		return FALSE;

	m_lpArea = new DWORD [nListLength];
	for(int i=0;i<nListLength;i++)
		m_lpArea[i]=0;

	SList *plist=m_pCurveListHdr;
	int xMax,xMin,yMax,yMin;
	SPoint *pPoint=NULL;
	int nCount=0,x=0,y=0;
	int row,col;

	for(int flag=0;plist;plist=plist->pNext,flag++)
	{
		pPoint=plist->ptrSPoint;
		xMax=xMin=pPoint->xPos;
	    yMax=yMin=pPoint->yPos;

//		pData=(BYTE*)lpDibStart+(pPoint->yPos)*m_nScanWidth+(pPoint->xPos)*3;
//		pData[0]=pData[1]=pData[2]=0;

		for(nCount=1;nCount<plist->Length;nCount++)
		{
			pPoint=pPoint->pNext;
//			pData=(BYTE*)lpDibStart+(pPoint->yPos)*m_nScanWidth+(pPoint->xPos)*3;
//			pData[0]=pData[1]=pData[2]=0;

			if(xMax<pPoint->xPos)
				xMax=pPoint->xPos;
			if(xMin>pPoint->xPos)
				xMin=pPoint->xPos;
			if(yMax<pPoint->yPos)
				yMax=pPoint->yPos;
			if(yMin>pPoint->yPos)
				yMin=pPoint->yPos;
		}

/*		for(y=yMin;y<=yMax;y++)
		{			
			for(x=xMin;x<=xMax;x++)
			{
				pData=(BYTE*)lpDibStart+y*m_nScanWidth+x*3;
				if(PtInRegion(x,y,plist->ptrSPoint,plist->Length))
				{
					if(pData[0]==100)
						pData[0]=0;
					m_lpArea[flag]++;
				}
					
			}
		}
		*/

	
//////////////////////////////////////////////////////////////////////����Ϊ�Ż����֣�ֻ����һ�ζ���Σ����ٶ�
		                                        //rgn.CreatePolygonRgn(pcPoint,plist->Length,ALTERNATE);�ĵ���
	CPoint *pcPoint=new CPoint[plist->Length];
	SPoint *ptrHeader=plist->ptrSPoint;

	
	CRgn rgn;
	for(int i=0;i<plist->Length;i++)//��������
	{
		pcPoint[i].x=ptrHeader->xPos;
		pcPoint[i].y=ptrHeader->yPos;									
		ptrHeader=ptrHeader->pNext;
	}																									
	rgn.CreatePolygonRgn(pcPoint,plist->Length,ALTERNATE);																											pData=(BYTE*)lpDibStart+row*m_nScanWidth+col*3;									
	
	for( row=yMin;row<=yMax;row++)
		for(col=xMin;col<=xMax;col++)
		{
			CPoint point(col,row);
			m_bFlag=TRUE;								
			pData=(BYTE*)lpDibStart+row*m_nScanWidth+col*3;
			for(int i=0;i<plist->Length;i++)
			{
				if(point==pcPoint[i])
				{
					if(pData[0]==100)	
						pData[0]=0;
					m_lpArea[flag]++;
					m_bFlag=FALSE;
					break;
				}
			}
			if(m_bFlag==TRUE)
			{
			     if(rgn.PtInRegion(point))
				{
					if(pData[0]==100)
					    pData[0]=0;
					m_lpArea[flag]++;
				}
			}
									
		}
		delete [] pcPoint;
		rgn.DeleteObject();

////////////////////////////////////////////////////////////////
		strMsg.Format("Current Progress of Caculate Area is %d%% Completed",(flag+1)*100/(nListLength));
		if(progress.IsWindowVisible())
		{
			pStatus->SetPaneText(0,strMsg);
			pStatus->UpdateWindow();
		}

	}


	pStatus->SetPaneText(0,"");
	pStatus->UpdateWindow();
	return TRUE;

}

int CMyBMPDoc::GetListLength()//��ȡ�����ĸ���
{
	int nListLength=0;
	SList *plist=NULL;
	for(plist=m_pCurveListHdr;plist;plist=plist->pNext)//���������ĸ���
		nListLength++;

	return nListLength;

}

BOOL CMyBMPDoc::ShowDataList(BOOL bTrace)
{
	if(!bTrace||!m_pCurveListHdr)
		return FALSE;
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrame);

	pMainFrame->m_wndDataList.SetCurveList(this,GetListLength());
	pMainFrame->m_wndDataList.SetCurvePos(0);//�տ�ʼ����ʾ��һ������������Item��֮�����CData::OnClickListCurve����ѡ��
	pMainFrame->m_wndDataList.ShowItem();
	pMainFrame->m_wndDataList.ShowAllCurves();//��ʾ�������list�ؼ��е�����
	pMainFrame->m_wndDataList.OnInitOtherCtrl();
	pMainFrame->m_wndDataList.Invalidate();

	return TRUE;

}

void CMyBMPDoc::OnThreshold()//��ͼ�������ֵ������ 
{
	// TODO: Add your command handler code here
	if(!m_hDib)
		return;
	LPSTR pDib=(LPSTR)::GlobalLock((HGLOBAL)m_hDib);
	LPBITMAPINFOHEADER lpmi=(LPBITMAPINFOHEADER)pDib;
	LPSTR lpszDib=pDib+lpmi->biSize;

	int nHistogram[256];
	for(int i=0;i<256;i++)
		nHistogram[i]=0;//�����е����ݽ��г�ʼ��

	BYTE *pData=NULL,gray=0,bMin,bMax;
	pData=(BYTE*)lpszDib;
	gray=(BYTE)((pData[0]+pData[1]+pData[2])/3);
	bMin=bMax=gray;//��bMin��bMax����ֵ������������Ⱦ

	for(int y=0;y<m_nHeight;y++)
		for(int x=0;x<m_nWidth;x++)
		{
			pData=(BYTE*)lpszDib+y*m_nScanWidth+x*3;
			gray=(BYTE)((pData[0]+pData[1]+pData[2])/3);
			pData[0]=gray;
            pData[1]=gray;
            pData[2]=gray;
			nHistogram[gray]++;

			bMin=bMin < gray ? bMin : gray;
			bMax=bMax > gray ? bMax : gray;
		}

	BYTE bCurThreshold=(BYTE)((bMax+bMin)/2);
	BYTE bThreshold=0,bAverage1=0,bAverage2=0;
	int nNumberOfGray=0,nSumOfGray=0;

	for(int nIteration=0; nIteration<500 && (bCurThreshold!=bThreshold); nIteration++)
	{
		bThreshold=bCurThreshold;
		bAverage1=bAverage2=0;
		nNumberOfGray=nSumOfGray=0;//ע��˴�Ҫ����

		for(BYTE bGrayValue=bMin; bGrayValue < bThreshold; bGrayValue++ )//������߾�ֵ
		{
			nNumberOfGray=nNumberOfGray + nHistogram[bGrayValue];
			nSumOfGray=nSumOfGray + nHistogram[bGrayValue]*bGrayValue;//ע��˴��ļ���

		}
		if(nNumberOfGray)
			bAverage1=(BYTE)(nSumOfGray / nNumberOfGray);

		nNumberOfGray=nSumOfGray=0;
		for(bGrayValue = bThreshold+1 ; bGrayValue<bMax ;bGrayValue++)//�����ұ߾�ֵ
		{
			nNumberOfGray=nNumberOfGray+nHistogram[bGrayValue];
			nSumOfGray=nSumOfGray+nHistogram[bGrayValue] * bGrayValue;//ע��˴��ļ���
		}
		if(nNumberOfGray)
			bAverage2=(BYTE)(nSumOfGray/nNumberOfGray);

	    bCurThreshold=(bAverage1 + bAverage2) / 2;
	
	}

	for(y=0; y<m_nHeight;y++)
		for(int x=0; x<m_nWidth;x++)
		{
			pData=(BYTE*)lpszDib+y*m_nScanWidth+x*3;
			pData[0]=pData[0]>bThreshold ? 255:0;
			pData[1]=pData[2]=pData[0];
		}

	::GlobalUnlock((HGLOBAL)m_hDib);
	SetModifiedFlag();
	UpdateAllViews(NULL);
	UpdateHistogram();
	m_bThreshold=TRUE;
	
	
}
