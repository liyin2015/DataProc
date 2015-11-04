
// DataProcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DataProcDlg.h"
#include "afxdialogex.h"
#include <string>
#include <atlconv.h>
#include "inputDlg.h"
#include "ChartCtrl_source\ChartAxisLabel.h"
#include "ChartCtrl_source\ChartAxis.h"
#include "ChartCtrl_source\draw_lib.h"
#include "ml\supervise\linear_regression.hpp"
#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyDataProcDlg 对话框




CMyDataProcDlg::CMyDataProcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyDataProcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//algo = "";
	algo_type = TYPE_BUTT;
	pAlgo = NULL;

	bDataLoad = false;
	preProcessSeted = false;
	bRand = false;
	new_dialog = NULL;
	para_set_dlg = NULL;
	pMonitor = NULL;
	num_label = 0;
	dlgPredict = NULL;
	index =0;
}
void CMyDataProcDlg::ResetAllParas()
{
	if(pAlgo != NULL)
	{
		delete pAlgo;
		pAlgo= NULL;
	}
	if(new_dialog != NULL)
	{
		delete new_dialog;
		new_dialog = NULL;
	}
	if(para_set_dlg != NULL)
	{
		delete para_set_dlg;
		para_set_dlg= NULL;
	}
	if(pMonitor !=NULL)
	{
		delete pMonitor;
		pMonitor = NULL;
	}
	if(dlgPredict !=NULL)
	{
		delete dlgPredict;
		dlgPredict = NULL;
	}
	preProcessSeted = false;
	paras.alpha = -1;
	paras.lambda = -1;
	paras.num_iters = -1;
	pre_para.bNorm = false;
	pre_para.bScale = false;
	pre_para.dim = 0;
	index =0;

	bDataLoad = false;
	bRand = false;
	num_label = 0;
	m_index.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL_INDEX)->ShowWindow(SW_HIDE);

	//restore the alpha
	SetDlgItemTextW(IDC_STATIC_ALPHA,_T("Alpha"));
	SetDlgItemTextW(IDC_STATIC_LAMBDA,_T("Lambda"));
	GetDlgItem(IDC_STATIC_CONVERGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_CONVERGE)->ShowWindow(SW_SHOW);

}
CMyDataProcDlg::~CMyDataProcDlg()
{
	if(pAlgo != NULL)
	{
		delete pAlgo;
		pAlgo= NULL;
	}
	if(new_dialog != NULL)
	{
		delete new_dialog;
		new_dialog = NULL;
	}
	if(para_set_dlg != NULL)
	{
		delete para_set_dlg;
		para_set_dlg= NULL;
	}
	if(paras.nn_struct.HiddenNodes != NULL)
	{
		delete paras.nn_struct.HiddenNodes;
		paras.nn_struct.HiddenNodes = NULL;
	}
	if(pMonitor !=NULL)
	{
		delete pMonitor;
		pMonitor = NULL;
	}
	if(dlgPredict !=NULL)
	{
		delete dlgPredict;
		dlgPredict = NULL;
	}
}

void CMyDataProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO, m_listbox_info);
	DDX_Control(pDX, IDC_INDEX, m_index);
	DDX_Control(pDX, IDC_COMBO_ALGO, m_commo_algo);
	DDX_Control(pDX, IDC_COMBO_NORM, m_combo_norm);
	DDX_Control(pDX, IDC_COMBO_SCALE, m_combo_scale);
	DDX_Control(pDX, IDC_COMBO_RAND, m_combo_rand);
	DDX_Control(pDX, IDC_COMBO_STOREMETHOD, m_combo_method);
}

BEGIN_MESSAGE_MAP(CMyDataProcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RUN_ALGO, &CMyDataProcDlg::OnBnClickedButtonRunAlgo)

	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMyDataProcDlg::OnBnClickedButtonLoad)
	ON_LBN_SELCHANGE(IDC_INFO, &CMyDataProcDlg::OnLbnSelchangeInfo)
	ON_EN_CHANGE(IDC_INPUTFILE, &CMyDataProcDlg::OnEnChangeInputfile)
	ON_EN_CHANGE(IDC_PREDICTFILE, &CMyDataProcDlg::OnEnChangePredictfile)

	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMyDataProcDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT, &CMyDataProcDlg::OnBnClickedButtonPredict)
	ON_CBN_SELCHANGE(IDC_COMBO_RAND, &CMyDataProcDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_PREDICTRSLT, &CMyDataProcDlg::OnEnChangePredictrslt)
	ON_BN_CLICKED(IDC_BUTTON_VISUAL, &CMyDataProcDlg::OnBnClickedButtonVisual)
	ON_BN_CLICKED(IDC_BUTTON_PARA, &CMyDataProcDlg::OnBnClickedButtonPara)
	ON_BN_CLICKED(IDC_BUTTON_MONITOR, &CMyDataProcDlg::OnBnClickedButtonMonitor)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_DECISION_BOUND, &CMyDataProcDlg::OnBnClickedButtonDrawDecisionBound)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_OUTPUT, &CMyDataProcDlg::OnBnClickedButtonDrawOutput)
	ON_BN_CLICKED(IDC_BUTTON_RATE, &CMyDataProcDlg::OnBnClickedButtonRate)
	ON_BN_CLICKED(IDC_MFCBUTTON_PRE, &CMyDataProcDlg::OnBnClickedMfcbuttonPre)
END_MESSAGE_MAP()


// CMyDataProcDlg 消息处理程序

BOOL CMyDataProcDlg::OnInitDialog()
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
	//store the old window size
	CRect rect;
	GetClientRect(&rect);
	oldWin.x = rect.right-rect.left;
	oldWin.y = rect.bottom-rect.top;

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_commo_algo.AddString(_T("Linear Regression"));
	m_commo_algo.AddString(_T("Logistic Regression"));
	m_commo_algo.AddString(_T("Neutral Netwrok"));
	m_commo_algo.AddString(_T("SVM"));

	//rand
	m_combo_rand.AddString(_T("Yes"));
	m_combo_rand.AddString(_T("No"));

	m_combo_scale.AddString(_T("Yes"));
	m_combo_scale.AddString(_T("No"));
	//
	m_combo_norm.AddString(_T("Yes"));
	m_combo_norm.AddString(_T("No"));

	//store method
	m_combo_method.AddString(_T("Mix"));//input data and predicted data
	m_combo_method.AddString(_T("Pure")); //only the predicted result

	m_listbox_info.AddString(_T("running state here"));

	
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyDataProcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		AboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyDataProcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyDataProcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



/*void CMyDataProcDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}*/

//USES_CONVERSION;





void CMyDataProcDlg::OnLbnSelchangeInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMyDataProcDlg::OnEnChangeInputfile()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CMyDataProcDlg::OnEnChangePredictfile()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//predictMl();
}
int CMyDataProcDlg::getAlgo()
{
	//if(pAlgo == NULL)
	//{
	//delete pAlgo; //free the former algo pointer and new another one
	if(pAlgo != NULL)
	{
		return 0;
	}
	//int i=m_commo_algo.GetCurSel();
	CString algo;
	m_commo_algo.GetWindowTextW(algo);
	LPCWSTR a = (LPCWSTR)algo;
	//AfxMessageBox(a);
	if(algo.Compare(TEXT("Linear Regression"))==0)
	{
		algo_type = LINEAR_REG_TYPE;
		pAlgo = new LinReg;
	}
	else if(algo.Compare(TEXT("Logistic Regression"))==0)
	{
		algo_type = LOGISTIC_REG_TYPE;
		pAlgo = new LogReg;
	}
	else if(algo.Compare(TEXT("Neutral Netwrok"))==0)
	{
		algo_type = NEURAL_NET_TYPE;
		pAlgo = new NeuralNetwork;

	}

	else if(algo.Compare(TEXT("SVM"))==0)
	{
		algo_type = SVM_TYPE;
		pAlgo = new SVM;

		SetDlgItemTextW(IDC_STATIC_ALPHA,_T("Tol"));
		SetDlgItemTextW(IDC_STATIC_LAMBDA,_T("C"));
		SetDlgItemTextW(IDC_ALPHA,_T("0.01"));
		SetDlgItemTextW(IDC_LAMBDA,_T("1.0"));
		GetDlgItem(IDC_STATIC_CONVERGE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CONVERGE)->ShowWindow(SW_HIDE);
	}
	//}
	return 0;
}
int CMyDataProcDlg::loadData()
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
		AfxMessageBox(TEXT("Plz select an input file!"));
		return -1;
		//strname =getDefFileName(algo_type);
	}
	else
	{
		fname = transFile2Path(fname);
		strname = CStr2Str(fname);
	}
		
	if(pAlgo !=NULL)
	{
			//randomize data
		CString rand;
		m_combo_rand.GetWindowTextW(rand);
		if(rand.Compare(TEXT("Yes"))==0)
		{
			bRand = true;
		}
		else
		{
			bRand = false;
		}	
		num_label =  pAlgo->loadData(strname,bRand,algo_type);
	}
	else
	{
		AfxMessageBox(TEXT("Plz choose algorithm at first"));
		return -1;
	}
	bDataLoad = true;

	return 0;
}
void CMyDataProcDlg::getTrainPara()
{
	//02.get train paras
	CString alpha=_T("");
	CString num_iters = _T("");
	CString lambda= _T("");		

	GetDlgItem(IDC_ALPHA)->GetWindowTextW(alpha);
	GetDlgItem(IDC_LAMBDA)->GetWindowTextW(lambda);
	GetDlgItem(IDC_ITER)->GetWindowTextW(num_iters);	

	paras.num_iters = (int)str2double(CStr2Str(num_iters).c_str());

	if(algo_type == SVM_TYPE)
	{
		paras.tol = str2double(CStr2Str(alpha).c_str());
		paras.C = str2double(CStr2Str(lambda).c_str());
	}
	else
	{
		CString converge = _T("");
		GetDlgItem(IDC_EDIT_CONVERGE)->GetWindowTextW(converge);
		paras.alpha = str2double(CStr2Str(alpha).c_str());
		paras.lambda = str2double(CStr2Str(lambda).c_str());
		paras.converge = str2double(CStr2Str(converge).c_str());
	}
}
int CMyDataProcDlg::runMl()
{
	//load data
	if(!bDataLoad)
	{
		AfxMessageBox(TEXT("Plz load data at first!"));
		return -1;
	}
	if(!preProcessSeted)
	{
		AfxMessageBox(TEXT("Plz set preProcessing parameters!"));
		return -1;
	}
	//get input proce method
	//get paras
	//multiclass labels right now only used for logistic regression

	if(num_label <2) //not multilabel datasets
	{
		trainSigLabel(0);
	}
	else
	{
		trainSigLabel(index);
	}
	return 0;
}
int CMyDataProcDlg::trainSigLabel(int index)
{
	getTrainPara();

	pAlgo->setTrainPara(paras,index);
	//train data
	if(pAlgo->trainData(index))
	{
		DEBUG_INFO(TEXT("It has been converged!"));
		return 0;
	}
	else
	{
		DEBUG_INFO(TEXT("It has not been converged!"));
		MIN_COST_PARA_RECORD minRecord = pAlgo->getMinCostPara(index);
		CString popMsg = generateText("It gets the minimum cost at iter =  ",minRecord.num_iters," ");
		DEBUG_INFO(popMsg);
		popMsg = generateText("minCost =  ",minRecord.minJ," ");
		DEBUG_INFO(popMsg);
		return -1;
	}
	return 0;
}
int CMyDataProcDlg::predictMl()
{
	if(!bDataLoad)
	{
		AfxMessageBox(TEXT("Plz load data at first!"));
		return -1;
	}
	if(!preProcessSeted)
	{
		AfxMessageBox(TEXT("Plz set preProcessing parameters!"));
		return -1;
	}
	CString fname = _T("");
	GetDlgItem(IDC_PREDICTFILE)->GetWindowTextW(fname);

	string strname = "";
	if(!fname.IsEmpty())
	{
		fname = transFile2Path(fname);
		strname = CStr2Str(fname);
	}
	else
	{
		//strname = getDefFileName(algo_type);
	}
	predict_rslt =  pAlgo->predict(strname);

	GetDlgItem(IDC_PREDICTRSLT)->GetWindowTextW(fname);

	strname = "";
	if(!fname.IsEmpty())
	{
		fname = transFile2Path(fname);
		strname = CStr2Str(fname);
		//get store method
		CString method;
		m_combo_method.GetWindowTextW(method);
		LPCWSTR a = (LPCWSTR)method;
		STORE_METHOD me;
		if(method.Compare(TEXT("Mix"))==0)
		{
			me = MIX;
		}
		else if(method.Compare(TEXT("Pure"))==0)
		{
			me = PURITY;
		}
		writePredict2File(predict_rslt.y,strname,me);
	}
	else
	{
		//strname = getDefFileName(algo_type);
	}


	return 0;
}
Algo_Basic* CMyDataProcDlg::getPAlgo()
{
	return pAlgo;
}
/*********************************************drawing pictures****************************************************************/
void CMyDataProcDlg::draw_input(CChartCtrl& m_chart)
{
	if(pAlgo == NULL)
	{
		AfxMessageBox(TEXT("Plz choose algorithm at first"));
		return;
	}

	inputdata = pAlgo->getParasDrawInput();
	Matrix x = inputdata.x_orig;
	Matrix y=inputdata.y;
	AXIS_SCALE sca;
	sca.min_bot = (0.9)*(x.getCol(0,0)).getMin()-2;
	sca.max_bot = (1.1)*(x.getCol(0,0)).getMax()+2;

	CString name = _T("Input Datasets");
	
	CString bottomaxis = _T("x1");
		
	if(algo_type == LINEAR_REG_TYPE)
	{
		sca.min_lef = (0.9)*y.getMin()-2;
		sca.max_lef = (1.1)*y.getMax()+2;
		sca.bAuto = false;
		CString leftaxis = _T("y");
		buildAxis(m_chart,sca,name,leftaxis,bottomaxis);
		draw_point(m_chart,x,y,name);
	}
	else
	{
		sca.min_lef = (0.9)*(x.getCol(1,1)).getMin()-2;
		sca.max_lef = (1.1)*(x.getCol(1,1)).getMax()+2;
		sca.bAuto = false;
		CString leftaxis = _T("x2");
		buildAxis(m_chart,sca, name,leftaxis,bottomaxis);
		Matrix label = inputdata.label;
		draw_point2(m_chart,x,y,label);
	}
}
void CMyDataProcDlg::draw_data_with_decision_boundary(CChartCtrl& m_chart)
{
	//draw original x and y
	
	XY_PARA input = pAlgo->getParasDrawInput();
	Matrix orig_x = input.x_orig;
	Matrix orig_y=input.y;

	AXIS_SCALE sca;
	sca.min_bot = (0.9)*orig_x.getCol(0,0).getMin()-1;
	sca.max_bot = (1.1)*orig_x.getCol(0,0).getMax()+1;

	CString name = _T("Input Datasets");
	
	CString bottomaxis = _T("x1");

	if(algo_type == LINEAR_REG_TYPE)
	{
		sca.min_lef = (0.9)*orig_y.getMin()-1;
		sca.max_lef = (1.1)*orig_y.getMax()+1;
		sca.bAuto = false;

		CString leftaxis = _T("y");
		buildAxis(m_chart,sca,name,leftaxis,bottomaxis);
		if(orig_x.brandth()>2)
		{
			AfxMessageBox(TEXT("Sorry, cant support 3D right now"));
		}
		draw_point(m_chart,orig_x,orig_y,name);
	}
	else
	{
		sca.min_lef = (0.9)*(orig_x.getCol(1,1).getMin())-1;
		sca.max_lef = (1.1)*(orig_x.getCol(1,1).getMax())+1;
		sca.bAuto = false;
		CString leftaxis = _T("x2");
		buildAxis(m_chart,sca,name,leftaxis,bottomaxis);

		Matrix label = input.label;

		draw_point2(m_chart,orig_x,orig_y,label);//multiclass
	}
	name = _T("Decision Boundary");
	//draw decision  boundary
	if(algo_type == LINEAR_REG_TYPE)
	{
		if(orig_x.brandth()>1)
		{
			return;
		}
		Matrix xm = generateByStep(orig_x,1000);
		Matrix y= pAlgo->predict(xm);	

		
		draw_line(m_chart,xm,y,name);
	}
	else if(algo_type == LOGISTIC_REG_TYPE)
	{
		
		if(orig_x.brandth()>3)
		{
			AfxMessageBox(TEXT("Sorry, cant support 3D right now"));
			return;
		}
		Matrix label = input.label;

		int num_label = label.length();
		if(num_label ==2)
		{
			num_label =1;
		}
		for(int i=0;i<num_label;i++)
		{
			ML_LOR_INFO("i"<<i);
			DECISION_PARAS para = pAlgo->getDesionBoundPara(i);
			Matrix theta = para.theta;
			//theta.matrix_[0] = -25;
			//theta.matrix_[1] = 0.206;
			//theta.matrix_[2] = 0.201;
			if(theta.length() == 3)
			{
				Matrix x1 =generateByStep(orig_x,1000);			
				Matrix x2 = x1;
				Matrix negones(x1.length(),1,-1);
				Matrix ones(x1.length(),1,1);
				int m = x1.length();
				if(para.bNorm)
				{
					Matrix mor;
					mor=x1.getNormWithMuSigma(para.normal.mu.getCol(0,0),para.normal.sigma.getCol(0,0));
					x2 = (ones.multiplyReal(*theta(0,0))+mor.multiplyReal(*theta(1,0)))^(negones/(*theta(2,0)));
					//x2 = x2.recoverFmNorm(para.normal.mu,para.normal.sigma);
					draw_line(m_chart,x1,x2,name);
				}
				else
				{
					x2 = (ones.multiplyReal(*theta(0,0))+x1.multiplyReal(*theta(1,0)))^(negones/(*theta(2,0)));
					//if bscaling, should restore from scaling
					//x2 = x2.recoverFmNorm(para.normal.mu,para.normal.sigma);
					draw_line(m_chart,x1,x2,name);
				}

				ML_LOR_INFO("theta"<<para.theta);
				ML_LOR_INFO("x1"<<x1);
				ML_LOR_INFO("x2"<<x2);
			}
			else if(theta.length() == 2) //its only a point
			{
				Matrix x1(1,1,0);
				x1.matrix_[0] = (-theta.matrix_[0])/theta.matrix_[1];
				if(para.bNorm)
				{
					x1 = x1.recoverFmNorm(para.normal.mu.getCol(0,0),para.normal.sigma.getCol(0,0));
				}
				Matrix y1(1,1,0);
				draw_point(m_chart,x1,y1,name);
			}

			
		}
	}
	else if(algo_type == SVM_TYPE)
	{
		if(orig_x.brandth()>3)
		{
			AfxMessageBox(TEXT("Sorry, cant support 3D right now"));
			return;
		}
		Matrix label = input.label;

		//x0*theta0+x1*theta1+b = 0,x1 =(b+x0*theta0)/(-theta1)
		DECISION_PARAS para = pAlgo->getDesionBoundPara(0); 
		Matrix x1 =generateByStep(para.x.getCol(1,1),1000);

		Matrix theta = para.theta;
		Matrix x2 = x1;
		Matrix mb(x1.length(),1,para.b);
		Matrix negones(x1.length(),1,-1);
		Matrix ones(x1.length(),1,1);
		int m = x1.length();
		x2 = (mb+x1.multiplyReal(*theta(0,0)))^(negones/(*theta(1,0)));
		draw_line(m_chart,x1,x2,name);
	}

}
/*void CMyDataProcDlg::draw_data_with_decision_boundary(CChartCtrl& m_chart)
{
	//draw original x and y
	
	XY_PARA input = pAlgo->getParasDrawInput();
	Matrix orig_x = input.x;
	Matrix orig_y=input.y;

	AXIS_SCALE sca;
	sca.min_bot = (0.9)*orig_x.getMin()-1;
	sca.max_bot = (1.1)*orig_x.getMax()+1;
	sca.min_lef = (0.9)*orig_y.getMin()-1;
	sca.max_lef = (1.1)*orig_y.getMax()+1;
	sca.bAuto = false;
	buildAxis(m_chart,sca);
	if(algo_type == LINEAR_REG_TYPE)
	{
		if(orig_x.brandth()>2)
		{
			AfxMessageBox(TEXT("Sorry, cant support 3D right now"));
		}
		draw_point(m_chart,orig_x,orig_y);
	}
	else
	{
		Matrix label = input.label;

		//ML_SVM_INFO("origi_x
		draw_point2(m_chart,orig_x,orig_y,label);//multiclass
	}
	//draw decision  boundary
	if(algo_type == LINEAR_REG_TYPE)
	{
		if(orig_x.brandth()>1)
		{
			return;
		}
		//Matrix col = orig_x.getCol(i,i);
		double min = (3.0/4)* orig_x.getMin();
		double max = (4.0/3)*orig_x.getMax();
		double space = max-min;
		double* x = new double[1000];
		for(int i=0;i<1000;i++)
		{
			x[i]=min+((double)i/1000)*space;
		}
		Matrix xm(x,1000,1);
		Matrix y= pAlgo->predict(xm);	
		draw_line(m_chart,xm,y);
	}
	else
	{
		if(orig_x.brandth()>2)
		{
			return;
		}
		for(int i=0;i<orig_x.brandth()-1;i++)//only one time
		{
			Matrix col1 = orig_x.getCol(i,i);
			double min1 = (3.0/4)* col1.getMin();
			double max1 = (4.0/3)*col1.getMax();
			double space1 = max1-min1;

			Matrix col2 = orig_x.getCol(i+1,i+1);
			double min2 = (3.0/4)* col2.getMin();
			double max2 = (4.0/3)*col2.getMax();
			double space2 = max2-min2;

			double**x = malloc_2(0,1000,2);
			for(int j=0;j<100;j++)
			{
				
				for(int k=0;k<100;k++)
				{
					x[k][i]=min1+((double)j/100)*space1;
					x[k][i+1]=max2-((double)k/100)*space2;
								

				}
				Matrix xm(x,100,2);
				Matrix y= pAlgo->predict(xm);
				draw_point2(m_chart,xm,y,input.label);
			}

		}
	}

}*/
void CMyDataProcDlg::draw_output(CChartCtrl& m_chart, Matrix y)
{
		XY_PARA input = pAlgo->getParasDrawInput();
		Matrix x = input.x_orig;

		CString name=_T("Predicted data");
		if(algo_type == LINEAR_REG_TYPE)
		{
			draw_line(m_chart,x,y,name);
		}
		else
		{
			Matrix label = input.label;
			draw_point2(m_chart,x,y,label);
		}
}


/********************************************button event response*******************************************************/

void CMyDataProcDlg::OnBnClickedButtonLoad() //use indepent chart, only with the input data
{
	// TODO: 在此添加控件通知处理程序代码
	if(loadData()==-1)
	{
		AfxMessageBox(TEXT("load data failed, try again"));
		return;
	}
	else if(num_label>2) //multilabels only used for logistic regresstion
	{
		CString popMsg = generateText("the datasets have ",num_label," labels, you should train them one by one");
		AfxMessageBox(popMsg);
		
		m_index.ShowWindow(SW_SHOW);
		SetDlgItemTextW(IDC_INDEX,_T("0"));
		GetDlgItem(IDC_LABEL_INDEX)->ShowWindow(SW_SHOW);
	}
	DEBUG_INFO(TEXT("Datasets loaded successed!"));
}
void CMyDataProcDlg::OnBnClickedButtonVisual()
{
	// TODO: 在此添加控件通知处理程序代码
	new_dialog = new inputDlg(this);
	BOOL valid = new_dialog->Create(IDD_INPUT,this);  //this way, the father is this dlg and both can live to execute

	if(valid==1)
	{
		if(!bDataLoad)
		{
			AfxMessageBox(TEXT("Plz load data at first"));
			return;
		}
		draw_input(new_dialog->m_chart);
		
		DEBUG_INFO(TEXT("visualizing input data"));
		new_dialog->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(TEXT("N ok"));
	}
}
void CMyDataProcDlg::OnBnClickedButtonRunAlgo() //use the member instance to record the input data, decesion boundat
{
	// TODO: 在此添加控件通知处理程序代码

	if(runMl()!=0)
	{
		return;
	}
}

void CMyDataProcDlg::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	if(AfxMessageBox(TEXT("Make sure you want to reset?"),MB_YESNO))
	{
		ResetAllParas();
	}
}


void CMyDataProcDlg::OnBnClickedButtonPredict()
{
	// TODO: 在此添加控件通知处理程序代码
	predictMl();
}


void CMyDataProcDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMyDataProcDlg::OnEnChangePredictrslt()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}




void CMyDataProcDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMyDataProcDlg::OnBnClickedButtonPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if(algo_type != NEURAL_NET_TYPE)
	{
		return;
	}
	if(para_set_dlg == NULL)
	{
		para_set_dlg = new DlgAdParas(this);
		BOOL valid = para_set_dlg->Create(IDD_DLG_PARAS,this);
		if(valid)
		{
			para_set_dlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		para_set_dlg->ShowWindow(SW_SHOW);
	}
}


void CMyDataProcDlg::OnBnClickedButtonMonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	pMonitor = new DlgRealTimeMonitor(this); //this way, both can use
	BOOL valid = pMonitor->Create(IDD_DLG_MONITOR,this);
	if(valid==1)
	{
		
		DEBUG_INFO(TEXT("Start to monitor in the Monitor Window"));
		pMonitor->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(TEXT("Start monitor failed!"));
	}

}

void CMyDataProcDlg::OnBnClickedButtonDrawDecisionBound()
{
	// TODO: 在此添加控件通知处理程序代码
	//draw decision boundary
	if(pAlgo==NULL)
	{
		AfxMessageBox(TEXT("Plz load data at first"));
	}
	pAlgo->getParasDrawInput();
	inputDlg* new_dialog = new inputDlg(this);
	BOOL valid = new_dialog->Create(IDD_INPUT,this);
	if(valid==1)
	{
		//draw_input(new_dialog->m_chart);
		draw_data_with_decision_boundary(new_dialog->m_chart);
		DEBUG_INFO(TEXT("visualizing input data"));
		new_dialog->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(TEXT("N ok"));
	}
}


void CMyDataProcDlg::OnBnClickedButtonDrawOutput()
{
	// TODO: 在此添加控件通知处理程序代码
	dlgPredict = new DlgPredict(this);
	BOOL valid = dlgPredict->Create(IDD_OUTPUT,this);  //this way, the father is this dlg and both can live to execute
	if(valid==1)
	{
		if(pAlgo== NULL || !bDataLoad)
		{
			AfxMessageBox(TEXT("Plz choose algorithm and train data, predict data at first"));
			return;
		}
		Matrix x = predict_rslt.x_orig;
		Matrix y = predict_rslt.y;
		//Matrix label = re.label;

		AXIS_SCALE sca;
		sca.bAuto = true;
	
		CString title=_T("Predict");
		CString leftaxis=_T("y");
		CString bottomaxis=_T("x1");
		buildAxis(dlgPredict->m_chart_predict,sca,title,leftaxis,bottomaxis);

		CString name=_T("Predicted data");
		if(algo_type == LINEAR_REG_TYPE)
		{
			draw_point(dlgPredict->m_chart_predict,x,y,name);
		}
		else
		{
			CString leftaxis=_T("x2");
			Matrix label = inputdata.label;
			draw_point2(dlgPredict->m_chart_predict,x,y,label);
		}
		
		DEBUG_INFO(TEXT("visualizing predict data"));
		dlgPredict->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxMessageBox(TEXT("N ok"));
	}
	return;
}


void CMyDataProcDlg::OnBnClickedButtonRate()
{
	// TODO: 在此添加控件通知处理程序代码
	if(pAlgo==NULL || !bDataLoad)
	{
		AfxMessageBox(TEXT("Plz choose algorithm and train data, predict data at first"));
		return;
	}
	CString train_rate = _T("");
	CString cv_rate = _T("");
	CString test_rate = _T("");

	GetDlgItem(IDC_EDIT_TRAIN_RATE)->GetWindowTextW(train_rate);
	GetDlgItem(IDC_EDIT_CROSS_VALIDATION)->GetWindowTextW(cv_rate);
	GetDlgItem(IDC_EDIT_TEST_RATE)->GetWindowTextW(test_rate);

	if(train_rate.IsEmpty())
	{
		AfxMessageBox(TEXT("Train rate is empty, will set it to 0.6"));
		rate.train_rate = DEFAULT_TRAIN_RATE;
	}
	else
	{
		rate.train_rate = str2double(CStr2Str(train_rate).c_str());
	}
	if(cv_rate.IsEmpty())
	{
		AfxMessageBox(TEXT("Cross Validation rate is empty, will set it to 0.2"));		
		rate.cv_rate = DEFAULT_CV_RATE;
	}
	else
	{
		rate.cv_rate = str2double(CStr2Str(cv_rate).c_str());
	}
	if(test_rate.IsEmpty())
	{
		AfxMessageBox(TEXT("Test rate is empty, will set it to 0.2"));	
		rate.test_rate = DEFAULT_TEST_RATE;
	}
	else
	{
		rate.test_rate = str2double(CStr2Str(test_rate).c_str());
	}

	pAlgo->setRates(rate);
}


void CMyDataProcDlg::OnBnClickedMfcbuttonPre()
{
	// TODO: 在此添加控件通知处理程序代码
		//01. get scale
	CString scale;
	CString norm;
	m_combo_scale.GetWindowTextW(scale);
	if(scale.Compare(TEXT("Yes"))==0)
	{
		pre_para.bScale = true;
	}
	else
	{
		pre_para.bScale = false;
	}
	//get norm
	m_combo_norm.GetWindowTextW(norm);
	if(norm.Compare(TEXT("Yes"))==0)
	{
		pre_para.bNorm = true;
	}
	else
	{
		pre_para.bNorm = false;
	}

	//get dim
	if(pre_para.bScale)
	{
		CString dimen = _T("");
		GetDlgItem(IDC_DIMENSION)->GetWindowTextW(dimen);
		pre_para.dim = (int)str2double(CStr2Str(dimen).c_str());
		CString tmp;
		tmp.Format(_T("%d"),pre_para.dim);
		AfxMessageBox(tmp);	
	}
	if(num_label>2)
	{
		CString index_str;
		m_index.GetWindowTextW(index_str);
		index = (int)str2double(CStr2Str(index_str).c_str());
	}
	pAlgo->setPreProcessPara(pre_para,index);
	preProcessSeted = true;
}
