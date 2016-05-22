// MainTabDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TS_UI.h"
#include "MainTabDlg.h"
#include "afxdialogex.h"
#include "Script/tool.h"


// CMainTabDlg 对话框

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


// CMainTabDlg 消息处理程序


BOOL CMainTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化芯片类型下拉框数据
	m_cbChipType.AddString("大唐T514芯片");
	m_cbChipType.AddString("复旦FM1232芯片");
	m_cbChipType.SetItemHeight(0,20);

	GetDlgItem(IDC_SEARIALNUM)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_SEARIAL)->ShowWindow(FALSE);
	m_Edit.SetLimitText(8);//限制流水号最长输入8位
	m_TRKeyEdit.SetLimitText(32);//限制传输密钥最长输入32位

	SetConfParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMainTabDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_cMyTab->EnableTab(1,m_bCheckKeys);
}


void CMainTabDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	GetDlgItem(IDC_SEARIALNUM)->ShowWindow(m_bCheckATR);
	GetDlgItem(IDC_EDIT_SEARIAL)->ShowWindow(m_bCheckATR);
}


HBRUSH CMainTabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CFont m_font;
	CBrush m_brush;
	m_font.CreatePointFont(120,"幼圆");
	m_brush.CreateSolidBrush(RGB(227,237,205));//画刷颜色
	if ( nCtlColor == CTLCOLOR_STATIC ) //更改静态控件的颜色
	{
		UINT uID = pWnd->GetDlgCtrlID(); //获取控件ID集合
		pDC->SetBkMode( TRANSPARENT );
		if ( uID == IDC_WARM )
			pDC->SetTextColor(RGB(229, 20, 0));
		else
			pDC->SetTextColor(RGB(0, 50, 255));
		pDC->SelectObject(&m_font);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CMainTabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( pMsg->message == WM_CHAR && (GetFocus() == (&m_Edit) || GetFocus() == (&m_TRKeyEdit)) )
	{
		if( pMsg->wParam==VK_BACK || (pMsg->wParam<='9' && pMsg->wParam>='0') || (pMsg->wParam<='F' && pMsg->wParam>='A') || (pMsg->wParam<='f' && pMsg->wParam>='a') )
		{}
		else
			return TRUE;//输入非十六进制数据的话，拦截消息
	}

	//屏蔽回车键和Esc按键消息
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
		strWarn = "错误：当前输入流水号的长度不足8位,请修改！";
		GetDlgItem(IDC_WARM)->SetWindowText(strWarn);
		GetDlgItem(IDC_WARM)->ShowWindow(TRUE);
		strSearial = "";
		return false;
	}
	if ( m_strTranKey.GetLength() != 32 )
	{
		strWarn = "错误：当前输入传输密钥的长度不够32位,请修改！";
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