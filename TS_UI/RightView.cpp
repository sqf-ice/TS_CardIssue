// RightView.cpp : ʵ���ļ�
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


// CRightView ���

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


// CRightView ��Ϣ�������


void CRightView::OnDraw(CDC* /*pDC*/)
{
	// TODO: �ڴ����ר�ô����/����û���
}


void CRightView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	pEditCtrl = &GetRichEditCtrl();
	pEditCtrl->SetReadOnly(TRUE);
	pEditCtrl->SetBackgroundColor(FALSE,RGB(255,255,255));//���ñ���ɫ
	
	//���ñ߾� ������ʾ�к�
	::SendMessage(GetSafeHwnd(),EM_SETMARGINS,EC_LEFTMARGIN,32);
	//��ֹ�Զ�����
	m_nWordWrap = WrapNone;
	WrapChanged(); 
}


LRESULT CRightView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
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

	MemDC.FillSolidRect(&rect, RGB(220,236,248));//���ñ���ɫ
	MemDC.SetTextColor(RGB(43,145,147));

	int nSNum = pWnd->GetFirstVisibleLine();//��ȡ���ӵ�һ��
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
//	// TODO: �ڴ����ר�ô����/����û���
//
//	return CRichEditView::PreCreateWindow(cs);
//}


//HBRUSH CRightView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CRichEditView::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  �ڴ˸��� DC ���κ�����
//	pDC->SetBkMode(TRANSPARENT);
//	CBrush m_brush;
//	m_brush.CreateSolidBrush(RGB(224,241,253));//��ˢ��ɫ
//	return m_brush;
//	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	//return hbr;
//}
