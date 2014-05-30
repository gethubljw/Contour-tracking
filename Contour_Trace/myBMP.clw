; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyBMPDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "myBMP.h"
LastPage=0

ClassCount=9
Class1=CMyBMPApp
Class2=CMyBMPDoc
Class3=CMyBMPView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_HISTOGRAM
Resource2=IDD_ABOUTBOX
Resource3=IDD_DIALOG_DATA
Class5=CChildFrame
Class6=CAboutDlg
Resource4=IDR_MYBMPTYPE
Resource5=IDR_MAINFRAME
Class7=CColorPalette
Class8=CData
Class9=CHistogram
Resource6=IDD_COLOR_PALETTE

[CLS:CMyBMPApp]
Type=0
HeaderFile=myBMP.h
ImplementationFile=myBMP.cpp
Filter=N

[CLS:CMyBMPDoc]
Type=0
HeaderFile=myBMPDoc.h
ImplementationFile=myBMPDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:CMyBMPView]
Type=0
HeaderFile=myBMPView.h
ImplementationFile=myBMPView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=IDM_THRESHOLD


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=myBMP.cpp
ImplementationFile=myBMP.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MYBMPTYPE]
Type=1
Class=CMyBMPView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
Command22=IDM_HISTOGRAM
Command23=IDM_COLOR_PALETTE
Command24=IDM_DATA_LIST
Command25=IDM_TRACE
Command26=IDM_ZOOM_IN
Command27=IDM_ZOOM_OUT
Command28=IDM_THRESHOLD
CommandCount=28

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_HISTOGRAM]
Type=1
Class=CHistogram
ControlCount=3
Control1=IDC_CHECK_RED,button,1342242819
Control2=IDC_CHECK_GREEN,button,1342242819
Control3=IDC_CHECK_BLUE,button,1342242819

[DLG:IDD_DIALOG_DATA]
Type=1
Class=CData
ControlCount=12
Control1=IDC_LIST_CURVE,SysListView32,1350631437
Control2=IDC_STATIC,button,1342177287
Control3=IDC_LIST_POS,SysListView32,1350631437
Control4=IDC_NEXT,button,1342242816
Control5=IDC_PREVIOUS,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_TOTAL_CURVES,edit,1350631552
Control11=IDC_EDIT_CURRENT_CURVE,edit,1350631552
Control12=IDC_EDIT_CURRENT_CURVE_LENGTH,edit,1350631552

[DLG:IDD_COLOR_PALETTE]
Type=1
Class=CColorPalette
ControlCount=17
Control1=IDC_STATIC,button,1342177287
Control2=IDC_FOREWARD_COLOR,static,1342177287
Control3=IDC_BACKWARD_COLOR,static,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308353
Control6=IDC_RED,static,1342177281
Control7=IDC_STATIC,static,1342308353
Control8=IDC_GREEN,static,1342308353
Control9=IDC_STATIC,static,1342308353
Control10=IDC_BLUE,static,1342308353
Control11=IDC_STATIC,static,1342308353
Control12=IDC_INTENSITY,static,1342308353
Control13=IDC_STATIC,static,1342308353
Control14=IDC_XPOS_SHOW,static,1342308353
Control15=IDC_STATIC,static,1342308353
Control16=IDC_YPOS_SHOW,static,1342308353
Control17=IDC_CURRENT_COLOR,static,1342177287

[CLS:CColorPalette]
Type=0
HeaderFile=ColorPalette.h
ImplementationFile=ColorPalette.cpp
BaseClass=CDialogBar
Filter=D
LastObject=CColorPalette
VirtualFilter=dWC

[CLS:CData]
Type=0
HeaderFile=Data.h
ImplementationFile=Data.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDC_LIST_CURVE
VirtualFilter=dWC

[CLS:CHistogram]
Type=0
HeaderFile=Histogram.h
ImplementationFile=Histogram.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDC_CHECK_RED
VirtualFilter=dWC

