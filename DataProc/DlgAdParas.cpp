// DlgAdParas.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgAdParas.h"
#include "afxdialogex.h"
//#include "dlgComm.h"


// DlgAdParas 对话框

IMPLEMENT_DYNAMIC(DlgAdParas, CDialogEx)

DlgAdParas::DlgAdParas(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgAdParas::IDD, pParent)
{
}

DlgAdParas::~DlgAdParas()
{
}

void DlgAdParas::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HIDLAYER, m_list_paras);
	//DDX_Control(pDX, IDC_EDIT1, m_edit_table);
	DDX_Control(pDX, IDC_COMBO_TRANSFUNC, m_combo_transfunc);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_combo_method);
	//DDX_Control(pDX, IDC_EDIT1, m_edit1);
}
/*void DlgAdParas::InsertItems()
{
	HWND hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_HIDLAYER);
	//set the lvcolumn structure with the required column information
	LVCOLUMN list;
	list.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT|LVCF_SUBITEM;
	list.fmt = LVCFMT_LEFT;
	list.cx = 150;
	list.pszText = _T("Hidden Layer Index");
	list.iSubItem = 0;
	//insert the column
	::SendMessageA(hwnd, LVM_INSERTCOLUMN,(WPARAM)0,(WPARAM)&list);
	list.cx=150;
	list.pszText = _T("No units/Hidden Layer");
	list.iSubItem = 1;
	::SendMessageA(hwnd, LVM_INSERTCOLUMN,(WPARAM)1,(WPARAM)&list);
	//insert the first row
	CString value = _T("1");
	mgr.SetCell(m_hWnd,value,0,0);
}*/
BOOL DlgAdParas::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	ListView_SetExtendedListViewStyle(::GetDlgItem(m_hWnd,IDC_LIST_HIDLAYER),LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	tempC =0;
	tempR = 0;

	m_list_paras.InsertColumn(0,_T("Hidden Layer Index"),LVCFMT_LEFT,100);
	m_list_paras.InsertColumn(1,_T("No units/Hidden Layer"),LVCFMT_LEFT,100);

	m_combo_transfunc.AddString(_T("Linear"));
	m_combo_transfunc.AddString(_T("Sigmoid"));

	m_combo_method.AddString(_T("BP_DM"));
	m_combo_method.AddString(_T("BP_DM_SELF_ADJUST"));
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	return TRUE;
}
/*void DlgAdParas::OnOK()
{
	CWnd* pwndCtrl = GetFocus();
	//get the control ID which presently having the focus
	int ctrl_ID = pwndCtrl->GetDlgCtrlID();
	CString str;
	switch(ctrl_ID)
	{
		//if the control is the edit box
	case IDC_EDIT1:
		//get the text from the edit box
		GetDlgItemText(IDC_EDIT1,str);
		//set the value in the list control with the specified item and subitem values
		mgr.SetCell(m_hWnd,str,mgr.nItem,mgr.nSubItem);
		::SendDlgItemMessageA(m_hWnd,IDC_EDIT1,WM_KILLFOCUS,0,0);
		::ShowWindow(::GetDlgItem(m_hWnd,IDC_EDIT1),SW_HIDE);
		break;
	default:
		break;
	}
}*/


BEGIN_MESSAGE_MAP(DlgAdParas, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgAdParas::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MFCBUTTON_TOTAL, &DlgAdParas::OnBnClickedMfcbuttonTotal)
END_MESSAGE_MAP()


// DlgAdParas 消息处理程序


void DlgAdParas::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//02. get parent dialog
	CMyDataProcDlg* pMain =(CMyDataProcDlg*)this->GetParent();

	//03. getCost
	if(pMain->pAlgo == NULL)
	{
		AfxMessageBox(TEXT("Plz choose algorithm, load data and train data at first!"));
		return;
	}
	pMain->paras.nn_struct.HiddenLayers = hiddenLayers;
	pMain->paras.nn_struct.HiddenNodes = new int[hiddenLayers];
	tempC=1;
	tempR =0;
	for(int i=0;i<hiddenLayers;i++)
	{
		CString temp;
		temp = m_list_paras.GetItemText(i,tempC);
		pMain->paras.nn_struct.HiddenNodes[i] = _wtoi(temp);
	}

	//set other paras
	CString trans_func = _T("");
	CString method = _T("");
	m_combo_transfunc.GetWindowTextW(trans_func);
	if(trans_func.Compare(TEXT("Linear"))==0)
	{
		pMain->paras.nn_struct.TransFunc = LINEAR_FUNC;
	}
	else if(trans_func.Compare(TEXT("Sigmoid"))==0)
	{
		pMain->paras.nn_struct.TransFunc = SIGMOID_FUNC;
	}
	m_combo_method.GetWindowTextW(method);
	if(method.Compare(TEXT("BP_DM"))==0)
	{
		pMain->paras.train_method = 0;
	}
	else if(method.Compare(TEXT("BP_DM_SELF_ADJUST"))==0)
	{
		pMain->paras.train_method = 1;
	}
	CDialogEx::OnOK();
	
}


/*void DlgAdParas::OnNMClickListHidlayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_list_paras.SetRedraw(TRUE);
	Invalidate();
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	mgr.OnClickList(m_hWnd,pNMItemActivate);
	m_list_paras.UpdateWindow();
	*pResult = 0;
}*/


/*void DlgAdParas::OnLvnItemchangedListHidlayer(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMHDR;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}*/


void DlgAdParas::OnBnClickedMfcbuttonTotal()
{
	// TODO: 在此添加控件通知处理程序代码
	while(m_list_paras.DeleteColumn(0));
	while(m_list_paras.DeleteItem(0));
	m_list_paras.InsertColumn(0,_T("Hidden Layer Index"),LVCFMT_LEFT,100);
	m_list_paras.InsertColumn(1,_T("No units/Hidden Layer"),LVCFMT_LEFT,100);
	CString temp;
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(temp);
	hiddenLayers = _wtoi(temp);
	for(int i = hiddenLayers;i>=1;i--)
	{
		temp.Format(_T("%d"),i);
		m_list_paras.InsertItem(0,temp,150);
	}
}
