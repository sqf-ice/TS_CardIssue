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


// CCleanRibbonApplicationButton ³ÉÔ±º¯Êý
void CMyRibbonApplicationButton::OnLButtonDblClk(CPoint point)
{
	return ;
}

void CMyRibbonApplicationButton::OnLButtonDown(CPoint point)
{
	return;
}