// RightView.cpp : 实现文件
//

#include "stdafx.h"
#include "TS_UI.h"
#include "RightView.h"


// CRightView

IMPLEMENT_DYNCREATE(CRightView, CRichEditView)

CRightView::CRightView()
{

}

CRightView::~CRightView()
{
}

BEGIN_MESSAGE_MAP(CRightView, CRichEditView)
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRightView 诊断

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightView 消息处理程序


void CRightView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
}


void CRightView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	pEditCtrl = &GetRichEditCtrl();
	pEditCtrl->SetReadOnly(TRUE);
	pEditCtrl->SetBackgroundColor(FALSE,RGB(255,255,255));//设置背景色
	
	//设置边距 用来显示行号
	::SendMessage(GetSafeHwnd(),EM_SETMARGINS,EC_LEFTMARGIN,32);
	//禁止自动换行
	m_nWordWrap = WrapNone;
	WrapChanged(); 
}


LRESULT CRightView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ( message == WM_PAINT )
	{
		LRESULT ret = CRichEditView::WindowProc(message, wParam, lParam);
		ShowLineNo(GetSafeHwnd());
		return ret;
	}

	return CRichEditView::WindowProc(message, wParam, lParam);
}

int CRightView::ShowLineNo( HWND hWnd )
{
	CRichEditCtrl *pWnd = NULL;
	pWnd = (CRichEditCtrl*)CWnd::FromHandlePermanent(hWnd);
	if (pWnd==NULL)
	{
		return 0;
	}

	HDC hDC = ::GetDC(GetSafeHwnd());
	CDC* pDC = new CDC;
	pDC->Attach(hDC);
	pDC->SetBkMode(TRANSPARENT);

	CDC MemDC;
	CRect rect;
	CBitmap m_Canvas;
	pWnd->GetClientRect(rect);
	rect.right = 37;
	MemDC.CreateCompatibleDC(pDC);
	m_Canvas.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	MemDC.SelectObject(&m_Canvas);

	MemDC.FillSolidRect(&rect, RGB(220,236,248));//设置背景色
	MemDC.SetTextColor(RGB(43,145,147));

	int nSNum = pWnd->GetFirstVisibleLine();//获取可视第一行
	int nLNum = pWnd->GetLineCount();
	int nLVis = nLNum - nSNum;

	CString sLNum;
	for ( int i=0; i<nLVis; ++i )
	{
		CPoint pt;
		pt = pWnd->PosFromChar(pWnd->LineIndex(nSNum+i));
		sLNum.Format("%4d", nSNum+1+i);
		MemDC.TextOut(0,pt.y,sLNum,sLNum.GetLength());
	}
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
	pDC->DeleteDC();
	delete pDC;
	return 0;

}

//BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CRichEditView::PreCreateWindow(cs);
//}


//HBRUSH CRightView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CRichEditView::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  在此更改 DC 的任何特性
//	pDC->SetBkMode(TRANSPARENT);
//	CBrush m_brush;
//	m_brush.CreateSolidBrush(RGB(224,241,253));//画刷颜色
//	return m_brush;
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	//return hbr;
//}
