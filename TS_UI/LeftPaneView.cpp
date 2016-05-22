// LeftPaneView.cpp : implementation file
//

#include "stdafx.h"
#include "LeftPaneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftPaneView

IMPLEMENT_DYNCREATE(CLeftPaneView, CTreeView)

CLeftPaneView::CLeftPaneView()
{
}

CLeftPaneView::~CLeftPaneView()
{
}


BEGIN_MESSAGE_MAP(CLeftPaneView, CTreeView)
	//{{AFX_MSG_MAP(CLeftPaneView)
	//ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftPaneView drawing

void CLeftPaneView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLeftPaneView diagnostics

#ifdef _DEBUG
void CLeftPaneView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftPaneView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftPaneView message handlers

void CLeftPaneView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();	
	// TODO: Add your specialized code here and/or call the base class
	//���������
	::SetWindowLong(m_hWnd,GWL_STYLE,WS_VISIBLE|WS_TABSTOP 
		|WS_CHILD|TVS_HASBUTTONS 
		|TVS_LINESATROOT|TVS_HASLINES);
	ptheTree=&GetTreeCtrl();
	ptheTree->SetBkColor(RGB(255,255,255));//���ñ���ɫ
}


//void CLeftPaneView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	// TODO: Add your control notification handler code here
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CTreeCtrl& treeCtrl=GetTreeCtrl();
//	CString str;
//	//��ȡ��ǰѡ�е�����
//	HTREEITEM hSelectedItem=treeCtrl.GetSelectedItem();
//	//��ȡ��ǰѡ�е������ı�
//	str=treeCtrl.GetItemText(hSelectedItem);
//	if(str.Find("�༭")!=-1)
//	{
//		//�л����༭��ͼ
//		pFrame->SwitchToView(EDITVIEW);
//	}
//	*pResult = 0;
//}
