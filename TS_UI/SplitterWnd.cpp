// SplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SplitterWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd

CMySplitterWnd::CMySplitterWnd()
{
	//更改分割条和窗格间距大小
	m_cxSplitter = m_cySplitter = 6;
	m_cxSplitterGap = m_cySplitterGap = 6;
}

CMySplitterWnd::~CMySplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMySplitterWnd)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMySplitterWnd message handlers

void CMySplitterWnd::OnDrawSplitter(CDC *pDC, ESplitType nType, const CRect &rectArg)
{
	// if pDC == NULL, then just invalidate
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	
	// otherwise, actually draw
	CRect rc = rectArg;
	switch(nType) 
	{ 
	case splitBorder:
		//重画分割窗口边界,灰白色
		pDC->Draw3dRect(rc,RGB(241,245,251),RGB(241,245,251));
		rc.InflateRect(-1,-1);
		pDC->Draw3dRect(rc,RGB(241,245,251),RGB(241,245,251));
		return; 
	case splitBox:
		pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
		rc.InflateRect(-1,-1); 
		pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
		rc.InflateRect(-1,-1);
		pDC->FillSolidRect(rc,RGB(0,0,0)); 
		pDC->Draw3dRect(rc,RGB(0,0,0),RGB(0,0,0));
		return; 
	case splitBar: 
		//重画分割条,天蓝色 
		pDC->FillSolidRect(rc,RGB(100,200,255));	
		return; 
	default: 
		ASSERT(FALSE); 
	} 
	// fill the middle
	pDC->FillSolidRect(rc, RGB(0,0,0));
}

void CMySplitterWnd::OnInvertTracker(const CRect &rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);
	
	// pat-blt without clip children on
	CDC* pDC = GetDC();
	// invert the brush pattern (looks just like frame window sizing)
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);

}

void CMySplitterWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CSplitterWnd::OnLButtonDown(nFlags, point);
}
