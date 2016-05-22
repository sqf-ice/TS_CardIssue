// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "TS_UI.h"
#include "MainFrm.h"
#include "Script/tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_MYSETTING, &CMainFrame::OnSetting)
	ON_COMMAND(ID_OPENFILE, &CMainFrame::OnOpenfile)
	ON_UPDATE_COMMAND_UI(ID_OPENFILE, &CMainFrame::OnUpdateOpenfile)
	ON_COMMAND(ID_DISCARD, &CMainFrame::OnDiscard)
	ON_UPDATE_COMMAND_UI(ID_DISCARD, &CMainFrame::OnUpdateDiscard)
	ON_COMMAND(ID_SAVEFILE, &CMainFrame::OnSavefile)
	ON_UPDATE_COMMAND_UI(ID_SAVEFILE, &CMainFrame::OnUpdateSavefile)
ON_WM_SIZE()
ON_WM_TIMER()
ON_WM_GETMINMAXINFO()
ON_COMMAND(ID_CHECK2, &CMainFrame::OnCheckFixedKey)
ON_UPDATE_COMMAND_UI(ID_CHECK2, &CMainFrame::OnUpdateCheckFixedKey)
ON_COMMAND(ID_CHECK3, &CMainFrame::OnCheckEncoder)
ON_UPDATE_COMMAND_UI(ID_CHECK3, &CMainFrame::OnUpdateCheckEncoder)
ON_COMMAND(ID_CHECK4, &CMainFrame::OnCheckPSAM)
ON_UPDATE_COMMAND_UI(ID_CHECK4, &CMainFrame::OnUpdateCheckPSAM)
ON_COMMAND(ID_LOADXML_YK, &CMainFrame::OnLoadXML_YK)
ON_COMMAND(ID_DISCARD_YK, &CMainFrame::OnDiscard_YK)
ON_UPDATE_COMMAND_UI(ID_DISCARD_YK, &CMainFrame::OnUpdateDiscard_YK)
ON_COMMAND(ID_SAVEFILE_YK, &CMainFrame::OnSavefile_YK)
ON_UPDATE_COMMAND_UI(ID_SAVEFILE_YK, &CMainFrame::OnUpdateSavefile_YK)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame():m_isSplitter(FALSE),cInformation(NULL)
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	char szPath[50] = "c:\\TS_Logs\\";
	RemoveLOG(szPath,15);//删除15天之前的日志文件
	m_nCheckLog = 1;
}

CMainFrame::~CMainFrame()
{
	KillTimer(1);
	if ( cInformation != NULL )
	{
		delete cInformation;
		cInformation = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);//加载图标 继承类中重写该方法可以屏蔽掉该功能

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, _T(" 当前时间：00:00:00 "), TRUE), _T("时间"));
	// 启动定时器，定时器ID为1，定时时间为1000ms，即1s   
	SetTimer(1, 1000, NULL); 

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	m_MainButton = new CMyRibbonApplicationButton;
	m_MainButton->SetImage(IDB_MAIN);//显示应用按钮
	m_wndRibbonBar.SetApplicationButton(m_MainButton,CSize(50,50));//屏蔽应用按钮的下拉功能菜单
	
	OnInniteDatas_YK();
	OnInniteDatas_FK();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE;//开放修改窗口标题
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSetting()//点击配置发卡参数事件
{
	// TODO: 在此添加命令处理程序代码
	CMainDlg cSetDlg;
	if ( IDOK == cSetDlg.DoModal() )//关闭对话框并保存了数据
	{
		OnInniteDatas_YK();
		m_pEditView->SetWindowTextA("");
		m_leftTreeView->ptheTree->DeleteAllItems();
		m_nCOSType = cSetDlg.m_nCOS;
		m_vecQCParam.clear();
		m_vecQCParam =cSetDlg.m_vecClearCardParam;
		m_nCheckKeys = cSetDlg.m_nCheckKeys;
		m_nCheckATR = cSetDlg.m_nCheckATR;
		m_strSearial = cSetDlg.m_strSearial;
		m_strTranKey = cSetDlg.m_strTanKey;
		m_nCheckLog = cSetDlg.m_nCheckLog;
		m_bSetting = true;
		m_bCheXML = false;
		m_bScript = false;
	}
}

void CMainFrame::OnOpenfile()//点击打开读取XML配置事件
{
	stringstream strsLogs,strsTemp,strsProInfo;
	strsLogs.str("");
	strsProInfo.str("");

	// TODO: 在此添加命令处理程序代码
	stringstream strsXmlPath;
	strsXmlPath.str("");
	//定义一个CFileDialog对象生成一个打开文件对话框
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"xml文件(*.xml)|*.xml||",AfxGetMainWnd());
	//CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All Files(*.*)|*.*|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls||",AfxGetMainWnd());
	CString strXmlPath;
	if(dlg.DoModal() == IDOK)//点击对话框的确定按钮
	{
		m_pEditView->SetWindowTextA("");
		m_leftTreeView->ptheTree->DeleteAllItems();
		strXmlPath=dlg.GetPathName();//取得选中要打开的文件路径
		strsXmlPath << strXmlPath;
		m_bSetting = true;
		m_bScript = false;

		//信息类实例化
		if ( cInformation != NULL )
		{
			delete cInformation;
			cInformation = NULL;
		}
		cInformation = new CInformation();
		if ( cInformation->ReadXMLDatas(strsXmlPath.str(),m_nCheckLog) )//读取XML数据
			cInformation->CheckXmlDatas();//是脚本配置文件的时候检测读取的数据信息

		if ( cInformation->Errors()	== 0 )//检测读取的XML数据无错误
		{
			ChangeXMLTreeDatas();
			GetProInfo(strsProInfo);
			PrintText(strsProInfo.str());
		}

		//输出错误信息
		strsLogs<<"检测到的错误信息数为："<<cInformation->Errors()<<endl;
		if ( cInformation->Errors()	== 0 )	
			PrintText(strsLogs.str(),1,1);
		else
			PrintText(strsLogs.str(),0,1);
		strsTemp.str("");
		vector<string>::const_iterator iter1 = cInformation->m_vecErrData.begin();
		while ( iter1 != cInformation->m_vecErrData.end() )
		{
			strsLogs<<*iter1<<endl;
			strsTemp<<*iter1<<endl;
			++iter1;
		}
		strsLogs<<endl;
		strsTemp<<endl;
		PrintText(strsTemp.str(),1);
		//输出警告信息
		strsLogs<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
		strsTemp.str("");
		strsTemp<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
		PrintText(strsTemp.str(),1,2);
		strsTemp.str("");
		vector<string>::const_iterator iter2 = cInformation->m_vecWrnData.begin();
		while ( iter2 != cInformation->m_vecWrnData.end() )
		{
			strsLogs<<*iter2<<endl;
			strsTemp<<*iter2<<endl;
			++iter2;
		}
		PrintText(strsTemp.str(),1);

		if ( cInformation->Errors()	== 0 )//检测读取的XML数据无错误
			m_bCheXML = true;
		else
		{
			m_bCheXML = false;
			if ( m_nCheckLog == 1 )
				WriteLog(strsLogs.str(),1);
			delete cInformation;
			cInformation = NULL;
		}

	}

}

void CMainFrame::ChangeXMLTreeDatas()
{
	m_leftTreeView->ptheTree->ModifyStyle(0,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
	//改变树结构的数据
	TVINSERTSTRUCT tvInsert;//父项的句柄。如果这个成员的值是TVI_ROOT或NULL，这项将被作为树形控件的根插入
	HTREEITEM hTreeItem,hSubItem,hSubItem1,hSubItem2;
	//插入的新项之后的项的句柄 TVI_FIRST在列表的开始插入项 TVI_LAST在列表的最后插入项 TVI_ROOT作为一个根项添加 TVI_SORT以字母顺序插入项
	tvInsert.hInsertAfter = NULL;
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = "MF";
	hTreeItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在根节点添加MF

	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//DDF
	{
		tvInsert.hParent = hTreeItem;//上级目录为MF
		if ( (*iterDDF).m_strFID == "" )//DDF0
		{}
		else
		{
			tvInsert.hInsertAfter = TVI_LAST;
			tvInsert.item.pszText = (LPSTR)(*iterDDF).m_strDDFName.c_str();
			hSubItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在MF下添加一个子结点DDF,节点添加在最后
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//ADF
		{
			if ( (*iterDDF).m_strFID == "" )//DDF0
				tvInsert.hParent = hTreeItem;//上级目录为MF
			else
				tvInsert.hParent = hSubItem;//上级目录为DDF
			if ( (*iterADF).m_strFID == "" )// DF00
			{}
			else
			{
				if ( (*iterDDF).m_strFID == "" )
					tvInsert.hInsertAfter = TVI_FIRST;
				else
					tvInsert.hInsertAfter = TVI_LAST;

				tvInsert.item.pszText = (LPSTR)(*iterADF).m_strFID.c_str();
				hSubItem1 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在上级目录下添加一个子结点ADF
			}

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//EFInfor
			{
				if ( (*iterDDF).m_strFID == "" && (*iterADF).m_strFID == "" )//DDF0/DF00
				{
					tvInsert.hParent = hTreeItem;//上级目录为MF
					tvInsert.hInsertAfter = TVI_FIRST;
				}
				else if ( (*iterDDF).m_strFID != "" && (*iterADF).m_strFID == "" )// DF00
				{
					tvInsert.hParent = hSubItem;//上级目录为DDF
					tvInsert.hInsertAfter = TVI_FIRST;
				}
				else
				{
					tvInsert.hParent = hSubItem1;//上级目录为ADF
					tvInsert.hInsertAfter = TVI_LAST;
				}

				tvInsert.item.pszText = (LPSTR)(*iterEFInfo).m_strSFI.c_str();
				hSubItem2 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在上级目录下添加一个子结点EF
				
				++iterEFInfo;
			}
			++iterADF;
		}
		++iterDDF;

		m_leftTreeView->ptheTree->Expand(hSubItem,TVE_EXPAND);//默认为合上的【TVE_COLLAPSE】
	}

	m_leftTreeView->ptheTree->Expand(hTreeItem,TVE_EXPAND);//默认为合上的【TVE_COLLAPSE】
}

void CMainFrame::OnUpdateOpenfile(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bSetting);//配置参数并保存后才可以做打开xml文件操作
}

void CMainFrame::OnDiscard()//点击生成发卡脚本事件
{
	stringstream strsLogs,strsTemp;
	strsLogs.str("");
	bool bFlag = false;//true 表示构建脚本成功
	m_bSetting = true;
	m_bCheXML = true;
	if ( cInformation == NULL )
		return;//已经释放了该对象
	if ( cInformation->Errors()	== 0 )//检测读取的XML数据无错误
	{
		//脚本类实例化
		CScript *cScript;
		switch ( m_nCOSType+1 )
		{
		case COSID_T514:
			cScript = new CScript_T514( m_nCOSType+1, cInformation );
			break;
		default:
			cScript = NULL;
			cInformation->Append_Error("无法识别的COS标志");
			PrintText("配置选择的芯片类型暂无发卡脚本生成,请期待后续开发！");
			break;
		}

		if ( cScript!=NULL )
		{
			cScript->ExchangeDatas( m_nCheckKeys, m_nCheckATR, m_strSearial, m_strTranKey, m_vecQCParam );
			if ( !cScript->IssueCardProcess() )//构建发卡脚本成功
			{
				//输出发卡脚本数据
				m_strsSript.str("");
				vector<string>::const_iterator iter3 = cInformation->m_vecScriptData.begin();
				while ( iter3 != cInformation->m_vecScriptData.end() )
				{
					m_strsSript<<*iter3<<endl;
					++iter3;
				}
				PrintText(m_strsSript.str());
				m_bScript = true;
				bFlag = true;
			}
			else
				m_bScript = false;
			if ( !bFlag )//构建脚本失败
			{
				//输出错误信息
				strsLogs<<"检测到的错误信息数为："<<cInformation->Errors()<<endl;
				PrintText(strsLogs.str(),0,1);
				strsTemp.str("");
				vector<string>::const_iterator iter1 = cInformation->m_vecErrData.begin();
				while ( iter1 != cInformation->m_vecErrData.end() )
				{
					strsLogs<<*iter1<<endl;
					strsTemp<<*iter1<<endl;
					++iter1;
				}
				strsLogs<<endl;
				strsTemp<<endl;
				PrintText(strsTemp.str(),1);
				//输出警告信息
				strsLogs<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
				strsTemp.str("");
				strsTemp<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
				PrintText(strsTemp.str(),1,2);
				strsTemp.str("");
				vector<string>::const_iterator iter2 = cInformation->m_vecWrnData.begin();
				while ( iter2 != cInformation->m_vecWrnData.end() )
				{
					strsLogs<<*iter2<<endl;
					strsTemp<<*iter2<<endl;
					++iter2;
				}
				PrintText(strsTemp.str(),1);
				if ( m_nCheckLog == 1 )
					WriteLog(strsLogs.str(),1);
			}

			delete cScript;
			delete cInformation;
			cInformation = NULL;
		}

	}

}

void CMainFrame::OnUpdateDiscard(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bCheXML);//检测XML文件无错误信息后才可以生成发卡脚本
}

void CMainFrame::OnSavefile()//点击保存发卡脚本事件
{
	// TODO: 在此添加命令处理程序代码
	Savefile("发卡脚本");
}

void CMainFrame::Savefile( CString strFileName )
{
	// TODO: 在此添加命令处理程序代码
	//定义一个CFileDialog对象生成一个保存文件对话框,缺省文件扩展名ibt,缺省文件名"发卡脚本"
	CFileDialog dlg(FALSE,"ibt",strFileName,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"ibt文件(*.ibt)|*.ibt||",AfxGetMainWnd());
	CString strFile;
	string strFilePath;
	if(dlg.DoModal() == IDOK)//点击对话框的确定按钮
	{
		strFile =dlg.GetPathName();//返回选定文件的完整路径
		strFilePath = strFile.GetBuffer();
		strFile.ReleaseBuffer();

		//判断是否有重名文件
		if (PathFileExists(strFile ))
		{
			//存在
			if (IDYES ==AfxMessageBox("文件已经存在是否要替换它?",MB_YESNO|MB_ICONQUESTION))
			{
				//存在相同文件名，替换
				SaveFile( strFilePath, m_strsSript.str() );
			}
			else
			{
				//存在相同文件名，不替换
			}
		}
		else
		{
			//不存在
			SaveFile( strFilePath, m_strsSript.str() );
		}
	}
}

void CMainFrame::OnUpdateSavefile(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bScript);//生成发卡脚本成功后才能保存脚本
}

void CMainFrame::OnInniteDatas_YK()
{
	m_bCheFixedKey = false;
	m_bChePSAM = false;
	m_bCheEncode = false;
	m_bCheXML_JMJ = false;
	m_bCheXML_GDMY = false;
	m_bSetting_YK = false;
	m_bScript_YK = false;
}

void CMainFrame::OnInniteDatas_FK()
{
	m_bSetting = false;
	m_bCheXML = false;
	m_bScript = false;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFrameWndEx::PreTranslateMessage(pMsg);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//分割窗口能够根据父窗口的大小自动调整
	if(m_isSplitter)
	{
		CRect crFram;
		GetClientRect(&crFram);
		m_wndSplitter.SetColumnInfo(0,crFram.Width()/5,10);
		m_wndSplitter.SetColumnInfo(1,crFram.Width()/5*4,10);
		m_wndSplitter.RecalcLayout();
	}
	
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	//静态切分视图为1行两列
	if(!m_wndSplitter.CreateStatic(this,1,2))
		return FALSE;
	m_isSplitter = TRUE;
	//绑定视图
	CRect	cRect;
	GetClientRect(&cRect);
	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CLeftPaneView),CSize(cRect.Width()/5,cRect.Height()),pContext))//0行0列
		return FALSE;
	if(!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CRightView),CSize(cRect.Width()/5*4,cRect.Height()),pContext))//0行1列
		return FALSE;
	//获取窗口对象
	m_leftTreeView = (CLeftPaneView*)m_wndSplitter.GetPane(0,0);
	m_pEditView = (CRightView*)m_wndSplitter.GetPane(0,1);
	
	return TRUE;
	//return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

void CMainFrame::PrintText( string str, int nMode/*=0*/, int nColoer/*=0 */ )
{
	//设置文字的样式
	CHARFORMAT cf; 
	ZeroMemory(&cf, sizeof(CHARFORMAT)); 
	cf.cbSize = sizeof(CHARFORMAT); 
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE; 
	switch(nColoer)//文字颜色
	{
	case 0:
		cf.yHeight = 230;//文字高度 
		cf.dwEffects = 0;
		cf.crTextColor = RGB(0, 0, 0); //黑色
		break;
	case 1:
		cf.yHeight = 280;
		cf.dwEffects = CFE_BOLD; //设置粗体 CFE_BOLD，取消用cf.dwEffects&=~CFE_BOLD;
		cf.crTextColor = RGB(255,0,0); //红色
		break;
	case 2:
		cf.yHeight = 280;
		cf.dwEffects = CFE_BOLD;
		cf.crTextColor = RGB(240, 150, 9); //黄色
		break;
	default:
		cf.yHeight = 230;
		cf.dwEffects = 0;
		cf.crTextColor = RGB(0, 0, 0); //黑色
		break;
	}
	strcpy_s(cf.szFaceName ,_T("幼圆"));//设置字体

	if ( nMode == 0 )
	{
		m_pEditView->pEditCtrl->SetDefaultCharFormat(cf); //设置区域的文字格式
		m_pEditView->SetWindowTextA((LPSTR)str.c_str());
	}
	else if ( nMode == 1 )
	{
		int nInsertionPoint=m_pEditView->pEditCtrl->GetWindowTextLength();
		m_pEditView->pEditCtrl->SetSel(nInsertionPoint, -1);  //定位到结尾位置
		m_pEditView->pEditCtrl->SetSelectionCharFormat(cf); //设置区域的文字格式
		m_pEditView->pEditCtrl->ReplaceSel(str.c_str());
	}
	m_pEditView->pEditCtrl->SetReadOnly(TRUE);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( nIDEvent== 1 )
	{
		CString strTime,strTime1;   
		// 获取系统当前时间，并保存到curTime   
		CTime curTime = CTime::GetCurrentTime();   
		// 格式化curTime，将字符串保存到strTime   
		strTime = curTime.Format(_T("%H:%M:%S")); 

		CMFCRibbonStatusBarPane* pStatus1 = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_PANE2));
		strTime1.Format(" 当前时间：%s ",strTime);
		// 在状态栏的时间窗格中显示系统时间字符串   
		pStatus1->SetText(strTime1); 
		pStatus1->Redraw();
	}  

	CFrameWndEx::OnTimer(nIDEvent);
}

// 限制窗口的大小
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x=600; //限定最小宽度
	lpMMI->ptMinTrackSize.y=600; //限定最小高度
	// lpMMI->ptMaxTrackSize.x=100; //限定最大宽度
	// lpMMI->ptMaxTrackSize.y=100; //限定最大高度

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::GetProInfo( stringstream &strsDatas )
{
	strsDatas.str("");
	strsDatas<<"项目名称:"<<S_SETTING.m_struProjInfo.m_strName<<"   项目描述:"<<S_SETTING.m_struProjInfo.m_strDesc<<endl;
	vector<PROJMOD>::const_iterator iterProj =S_SETTING.m_struProjInfo.m_vecProjMod.begin();
	while(iterProj!=S_SETTING.m_struProjInfo.m_vecProjMod.end())
	{
		strsDatas<<"	作者:"<<(*iterProj).m_strAuthor<<"   修改备注信息:"<<(*iterProj).m_strModify
			<<"   修改日期:"<<(*iterProj).m_strDate<<"   版本号:"<<(*iterProj).m_strVer<<endl;
		++iterProj;
	}
	strsDatas<<"城市代码:"<<S_SETTING.m_struCityCode.m_strValue<<"   省:"<<S_SETTING.m_struCityCode.m_strProvince<<"   市:"<<S_SETTING.m_struCityCode.m_strCity<<endl;
	strsDatas<<endl;
}

void CMainFrame::OnLoadXML_YK()//打开验卡配置文件
{
	// TODO: 在此添加命令处理程序代码
	stringstream strsLogs,strsTemp,strsProInfo;
	string strLoadKey;//装载密钥的方式
	strsLogs.str("");
	strsProInfo.str("");

	// TODO: 在此添加命令处理程序代码
	stringstream strsXmlPath;
	strsXmlPath.str("");
	//定义一个CFileDialog对象生成一个打开文件对话框
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"xml文件(*.xml)|*.xml||",AfxGetMainWnd());
	CString strXmlPath;
	if(dlg.DoModal() == IDOK)//点击对话框的确定按钮
	{
		OnInniteDatas_FK();
		m_pEditView->SetWindowTextA("");
		m_leftTreeView->ptheTree->DeleteAllItems();
		strXmlPath=dlg.GetPathName();//取得选中要打开的文件路径
		strsXmlPath << strXmlPath;
		m_bSetting_YK = false;
		m_bScript_YK = false;

		//信息类实例化
		if ( cInformation != NULL )
		{
			delete cInformation;
			cInformation = NULL;
		}
		cInformation = new CInformation();
		if ( cInformation->ReadXMLDatas_YK(strsXmlPath.str(),strLoadKey,m_nCheckLog) )//读取XML数据
			cInformation->CheckXmlDatas();//是脚本配置文件的时候检测读取的数据信息

		if ( cInformation->Errors()	== 0 )//检测读取的XML数据无错误
		{
			ChangeXMLTreeDatas_YK();
			GetProInfo(strsProInfo);
			PrintText(strsProInfo.str());
		}

		//输出错误信息
		strsLogs<<"检测到的错误信息数为："<<cInformation->Errors()<<endl;
		if ( cInformation->Errors()	== 0 )	
			PrintText(strsLogs.str(),1,1);
		else
			PrintText(strsLogs.str(),0,1);
		strsTemp.str("");
		vector<string>::const_iterator iter1 = cInformation->m_vecErrData.begin();
		while ( iter1 != cInformation->m_vecErrData.end() )
		{
			strsLogs<<*iter1<<endl;
			strsTemp<<*iter1<<endl;
			++iter1;
		}
		strsLogs<<endl;
		strsTemp<<endl;
		PrintText(strsTemp.str(),1);
		//输出警告信息
		strsLogs<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
		strsTemp.str("");
		strsTemp<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
		PrintText(strsTemp.str(),1,2);
		strsTemp.str("");
		vector<string>::const_iterator iter2 = cInformation->m_vecWrnData.begin();
		while ( iter2 != cInformation->m_vecWrnData.end() )
		{
			strsLogs<<*iter2<<endl;
			strsTemp<<*iter2<<endl;
			++iter2;
		}
		PrintText(strsTemp.str(),1);

		if ( cInformation->Errors()	== 0 )//检测读取的XML数据无错误
		{
			if ( strLoadKey == "1" )//XML里面配置加密机装载，界面上可以选择加密机或PSAM验卡
			{
				OnCheckEncoder();
				m_bCheXML_JMJ = true;
				m_bCheXML_GDMY = false;
			}
			else if ( strLoadKey == "2" )//XML里面配置固定密钥装载，界面上只能选择固定密钥验卡
			{
				OnCheckFixedKey();
				m_bCheXML_JMJ = false;
				m_bCheXML_GDMY = true;
			}
		}
		else
		{
			m_bCheXML_GDMY = false;
			m_bCheXML_JMJ = false;
			m_bCheFixedKey = false;
			m_bChePSAM = false;
			m_bCheEncode = false;
			if ( m_nCheckLog == 1 )
				WriteLog(strsLogs.str(),1);
			delete cInformation;
			cInformation = NULL;
		}

	}
}

void CMainFrame::ChangeXMLTreeDatas_YK()
{
	m_leftTreeView->ptheTree->ModifyStyle(0,TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);
	//改变树结构的数据
	TVINSERTSTRUCT tvInsert;//父项的句柄。如果这个成员的值是TVI_ROOT或NULL，这项将被作为树形控件的根插入
	HTREEITEM hTreeItem,hSubItem,hSubItem1,hSubItem2;
	//插入的新项之后的项的句柄 TVI_FIRST在列表的开始插入项 TVI_LAST在列表的最后插入项 TVI_ROOT作为一个根项添加 TVI_SORT以字母顺序插入项
	tvInsert.hInsertAfter = NULL;
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = "SSSE";
	hTreeItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在根节点添加SSSE

	//遍历SSSE下的密钥数据并显示成树形结构
	vector<KEYS>::const_iterator iterKeys = V_EKEYS.begin();
	while ( iterKeys!=V_EKEYS.end() )
	{
		if ( (*iterKeys).m_strDDFName != "SSSE" )
		{
			++iterKeys;
			continue;
		}
		vector<ADFKEY>::const_iterator iterKey = (*iterKeys).m_vecADFKEY.begin();
		while ( iterKey!=(*iterKeys).m_vecADFKEY.end() )
		{
			tvInsert.hParent = hTreeItem;
			if ( (*iterKey).m_strADFName != "DF00" )
			{
				tvInsert.hInsertAfter = TVI_LAST;
				tvInsert.item.pszText = (LPSTR)(*iterKey).m_strADFName.c_str();
				hSubItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在SSSE下添加一个子结点ADF,节点添加在最后
			}
			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				if ( (*iterKey).m_strADFName == "DF00" )//直接在SSSE下添加密钥节点
				{
					tvInsert.hInsertAfter = TVI_FIRST;
					tvInsert.item.pszText = (LPSTR)(*iterKeyDatas).m_strName.c_str();
					hSubItem1 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在SSSE下添加一个密钥子结点,节点添加在前面
				}
				else
				{
					tvInsert.hParent = hSubItem;
					tvInsert.hInsertAfter = TVI_LAST;
					tvInsert.item.pszText = (LPSTR)(*iterKeyDatas).m_strName.c_str();
					hSubItem2 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//在ADF节点下添加一个密钥子结点,节点添加在后面
				}
				++iterKeyDatas;
			}
			++iterKey;
		}

		break;
	}

	m_leftTreeView->ptheTree->Expand(hTreeItem,TVE_EXPAND);//默认为合上的【TVE_COLLAPSE】
}

void CMainFrame::OnCheckFixedKey()//配置固定密钥方式
{
	// TODO: 在此添加命令处理程序代码
	m_bCheFixedKey = true;
	m_bChePSAM = false;
	m_bCheEncode = false;
	m_bSetting_YK = true;
	m_nYKStyle = 0;
}

void CMainFrame::OnUpdateCheckFixedKey(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bCheFixedKey);
	pCmdUI->Enable(m_bCheXML_GDMY);
}

void CMainFrame::OnCheckEncoder()//配置加密机方式
{
	// TODO: 在此添加命令处理程序代码
	m_bCheFixedKey = false;
	m_bChePSAM = false;
	m_bCheEncode = true;
	m_bSetting_YK = true;
	m_nYKStyle = 1;
}

void CMainFrame::OnUpdateCheckEncoder(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bCheEncode);
	pCmdUI->Enable(m_bCheXML_JMJ);
}

void CMainFrame::OnCheckPSAM()//配置PSAM卡方式
{
	// TODO: 在此添加命令处理程序代码
	m_bCheFixedKey = false;
	m_bChePSAM = true;
	m_bCheEncode = false;
	m_bSetting_YK = true;
	m_nYKStyle = 2;
}

void CMainFrame::OnUpdateCheckPSAM(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bChePSAM);
	pCmdUI->Enable(m_bCheXML_JMJ);
}

void CMainFrame::OnDiscard_YK()//生成验卡脚本
{
	// TODO: 在此添加命令处理程序代码
	stringstream strsLogs,strsTemp;
	strsLogs.str("");
	bool bFlag = false;//true 表示构建脚本成功
	if ( cInformation == NULL )
		return;//已经释放了该对象
	if ( cInformation->Errors()	== 0 )//检测读取的XML数据无错误
	{
		//脚本类实例化
		CScript_YK *cScript_YK = new CScript_YK(cInformation);
		cInformation->Clear_Script();
		if ( !cScript_YK->IssueCardProcess(m_nYKStyle) )//构建验卡脚本成功
		{
			//输出验卡脚本数据
			m_strsSript.str("");
			vector<string>::const_iterator iter3 = cInformation->m_vecScriptData.begin();
			while ( iter3 != cInformation->m_vecScriptData.end() )
			{
				m_strsSript<<*iter3<<endl;
				++iter3;
			}
			PrintText(m_strsSript.str());
			m_bScript_YK = true;
			bFlag = true;
		}
		else
			m_bScript_YK = false;
		if ( !bFlag )//构建脚本失败
		{
			//输出错误信息
			strsLogs<<"检测到的错误信息数为："<<cInformation->Errors()<<endl;
			PrintText(strsLogs.str(),0,1);
			strsTemp.str("");
			vector<string>::const_iterator iter1 = cInformation->m_vecErrData.begin();
			while ( iter1 != cInformation->m_vecErrData.end() )
			{
				strsLogs<<*iter1<<endl;
				strsTemp<<*iter1<<endl;
				++iter1;
			}
			strsLogs<<endl;
			strsTemp<<endl;
			PrintText(strsTemp.str(),1);
			//输出警告信息
			strsLogs<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
			strsTemp.str("");
			strsTemp<<"检测到的警告信息数为："<<cInformation->Warnings()<<endl;
			PrintText(strsTemp.str(),1,2);
			strsTemp.str("");
			vector<string>::const_iterator iter2 = cInformation->m_vecWrnData.begin();
			while ( iter2 != cInformation->m_vecWrnData.end() )
			{
				strsLogs<<*iter2<<endl;
				strsTemp<<*iter2<<endl;
				++iter2;
			}
			PrintText(strsTemp.str(),1);
			if ( m_nCheckLog == 1 )
				WriteLog(strsLogs.str(),1);
		}

		delete cScript_YK;

	}
}


void CMainFrame::OnUpdateDiscard_YK(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bSetting_YK);
}


void CMainFrame::OnSavefile_YK()
{
	// TODO: 在此添加命令处理程序代码
	Savefile("验卡脚本");
}


void CMainFrame::OnUpdateSavefile_YK(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(m_bScript_YK);
}
