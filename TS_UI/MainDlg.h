#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"
#include "SettingDlg.h"
#include "MainTabDlg.h"


// CMainDlg �Ի���

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CImageList m_cImagelistTab;
	CMyTabCtrl m_cTabctrl;
	CMainTabDlg *m_cMainTab;
	CSettingDlg *m_cKeyConf;
	COperateXML m_cOpXml;

public:
	int m_nCOS;//оƬ����
	string m_strSearial;//��ˮ�� ����ATRʱ��
	string m_strTanKey;//������Կ
	int m_nCheckKeys,m_nCheckATR,m_nCheckLog;//�ֱ�Ϊ1ʱ��ʾ�����忨�͸���ATR�ͼ�¼��־
	vector<CLEARCARDPARAM> m_vecClearCardParam;//�忨����Ĳ���

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
