// DlgOtherAlgo.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgOtherAlgo.h"
#include "afxdialogex.h"


// DlgOtherAlgo 对话框

IMPLEMENT_DYNAMIC(DlgOtherAlgo, CDialogEx)

DlgOtherAlgo::DlgOtherAlgo(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgOtherAlgo::IDD, pParent)
{
	bDataLoad = false;
	pOtherAlgo =NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

DlgOtherAlgo::~DlgOtherAlgo()
{
	if(pOtherAlgo!=NULL)
	{
		delete pOtherAlgo;
		pOtherAlgo = NULL;
	}
}
void DlgOtherAlgo::ResetAllParas()
{
	if(pOtherAlgo != NULL)
	{
		delete pOtherAlgo;
		pOtherAlgo= NULL;
	}
	para.minSupport=0;

	bDataLoad = false;
}
BOOL DlgOtherAlgo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	m_combo_algo.AddString(_T("Apriori"));
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	return TRUE;  		
}
void DlgOtherAlgo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ALGO, m_combo_algo);
	DDX_Control(pDX, IDC_INPUTFILE, m_editbrowse_input);
	DDX_Control(pDX, IDC_INFO, m_other_info);
	DDX_Control(pDX, IDC_EDIT_MIN, m_edit_min);
}


BEGIN_MESSAGE_MAP(DlgOtherAlgo, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &DlgOtherAlgo::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_RUN_ALGO, &DlgOtherAlgo::OnBnClickedButtonRunAlgo)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &DlgOtherAlgo::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// DlgOtherAlgo 消息处理程序
/************************functions used to respond********************************/
int DlgOtherAlgo::getAlgo()
{
	if(pOtherAlgo!= NULL)
	{
		return 0;
	}
	CString algo;
	m_combo_algo.GetWindowTextW(algo);
	LPCWSTR a = (LPCWSTR)algo;
	AfxMessageBox(a);
	if(algo.Compare(TEXT("Apriori"))==0)
	{
		algo_type = APRIORI_TYPE;
		pOtherAlgo = new Apriori;
	}
	return 0;
}
int DlgOtherAlgo::loadData()
{
	//AfxMessageBox(TEXT("enter load"));
	if(bDataLoad == true)
	{
		AfxMessageBox(TEXT("sorry, you have loaded data already, if you want to reload, click the RESET button at first"));
		return -1;
	}
	if(getAlgo()!=0)
	{
		AfxMessageBox(TEXT("loadData, but cant get algorithm, try again"));
		return -1;
	}
	CString fname = _T("");
	GetDlgItem(IDC_INPUTFILE)->GetWindowTextW(fname);
	string strname = "";
	if(fname.IsEmpty())
	{
		//strname =getDefFileName(algo_type);
	}
	else
	{
		fname = transFile2Path(fname);
		strname = CStr2Str(fname);
	}
	if(pOtherAlgo ==NULL)
	{
		AfxMessageBox(TEXT("Plz choose algorithm at first"));
		return -1;
	}
	pOtherAlgo->loadData(strname);

	bDataLoad = true;
	return 0;
}
void DlgOtherAlgo::getPara()
{
	CString mins = _T("");
	m_edit_min.GetWindowTextW(mins);
	para.minSupport = _wtoi(mins);
}
/**********************************running the clustering algorithem starts from here******************************************************/
int DlgOtherAlgo::runMl()
{
	if(!bDataLoad)
	{
		AfxMessageBox(TEXT("Plz load data at first!"));
		return -1;
	}
	getPara();
	//getPreprocessMethod();

	pOtherAlgo->setPara(para);
	//train data
	pOtherAlgo->executeAlgo();

	return 0;
}

/**************************************respond functions**********************************/
void DlgOtherAlgo::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	if(loadData()==-1)
	{
		AfxMessageBox(TEXT("load data failed, try again"));
		return;
	}
	DEBUG_OTHERALGO(TEXT("Datasets loaded successed!"));
}


void DlgOtherAlgo::OnBnClickedButtonRunAlgo()
{
	// TODO: 在此添加控件通知处理程序代码
	runMl();
}


void DlgOtherAlgo::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetAllParas();
}
