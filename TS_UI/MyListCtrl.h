#pragma once
#include "afxcmn.h"
#include "HeaderCtrlCl.h"

#define WM_MYLISTCTRL 30004

class CMyListCtrl :
	public CListCtrl
{
public:
	CMyListCtrl(void);
	~CMyListCtrl(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

public:
	CHeaderCtrlCl m_Header;
	int m_nRowHeight;
	void SetRowHeigt(int nHeight);//设置行高
	void DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	// Gradient - 渐变系数，立体背景用,不用渐变设为0
	void SetHeaderBKColor(int R, int G, int B, int Gradient);//设置头部背景色
	void SetHeaderHeight(float Height);// 设置表头高度
	CPtrList m_ptrListCol;  //保存列颜色
	CPtrList m_ptrListItem; //保存Item颜色表
	CPtrList m_colTextColor; //保存列字体颜色
	CPtrList m_ItemTextColor; //保存单元格字体颜色
	bool FindColColor(int col ,COLORREF &color); //查找列颜色
	bool FindItemColor(int col,int row,COLORREF &color); //设置指定单元背景色
	bool FindColTextColor(int col,COLORREF &color); //查找列字体颜色
	bool FindItemTextColor(int col,int row,COLORREF &color);
	void SetColColor(int col,COLORREF color);  //设置列颜色
	void SetItemColor(int col,int row,COLORREF color); //设置Item颜色
	void SetColTextColor(int col,COLORREF color);   //设置列文本颜色
	void SetItemTextColor(int col,int row,COLORREF color);//设置单元格字体颜色
	void SetHeaderFontHW(int nHeight,int nWith); //设置头部字体高度,和宽度,0表示缺省，自适应
	void SetHeaderTextColor(COLORREF color);//设置头部字体颜色
	COLORREF m_color;
	BOOL SetTextColor(COLORREF cr);//设置文本颜色SetTextColor(RGB(0,255,255))
	void SetFontHW(int nHeight,int nWith, int nWeight=0);  //设置字体的高和宽
	int m_fontHeight;// 字体高度
	int m_fontWith;// 字体宽度
	int m_fontWeight;//磅数，0-1000有效，400表示标准体，700表示黑（粗）体，如果此值为0，则使用缺省的权值

	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);

protected:
	virtual void PreSubclassWindow();
};

