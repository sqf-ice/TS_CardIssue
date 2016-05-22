#if !defined(AFX_LEFTPANEVIEW_H__941A5336_7928_49A4_8605_25DB77B48870__INCLUDED_)
#define AFX_LEFTPANEVIEW_H__941A5336_7928_49A4_8605_25DB77B48870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftPaneView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftPaneView view
#include <afxcview.h>
class CLeftPaneView : public CTreeView
{
protected:
	CLeftPaneView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLeftPaneView)

// Attributes
public:
	CImageList m_ImageList;
	CTreeCtrl* ptheTree;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftPaneView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLeftPaneView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLeftPaneView)
	//afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTPANEVIEW_H__941A5336_7928_49A4_8605_25DB77B48870__INCLUDED_)
