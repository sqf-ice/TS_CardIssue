#if !defined(AFX_MyTabCtrl_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
#define AFX_MyTabCtrl_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window
#include <afxtempl.h>

class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();

// Attributes
public:

// Operations
public:
	void AddTab(CWnd* pWnd, LPTSTR lpszCaption, int iImage =0);
	void EnableTab(int iIndex, BOOL bEnable = TRUE);
	BOOL SelectTab(int iIndex);
	void DeleteAllTabs();
	void DeleteTab(int iIndex);
	void SetTopLeftCorner(CPoint pt);
	BOOL IsTabEnabled(int iIndex);

	void SetDisabledColor(COLORREF cr);//设置变灰的标签名颜色
	void SetSelectedColor(COLORREF cr);//设置选中的标签名颜色
	void SetNormalColor(COLORREF cr);//设置标签名的默认颜色
	void SetMouseOverColor(COLORREF cr);//设置鼠标移动过去显示的标签名颜色

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SelectNextTab(BOOL bForward);//选择下一个tab标签页
	BOOL SelectTabByName(LPCTSTR lpszTabCaption);//根据tab标签页的名字选择标签页
	void ChangeTab(int iIndex, CWnd* pNewTab, LPTSTR lpszCaption, int iImage);//更换某个对应iIndex的标签页绑定的对话框子类
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	CArray<BOOL, BOOL> m_arrayStatusTab; //** enabled Y\N
	int m_iSelectedTab;
	POINT m_ptTabs;
	COLORREF m_crSelected;
	COLORREF m_crDisabled;
	COLORREF m_crNormal;
	COLORREF m_crMouseOver;

	int m_iIndexMouseOver;
	
	bool m_bMouseOver;
	bool m_bColorMouseOver;
	bool m_bColorNormal;
	bool m_bColorDisabled;
	bool m_bColorSelected;
	
	//{{AFX_MSG(CMyTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MyTabCtrl_H__A11951B3_2F95_11D3_A896_00A0C9B6FB28__INCLUDED_)
