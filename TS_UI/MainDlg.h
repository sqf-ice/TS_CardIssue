#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"
#include "SettingDlg.h"
#include "MainTabDlg.h"


// CMainDlg 对话框

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CImageList m_cImagelistTab;
	CMyTabCtrl m_cTabctrl;
	CMainTabDlg *m_cMainTab;
	CSettingDlg *m_cKeyConf;
	COperateXML m_cOpXml;

public:
	int m_nCOS;//芯片类型
	string m_strSearial;//流水号 更新ATR时用
	string m_strTanKey;//传输密钥
	int m_nCheckKeys,m_nCheckATR,m_nCheckLog;//分别为1时表示进行清卡和更新ATR和记录日志
	vector<CLEARCARDPARAM> m_vecClearCardParam;//清卡所需的参数

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
