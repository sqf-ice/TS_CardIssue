#include "StdAfx.h"
#include "MyRibbonApplicationButton.h"


CMyRibbonApplicationButton::CMyRibbonApplicationButton():CMFCRibbonApplicationButton()
{
}

CMyRibbonApplicationButton::~CMyRibbonApplicationButton(void)
{
}

// CCleanRibbonApplicationButton
IMPLEMENT_DYNCREATE(CMyRibbonApplicationButton,CMFCRibbonApplicationButton)


// CCleanRibbonApplicationButton ��Ա����
void CMyRibbonApplicationButton::OnLButtonDblClk(CPoint point)
{
	return ;
}

void CMyRibbonApplicationButton::OnLButtonDown(CPoint point)
{
	return;
}