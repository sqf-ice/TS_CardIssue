#if !defined(AFX_LISTPANEVIEW_H__C47BE275_1F84_4A04_B415_F3B63D1AD5A8__INCLUDED_)
#define AFX_LISTPANEVIEW_H__C47BE275_1F84_4A04_B415_F3B63D1AD5A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListPaneView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListPaneView view
#include <afxcview.h>
class CListPaneView : public CListView
{
protected:
	CListPaneView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CListPaneView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListPaneView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CListPaneView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CListPaneView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTPANEVIEW_H__C47BE275_1F84_4A04_B415_F3B63D1AD5A8__INCLUDED_)
