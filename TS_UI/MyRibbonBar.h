#pragma once


class CMyRibbonBar:public CMFCRibbonBar
{
	DECLARE_DYNAMIC(CMyRibbonBar);

public:
	CMyRibbonBar(void);
	~CMyRibbonBar(void); 

	virtual BOOL LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType = RT_RIBBON, HINSTANCE hInstance = NULL);

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	virtual BOOL OnShowRibbonQATMenu(CWnd* pWnd,int x,int y,CMFCRibbonBaseElement* pHit);
};
