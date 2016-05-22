// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TS_UI.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "Script/tool.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CSettingDlg::OnNMClickList2)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CSettingDlg::OnNMRClickList2)
ON_MESSAGE(WM_MYLISTCTRL, &CSettingDlg::OnMylistctrl)
ON_CBN_SELCHANGE(IDC_COMBOX_CREATEID, &CSettingDlg::OnSelchangeCombo1)
ON_COMMAND(ID_MenuAdd, &CSettingDlg::OnMenuadd)
ON_COMMAND(ID_MenuDelte, &CSettingDlg::OnMenudelte)
ON_COMMAND(ID_MenuClear, &CSettingDlg::OnMenuclear)
ON_BN_CLICKED(IDC_BUTTON1, &CSettingDlg::ResetClParam)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化ListCtrl控件的视图样式
	haveeditcreate = false;//初始化标志位，表示还没有创建编辑框
	haveccomboboxcreate = false;//初始化标志位，表示还没有创建下拉列表框
	RECT  m_rect;
	m_list.GetClientRect(&m_rect); //获取list的客户区,方便调节每一列的宽度  
	//设置list风格，LVS_EX_GRIDLINES为网格线（只适用与report风格的listctrl）
	//LVS_EX_FULLROWSELECT为选中某行使整行高亮（只适用与report风格的listctrl）
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); 

	m_list.InsertColumn(0, "行", LVCFMT_CENTER, m_rect.right / 51 * 2);
	m_list.InsertColumn(1, "目录级别", LVCFMT_LEFT, m_rect.right / 51 * 6);
	m_list.InsertColumn(2, "应用环境", LVCFMT_LEFT, m_rect.right / 51 * 7);
	m_list.InsertColumn(3, "FID", LVCFMT_LEFT, m_rect.right / 51 * 3);
	m_list.InsertColumn(4, "AID", LVCFMT_LEFT, m_rect.right / 51 * 17);
	m_list.InsertColumn(5, "应用的主控密钥值", LVCFMT_LEFT, m_rect.right / 51 * 18);
	m_list.SetRowHeigt(30);//设置行高
	m_list.SetHeaderHeight(1.5);//设置头部高度
	m_list.SetHeaderBKColor(255, 117, 117, 2);//设置头部背景色
	m_list.SetHeaderTextColor(RGB(255,255,255));//设置头部字体颜色
	m_list.SetFontHW(16,0);//设置字体高度、宽度和磅数,0表示缺省宽度
	m_list.SetTextColor(RGB(0, 50, 255));  //设置文本颜色
	m_list.SetColTextColor(0,RGB(0,0,0)); //设置列文本颜色
	m_list.SetColColor(0, RGB(223,227,232));  //设置列颜色
	//m_list.SetHeaderFontHW(16,0);//设置头部字体大小
	//m_list.SetBkColor(RGB(255,255,255));        //设置背景色
	//m_list.SetItemColor(1,1,RGB(100,100,10)); //设置指定单元背景色
	//m_list.SetItemTextColor(3,1,RGB(255,0,0));  //设置单元格字体颜色
	SetConfParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//限制输入文本框只能输入十六进制字符
	if ( pMsg->message == WM_CHAR && GetFocus() == (&m_Edit) )
	{
		//退格键和同时按下的Ctrl按键都生效
		if( pMsg->wParam==VK_BACK || ::GetKeyState(VK_CONTROL) < 0 || (pMsg->wParam<='9' && pMsg->wParam>='0') || (pMsg->wParam<='F' && pMsg->wParam>='A') || (pMsg->wParam<='f' && pMsg->wParam>='a') )
		{}
		else
			return TRUE;//输入非十六进制数据的话，拦截消息
	}

	//屏蔽回车键和Esc按键消息
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return TRUE;    
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE; 

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSettingDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)//创建单元格编辑框函数
	//pEditCtrl为列表对象指针，createdit为编辑框指针对象，
	//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的列赋值给“刚编辑过的列”以便后期处理
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);//创建编辑框对象,IDC_EDIT_CREATEID为控件ID号3000，在文章开头定义
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉
	createdit->SetParent(&m_list);//将list control设置为父窗口,生成的Edit才能正确定位,这个也很重要
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	//EditRect.SetRect(EditRect.left+1, EditRect.top+1, EditRect.left + m_list.GetColumnWidth(e_SubItem)-1, EditRect.bottom-1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	EditRect.SetRect(EditRect.left+5, EditRect.top+8, EditRect.left + m_list.GetColumnWidth(e_SubItem)-1, EditRect.bottom-1);
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//获得相应单元格字符
	createdit->SetWindowText(strItem);//将单元格字符显示在编辑框上
	createdit->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上
	createdit->ShowWindow(SW_SHOW);//显示编辑框在单元格上面
	createdit->SetFocus();//设置为焦点 
	createdit->SetSel(-1);//设置光标在文本框文字的最后
	if ( SubItem == 5 )//密钥限制最长输入32位
		createdit->SetLimitText(32);
	if ( SubItem == 3 )//FID限制最长输入4位
		createdit->SetLimitText(4);
}

void CSettingDlg::distroyEdit(CListCtrl *list,CEdit* distroyedit, int &Item, int &SubItem)
{
	CString meditdata;
	distroyedit->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//获得相应单元格字符
	distroyedit->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
}

void CSettingDlg::createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//创建单元格下拉列表框函数
	//pEditCtrl为列表对象指针，createccombobox为下拉列表框指针对象，
	//Item为创建单元格在列表中的行，SubItem则为列，havecreat为对象创建标准
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的列赋值给“刚编辑过的列”以便后期处理
	havecreat = true;
	CRect rect(0, 0, 0, 0);
	rect.bottom += 100;//下拉框显示的下拉列表的高度
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE |  CBS_DROPDOWNLIST | CBS_OEMCONVERT, rect, this, IDC_COMBOX_CREATEID);
	createccomboboxobj->SetItemHeight(0,15);//下拉列表的行间距
	createccomboboxobj->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉
	createccomboboxobj->SetParent(&m_list);//将list control设置为父窗口,生成的Ccombobox才能正确定位,这个也很重要
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	//EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	EditRect.SetRect(EditRect.left+1, EditRect.top+6, EditRect.left + m_list.GetColumnWidth(e_SubItem)-1, EditRect.bottom);
	createccomboboxobj->MoveWindow(&EditRect);//将下拉框位置放在相应单元格上
	createccomboboxobj->ShowWindow(SW_SHOW);//显示下拉框在单元格上面
}

void CSettingDlg::distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	distroyccomboboxobj->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//更新相应单元格字符
	distroyccomboboxobj->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
}

void CSettingDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)//左键点击事件显示编辑框和下拉框
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//行 pEditCtrl->iItem	列 pEditCtrl->iSubItem
	if (pEditCtrl->iItem==-1 || pEditCtrl->iSubItem == 0 )//点击到非工作区或者点到第一列
	{
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem != 1 && pEditCtrl->iSubItem != 2 )//如果点中的不是下拉框列
	{
		if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的
			{
				distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
			}
			else//点中的单元格是之前创建好的
			{
				m_Edit.SetFocus();//设置为焦点 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的列赋值给“刚编辑过的列”以便后期处理
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
		}
	}
	else//点中第一第二列，则在单元格处生成下拉列表项
	{
		if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的
			{
				distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
				if ( pEditCtrl->iSubItem == 1 )//当前点中的是第一列
				{
					m_comBox.AddString("一级目录");
					m_comBox.AddString("二级目录");
				}
				else if ( pEditCtrl->iSubItem == 2 )//当前点中的是第二列
				{
					if ( m_list.GetItemText(pEditCtrl->iItem, 1) == "一级目录" )
						m_comBox.AddString("MF");
					else
					{
						m_comBox.AddString("社保应用");
						m_comBox.AddString("非社保应用");
					}
				}
				SetComboxSelect();
				m_comBox.ShowDropDown();//单击自动显示下拉列表
			}
			else//点中的单元格是之前创建好的
			{
				m_comBox.SetFocus();//设置为焦点 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
			if ( pEditCtrl->iSubItem == 1 )//当前点中的是第一列
			{
				m_comBox.AddString("一级目录");
				m_comBox.AddString("二级目录");
			}
			else if ( pEditCtrl->iSubItem == 2 )//当前点中的是第二列
			{
				if ( m_list.GetItemText(pEditCtrl->iItem, 1) == "一级目录" )
					m_comBox.AddString("MF");
				else
				{
					m_comBox.AddString("社保应用");
					m_comBox.AddString("非社保应用");
				}
			}
			SetComboxSelect();
			m_comBox.ShowDropDown();//单击自动显示下拉列表
		}
	}

	*pResult = 0;
}

void CSettingDlg::SetComboxSelect()
{
	CString strTmp;
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//获得ListCtrl单元格字符
	for ( int i=0; i<m_comBox.GetCount(); ++i )
	{
		m_comBox.GetLBText(i, strTmp);//获取下拉框的文本
		if ( strTmp.CompareNoCase(strItem)==0 )//找到匹配的数据,选中该下拉列表框的该项
		{
			m_comBox.SetCurSel(i);
			return;
		}
	}
	m_comBox.SetCurSel(0);
}

HBRUSH CSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CFont m_font;
	CBrush m_brush;
	m_font.CreatePointFont(120,"幼圆");
	m_brush.CreateSolidBrush(RGB(227,237,205));//画刷颜色
	if ( nCtlColor == CTLCOLOR_STATIC ) //更改静态控件的颜色
	{
		UINT uID = pWnd->GetDlgCtrlID(); //获取控件ID集合
		pDC->SetBkMode( TRANSPARENT );
		if ( uID == IDC_STATIC1 )
		{
			pDC->SetTextColor(RGB(0, 50, 255));
			pDC->SelectObject(&m_font);
		}
		if ( uID == IDC_STATIC3 )
		{
			pDC->SetTextColor(RGB(229, 20, 0));
			pDC->SelectObject(&m_font);
		}
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CSettingDlg::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)//右键显示功能菜单
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	bool bBtState;//用来标记右键菜单的添加和删除按钮是否变为灰色 false变灰

	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}

	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	m_nCurRow = pEditCtrl->iItem;
	if (pEditCtrl->iItem==-1)//点击到非工作区,屏蔽掉右键菜单的删除功能
		bBtState = FALSE;
	else
		bBtState = TRUE;

	POINT pt;
	GetCursorPos(&pt);//获取光标的位置，以屏幕坐标表示
	//弹出menu菜单
	CMenu temp,*ptr;
	temp.LoadMenuA(IDR_MENU1);
	ptr = temp.GetSubMenu(0); 
	if ( bBtState )
		ptr->EnableMenuItem(ID_MenuDelte,MF_ENABLED);
	else
		ptr->EnableMenuItem(ID_MenuDelte,MF_GRAYED);
	//在指定位置显示浮动弹出菜单，并追踪弹出菜单中被选择的项
	ptr->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
	
	*pResult = 0;
}

//自定义消息，用来销毁ListCtrl中创建的编辑框和下拉框控件，防止滚动滚动条时候控件位置错乱,滚动条的消息在MyListCtrl中处理，因为滚动条是MyListCtrl的子控件，对话框的响应事件不响应，只能在MyListCtrl中处理消息
afx_msg LRESULT CSettingDlg::OnMylistctrl(WPARAM wParam, LPARAM lParam)
{
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}
	return 0;
}

void CSettingDlg::InsertNewLine( int nRow, CString str1 /*= ""*/, CString str2/*=""*/, CString str3/*=""*/, CString str4/*="" */, CString str5/*="" */ )
{
	if ( str1 == "" )//添加一行默认数据
	{
		CString strSeq; 
		strSeq.Format( "%d", nRow+1 );
		m_list.InsertItem(nRow, strSeq);
		m_list.SetItemText(nRow, 1, "一级目录");
		m_list.SetItemText(nRow, 2, "MF");
		m_list.SetItemText(nRow, 3, "3F00");
		m_list.SetItemText(nRow, 4, "A0000000030000");
		m_list.SetItemText(nRow, 5, "11111111111111111111111111111111");
	}
	else
	{
		CString strSeq; 
		strSeq.Format( "%d", nRow+1 );
		m_list.InsertItem(nRow, strSeq);
		m_list.SetItemText(nRow, 1, str1);
		m_list.SetItemText(nRow, 2, str2);
		m_list.SetItemText(nRow, 3, str3);
		m_list.SetItemText(nRow, 4, str4);
		m_list.SetItemText(nRow, 5, str5);
	}
}

void CSettingDlg::UpdataListSeq()
{
	CString strSeq;
	for ( int i=0; i<m_list.GetItemCount(); ++i)
	{
		strSeq.Format( "%d", i+1 );
		m_list.SetItemText(i, 0, strSeq);
	}
}

void CSettingDlg::OnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str1;
	if ( e_SubItem == 1 )//选择第一列的时候
	{
		m_comBox.GetLBText(m_comBox.GetCurSel(), str1);
		if ( str1 == "一级目录" )
		{
			m_list.SetItemText(e_Item, e_SubItem+1, "MF");
			m_list.SetItemText(e_Item, e_SubItem+2, "3F00");
			m_list.SetItemText(e_Item, e_SubItem+3, "A0000000030000");
		}
		else if ( str1 == "二级目录" )
		{
			m_list.SetItemText(e_Item, e_SubItem+1, "社保应用");
			m_list.SetItemText(e_Item, e_SubItem+2, "");
			m_list.SetItemText(e_Item, e_SubItem+3, "");
		}
	}
}

void CSettingDlg::OnMenuadd()
{
	// TODO: 在此添加命令处理程序代码
	int nColum = m_list.GetItemCount();//在最后添加新行数据
	InsertNewLine(nColum);
}

void CSettingDlg::OnMenudelte()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteItem(m_nCurRow);
	UpdataListSeq();
}

void CSettingDlg::OnMenuclear()
{
	// TODO: 在此添加命令处理程序代码
	m_list.DeleteAllItems();
}

bool CSettingDlg::SaveKeysList(vector<CLEARCARDPARAM> &vecClearCardParam)
{
	CLEARCARDPARAM struParam;
	CString strText,strWarn;
	vecClearCardParam.clear();
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}

	//检测数据的合法性
	for ( int i=0; i<m_list.GetItemCount(); ++i)
	{
		//检测第四列FID数据合法性
		strText = m_list.GetItemText(i, 3);
		if ( strText.GetLength() != 4 )
		{
			strWarn.Format("错误：第【%d】行【FID】列FID数据的长度不够4位,请修改！", i+1);
			GetDlgItem(IDC_STATIC3)->SetWindowText(strWarn);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			return false;
		}
		//检测第五列AID数据的位数是否为偶数位
		strText = m_list.GetItemText(i, 4);
		if ( strText.GetLength()%2 != 0 || strText=="" )
		{
			strWarn.Format("错误：第【%d】行【AID】列数据的长度必须为偶数且数据不能为空,请修改！", i+1);
			GetDlgItem(IDC_STATIC3)->SetWindowText(strWarn);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			return false;
		}
		//检测第六列密钥数据合法性
		strText = m_list.GetItemText(i, 5);
		if ( strText.GetLength() != 32 )
		{
			strWarn.Format("错误：第【%d】行【应用的主控密钥值】列密钥数据的长度不够32位,请修改！", i+1);
			GetDlgItem(IDC_STATIC3)->SetWindowText(strWarn);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			return false;
		}
	}
	//数据全部正常保存数据
	for ( int i=0; i<m_list.GetItemCount(); ++i)
	{
		struParam.m_strDirLv = m_list.GetItemText(i, 1);
		struParam.m_strAE = m_list.GetItemText(i, 2);
		struParam.m_strFID = m_list.GetItemText(i, 3);
		struParam.m_strAID = m_list.GetItemText(i, 4);
		struParam.m_strMK = m_list.GetItemText(i, 5);
		vecClearCardParam.push_back(struParam);
	}
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	return true;
}

void CSettingDlg::SetConfParam()
{
	vector<CLEARCARDPARAM> vecClearCardParam;
	string strPath = "ScriptConfig.xml";
	CString strDir,strAE,strFID,strAID,strMK;
	int nColum;

	m_cOpXml.ReadKeysDatas(strPath,vecClearCardParam);
	vector<CLEARCARDPARAM>::const_iterator iter = vecClearCardParam.begin();
	while ( iter != vecClearCardParam.end() )
	{
		//检测配置读取的数据的合法性，并做自动纠错
		if ( (*iter).m_strDirLv=="1" )
		{
			strDir="一级目录";
			strAE="MF";
		}
		else if ( (*iter).m_strDirLv=="2" )
		{
			strDir="二级目录";
			if ( (*iter).m_strAE=="3" )
				strAE="非社保应用";
			else
				strAE="社保应用";
		}
		else
		{
			if ( (*iter).m_strAE=="1" )
			{
				strDir="一级目录";
				strAE="MF";
			}
			else if ( (*iter).m_strAE=="3" )
			{
				strDir="二级目录";
				strAE="非社保应用";
			}
			else
			{
				strDir="二级目录";
				strAE="社保应用";
			}
		}

		strFID.Format("%s",(*iter).m_strFID.c_str());
		if ( (*iter).m_strFID.length()%2!=0 || (*iter).m_strFID.empty() || !JudgeHex((*iter).m_strFID) )
		{
			if ( strAE=="MF" )
				strFID="3F00";
			else if ( strAE=="社保应用" )
				strFID="DDF1";
			else if ( strAE=="非社保应用" )
				strFID="DDF2";
		}

		strAID.Format("%s",(*iter).m_strAID.c_str());
		if ( (*iter).m_strAID.length()%2!=0 || (*iter).m_strAID.empty() || !JudgeHex((*iter).m_strAID) )
		{
			if ( strAE=="MF" )
				strAID="A0000000030000";
			else if ( strAE=="社保应用" )
				strAID="7378312E73682EC9E7BBE1B1A3D5CF";
			else if ( strAE=="非社保应用" )
				strAID="315041592E5359532E4444463031";
		}

		strMK.Format("%s",(*iter).m_strMK.c_str());
		if ( (*iter).m_strMK.length()!=32 || !JudgeHex((*iter).m_strMK) )
			strMK = "11111111111111111111111111111111";

		nColum = m_list.GetItemCount();
		InsertNewLine(nColum, strDir, strAE, strFID, strAID, strMK);
		++iter;
	}

	if ( vecClearCardParam.size()==0 )
		ResetClParam();

}


void CSettingDlg::ResetClParam()
{
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();
	InsertNewLine(0, "一级目录", "MF", "3F00", "A0000000030000", "11111111111111111111111111111111");//添加MF数据
	InsertNewLine(1, "二级目录", "社保应用", "DDF1", "7378312E73682EC9E7BBE1B1A3D5CF", "11111111111111111111111111111111");//添加SSSE数据
	InsertNewLine(2, "二级目录", "非社保应用", "DDF2", "315041592E5359532E4444463031", "11111111111111111111111111111111");//添加PSE数据
}
