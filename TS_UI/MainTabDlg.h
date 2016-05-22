#pragma once
#include "MyTabCtrl.h"
#include "afxwin.h"
#include <string>
#include "OperateXML.h"

using namespace std;

// CMainTabDlg 对话框

class CMainTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainTabDlg)

public:
	CMainTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainTabDlg();

// 对话框数据
	enum { IDD = IDD_MAINTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_cbChipType;
	int m_nCOS;//芯片类型
	BOOL m_bCheckATR;
	COperateXML m_cOpXml;
	CString m_strSearialNum;
	CString m_strTranKey;
	BOOL m_bCheckLog;

	void SetConfParam();//读取配置参数并显示到界面上

public:
	CMyTabCtrl *m_cMyTab;
	BOOL m_bCheckKeys;

	/************************************************************************/
	/*名    称:	SaveOption
	/*功    能:	保存选项配置数据
	/*输入参数:	null
	/*输出参数:	nCheckKeys:是否清卡 1为清卡	nCheckATR:是否更新ATR 1为更新	nCOS：选择的芯片类型  nLog:1为写日志	strSearial：流水号  strTranKey：传输密钥值
	/*返 回 值:	true	成功	false	存在需要输入的数据格式不正确
	/************************************************************************/
	bool SaveOption(int &nCheckKeys, int &nCheckATR, int &nCOS, int &nLog, string &strSearial, string &strTranKey);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_Edit;
	CEdit m_TRKeyEdit;
};
