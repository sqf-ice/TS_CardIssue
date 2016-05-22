#include "StdAfx.h"
#include "MyRibbonBar.h"


CMyRibbonBar::CMyRibbonBar(void)
{
}


CMyRibbonBar::~CMyRibbonBar(void)
{
}

IMPLEMENT_DYNAMIC(CMyRibbonBar, CMFCRibbonBar)

BEGIN_MESSAGE_MAP(CMyRibbonBar, CMFCRibbonBar)
END_MESSAGE_MAP()

//屏蔽右键弹出 添加快速访问栏 等功能菜单
BOOL CMyRibbonBar::OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit)
{
	//return CMFCRibbonBar::OnShowRibbonContextMenu(pWnd, x, y, pHit);
	return FALSE;
}

//屏蔽左上角 QAT 按钮的功能
BOOL CMyRibbonBar::OnShowRibbonQATMenu( CWnd* pWnd,int x,int y,CMFCRibbonBaseElement* pHit )
{
	//return CMFCRibbonBar::OnShowRibbonQATMenu( pWnd, x, y, pHit );
	return FALSE;
}

//隐藏 QAT 图标
BOOL CMyRibbonBar::LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType , HINSTANCE hInstance)
{
	BOOL bl= CMFCRibbonBar::LoadFromResource( uiXMLResID,lpszResType , hInstance );
	m_QAToolbar.RemoveAll();	
	return bl;
}