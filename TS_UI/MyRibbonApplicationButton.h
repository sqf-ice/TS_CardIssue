#pragma once
class CMyRibbonApplicationButton : public CMFCRibbonApplicationButton
{

	DECLARE_DYNCREATE(CMyRibbonApplicationButton);

public:
	CMyRibbonApplicationButton(void);
	~CMyRibbonApplicationButton(void);

protected:
	virtual void OnLButtonDown(CPoint point);
	virtual void OnLButtonDblClk(CPoint point);
};

