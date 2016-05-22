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

// CSettingDlg �Ի���
class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	int e_Item;    //�ձ༭����  
	int e_SubItem; //�ձ༭����
	CMyListCtrl m_list;
	int m_nCurRow;//������Ҽ�ѡ�е���
	COperateXML m_cOpXml;

	CEdit m_Edit;  //���ɵ�Ԫ�༭�����
	bool haveeditcreate;//��־�༭���Ѿ�������
	void createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat);//������Ԫ��༭����
	void distroyEdit(CListCtrl *list, CEdit* distroyedit, int &Item, int &SubItem);//���ٵ�Ԫ��༭�����

	CComboBox m_comBox;//������Ԫ�������б����
	bool haveccomboboxcreate;//��־�����б���Ѿ�������
	void createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat);//������Ԫ�������б����
	void distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem);//���ٵ�Ԫ�������б��
	void SetComboxSelect();//���ݵ�ǰѡ��������ı�ѡ���������Ӧ����
	void InsertNewLine(int nRow, CString str1="", CString str2="", CString str3="", CString str4="", CString str5="" );//���ListCtrl�������ݣ��ӵڶ��п�ʼ����������ݣ���һ��Ԥ������к�
	void UpdataListSeq();
	void SetConfParam();//��ȡ���ò�������ʾ��������

public:
	//�����忨����Կ������Ϣ true���ݼ��ͨ�������� false���ݼ�ⲻͨ��	���:vecClearCardParam�忨����Ĳ���
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
