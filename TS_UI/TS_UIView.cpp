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

// TS_UIView.cpp : CTS_UIView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CTS_UIView ����/����

CTS_UIView::CTS_UIView()
{
	// TODO: �ڴ˴���ӹ������

}

CTS_UIView::~CTS_UIView()
{
}

BOOL CTS_UIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CTS_UIView ����

void CTS_UIView::OnDraw(CDC* /*pDC*/)
{
	CTS_UIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CTS_UIView ���

#ifdef _DEBUG
void CTS_UIView::AssertValid() const
{
	CView::AssertValid();
}

void CTS_UIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTS_UIDoc* CTS_UIView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTS_UIDoc)));
	return (CTS_UIDoc*)m_pDocument;
}
#endif //_DEBUG


// CTS_UIView ��Ϣ�������


//HBRUSH CTS_UIView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  �ڴ˸��� DC ���κ�����
//	//CFont m_font;
//	//CBrush m_brush;
//	//m_font.CreatePointFont(150,"��Բ");
//	//m_brush.CreateSolidBrush(RGB(227,237,205));//��ˢ��ɫ
//	//pDC->SetTextColor(RGB(0, 50, 255));
//	//pDC->SelectObject(&m_font);
//	//return m_brush;
//	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	return hbr;
//}
