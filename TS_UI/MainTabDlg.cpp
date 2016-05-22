// MainTabDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TS_UI.h"
#include "MainTabDlg.h"
#include "afxdialogex.h"
#include "Script/tool.h"


// CMainTabDlg �Ի���

IMPLEMENT_DYNAMIC(CMainTabDlg, CDialog)

CMainTabDlg::CMainTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainTabDlg::IDD, pParent)
	, m_bCheckLog(FALSE)
{
	m_cMyTab = NULL;
	m_nCOS = 0;
	m_strSearialNum = _T("");
	m_strTranKey = _T("");
}

CMainTabDlg::~CMainTabDlg()
{
}

void CMainTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bCheckKeys);
	DDX_Check(pDX, IDC_CHECK2, m_bCheckATR);
	//  DDX_Control(pDX, IDC_COMBO1, m_comBox);
	DDX_Control(pDX, IDC_COMBO1, m_cbChipType);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nCOS);
	DDX_Control(pDX, IDC_EDIT_SEARIAL, m_Edit);
	DDX_Text(pDX, IDC_EDIT_SEARIAL, m_strSearialNum);
	DDX_Check(pDX, IDC_CHECK3, m_bCheckLog);
	DDX_Control(pDX, IDC_EDIT_TRKEY, m_TRKeyEdit);
	DDX_Text(pDX, IDC_EDIT_TRKEY, m_strTranKey);
}


BEGIN_MESSAGE_MAP(CMainTabDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CMainTabDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMainTabDlg::OnBnClickedCheck2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMainTabDlg ��Ϣ�������


BOOL CMainTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��оƬ��������������
	m_cbChipType.AddString("����T514оƬ");
	m_cbChipType.AddString("����FM1232оƬ");
	m_cbChipType.SetItemHeight(0,20);

	GetDlgItem(IDC_SEARIALNUM)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_SEARIAL)->ShowWindow(FALSE);
	m_Edit.SetLimitText(8);//������ˮ�������8λ
	m_TRKeyEdit.SetLimitText(32);//���ƴ�����Կ�����32λ

	SetConfParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMainTabDlg::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	m_cMyTab->EnableTab(1,m_bCheckKeys);
}


void CMainTabDlg::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	GetDlgItem(IDC_SEARIALNUM)->ShowWindow(m_bCheckATR);
	GetDlgItem(IDC_EDIT_SEARIAL)->ShowWindow(m_bCheckATR);
}


HBRUSH CMainTabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	CFont m_font;
	CBrush m_brush;
	m_font.CreatePointFont(120,"��Բ");
	m_brush.CreateSolidBrush(RGB(227,237,205));//��ˢ��ɫ
	if ( nCtlColor == CTLCOLOR_STATIC ) //���ľ�̬�ؼ�����ɫ
	{
		UINT uID = pWnd->GetDlgCtrlID(); //��ȡ�ؼ�ID����
		pDC->SetBkMode( TRANSPARENT );
		if ( uID == IDC_WARM )
			pDC->SetTextColor(RGB(229, 20, 0));
		else
			pDC->SetTextColor(RGB(0, 50, 255));
		pDC->SelectObject(&m_font);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CMainTabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ( pMsg->message == WM_CHAR && (GetFocus() == (&m_Edit) || GetFocus() == (&m_TRKeyEdit)) )
	{
		if( pMsg->wParam==VK_BACK || (pMsg->wParam<='9' && pMsg->wParam>='0') || (pMsg->wParam<='F' && pMsg->wParam>='A') || (pMsg->wParam<='f' && pMsg->wParam>='a') )
		{}
		else
			return TRUE;//�����ʮ���������ݵĻ���������Ϣ
	}

	//���λس�����Esc������Ϣ
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return TRUE;    
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE; 

	return CDialog::PreTranslateMessage(pMsg);
}

bool CMainTabDlg::SaveOption( int &nCheckKeys, int &nCheckATR, int &nCOS, int &nLog, string &strSearial, string &strTranKey )
{
	CString strWarn;
	UpdateData(TRUE);
	nCheckKeys = m_bCheckKeys;
	nCheckATR = m_bCheckATR;
	nCOS = m_nCOS;
	nLog = m_bCheckLog;
	if ( nCheckATR && m_strSearialNum.GetLength() != 8 )
	{
		strWarn = "���󣺵�ǰ������ˮ�ŵĳ��Ȳ���8λ,���޸ģ�";
		GetDlgItem(IDC_WARM)->SetWindowText(strWarn);
		GetDlgItem(IDC_WARM)->ShowWindow(TRUE);
		strSearial = "";
		return false;
	}
	if ( m_strTranKey.GetLength() != 32 )
	{
		strWarn = "���󣺵�ǰ���봫����Կ�ĳ��Ȳ���32λ,���޸ģ�";
		GetDlgItem(IDC_WARM)->SetWindowText(strWarn);
		GetDlgItem(IDC_WARM)->ShowWindow(TRUE);
		strTranKey = "";
		return false;
	}
	GetDlgItem(IDC_WARM)->ShowWindow(FALSE);
	strSearial = m_strSearialNum;
	strTranKey = m_strTranKey;
	return true;
}

void CMainTabDlg::SetConfParam()
{
	string strPath = "ScriptConfig.xml";
	int nCOS,nCheckKeys,nCheckATR,nCheckLog;
	string strSerialNum,strTanKey;
	
	m_cOpXml.ReadOptionDatas(strPath,nCOS,nCheckKeys,nCheckATR,strSerialNum,strTanKey,nCheckLog);
	if ( nCOS!=0 && nCOS!=1 )
		m_nCOS = 0;
	else
		m_nCOS = nCOS;

	if ( nCheckKeys!=0 && nCheckKeys!=1 )
		m_bCheckKeys = 0;
	else
		m_bCheckKeys = nCheckKeys;

	if (!JudgeHex(strTanKey))
		m_strTranKey = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
	else
		m_strTranKey.Format("%s",strTanKey.c_str());

	if ( nCheckATR!=0 && nCheckATR!=1 )
		m_bCheckATR = 0;
	else
		m_bCheckATR = nCheckATR;

	if (!JudgeHex(strSerialNum))
		m_strSearialNum = "";
	else
		m_strSearialNum.Format("%s",strSerialNum.c_str());

	if ( nCheckLog!=0 && nCheckLog!=1 )
		m_bCheckLog = 0;
	else
		m_bCheckLog = nCheckLog;
	UpdateData(FALSE);
	OnBnClickedCheck2();
}