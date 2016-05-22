// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TS_UI.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "Script/tool.h"


// CSettingDlg �Ի���

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


// CSettingDlg ��Ϣ�������


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��ListCtrl�ؼ�����ͼ��ʽ
	haveeditcreate = false;//��ʼ����־λ����ʾ��û�д����༭��
	haveccomboboxcreate = false;//��ʼ����־λ����ʾ��û�д��������б��
	RECT  m_rect;
	m_list.GetClientRect(&m_rect); //��ȡlist�Ŀͻ���,�������ÿһ�еĿ��  
	//����list���LVS_EX_GRIDLINESΪ�����ߣ�ֻ������report����listctrl��
	//LVS_EX_FULLROWSELECTΪѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); 

	m_list.InsertColumn(0, "��", LVCFMT_CENTER, m_rect.right / 51 * 2);
	m_list.InsertColumn(1, "Ŀ¼����", LVCFMT_LEFT, m_rect.right / 51 * 6);
	m_list.InsertColumn(2, "Ӧ�û���", LVCFMT_LEFT, m_rect.right / 51 * 7);
	m_list.InsertColumn(3, "FID", LVCFMT_LEFT, m_rect.right / 51 * 3);
	m_list.InsertColumn(4, "AID", LVCFMT_LEFT, m_rect.right / 51 * 17);
	m_list.InsertColumn(5, "Ӧ�õ�������Կֵ", LVCFMT_LEFT, m_rect.right / 51 * 18);
	m_list.SetRowHeigt(30);//�����и�
	m_list.SetHeaderHeight(1.5);//����ͷ���߶�
	m_list.SetHeaderBKColor(255, 117, 117, 2);//����ͷ������ɫ
	m_list.SetHeaderTextColor(RGB(255,255,255));//����ͷ��������ɫ
	m_list.SetFontHW(16,0);//��������߶ȡ���ȺͰ���,0��ʾȱʡ���
	m_list.SetTextColor(RGB(0, 50, 255));  //�����ı���ɫ
	m_list.SetColTextColor(0,RGB(0,0,0)); //�������ı���ɫ
	m_list.SetColColor(0, RGB(223,227,232));  //��������ɫ
	//m_list.SetHeaderFontHW(16,0);//����ͷ�������С
	//m_list.SetBkColor(RGB(255,255,255));        //���ñ���ɫ
	//m_list.SetItemColor(1,1,RGB(100,100,10)); //����ָ����Ԫ����ɫ
	//m_list.SetItemTextColor(3,1,RGB(255,0,0));  //���õ�Ԫ��������ɫ
	SetConfParam();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//���������ı���ֻ������ʮ�������ַ�
	if ( pMsg->message == WM_CHAR && GetFocus() == (&m_Edit) )
	{
		//�˸����ͬʱ���µ�Ctrl��������Ч
		if( pMsg->wParam==VK_BACK || ::GetKeyState(VK_CONTROL) < 0 || (pMsg->wParam<='9' && pMsg->wParam>='0') || (pMsg->wParam<='F' && pMsg->wParam>='A') || (pMsg->wParam<='f' && pMsg->wParam>='a') )
		{}
		else
			return TRUE;//�����ʮ���������ݵĻ���������Ϣ
	}

	//���λس�����Esc������Ϣ
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE)
		return TRUE;    
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE; 

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSettingDlg::createEdit(NM_LISTVIEW  *pEditCtrl, CEdit *createdit, int &Item, int &SubItem, bool &havecreat)//������Ԫ��༭����
	//pEditCtrlΪ�б����ָ�룬createditΪ�༭��ָ�����
	//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);//�����༭�����,IDC_EDIT_CREATEIDΪ�ؼ�ID��3000�������¿�ͷ����
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//��������,����������Ļ�������ֻ��ͻأ�ĸо�
	createdit->SetParent(&m_list);//��list control����Ϊ������,���ɵ�Edit������ȷ��λ,���Ҳ����Ҫ
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ
	//EditRect.SetRect(EditRect.left+1, EditRect.top+1, EditRect.left + m_list.GetColumnWidth(e_SubItem)-1, EditRect.bottom-1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������
	EditRect.SetRect(EditRect.left+5, EditRect.top+8, EditRect.left + m_list.GetColumnWidth(e_SubItem)-1, EditRect.bottom-1);
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//�����Ӧ��Ԫ���ַ�
	createdit->SetWindowText(strItem);//����Ԫ���ַ���ʾ�ڱ༭����
	createdit->MoveWindow(&EditRect);//���༭��λ�÷�����Ӧ��Ԫ����
	createdit->ShowWindow(SW_SHOW);//��ʾ�༭���ڵ�Ԫ������
	createdit->SetFocus();//����Ϊ���� 
	createdit->SetSel(-1);//���ù�����ı������ֵ����
	if ( SubItem == 5 )//��Կ���������32λ
		createdit->SetLimitText(32);
	if ( SubItem == 3 )//FID���������4λ
		createdit->SetLimitText(4);
}

void CSettingDlg::distroyEdit(CListCtrl *list,CEdit* distroyedit, int &Item, int &SubItem)
{
	CString meditdata;
	distroyedit->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//�����Ӧ��Ԫ���ַ�
	distroyedit->DestroyWindow();//���ٶ����д�����Ҫ�����٣���Ȼ�ᱨ��
}

void CSettingDlg::createCcombobox(NM_LISTVIEW  *pEditCtrl, CComboBox *createccomboboxobj, int &Item, int &SubItem, bool &havecreat)//������Ԫ�������б����
	//pEditCtrlΪ�б����ָ�룬createccomboboxΪ�����б��ָ�����
	//ItemΪ������Ԫ�����б��е��У�SubItem��Ϊ�У�havecreatΪ���󴴽���׼
{
	Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
	havecreat = true;
	CRect rect(0, 0, 0, 0);
	rect.bottom += 100;//��������ʾ�������б�ĸ߶�
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE |  CBS_DROPDOWNLIST | CBS_OEMCONVERT, rect, this, IDC_COMBOX_CREATEID);
	createccomboboxobj->SetItemHeight(0,15);//�����б���м��
	createccomboboxobj->SetFont(this->GetFont(), FALSE);//��������,����������Ļ�������ֻ��ͻأ�ĸо�
	createccomboboxobj->SetParent(&m_list);//��list control����Ϊ������,���ɵ�Ccombobox������ȷ��λ,���Ҳ����Ҫ
	CRect  EditRect;
	m_list.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//��ȡ��Ԫ��Ŀռ�λ����Ϣ
	//EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + m_list.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1��-1�����ñ༭�����ڵ�ס�б���е�������
	EditRect.SetRect(EditRect.left+1, EditRect.top+6, EditRect.left + m_list.GetColumnWidth(e_SubItem)-1, EditRect.bottom);
	createccomboboxobj->MoveWindow(&EditRect);//��������λ�÷�����Ӧ��Ԫ����
	createccomboboxobj->ShowWindow(SW_SHOW);//��ʾ�������ڵ�Ԫ������
}

void CSettingDlg::distroyCcombobox(CListCtrl *list, CComboBox* distroyccomboboxobj, int &Item, int &SubItem)
{
	CString meditdata;
	distroyccomboboxobj->GetWindowText(meditdata);
	list->SetItemText(Item, SubItem, meditdata);//������Ӧ��Ԫ���ַ�
	distroyccomboboxobj->DestroyWindow();//���ٶ����д�����Ҫ�����٣���Ȼ�ᱨ��
}

void CSettingDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)//�������¼���ʾ�༭���������
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//�� pEditCtrl->iItem	�� pEditCtrl->iSubItem
	if (pEditCtrl->iItem==-1 || pEditCtrl->iSubItem == 0 )//������ǹ��������ߵ㵽��һ��
	{
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
	}
	else if (pEditCtrl->iSubItem != 1 && pEditCtrl->iSubItem != 2 )//������еĲ�����������
	{
		if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�
		{
			distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
			haveccomboboxcreate = false;
		}
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�
			{
				distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//�����༭��
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�
			{
				m_Edit.SetFocus();//����Ϊ���� 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			e_SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//�����༭��
		}
	}
	else//���е�һ�ڶ��У����ڵ�Ԫ�����������б���
	{
		if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
		{
			distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);
			haveeditcreate = false;
		}
		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//������еĵ�Ԫ����֮ǰ�����õ�
			{
				distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//�����༭��
				if ( pEditCtrl->iSubItem == 1 )//��ǰ���е��ǵ�һ��
				{
					m_comBox.AddString("һ��Ŀ¼");
					m_comBox.AddString("����Ŀ¼");
				}
				else if ( pEditCtrl->iSubItem == 2 )//��ǰ���е��ǵڶ���
				{
					if ( m_list.GetItemText(pEditCtrl->iItem, 1) == "һ��Ŀ¼" )
						m_comBox.AddString("MF");
					else
					{
						m_comBox.AddString("�籣Ӧ��");
						m_comBox.AddString("���籣Ӧ��");
					}
				}
				SetComboxSelect();
				m_comBox.ShowDropDown();//�����Զ���ʾ�����б�
			}
			else//���еĵ�Ԫ����֮ǰ�����õ�
			{
				m_comBox.SetFocus();//����Ϊ���� 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			e_SubItem = pEditCtrl->iSubItem;//�����еĵ�Ԫ����и�ֵ�����ձ༭�����С��Ա���ڴ���
			createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//�����༭��
			if ( pEditCtrl->iSubItem == 1 )//��ǰ���е��ǵ�һ��
			{
				m_comBox.AddString("һ��Ŀ¼");
				m_comBox.AddString("����Ŀ¼");
			}
			else if ( pEditCtrl->iSubItem == 2 )//��ǰ���е��ǵڶ���
			{
				if ( m_list.GetItemText(pEditCtrl->iItem, 1) == "һ��Ŀ¼" )
					m_comBox.AddString("MF");
				else
				{
					m_comBox.AddString("�籣Ӧ��");
					m_comBox.AddString("���籣Ӧ��");
				}
			}
			SetComboxSelect();
			m_comBox.ShowDropDown();//�����Զ���ʾ�����б�
		}
	}

	*pResult = 0;
}

void CSettingDlg::SetComboxSelect()
{
	CString strTmp;
	CString strItem = m_list.GetItemText(e_Item, e_SubItem);//���ListCtrl��Ԫ���ַ�
	for ( int i=0; i<m_comBox.GetCount(); ++i )
	{
		m_comBox.GetLBText(i, strTmp);//��ȡ��������ı�
		if ( strTmp.CompareNoCase(strItem)==0 )//�ҵ�ƥ�������,ѡ�и������б��ĸ���
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

	// TODO:  �ڴ˸��� DC ���κ�����
	CFont m_font;
	CBrush m_brush;
	m_font.CreatePointFont(120,"��Բ");
	m_brush.CreateSolidBrush(RGB(227,237,205));//��ˢ��ɫ
	if ( nCtlColor == CTLCOLOR_STATIC ) //���ľ�̬�ؼ�����ɫ
	{
		UINT uID = pWnd->GetDlgCtrlID(); //��ȡ�ؼ�ID����
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
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CSettingDlg::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)//�Ҽ���ʾ���ܲ˵�
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bBtState;//��������Ҽ��˵�����Ӻ�ɾ����ť�Ƿ��Ϊ��ɫ false���

	if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}

	NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	m_nCurRow = pEditCtrl->iItem;
	if (pEditCtrl->iItem==-1)//������ǹ�����,���ε��Ҽ��˵���ɾ������
		bBtState = FALSE;
	else
		bBtState = TRUE;

	POINT pt;
	GetCursorPos(&pt);//��ȡ����λ�ã�����Ļ�����ʾ
	//����menu�˵�
	CMenu temp,*ptr;
	temp.LoadMenuA(IDR_MENU1);
	ptr = temp.GetSubMenu(0); 
	if ( bBtState )
		ptr->EnableMenuItem(ID_MenuDelte,MF_ENABLED);
	else
		ptr->EnableMenuItem(ID_MenuDelte,MF_GRAYED);
	//��ָ��λ����ʾ���������˵�����׷�ٵ����˵��б�ѡ�����
	ptr->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
	
	*pResult = 0;
}

//�Զ�����Ϣ����������ListCtrl�д����ı༭���������ؼ�����ֹ����������ʱ��ؼ�λ�ô���,����������Ϣ��MyListCtrl�д�����Ϊ��������MyListCtrl���ӿؼ����Ի������Ӧ�¼�����Ӧ��ֻ����MyListCtrl�д�����Ϣ
afx_msg LRESULT CSettingDlg::OnMylistctrl(WPARAM wParam, LPARAM lParam)
{
	if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}
	return 0;
}

void CSettingDlg::InsertNewLine( int nRow, CString str1 /*= ""*/, CString str2/*=""*/, CString str3/*=""*/, CString str4/*="" */, CString str5/*="" */ )
{
	if ( str1 == "" )//���һ��Ĭ������
	{
		CString strSeq; 
		strSeq.Format( "%d", nRow+1 );
		m_list.InsertItem(nRow, strSeq);
		m_list.SetItemText(nRow, 1, "һ��Ŀ¼");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str1;
	if ( e_SubItem == 1 )//ѡ���һ�е�ʱ��
	{
		m_comBox.GetLBText(m_comBox.GetCurSel(), str1);
		if ( str1 == "һ��Ŀ¼" )
		{
			m_list.SetItemText(e_Item, e_SubItem+1, "MF");
			m_list.SetItemText(e_Item, e_SubItem+2, "3F00");
			m_list.SetItemText(e_Item, e_SubItem+3, "A0000000030000");
		}
		else if ( str1 == "����Ŀ¼" )
		{
			m_list.SetItemText(e_Item, e_SubItem+1, "�籣Ӧ��");
			m_list.SetItemText(e_Item, e_SubItem+2, "");
			m_list.SetItemText(e_Item, e_SubItem+3, "");
		}
	}
}

void CSettingDlg::OnMenuadd()
{
	// TODO: �ڴ���������������
	int nColum = m_list.GetItemCount();//����������������
	InsertNewLine(nColum);
}

void CSettingDlg::OnMenudelte()
{
	// TODO: �ڴ���������������
	m_list.DeleteItem(m_nCurRow);
	UpdataListSeq();
}

void CSettingDlg::OnMenuclear()
{
	// TODO: �ڴ���������������
	m_list.DeleteAllItems();
}

bool CSettingDlg::SaveKeysList(vector<CLEARCARDPARAM> &vecClearCardParam)
{
	CLEARCARDPARAM struParam;
	CString strText,strWarn;
	vecClearCardParam.clear();
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	if (haveeditcreate == true)//���֮ǰ�����˱༭������ٵ�
	{
		distroyEdit(&m_list, &m_Edit, e_Item, e_SubItem);//���ٵ�Ԫ��༭�����
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//���֮ǰ�����������б������ٵ�
	{
		distroyCcombobox(&m_list, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}

	//������ݵĺϷ���
	for ( int i=0; i<m_list.GetItemCount(); ++i)
	{
		//��������FID���ݺϷ���
		strText = m_list.GetItemText(i, 3);
		if ( strText.GetLength() != 4 )
		{
			strWarn.Format("���󣺵ڡ�%d���С�FID����FID���ݵĳ��Ȳ���4λ,���޸ģ�", i+1);
			GetDlgItem(IDC_STATIC3)->SetWindowText(strWarn);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			return false;
		}
		//��������AID���ݵ�λ���Ƿ�Ϊż��λ
		strText = m_list.GetItemText(i, 4);
		if ( strText.GetLength()%2 != 0 || strText=="" )
		{
			strWarn.Format("���󣺵ڡ�%d���С�AID�������ݵĳ��ȱ���Ϊż�������ݲ���Ϊ��,���޸ģ�", i+1);
			GetDlgItem(IDC_STATIC3)->SetWindowText(strWarn);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			return false;
		}
		//����������Կ���ݺϷ���
		strText = m_list.GetItemText(i, 5);
		if ( strText.GetLength() != 32 )
		{
			strWarn.Format("���󣺵ڡ�%d���С�Ӧ�õ�������Կֵ������Կ���ݵĳ��Ȳ���32λ,���޸ģ�", i+1);
			GetDlgItem(IDC_STATIC3)->SetWindowText(strWarn);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			return false;
		}
	}
	//����ȫ��������������
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
		//������ö�ȡ�����ݵĺϷ��ԣ������Զ�����
		if ( (*iter).m_strDirLv=="1" )
		{
			strDir="һ��Ŀ¼";
			strAE="MF";
		}
		else if ( (*iter).m_strDirLv=="2" )
		{
			strDir="����Ŀ¼";
			if ( (*iter).m_strAE=="3" )
				strAE="���籣Ӧ��";
			else
				strAE="�籣Ӧ��";
		}
		else
		{
			if ( (*iter).m_strAE=="1" )
			{
				strDir="һ��Ŀ¼";
				strAE="MF";
			}
			else if ( (*iter).m_strAE=="3" )
			{
				strDir="����Ŀ¼";
				strAE="���籣Ӧ��";
			}
			else
			{
				strDir="����Ŀ¼";
				strAE="�籣Ӧ��";
			}
		}

		strFID.Format("%s",(*iter).m_strFID.c_str());
		if ( (*iter).m_strFID.length()%2!=0 || (*iter).m_strFID.empty() || !JudgeHex((*iter).m_strFID) )
		{
			if ( strAE=="MF" )
				strFID="3F00";
			else if ( strAE=="�籣Ӧ��" )
				strFID="DDF1";
			else if ( strAE=="���籣Ӧ��" )
				strFID="DDF2";
		}

		strAID.Format("%s",(*iter).m_strAID.c_str());
		if ( (*iter).m_strAID.length()%2!=0 || (*iter).m_strAID.empty() || !JudgeHex((*iter).m_strAID) )
		{
			if ( strAE=="MF" )
				strAID="A0000000030000";
			else if ( strAE=="�籣Ӧ��" )
				strAID="7378312E73682EC9E7BBE1B1A3D5CF";
			else if ( strAE=="���籣Ӧ��" )
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();
	InsertNewLine(0, "һ��Ŀ¼", "MF", "3F00", "A0000000030000", "11111111111111111111111111111111");//���MF����
	InsertNewLine(1, "����Ŀ¼", "�籣Ӧ��", "DDF1", "7378312E73682EC9E7BBE1B1A3D5CF", "11111111111111111111111111111111");//���SSSE����
	InsertNewLine(2, "����Ŀ¼", "���籣Ӧ��", "DDF2", "315041592E5359532E4444463031", "11111111111111111111111111111111");//���PSE����
}
