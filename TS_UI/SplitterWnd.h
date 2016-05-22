#if !defined(AFX_SPLITTERWND_H__5A32C2D9_8383_4046_9F1D_7B3F6F36BD6D__INCLUDED_)
#define AFX_SPLITTERWND_H__5A32C2D9_8383_4046_9F1D_7B3F6F36BD6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx window

class CMySplitterWnd : public CSplitterWnd
{
// Construction
public:
	CMySplitterWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWndEx)
	//}}AFX_VIRTUAL
//	virtual void OnDrawSplitter( CDC* pDC, ESplitType nType, const CRect& rect );
//	virtual void OnInvertTracker( const CRect& rect );

// Implementation
public:
	void OnInvertTracker(const CRect& rect);
	void OnDrawSplitter( CDC* pDC, ESplitType nType, const CRect& rectArg );
	virtual ~CMySplitterWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterWndEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTERWND_H__5A32C2D9_8383_4046_9F1D_7B3F6F36BD6D__INCLUDED_)
