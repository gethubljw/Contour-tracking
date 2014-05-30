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
	if(!file.Open(lpszPathName,CFile::modeRead|CFile::shareDenyWrite))//文件打开
	{
		AfxMessageBox("Error:Fail to open the file",MB_OK,0);
		return false;
	}

	BITMAPFILEHEADER bmheader;
	if(file.Read((LPSTR)&bmheader,sizeof(BITMAPFILEHEADER))!=sizeof(BITMAPFILEHEADER))//读文件头
	{
		AfxMessageBox("Error:Size of File Header Go Wrong",0,0);//该函数的第二个参数可以为0？？？
		file.Close();
		return false;
	}

	if(bmheader.bfType!=*(WORD*)"BM")//文件类型判断
	{
		AfxMessageBox("Error: Type of File don't match",0,0);
		return false;
	}

	m_dwFileLength=file.GetLength();
	HDIB hDib=(HDIB)::GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,m_dwFileLength);
	
	if(!hDib)//内存分配成功与否判断
	{
		AfxMessageBox("Error:Fail to Allocate Global Memory ",0,0);
		file.Close();
		return false;
	}

	m_hDib=hDib;
	LPSTR pDib=(LPSTR)::GlobalLock((HGLOBAL)hDib);
	if(!pDib)//获取指针成功与否判断
	{
		AfxMessageBox("Error: Fail to Gain the Pointer to the First Byte of Memory",0,0);
		::GlobalUnlock((HGLOBAL)hDib);
		ASSERT(!::GlobalFree((HGLOBAL)hDib));
		file.Close();
		return false;
	}

	DWORD bitLength=m_dwFileLength-sizeof(BITMAPFILEHEADER);
	if(file.ReadHuge(pDib,bitLength)!=bitLength)//读取图像数据
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

	for(int i;i<Length;i++)//对数组进行初始化
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

	for(int i=0;i<Length;i++)//对数组进行初始化
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

	for(int i=0;i<nLen;i++)//检测该点是否在线条边沿上
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

void CMyBMPDoc::OnTrace()//该函数需要在进行阈值化后才能调用 
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

	int x,y;//作为循环变量
	int xStart=0,yStart=0;
	bool bFirst=true;
	SPoint *pHeader,*pPoint;//用于记录扫描点

	BYTE *pData=NULL;
    int nCount=0;
	SList *plistCur,*plistHeader,*plist;

	bool bFindStart=false,bFlag=false;
	int nDir=0;

	int nDirection[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};//方向向量
	int nNumDirChange=0;
	int xCur=0,yCur=0;//在轮廓检测当中表示当前点

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
				yStart=y;//记录起始点坐标
				pHeader=pPoint=new SPoint;//pHeader用于指向一个线条当中的第一个SPoint
				pPoint->xPos=x;
				pPoint->yPos=y;
				nCount=1;

				if(!bFirst)
				{
					plistCur=new SList;
					plistCur->ptrSPoint=pHeader;
					plistCur->pNext=NULL;

					plist->pNext=plistCur;
					plist=plistCur;//进行链表的连接

				}
				else//该子句代码在整个trace过程之中只执行一次，只在第一次建立SList对象的时候执行
				{
					plistHeader=plist=new SList;
					plist->ptrSPoint=pHeader;
					plist->pNext=NULL;
					bFirst=false;
				}

				bFindStart=true;//表示已经找到了新的轮廓线条的起始点
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
						pNew->yPos=yCur;//创建一个新的点并记录新的点的信息
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
//以下为优化部分，只创建一次多边形，目的是减少对rgn.CreatePolygonRgn(pPoint,plist->Length,ALTERNATE);的调用，提高效率							
							CPoint *pPoint=new CPoint[plist->Length];
							SPoint *ptrHeader=plist->ptrSPoint;
							
							CRgn rgn;
							for(int i=0;i<plist->Length;i++)//创建数组
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

BOOL CMyBMPDoc::GetAllCurvesArea()//将每个线条包围的面积存入m_lpArea指向的内存当中
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

	
//////////////////////////////////////////////////////////////////////以下为优化部分，只创建一次多边形，减少对
		                                        //rgn.CreatePolygonRgn(pcPoint,plist->Length,ALTERNATE);的调用
	CPoint *pcPoint=new CPoint[plist->Length];
	SPoint *ptrHeader=plist->ptrSPoint;

	
	CRgn rgn;
	for(int i=0;i<plist->Length;i++)//创建数组
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

int CMyBMPDoc::GetListLength()//获取线条的个数
{
	int nListLength=0;
	SList *plist=NULL;
	for(plist=m_pCurveListHdr;plist;plist=plist->pNext)//计算线条的个数
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
	pMainFrame->m_wndDataList.SetCurvePos(0);//刚开始是显示第一个线条的所有Item，之后可在CData::OnClickListCurve进行选择
	pMainFrame->m_wndDataList.ShowItem();
	pMainFrame->m_wndDataList.ShowAllCurves();//显示的是左边list控件中的内容
	pMainFrame->m_wndDataList.OnInitOtherCtrl();
	pMainFrame->m_wndDataList.Invalidate();

	return TRUE;

}

void CMyBMPDoc::OnThreshold()//对图像进行阈值化操作 
{
	// TODO: Add your command handler code here
	if(!m_hDib)
		return;
	LPSTR pDib=(LPSTR)::GlobalLock((HGLOBAL)m_hDib);
	LPBITMAPINFOHEADER lpmi=(LPBITMAPINFOHEADER)pDib;
	LPSTR lpszDib=pDib+lpmi->biSize;

	int nHistogram[256];
	for(int i=0;i<256;i++)
		nHistogram[i]=0;//对其中的数据进行初始化

	BYTE *pData=NULL,gray=0,bMin,bMax;
	pData=(BYTE*)lpszDib;
	gray=(BYTE)((pData[0]+pData[1]+pData[2])/3);
	bMin=bMax=gray;//对bMin和bMax赋初值，避免数据污染

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
		nNumberOfGray=nSumOfGray=0;//注意此处要清零

		for(BYTE bGrayValue=bMin; bGrayValue < bThreshold; bGrayValue++ )//计算左边均值
		{
			nNumberOfGray=nNumberOfGray + nHistogram[bGrayValue];
			nSumOfGray=nSumOfGray + nHistogram[bGrayValue]*bGrayValue;//注意此处的计算

		}
		if(nNumberOfGray)
			bAverage1=(BYTE)(nSumOfGray / nNumberOfGray);

		nNumberOfGray=nSumOfGray=0;
		for(bGrayValue = bThreshold+1 ; bGrayValue<bMax ;bGrayValue++)//计算右边均值
		{
			nNumberOfGray=nNumberOfGray+nHistogram[bGrayValue];
			nSumOfGray=nSumOfGray+nHistogram[bGrayValue] * bGrayValue;//注意此处的计算
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
