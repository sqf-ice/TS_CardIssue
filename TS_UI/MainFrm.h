// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
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
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CMySplitterWnd m_wndSplitter;
	CRightView* m_pEditView;
	CLeftPaneView* m_leftTreeView;
	bool m_isSplitter;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	//CMFCRibbonBar     m_wndRibbonBar;//分别用于控制Ribbon界面的命令面板
	CMyRibbonBar m_wndRibbonBar;//CMFCRibbonBar派生类，为了屏蔽快速工具栏的功能
	//CMFCRibbonApplicationButton m_MainButton;//分别用于控制Ribbon界面的应用程序按钮(Application Button)
	CMyRibbonApplicationButton *m_MainButton;//CMFCRibbonApplicationButton派生类，为了屏蔽点击按钮功能
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;//分别用于控制Ribbon界面的状态栏

private:
	bool m_bSetting, m_bCheXML, m_bScript;//用来设置功能图标的变灰和变亮
	bool m_bCheFixedKey,m_bChePSAM,m_bCheEncode;//用来实现设置验卡选项单选框模式
	bool m_bCheXML_JMJ,m_bCheXML_GDMY,m_bSetting_YK,m_bScript_YK;//用来实现设置验卡单选框变灰和变亮
	int m_nYKStyle;//验卡方式 0：固定密钥  1：加密机  2：PSAM卡
	int m_nCOSType;//芯片类型
	string m_strSearial;//流水号 更新ATR时用
	string m_strTranKey;//传输密钥值
	int m_nCheckKeys,m_nCheckATR,m_nCheckLog;//分别为1时表示进行清卡和更新ATR和记录日志
	vector<CLEARCARDPARAM> m_vecQCParam;//清卡所需的参数
	CInformation *cInformation;//信息类
	stringstream m_strsSript;

	void OnInniteDatas_FK();//初始化发卡功能的参数，一些界面的变灰变亮参数
	void OnInniteDatas_YK();
	void ChangeXMLTreeDatas();//改变发卡脚本树视图的数据，即显示MF-DDF-ADF-EF视图
	void ChangeXMLTreeDatas_YK();//改变验卡脚本树视图的数据，即显示SSSEKEY-ADFKEY视图
	/************************************************************************/
	/*名    称:	PrintText
	/*功    能:	输出文字到视图窗口CRichEditView中
	/*输入参数:	str: 输出的文字	nMode：输出方式 0覆盖替换所有文字 1在最后追加文字	nColoer：字体颜色 0黑色 1红色 2黄色
	/*输出参数:	null
	/*返 回 值:	null
	/************************************************************************/
	void PrintText( string str, int nMode=0, int nColoer=0 );
	void GetProInfo( stringstream &strsDatas );//获取XML脚本配置文件中的项目信息
	void Savefile(CString strFileName);//保存脚本文件 strFileName：文件名称

// 生成的消息映射函数
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


