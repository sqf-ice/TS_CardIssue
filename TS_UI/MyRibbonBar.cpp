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

//�����Ҽ����� ��ӿ��ٷ����� �ȹ��ܲ˵�
BOOL CMyRibbonBar::OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit)
{
	//return CMFCRibbonBar::OnShowRibbonContextMenu(pWnd, x, y, pHit);
	return FALSE;
}

//�������Ͻ� QAT ��ť�Ĺ���
BOOL CMyRibbonBar::OnShowRibbonQATMenu( CWnd* pWnd,int x,int y,CMFCRibbonBaseElement* pHit )
{
	//return CMFCRibbonBar::OnShowRibbonQATMenu( pWnd, x, y, pHit );
	return FALSE;
}

//���� QAT ͼ��
BOOL CMyRibbonBar::LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType , HINSTANCE hInstance)
{
	BOOL bl= CMFCRibbonBar::LoadFromResource( uiXMLResID,lpszResType , hInstance );
	m_QAToolbar.RemoveAll();	
	return bl;
}