#pragma once


// CRightView 视图

class CRightView : public CRichEditView
{
	DECLARE_DYNCREATE(CRightView)

protected:
	CRightView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRightView();
	int ShowLineNo(HWND hWnd);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
public:
	CRichEditCtrl* pEditCtrl;

	virtual void OnInitialUpdate();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


