// DlgCluster.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgCluster.h"
#include "afxdialogex.h"


// DlgCluster 对话框

IMPLEMENT_DYNAMIC(DlgCluster, CDialogEx)

DlgCluster::DlgCluster(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgCluster::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bDataLoad = false;
	pCluster = NULL;
	algo_type = TYPE_BUTT;
	pDlgChart = NULL;
	para.k = 0;

}

DlgCluster::~DlgCluster()
{
	if(pCluster!=NULL)
	{
		delete pCluster;
		pCluster=NULL;
	}
	if(pDlgChart!=NULL)
	{
		delete pDlgChart;
		pDlgChart=NULL;
	}
}
void DlgCluster::ResetAllParas()
{
	if(pCluster != NULL)
	{
		delete pCluster;
		pCluster= NULL;
	}
	if(pDlgChart != NULL)
	{
		delete pDlgChart;
		pDlgChart = NULL;
	}
	para.k = 0;

	bDataLoad = false;
}
BOOL DlgCluster::OnInitDialog()
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

	m_combo_cluster.AddString(_T("Hierarchical Clustering"));
	m_cluster_info.AddString(_T("running state here"));

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	return TRUE;  
	
	
}

void DlgCluster::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ALGO, m_combo_cluster);
	DDX_Control(pDX, IDC_INFO, m_cluster_info);
}


BEGIN_MESSAGE_MAP(DlgCluster, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &DlgCluster::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_VISUAL, &DlgCluster::OnBnClickedButtonVisual)
	ON_BN_CLICKED(IDC_BUTTON_RUN_ALGO, &DlgCluster::OnBnClickedButtonRunAlgo)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &DlgCluster::OnBnClickedButtonReset)
	ON_LBN_SELCHANGE(IDC_INFO, &DlgCluster::OnLbnSelchangeInfo)
END_MESSAGE_MAP()
/**********************************load data and get parameters here******************************************************/
void DlgCluster::getTrainPara()
{
	CString k;
	GetDlgItem(IDC_EDIT_K)->GetWindowTextW(k);
	para.k = (int)str2double(CStr2Str(k).c_str());
	return;
}
int DlgCluster::getAlgo()
{
	if(pCluster!= NULL)
	{
		return 0;
	}
	CString algo;
	m_combo_cluster.GetWindowTextW(algo);
	LPCWSTR a = (LPCWSTR)algo;
	AfxMessageBox(a);
	if(algo.Compare(TEXT("Hierarchical Clustering"))==0)
	{
		algo_type = HIER_CLUSTER;
		pCluster = new LevelCluster;
	}
	return 0;
}
int DlgCluster::loadData()
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
	if(pCluster ==NULL)
	{
		AfxMessageBox(TEXT("Plz choose algorithm at first"));
		return -1;
	}
	if(pCluster->loadData(strname)==-1)
	{
		AfxMessageBox(TEXT("Load data failed!"));
		return -1;
	}

	bDataLoad = true;
	return 0;
}
/**********************************running the clustering algorithem starts from here******************************************************/
int DlgCluster::runMl()
{
	if(!bDataLoad)
	{
		AfxMessageBox(TEXT("Plz load data at first!"));
		return -1;
	}
	getTrainPara();
	//getPreprocessMethod();

	pCluster->setTrainPara(para);
	//train data
	if(pCluster->exectCluster()!= -1)
	{
		DEBUG_CLUSTER(TEXT("Clustering succeed!"));
	}
	else
	{
		DEBUG_CLUSTER(TEXT("Clustering failed!"));
		return -1;
	}
	return 0;
}

/**********************************draw pictures starts from here******************************************************/
void DlgCluster::draw_input(CChartCtrl& m_chart)
{
	if(pCluster != NULL)
	{
		XY_PARA input = pCluster->getParasDrawInput();
		Matrix x = input.x_orig;
		AXIS_SCALE sca;
		if(x.brandth()>2)
		{
			AfxMessageBox(TEXT("Sorry, cant support 3D or more right now"));
			return;
		}
		else if(x.brandth()==2)
		{
			Matrix x1 = x.getCol(0,0);
			Matrix x2 = x.getCol(1,1);
			sca.min_bot = (0.9)*x1.getMin()-2;
			sca.max_bot = (1.1)*x1.getMax()+2;
			sca.min_lef = (0.9)*x2.getMin()-2;
			sca.max_lef = (1.1)*x2.getMax()+2;
			sca.bAuto = false;
			CString name = _T("input datasets");
			CString leftaxis = _T("x2");
			CString bottomaxis = _T("x1");
			buildAxis(m_chart,sca,name,leftaxis,bottomaxis);

			
			draw_point(m_chart,x1,x2,name);
		}
		return;
	}
	else 
	{
		AfxMessageBox(TEXT("Plz choose algorithm at first"));
		return;
	}
}
/**********************************draw pictures end to here******************************************************/
// DlgCluster 消息处理程序


void DlgCluster::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	if(loadData()==-1)
	{
		AfxMessageBox(TEXT("load data failed, try again"));
		return;
	}
	DEBUG_CLUSTER(TEXT("Datasets loaded successed!"));
}


void DlgCluster::OnBnClickedButtonVisual()
{
	// TODO: 在此添加控件通知处理程序代码
	pDlgChart = new inputDlg(this);
	BOOL valid = pDlgChart->Create(IDD_INPUT,this);
	if(valid==1)
	{
		if(!bDataLoad)
		{
			AfxMessageBox(TEXT("Plz load data at first"));
			return;
		}
		draw_input(pDlgChart->m_chart);
		
		DEBUG_CLUSTER(TEXT("visualizing input data"));
		pDlgChart->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(TEXT("N ok"));
	}
}


void DlgCluster::OnBnClickedButtonRunAlgo()
{
	// TODO: 在此添加控件通知处理程序代码
	if(runMl()!=0)
	{
		return;
	}
}


void DlgCluster::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetAllParas();
}


void DlgCluster::OnLbnSelchangeInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}
