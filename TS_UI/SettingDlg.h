#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "Common.h"
#include "OperateXML.h"
#include <string>
using namespace std;

#define IDC_EDIT_CREATEID 3000
#define IDC_COMBOX_CREATEID 3002

// CSettingDlg 对话框
class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	int e_Item;    //刚编辑的行  
	int e_SubItem; //刚编辑的列
	CMyListCtrl m_list;
	int m_nCurRow;//点鼠标右键选中的行
	COperateXML m_cOpXml;

	CEdit m_Edit;  //生成单元编辑框对象
	bool haveeditcreate;//标志编辑框已经被创建
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//创建单元格编辑框函数
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//销毁单元格编辑框对象

	CComboBox m_comBox;//生产单元格下拉列表对象
	bool haveccomboboxcreate;//标志下拉列表框已经被创建
	void createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);//创建单元格下拉列表框函数
	void distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);//销毁单元格下拉列表框
	void SetComboxSelect();//根据当前选中行项的文本选择下拉框对应的项
	void InsertNewLine(int nRow, CString str1="", CString str2="", CString str3="", CString str4="", CString str5="" );//添加ListCtrl新行数据，从第二列开始往后添加数据，第一列预留填充行号
	void UpdataListSeq();
	void SetConfParam();//读取配置参数并显示到界面上

public:
	//保存清卡的密钥数据信息 true数据检测通过并保存 false数据检测不通过	输出:vecClearCardParam清卡所需的参数
	bool SaveKeysList(vector<CLEARCARDPARAM> &vecClearCardParam);

	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnMylistctrl(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnMenuadd();
	afx_msg void OnMenudelte();
	afx_msg void OnMenuclear();
	afx_msg void ResetClParam();
};
