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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "MyRibbonBar.h"
#include "MyRibbonApplicationButton.h"
#include "MainDlg.h"
#include "Common.h"
#include "Script/Chip_Script.h"
#include "Script/Script_YK.h"
#include "SplitterWnd.h"
#include "LeftPaneView.h"
#include "RightView.h"


#define IDI_REFRESH 3004
#define ID_STATUSBAR_REFRESH 3006
class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CMySplitterWnd m_wndSplitter;
	CRightView* m_pEditView;
	CLeftPaneView* m_leftTreeView;
	bool m_isSplitter;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	//CMFCRibbonBar     m_wndRibbonBar;//�ֱ����ڿ���Ribbon������������
	CMyRibbonBar m_wndRibbonBar;//CMFCRibbonBar�����࣬Ϊ�����ο��ٹ������Ĺ���
	//CMFCRibbonApplicationButton m_MainButton;//�ֱ����ڿ���Ribbon�����Ӧ�ó���ť(Application Button)
	CMyRibbonApplicationButton *m_MainButton;//CMFCRibbonApplicationButton�����࣬Ϊ�����ε����ť����
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;//�ֱ����ڿ���Ribbon�����״̬��

private:
	bool m_bSetting, m_bCheXML, m_bScript;//�������ù���ͼ��ı�Һͱ���
	bool m_bCheFixedKey,m_bChePSAM,m_bCheEncode;//����ʵ�������鿨ѡ�ѡ��ģʽ
	bool m_bCheXML_JMJ,m_bCheXML_GDMY,m_bSetting_YK,m_bScript_YK;//����ʵ�������鿨��ѡ���Һͱ���
	int m_nYKStyle;//�鿨��ʽ 0���̶���Կ  1�����ܻ�  2��PSAM��
	int m_nCOSType;//оƬ����
	string m_strSearial;//��ˮ�� ����ATRʱ��
	string m_strTranKey;//������Կֵ
	int m_nCheckKeys,m_nCheckATR,m_nCheckLog;//�ֱ�Ϊ1ʱ��ʾ�����忨�͸���ATR�ͼ�¼��־
	vector<CLEARCARDPARAM> m_vecQCParam;//�忨����Ĳ���
	CInformation *cInformation;//��Ϣ��
	stringstream m_strsSript;

	void OnInniteDatas_FK();//��ʼ���������ܵĲ�����һЩ����ı�ұ�������
	void OnInniteDatas_YK();
	void ChangeXMLTreeDatas();//�ı䷢���ű�����ͼ�����ݣ�����ʾMF-DDF-ADF-EF��ͼ
	void ChangeXMLTreeDatas_YK();//�ı��鿨�ű�����ͼ�����ݣ�����ʾSSSEKEY-ADFKEY��ͼ
	/************************************************************************/
	/*��    ��:	PrintText
	/*��    ��:	������ֵ���ͼ����CRichEditView��
	/*�������:	str: ���������	nMode�������ʽ 0�����滻�������� 1�����׷������	nColoer��������ɫ 0��ɫ 1��ɫ 2��ɫ
	/*�������:	null
	/*�� �� ֵ:	null
	/************************************************************************/
	void PrintText( string str, int nMode=0, int nColoer=0 );
	void GetProInfo( stringstream &strsDatas );//��ȡXML�ű������ļ��е���Ŀ��Ϣ
	void Savefile(CString strFileName);//����ű��ļ� strFileName���ļ�����

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSetting();
	afx_msg void OnOpenfile();
	afx_msg void OnUpdateOpenfile(CCmdUI *pCmdUI);
	afx_msg void OnDiscard();
	afx_msg void OnUpdateDiscard(CCmdUI *pCmdUI);
	afx_msg void OnSavefile();
	afx_msg void OnUpdateSavefile(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnCheckFixedKey();
	afx_msg void OnUpdateCheckFixedKey(CCmdUI *pCmdUI);
	afx_msg void OnCheckEncoder();
	afx_msg void OnUpdateCheckEncoder(CCmdUI *pCmdUI);
	afx_msg void OnCheckPSAM();
	afx_msg void OnUpdateCheckPSAM(CCmdUI *pCmdUI);
	afx_msg void OnLoadXML_YK();
	afx_msg void OnDiscard_YK();
	afx_msg void OnUpdateDiscard_YK(CCmdUI *pCmdUI);
	afx_msg void OnSavefile_YK();
	afx_msg void OnUpdateSavefile_YK(CCmdUI *pCmdUI);
};


