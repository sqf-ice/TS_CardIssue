// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TS_UI.h"
#include "MainDlg.h"
#include "afxdialogex.h"


// CMainDlg �Ի���

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


// CMainDlg ��Ϣ�������


BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HICON   hIcon=AfxGetApp()->LoadIcon(IDI_ICON1); //����ͼ��
	SetIcon(hIcon,FALSE);//����ΪFALSE��ʾ���ô����ϵ�Сͼ��

	m_cImagelistTab.Create(IDB_BITMAP2, 18, 0, RGB(255,255,255));
	m_cTabctrl.SetImageList(&m_cImagelistTab);

	m_cMainTab = new CMainTabDlg();
	m_cMainTab->Create(CMainTabDlg::IDD, &m_cTabctrl);
	m_cTabctrl.AddTab(m_cMainTab, "����ѡ��", 0);
	m_cMainTab->m_cMyTab = &m_cTabctrl;

	m_cKeyConf = new CSettingDlg();
	m_cKeyConf->Create(CSettingDlg::IDD, &m_cTabctrl);
	m_cTabctrl.AddTab(m_cKeyConf, "�����忨����", 1);

	m_cTabctrl.SetSelectedColor(RGB(255, 117, 117));
	m_cTabctrl.SetMouseOverColor(RGB(255, 255, 255));
	m_cTabctrl.SetNormalColor(RGB(0, 50, 255));
	m_cTabctrl.EnableTab(1, m_cMainTab->m_bCheckKeys);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_cMainTab)
		delete m_cMainTab;
	if (m_cKeyConf)
		delete m_cKeyConf;
}



BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//���λس�����Esc������Ϣ
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return TRUE;    
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE; 

	return CDialog::PreTranslateMessage(pMsg);
}


void CMainDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if ( IDOK == ::MessageBox( m_hWnd, "�˳�֮ǰ�Ƿ�Ҫ���浱ǰ������", "�˳���ʾ", MB_OK||MB_OKCANCEL )  )
	{
		bool bfalg1=true,bfalg2=true;//���ѡ��������Ƿ��д���";

		bfalg1 = m_cMainTab->SaveOption(m_nCheckKeys, m_nCheckATR, m_nCOS, m_nCheckLog, m_strSearial, m_strTanKey);
		if ( m_nCheckKeys )//��Ҫ�忨
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bfalg1=true,bfalg2=true;//���ѡ��������Ƿ��д���

	bfalg1 = m_cMainTab->SaveOption(m_nCheckKeys, m_nCheckATR, m_nCOS, m_nCheckLog, m_strSearial, m_strTanKey);
	if ( m_nCheckKeys )//��Ҫ�忨
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainDlg::OnCancel();
}
