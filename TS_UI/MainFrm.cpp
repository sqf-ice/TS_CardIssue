// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// MainFrm.cpp : CMainFrame ���ʵ��
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

// CMainFrame ����/����

CMainFrame::CMainFrame():m_isSplitter(FALSE),cInformation(NULL)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	char szPath[50] = "c:\\TS_Logs\\";
	RemoveLOG(szPath,15);//ɾ��15��֮ǰ����־�ļ�
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

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);//����ͼ�� �̳�������д�÷����������ε��ù���

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, _T(" ��ǰʱ�䣺00:00:00 "), TRUE), _T("ʱ��"));
	// ������ʱ������ʱ��IDΪ1����ʱʱ��Ϊ1000ms����1s   
	SetTimer(1, 1000, NULL); 

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	m_MainButton = new CMyRibbonApplicationButton;
	m_MainButton->SetImage(IDB_MAIN);//��ʾӦ�ð�ť
	m_wndRibbonBar.SetApplicationButton(m_MainButton,CSize(50,50));//����Ӧ�ð�ť���������ܲ˵�
	
	OnInniteDatas_YK();
	OnInniteDatas_FK();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style &= ~FWS_ADDTOTITLE;//�����޸Ĵ��ڱ���
	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

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

void CMainFrame::OnSetting()//������÷��������¼�
{
	// TODO: �ڴ���������������
	CMainDlg cSetDlg;
	if ( IDOK == cSetDlg.DoModal() )//�رնԻ��򲢱���������
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

void CMainFrame::OnOpenfile()//����򿪶�ȡXML�����¼�
{
	stringstream strsLogs,strsTemp,strsProInfo;
	strsLogs.str("");
	strsProInfo.str("");

	// TODO: �ڴ���������������
	stringstream strsXmlPath;
	strsXmlPath.str("");
	//����һ��CFileDialog��������һ�����ļ��Ի���
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"xml�ļ�(*.xml)|*.xml||",AfxGetMainWnd());
	//CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"All Files(*.*)|*.*|Worksheet Files (*.xls)|*.xls|Data Files (*.xlc;*.xls)|*.xlc; *.xls||",AfxGetMainWnd());
	CString strXmlPath;
	if(dlg.DoModal() == IDOK)//����Ի����ȷ����ť
	{
		m_pEditView->SetWindowTextA("");
		m_leftTreeView->ptheTree->DeleteAllItems();
		strXmlPath=dlg.GetPathName();//ȡ��ѡ��Ҫ�򿪵��ļ�·��
		strsXmlPath << strXmlPath;
		m_bSetting = true;
		m_bScript = false;

		//��Ϣ��ʵ����
		if ( cInformation != NULL )
		{
			delete cInformation;
			cInformation = NULL;
		}
		cInformation = new CInformation();
		if ( cInformation->ReadXMLDatas(strsXmlPath.str(),m_nCheckLog) )//��ȡXML����
			cInformation->CheckXmlDatas();//�ǽű������ļ���ʱ�����ȡ��������Ϣ

		if ( cInformation->Errors()	== 0 )//����ȡ��XML�����޴���
		{
			ChangeXMLTreeDatas();
			GetProInfo(strsProInfo);
			PrintText(strsProInfo.str());
		}

		//���������Ϣ
		strsLogs<<"��⵽�Ĵ�����Ϣ��Ϊ��"<<cInformation->Errors()<<endl;
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
		//���������Ϣ
		strsLogs<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
		strsTemp.str("");
		strsTemp<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
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

		if ( cInformation->Errors()	== 0 )//����ȡ��XML�����޴���
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
	//�ı����ṹ������
	TVINSERTSTRUCT tvInsert;//����ľ������������Ա��ֵ��TVI_ROOT��NULL���������Ϊ���οؼ��ĸ�����
	HTREEITEM hTreeItem,hSubItem,hSubItem1,hSubItem2;
	//���������֮�����ľ�� TVI_FIRST���б�Ŀ�ʼ������ TVI_LAST���б���������� TVI_ROOT��Ϊһ��������� TVI_SORT����ĸ˳�������
	tvInsert.hInsertAfter = NULL;
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = "MF";
	hTreeItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//�ڸ��ڵ����MF

	vector<DDFINFO>::const_iterator iterDDF = S_CARDNFO.m_vecDDFInfo.begin();
	while ( iterDDF!=S_CARDNFO.m_vecDDFInfo.end() )//DDF
	{
		tvInsert.hParent = hTreeItem;//�ϼ�Ŀ¼ΪMF
		if ( (*iterDDF).m_strFID == "" )//DDF0
		{}
		else
		{
			tvInsert.hInsertAfter = TVI_LAST;
			tvInsert.item.pszText = (LPSTR)(*iterDDF).m_strDDFName.c_str();
			hSubItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//��MF�����һ���ӽ��DDF,�ڵ���������
		}

		vector<ADFINFO>::const_iterator iterADF = (*iterDDF).m_vecADFInfo.begin();
		while ( iterADF!=(*iterDDF).m_vecADFInfo.end() )//ADF
		{
			if ( (*iterDDF).m_strFID == "" )//DDF0
				tvInsert.hParent = hTreeItem;//�ϼ�Ŀ¼ΪMF
			else
				tvInsert.hParent = hSubItem;//�ϼ�Ŀ¼ΪDDF
			if ( (*iterADF).m_strFID == "" )// DF00
			{}
			else
			{
				if ( (*iterDDF).m_strFID == "" )
					tvInsert.hInsertAfter = TVI_FIRST;
				else
					tvInsert.hInsertAfter = TVI_LAST;

				tvInsert.item.pszText = (LPSTR)(*iterADF).m_strFID.c_str();
				hSubItem1 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//���ϼ�Ŀ¼�����һ���ӽ��ADF
			}

			vector<EFINFO>::const_iterator iterEFInfo = (*iterADF).m_vecEFInfo.begin();
			while ( iterEFInfo!=(*iterADF).m_vecEFInfo.end() )//EFInfor
			{
				if ( (*iterDDF).m_strFID == "" && (*iterADF).m_strFID == "" )//DDF0/DF00
				{
					tvInsert.hParent = hTreeItem;//�ϼ�Ŀ¼ΪMF
					tvInsert.hInsertAfter = TVI_FIRST;
				}
				else if ( (*iterDDF).m_strFID != "" && (*iterADF).m_strFID == "" )// DF00
				{
					tvInsert.hParent = hSubItem;//�ϼ�Ŀ¼ΪDDF
					tvInsert.hInsertAfter = TVI_FIRST;
				}
				else
				{
					tvInsert.hParent = hSubItem1;//�ϼ�Ŀ¼ΪADF
					tvInsert.hInsertAfter = TVI_LAST;
				}

				tvInsert.item.pszText = (LPSTR)(*iterEFInfo).m_strSFI.c_str();
				hSubItem2 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//���ϼ�Ŀ¼�����һ���ӽ��EF
				
				++iterEFInfo;
			}
			++iterADF;
		}
		++iterDDF;

		m_leftTreeView->ptheTree->Expand(hSubItem,TVE_EXPAND);//Ĭ��Ϊ���ϵġ�TVE_COLLAPSE��
	}

	m_leftTreeView->ptheTree->Expand(hTreeItem,TVE_EXPAND);//Ĭ��Ϊ���ϵġ�TVE_COLLAPSE��
}

void CMainFrame::OnUpdateOpenfile(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bSetting);//���ò����������ſ�������xml�ļ�����
}

void CMainFrame::OnDiscard()//������ɷ����ű��¼�
{
	stringstream strsLogs,strsTemp;
	strsLogs.str("");
	bool bFlag = false;//true ��ʾ�����ű��ɹ�
	m_bSetting = true;
	m_bCheXML = true;
	if ( cInformation == NULL )
		return;//�Ѿ��ͷ��˸ö���
	if ( cInformation->Errors()	== 0 )//����ȡ��XML�����޴���
	{
		//�ű���ʵ����
		CScript *cScript;
		switch ( m_nCOSType+1 )
		{
		case COSID_T514:
			cScript = new CScript_T514( m_nCOSType+1, cInformation );
			break;
		default:
			cScript = NULL;
			cInformation->Append_Error("�޷�ʶ���COS��־");
			PrintText("����ѡ���оƬ�������޷����ű�����,���ڴ�����������");
			break;
		}

		if ( cScript!=NULL )
		{
			cScript->ExchangeDatas( m_nCheckKeys, m_nCheckATR, m_strSearial, m_strTranKey, m_vecQCParam );
			if ( !cScript->IssueCardProcess() )//���������ű��ɹ�
			{
				//��������ű�����
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
			if ( !bFlag )//�����ű�ʧ��
			{
				//���������Ϣ
				strsLogs<<"��⵽�Ĵ�����Ϣ��Ϊ��"<<cInformation->Errors()<<endl;
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
				//���������Ϣ
				strsLogs<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
				strsTemp.str("");
				strsTemp<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
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
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bCheXML);//���XML�ļ��޴�����Ϣ��ſ������ɷ����ű�
}

void CMainFrame::OnSavefile()//������淢���ű��¼�
{
	// TODO: �ڴ���������������
	Savefile("�����ű�");
}

void CMainFrame::Savefile( CString strFileName )
{
	// TODO: �ڴ���������������
	//����һ��CFileDialog��������һ�������ļ��Ի���,ȱʡ�ļ���չ��ibt,ȱʡ�ļ���"�����ű�"
	CFileDialog dlg(FALSE,"ibt",strFileName,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"ibt�ļ�(*.ibt)|*.ibt||",AfxGetMainWnd());
	CString strFile;
	string strFilePath;
	if(dlg.DoModal() == IDOK)//����Ի����ȷ����ť
	{
		strFile =dlg.GetPathName();//����ѡ���ļ�������·��
		strFilePath = strFile.GetBuffer();
		strFile.ReleaseBuffer();

		//�ж��Ƿ��������ļ�
		if (PathFileExists(strFile ))
		{
			//����
			if (IDYES ==AfxMessageBox("�ļ��Ѿ������Ƿ�Ҫ�滻��?",MB_YESNO|MB_ICONQUESTION))
			{
				//������ͬ�ļ������滻
				SaveFile( strFilePath, m_strsSript.str() );
			}
			else
			{
				//������ͬ�ļ��������滻
			}
		}
		else
		{
			//������
			SaveFile( strFilePath, m_strsSript.str() );
		}
	}
}

void CMainFrame::OnUpdateSavefile(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bScript);//���ɷ����ű��ɹ�����ܱ���ű�
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
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWndEx::PreTranslateMessage(pMsg);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//�ָ���ܹ����ݸ����ڵĴ�С�Զ�����
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
	// TODO: �ڴ����ר�ô����/����û���

	//��̬�з���ͼΪ1������
	if(!m_wndSplitter.CreateStatic(this,1,2))
		return FALSE;
	m_isSplitter = TRUE;
	//����ͼ
	CRect	cRect;
	GetClientRect(&cRect);
	if(!m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CLeftPaneView),CSize(cRect.Width()/5,cRect.Height()),pContext))//0��0��
		return FALSE;
	if(!m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CRightView),CSize(cRect.Width()/5*4,cRect.Height()),pContext))//0��1��
		return FALSE;
	//��ȡ���ڶ���
	m_leftTreeView = (CLeftPaneView*)m_wndSplitter.GetPane(0,0);
	m_pEditView = (CRightView*)m_wndSplitter.GetPane(0,1);
	
	return TRUE;
	//return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

void CMainFrame::PrintText( string str, int nMode/*=0*/, int nColoer/*=0 */ )
{
	//�������ֵ���ʽ
	CHARFORMAT cf; 
	ZeroMemory(&cf, sizeof(CHARFORMAT)); 
	cf.cbSize = sizeof(CHARFORMAT); 
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE; 
	switch(nColoer)//������ɫ
	{
	case 0:
		cf.yHeight = 230;//���ָ߶� 
		cf.dwEffects = 0;
		cf.crTextColor = RGB(0, 0, 0); //��ɫ
		break;
	case 1:
		cf.yHeight = 280;
		cf.dwEffects = CFE_BOLD; //���ô��� CFE_BOLD��ȡ����cf.dwEffects&=~CFE_BOLD;
		cf.crTextColor = RGB(255,0,0); //��ɫ
		break;
	case 2:
		cf.yHeight = 280;
		cf.dwEffects = CFE_BOLD;
		cf.crTextColor = RGB(240, 150, 9); //��ɫ
		break;
	default:
		cf.yHeight = 230;
		cf.dwEffects = 0;
		cf.crTextColor = RGB(0, 0, 0); //��ɫ
		break;
	}
	strcpy_s(cf.szFaceName ,_T("��Բ"));//��������

	if ( nMode == 0 )
	{
		m_pEditView->pEditCtrl->SetDefaultCharFormat(cf); //������������ָ�ʽ
		m_pEditView->SetWindowTextA((LPSTR)str.c_str());
	}
	else if ( nMode == 1 )
	{
		int nInsertionPoint=m_pEditView->pEditCtrl->GetWindowTextLength();
		m_pEditView->pEditCtrl->SetSel(nInsertionPoint, -1);  //��λ����βλ��
		m_pEditView->pEditCtrl->SetSelectionCharFormat(cf); //������������ָ�ʽ
		m_pEditView->pEditCtrl->ReplaceSel(str.c_str());
	}
	m_pEditView->pEditCtrl->SetReadOnly(TRUE);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( nIDEvent== 1 )
	{
		CString strTime,strTime1;   
		// ��ȡϵͳ��ǰʱ�䣬�����浽curTime   
		CTime curTime = CTime::GetCurrentTime();   
		// ��ʽ��curTime�����ַ������浽strTime   
		strTime = curTime.Format(_T("%H:%M:%S")); 

		CMFCRibbonStatusBarPane* pStatus1 = DYNAMIC_DOWNCAST(CMFCRibbonStatusBarPane, m_wndStatusBar.FindElement(ID_STATUSBAR_PANE2));
		strTime1.Format(" ��ǰʱ�䣺%s ",strTime);
		// ��״̬����ʱ�䴰������ʾϵͳʱ���ַ���   
		pStatus1->SetText(strTime1); 
		pStatus1->Redraw();
	}  

	CFrameWndEx::OnTimer(nIDEvent);
}

// ���ƴ��ڵĴ�С
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI->ptMinTrackSize.x=600; //�޶���С���
	lpMMI->ptMinTrackSize.y=600; //�޶���С�߶�
	// lpMMI->ptMaxTrackSize.x=100; //�޶������
	// lpMMI->ptMaxTrackSize.y=100; //�޶����߶�

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::GetProInfo( stringstream &strsDatas )
{
	strsDatas.str("");
	strsDatas<<"��Ŀ����:"<<S_SETTING.m_struProjInfo.m_strName<<"   ��Ŀ����:"<<S_SETTING.m_struProjInfo.m_strDesc<<endl;
	vector<PROJMOD>::const_iterator iterProj =S_SETTING.m_struProjInfo.m_vecProjMod.begin();
	while(iterProj!=S_SETTING.m_struProjInfo.m_vecProjMod.end())
	{
		strsDatas<<"	����:"<<(*iterProj).m_strAuthor<<"   �޸ı�ע��Ϣ:"<<(*iterProj).m_strModify
			<<"   �޸�����:"<<(*iterProj).m_strDate<<"   �汾��:"<<(*iterProj).m_strVer<<endl;
		++iterProj;
	}
	strsDatas<<"���д���:"<<S_SETTING.m_struCityCode.m_strValue<<"   ʡ:"<<S_SETTING.m_struCityCode.m_strProvince<<"   ��:"<<S_SETTING.m_struCityCode.m_strCity<<endl;
	strsDatas<<endl;
}

void CMainFrame::OnLoadXML_YK()//���鿨�����ļ�
{
	// TODO: �ڴ���������������
	stringstream strsLogs,strsTemp,strsProInfo;
	string strLoadKey;//װ����Կ�ķ�ʽ
	strsLogs.str("");
	strsProInfo.str("");

	// TODO: �ڴ���������������
	stringstream strsXmlPath;
	strsXmlPath.str("");
	//����һ��CFileDialog��������һ�����ļ��Ի���
	CFileDialog dlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST |OFN_HIDEREADONLY,"xml�ļ�(*.xml)|*.xml||",AfxGetMainWnd());
	CString strXmlPath;
	if(dlg.DoModal() == IDOK)//����Ի����ȷ����ť
	{
		OnInniteDatas_FK();
		m_pEditView->SetWindowTextA("");
		m_leftTreeView->ptheTree->DeleteAllItems();
		strXmlPath=dlg.GetPathName();//ȡ��ѡ��Ҫ�򿪵��ļ�·��
		strsXmlPath << strXmlPath;
		m_bSetting_YK = false;
		m_bScript_YK = false;

		//��Ϣ��ʵ����
		if ( cInformation != NULL )
		{
			delete cInformation;
			cInformation = NULL;
		}
		cInformation = new CInformation();
		if ( cInformation->ReadXMLDatas_YK(strsXmlPath.str(),strLoadKey,m_nCheckLog) )//��ȡXML����
			cInformation->CheckXmlDatas();//�ǽű������ļ���ʱ�����ȡ��������Ϣ

		if ( cInformation->Errors()	== 0 )//����ȡ��XML�����޴���
		{
			ChangeXMLTreeDatas_YK();
			GetProInfo(strsProInfo);
			PrintText(strsProInfo.str());
		}

		//���������Ϣ
		strsLogs<<"��⵽�Ĵ�����Ϣ��Ϊ��"<<cInformation->Errors()<<endl;
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
		//���������Ϣ
		strsLogs<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
		strsTemp.str("");
		strsTemp<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
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

		if ( cInformation->Errors()	== 0 )//����ȡ��XML�����޴���
		{
			if ( strLoadKey == "1" )//XML�������ü��ܻ�װ�أ������Ͽ���ѡ����ܻ���PSAM�鿨
			{
				OnCheckEncoder();
				m_bCheXML_JMJ = true;
				m_bCheXML_GDMY = false;
			}
			else if ( strLoadKey == "2" )//XML�������ù̶���Կװ�أ�������ֻ��ѡ��̶���Կ�鿨
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
	//�ı����ṹ������
	TVINSERTSTRUCT tvInsert;//����ľ������������Ա��ֵ��TVI_ROOT��NULL���������Ϊ���οؼ��ĸ�����
	HTREEITEM hTreeItem,hSubItem,hSubItem1,hSubItem2;
	//���������֮�����ľ�� TVI_FIRST���б�Ŀ�ʼ������ TVI_LAST���б���������� TVI_ROOT��Ϊһ��������� TVI_SORT����ĸ˳�������
	tvInsert.hInsertAfter = NULL;
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.mask = TVIF_TEXT;
	tvInsert.item.pszText = "SSSE";
	hTreeItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//�ڸ��ڵ����SSSE

	//����SSSE�µ���Կ���ݲ���ʾ�����νṹ
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
				hSubItem = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//��SSSE�����һ���ӽ��ADF,�ڵ���������
			}
			vector<KEYDATAS>::const_iterator iterKeyDatas = (*iterKey).m_vecKeyDatas.begin();
			while ( iterKeyDatas!=(*iterKey).m_vecKeyDatas.end() )
			{
				if ( (*iterKey).m_strADFName == "DF00" )//ֱ����SSSE�������Կ�ڵ�
				{
					tvInsert.hInsertAfter = TVI_FIRST;
					tvInsert.item.pszText = (LPSTR)(*iterKeyDatas).m_strName.c_str();
					hSubItem1 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//��SSSE�����һ����Կ�ӽ��,�ڵ������ǰ��
				}
				else
				{
					tvInsert.hParent = hSubItem;
					tvInsert.hInsertAfter = TVI_LAST;
					tvInsert.item.pszText = (LPSTR)(*iterKeyDatas).m_strName.c_str();
					hSubItem2 = m_leftTreeView->ptheTree->InsertItem(&tvInsert);//��ADF�ڵ������һ����Կ�ӽ��,�ڵ�����ں���
				}
				++iterKeyDatas;
			}
			++iterKey;
		}

		break;
	}

	m_leftTreeView->ptheTree->Expand(hTreeItem,TVE_EXPAND);//Ĭ��Ϊ���ϵġ�TVE_COLLAPSE��
}

void CMainFrame::OnCheckFixedKey()//���ù̶���Կ��ʽ
{
	// TODO: �ڴ���������������
	m_bCheFixedKey = true;
	m_bChePSAM = false;
	m_bCheEncode = false;
	m_bSetting_YK = true;
	m_nYKStyle = 0;
}

void CMainFrame::OnUpdateCheckFixedKey(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bCheFixedKey);
	pCmdUI->Enable(m_bCheXML_GDMY);
}

void CMainFrame::OnCheckEncoder()//���ü��ܻ���ʽ
{
	// TODO: �ڴ���������������
	m_bCheFixedKey = false;
	m_bChePSAM = false;
	m_bCheEncode = true;
	m_bSetting_YK = true;
	m_nYKStyle = 1;
}

void CMainFrame::OnUpdateCheckEncoder(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bCheEncode);
	pCmdUI->Enable(m_bCheXML_JMJ);
}

void CMainFrame::OnCheckPSAM()//����PSAM����ʽ
{
	// TODO: �ڴ���������������
	m_bCheFixedKey = false;
	m_bChePSAM = true;
	m_bCheEncode = false;
	m_bSetting_YK = true;
	m_nYKStyle = 2;
}

void CMainFrame::OnUpdateCheckPSAM(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bChePSAM);
	pCmdUI->Enable(m_bCheXML_JMJ);
}

void CMainFrame::OnDiscard_YK()//�����鿨�ű�
{
	// TODO: �ڴ���������������
	stringstream strsLogs,strsTemp;
	strsLogs.str("");
	bool bFlag = false;//true ��ʾ�����ű��ɹ�
	if ( cInformation == NULL )
		return;//�Ѿ��ͷ��˸ö���
	if ( cInformation->Errors()	== 0 )//����ȡ��XML�����޴���
	{
		//�ű���ʵ����
		CScript_YK *cScript_YK = new CScript_YK(cInformation);
		cInformation->Clear_Script();
		if ( !cScript_YK->IssueCardProcess(m_nYKStyle) )//�����鿨�ű��ɹ�
		{
			//����鿨�ű�����
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
		if ( !bFlag )//�����ű�ʧ��
		{
			//���������Ϣ
			strsLogs<<"��⵽�Ĵ�����Ϣ��Ϊ��"<<cInformation->Errors()<<endl;
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
			//���������Ϣ
			strsLogs<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
			strsTemp.str("");
			strsTemp<<"��⵽�ľ�����Ϣ��Ϊ��"<<cInformation->Warnings()<<endl;
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
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bSetting_YK);
}


void CMainFrame::OnSavefile_YK()
{
	// TODO: �ڴ���������������
	Savefile("�鿨�ű�");
}


void CMainFrame::OnUpdateSavefile_YK(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->Enable(m_bScript_YK);
}
