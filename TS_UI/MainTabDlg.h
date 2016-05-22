#pragma once
#include "MyTabCtrl.h"
#include "afxwin.h"
#include <string>
#include "OperateXML.h"

using namespace std;

// CMainTabDlg �Ի���

class CMainTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainTabDlg)

public:
	CMainTabDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainTabDlg();

// �Ի�������
	enum { IDD = IDD_MAINTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_cbChipType;
	int m_nCOS;//оƬ����
	BOOL m_bCheckATR;
	COperateXML m_cOpXml;
	CString m_strSearialNum;
	CString m_strTranKey;
	BOOL m_bCheckLog;

	void SetConfParam();//��ȡ���ò�������ʾ��������

public:
	CMyTabCtrl *m_cMyTab;
	BOOL m_bCheckKeys;

	/************************************************************************/
	/*��    ��:	SaveOption
	/*��    ��:	����ѡ����������
	/*�������:	null
	/*�������:	nCheckKeys:�Ƿ��忨 1Ϊ�忨	nCheckATR:�Ƿ����ATR 1Ϊ����	nCOS��ѡ���оƬ����  nLog:1Ϊд��־	strSearial����ˮ��  strTranKey��������Կֵ
	/*�� �� ֵ:	true	�ɹ�	false	������Ҫ��������ݸ�ʽ����ȷ
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
