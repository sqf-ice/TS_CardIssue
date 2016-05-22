// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TS_UI.h"
#include "MainDlg.h"
#include "afxdialogex.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_cMainTab = NULL;
	m_cKeyConf = NULL;
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_cTabctrl);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMainDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序


BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HICON   hIcon=AfxGetApp()->LoadIcon(IDI_ICON1); //加载图标
	SetIcon(hIcon,FALSE);//参数为FALSE表示设置窗口上的小图标

	m_cImagelistTab.Create(IDB_BITMAP2, 18, 0, RGB(255,255,255));
	m_cTabctrl.SetImageList(&m_cImagelistTab);

	m_cMainTab = new CMainTabDlg();
	m_cMainTab->Create(CMainTabDlg::IDD, &m_cTabctrl);
	m_cTabctrl.AddTab(m_cMainTab, "配置选项", 0);
	m_cMainTab->m_cMyTab = &m_cTabctrl;

	m_cKeyConf = new CSettingDlg();
	m_cKeyConf->Create(CSettingDlg::IDD, &m_cTabctrl);
	m_cTabctrl.AddTab(m_cKeyConf, "配置清卡参数", 1);

	m_cTabctrl.SetSelectedColor(RGB(255, 117, 117));
	m_cTabctrl.SetMouseOverColor(RGB(255, 255, 255));
	m_cTabctrl.SetNormalColor(RGB(0, 50, 255));
	m_cTabctrl.EnableTab(1, m_cMainTab->m_bCheckKeys);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_cMainTab)
		delete m_cMainTab;
	if (m_cKeyConf)
		delete m_cKeyConf;
}



BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	//屏蔽回车键和Esc按键消息
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return TRUE;    
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE; 

	return CDialog::PreTranslateMessage(pMsg);
}


void CMainDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if ( IDOK == ::MessageBox( m_hWnd, "退出之前是否要保存当前的配置", "退出提示", MB_OK||MB_OKCANCEL )  )
	{
		bool bfalg1=true,bfalg2=true;//标记选项卡中数据是否有错误";

		bfalg1 = m_cMainTab->SaveOption(m_nCheckKeys, m_nCheckATR, m_nCOS, m_nCheckLog, m_strSearial, m_strTanKey);
		if ( m_nCheckKeys )//需要清卡
			bfalg2 = m_cKeyConf->SaveKeysList(m_vecClearCardParam);

		if ( !bfalg1 )
		{
			m_cTabctrl.SelectTab(0);
			return;
		}
		if ( !bfalg2 )
		{
			m_cTabctrl.SelectTab(1);
			return;
		}
		string strPath = "ScriptConfig.xml";
		if ( bfalg1 && bfalg2 )
			m_cOpXml.SaveConfDatas(strPath,m_nCOS,m_nCheckATR,m_strSearial,m_strTanKey,m_nCheckKeys,m_nCheckLog,m_vecClearCardParam);

		CDialog::OnOK();
	}

	CDialog::OnClose();
}


void CMainDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bfalg1=true,bfalg2=true;//标记选项卡中数据是否有错误

	bfalg1 = m_cMainTab->SaveOption(m_nCheckKeys, m_nCheckATR, m_nCOS, m_nCheckLog, m_strSearial, m_strTanKey);
	if ( m_nCheckKeys )//需要清卡
		bfalg2 = m_cKeyConf->SaveKeysList(m_vecClearCardParam);

	if ( !bfalg1 )
	{
		m_cTabctrl.SelectTab(0);
		return;
	}
	if ( !bfalg2 )
	{
		m_cTabctrl.SelectTab(1);
		return;
	}
	string strPath = "ScriptConfig.xml";
	if ( bfalg1 && bfalg2 )
		m_cOpXml.SaveConfDatas(strPath,m_nCOS,m_nCheckATR,m_strSearial,m_strTanKey,m_nCheckKeys,m_nCheckLog,m_vecClearCardParam);

	CDialog::OnOK();
}


void CMainDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainDlg::OnCancel();
}
