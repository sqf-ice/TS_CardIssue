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

// TS_UIView.cpp : CTS_UIView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TS_UI.h"
#endif

#include "TS_UIDoc.h"
#include "TS_UIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTS_UIView

IMPLEMENT_DYNCREATE(CTS_UIView, CView)

BEGIN_MESSAGE_MAP(CTS_UIView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CTS_UIView 构造/析构

CTS_UIView::CTS_UIView()
{
	// TODO: 在此处添加构造代码

}

CTS_UIView::~CTS_UIView()
{
}

BOOL CTS_UIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTS_UIView 绘制

void CTS_UIView::OnDraw(CDC* /*pDC*/)
{
	CTS_UIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CTS_UIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTS_UIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTS_UIView 诊断

#ifdef _DEBUG
void CTS_UIView::AssertValid() const
{
	CView::AssertValid();
}

void CTS_UIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTS_UIDoc* CTS_UIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTS_UIDoc)));
	return (CTS_UIDoc*)m_pDocument;
}
#endif //_DEBUG


// CTS_UIView 消息处理程序


//HBRUSH CTS_UIView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  在此更改 DC 的任何特性
//	//CFont m_font;
//	//CBrush m_brush;
//	//m_font.CreatePointFont(150,"幼圆");
//	//m_brush.CreateSolidBrush(RGB(227,237,205));//画刷颜色
//	//pDC->SetTextColor(RGB(0, 50, 255));
//	//pDC->SelectObject(&m_font);
//	//return m_brush;
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	return hbr;
//}
