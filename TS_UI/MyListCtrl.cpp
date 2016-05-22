#include "StdAfx.h"
#include "MyListCtrl.h"

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
CMyListCtrl::CMyListCtrl(void): m_nRowHeight(0), m_fontHeight(12), m_fontWith(0)
{
	m_color = RGB(0,0,0);
}


CMyListCtrl::~CMyListCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CMyListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

void CMyListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CMyListCtrl::SetRowHeigt(int nHeight)
{
	m_nRowHeight = nHeight;
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpMeasureItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpMeasureItemStruct->hDC);
	CRect rtClient;
	GetClientRect(&rtClient);
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpMeasureItemStruct->rcItem.left += lvcprev.cx ;
			lpMeasureItemStruct->rcItem.right += lpMeasureItemStruct->rcItem.left; 
		}
		
		CRect rcItem;   
		if (!GetSubItemRect(lpMeasureItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpMeasureItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol==0)
		{
			rcTemp.left -=2;
		}

		if ( lpMeasureItemStruct->itemState & ODS_SELECTED )
		{
			//高亮显示选中行
			pDC->FillSolidRect(&rcTemp, GetSysColor(COLOR_WINDOW)) ;//设定选中高亮色为背景色
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			COLORREF color;
			color = GetBkColor();
			pDC->FillSolidRect(rcTemp,color);

			if (FindColColor(nCol,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			if (FindItemColor(nCol,lpMeasureItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}

			pDC->SetTextColor(m_color);
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol]=='0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_Format[nCol]=='1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_Format[nCol]=='2')
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);
		pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol,color))
		{
			pDC->SetTextColor(color);
		}
		if (FindItemTextColor(nCol,lpMeasureItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		CFont nFont ,* nOldFont; 
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,m_fontWeight,FALSE,FALSE,0,0,0,0,0,0,_TEXT("幼圆"));//创建字体 
		nOldFont = pDC->SelectObject(&nFont);
		//调整文本的位置
		rcItem.left += 5; 
		rcItem.right -= 1; 
		DrawText(lpMeasureItemStruct->hDC, lpBuffer, strlen(lpBuffer), &rcItem, uFormat) ;//写入文本

		pDC->SelectStockObject(SYSTEM_FONT);

	}
}



void CMyListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)//点击水平滚动条
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(GetParent()->m_hWnd, WM_MYLISTCTRL, 0, 0);
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMyListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)//点击垂直滚动条
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(GetParent()->m_hWnd, WM_MYLISTCTRL, 0, 0);//非阻塞发送消息

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CMyListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)//鼠标滚轮事件
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::PostMessage(GetParent()->m_hWnd, WM_MYLISTCTRL, 0, 0);//非阻塞发送消息

	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

//重载PreSubclassWindow()，修改控件类型为自绘模式，然后子类化表头
void CMyListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}


// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CMyListCtrl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// 设置表头高度
void CMyListCtrl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}
bool CMyListCtrl::FindColColor(int col,COLORREF &color) //查找列颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CMyListCtrl::FindItemColor(int col,int row,COLORREF &color) //查找颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CMyListCtrl::SetColColor(int col,COLORREF color) //设置列颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}
void CMyListCtrl::SetItemColor(int col,int row,COLORREF color) //设置格子颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);
}


void CMyListCtrl::SetHeaderFontHW(int nHeight,int nWith) //设置头部字体宽和高
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CMyListCtrl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}
BOOL CMyListCtrl::SetTextColor(COLORREF cr)  //设置字体颜色
{
	m_color = cr;
	return TRUE;
}
void CMyListCtrl::SetFontHW(int nHeight,int nWith, int nWeight/*=0*/) //设置字体高和宽
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
	m_fontWeight = nWeight;
}

void CMyListCtrl::SetColTextColor(int col,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}
bool CMyListCtrl::FindColTextColor(int col,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CMyListCtrl::FindItemTextColor(int col,int row,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CMyListCtrl::SetItemTextColor(int col,int row,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);
}


int CMyListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat==LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + "0";
	}
	else if (nFormat==LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	else if (nFormat==LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + "2";
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}