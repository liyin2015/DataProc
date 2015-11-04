// DlgRealTimeMonitor.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgRealTimeMonitor.h"
#include "afxdialogex.h"
#include <CommCtrl.h>


// DlgRealTimeMonitor 对话框

IMPLEMENT_DYNAMIC(DlgRealTimeMonitor, CDialogEx)

DlgRealTimeMonitor::DlgRealTimeMonitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgRealTimeMonitor::IDD, pParent)
{
	mgr.setListControlId(IDC_LIST_EVALRSLT);
}

DlgRealTimeMonitor::~DlgRealTimeMonitor()
{
}

void DlgRealTimeMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_COSTCURVE, m_chart); 
	DDX_Control(pDX, IDC_LIST_EVALRSLT, m_list_evalrslt);
}

BOOL DlgRealTimeMonitor::OnInitDialog()
{
	CDialog::OnInitDialog();
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
	DWORD dwStyle = m_list_evalrslt.GetExtendedStyle(); 
	m_list_evalrslt.SetExtendedStyle(dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_list_evalrslt.InsertColumn(0,TEXT("Item"),LVCFMT_CENTER,80,0);
	m_list_evalrslt.InsertColumn(1,TEXT("Value"),LVCFMT_CENTER,100,1);
	m_list_evalrslt.InsertColumn(2,TEXT("Explaination"),LVCFMT_LEFT,400,2);
	HWND hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_EVALRSLT);
	LVITEM item;
	item.iItem = 1;
	//first col
	int row = 0;
	CString value = _T("True Positive");
	mgr.SetCell(m_hWnd, value,0,0);
	value = _T("False Positive");
	mgr.SetCell(m_hWnd, value,1,0);
	value = _T("True Negative");
	mgr.SetCell(m_hWnd,value,2,0);
	value = _T("False Negative");
	mgr.SetCell(m_hWnd,value,3,0);

	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,4,0);

	//accuracy precision
	value = _T("Train Accuracy");
	mgr.SetCell(m_hWnd,value,5,0);
	value = _T("Test Accuracy");
	mgr.SetCell(m_hWnd, value,6,0);
	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,7,0);

	value = _T("Precision");
	mgr.SetCell(m_hWnd, value,8,0);
	value =  _T("Recall");
	mgr.SetCell(m_hWnd, value,9,0);
	value = _T("PR");
	mgr.SetCell(m_hWnd,value,10,0);

	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,11,0);
	//cost
	value = _T("Jtrain");
	mgr.SetCell(m_hWnd, value,12,0);
	value = _T("Jcv");
	mgr.SetCell(m_hWnd, value,13,0);
	value = _T("Jtest");
	mgr.SetCell(m_hWnd, value,14,0);

	//3rd col, explaination
	//true postive
	int col = 2;
	value = _T("Both the predict and the original result are positive");

	mgr.SetCell(m_hWnd, value,0,col);
	//False Positive
	value = _T("The predict is positive but the orinal is negative");
	mgr.SetCell(m_hWnd, value,1,col);
	//True Negative
	value = _T("Both the predict and the original result are negative");
	mgr.SetCell(m_hWnd, value,2,col);
	//False Negative
	value = _T("The predict is negative but the original is positive");
	mgr.SetCell(m_hWnd, value,3,col);

	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,4,col);

	//accuracy precision
	//Train Accuracy
	value = _T("The Accuracy of the trainsets that got from the train rate");
	mgr.SetCell(m_hWnd, value,5,col);
	//Test Accuracy
	value = _T("The Accuracy of the testsets that got from 1-train rate");
	mgr.SetCell(m_hWnd, value,6,col);
	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,7,col);

	//Precision
	value = _T("Of all testsets(1-train_rate) that we predicted y=1, what fraction actually is 1");
	mgr.SetCell(m_hWnd, value,8,col);
	//Recall
	value =  _T("Of all testsets(1-train_rate) that actually is y=1, what fraction did we correctly detected as 1");
	mgr.SetCell(m_hWnd, value,9,col);
	value = _T("PR");
	mgr.SetCell(m_hWnd, value,10,col);

	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,11,col);
	//cost
	//Jtrain
	value = _T("Train error(train_rate)");
	mgr.SetCell(m_hWnd, value,12,col);
	//Jcv
	value = _T("Cross Validation error(cv_rate)");
	mgr.SetCell(m_hWnd, value,13,col);
	//Jtest
	value = _T("Test error(test_rate)");
	mgr.SetCell(m_hWnd, value,14,col);
	return true;
}

BEGIN_MESSAGE_MAP(DlgRealTimeMonitor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_COST, &DlgRealTimeMonitor::OnBnClickedButtonDrawCost)
	//ON_BN_CLICKED(IDC_MFCBUTTON_LEANCURVE, &DlgRealTimeMonitor::OnBnClickedMfcbuttonLeancurve)
	ON_BN_CLICKED(IDC_MFCBUTTON_HELP, &DlgRealTimeMonitor::OnBnClickedMfcbuttonHelp)
	ON_BN_CLICKED(IDC_MFCBUTTON_EVAL, &DlgRealTimeMonitor::OnBnClickedMfcbuttonEval)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EVALRSLT, &DlgRealTimeMonitor::OnLvnItemchangedListEvalrslt)
END_MESSAGE_MAP()
/************************************internal functions*******************************************************/
void DlgRealTimeMonitor::drawCostCurve()
{
	//01. clear the former chart
	m_chart.GetTitle()->RemoveAll();
	m_chart.RemoveAllSeries();
	//02. get parent dialog
	CMyDataProcDlg* pMain =(CMyDataProcDlg*)this->GetParent();

	//03. getCost
	if(pMain->pAlgo == NULL)
	{
		AfxMessageBox(TEXT("Plz choose algorithm, load data and train data at first!"));
		return;
	}
	if(pMain->algo_type == SVM_TYPE)
	{
		AfxMessageBox(TEXT("Svm have no cost curve!"));
		return;
	}
	Matrix j= pMain->pAlgo->mJ;
	int m = j.length();

	Matrix x(m,1,0);
	for(int i=0;i<m;i++)
	{
		*x(i,0)=i+1;
	}
	
	AXIS_SCALE sca;
	sca.min_bot = x.getMin()-2;
	sca.max_bot = x.getMax()+2;

	sca.min_lef = j.getMin()-2;
	sca.max_lef = j.getMax()+2;
	sca.bAuto = false;
	CString name = _T("cost curve");
	CString leftaxis = _T("cost");
	CString bottomaxis = _T("number of iteration");
	buildAxis(m_chart,sca,name,leftaxis,bottomaxis);

	name =_T("cost curve");
	draw_line(m_chart,x,j, name);
}

// DlgRealTimeMonitor 消息处理程序

/*********************************************even handle functions*******************************************/
void DlgRealTimeMonitor::OnBnClickedButtonDrawCost()
{
	// TODO: 在此添加控件通知处理程序代码
	drawCostCurve();
}


/*void DlgRealTimeMonitor::OnBnClickedMfcbuttonLeancurve()
{
	// TODO: 在此添加控件通知处理程序代码
}*/


void DlgRealTimeMonitor::OnBnClickedMfcbuttonHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox(TEXT("Plz choose algorithm, load data and train data at first!"));
}


void DlgRealTimeMonitor::OnBnClickedMfcbuttonEval()
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
	pMain->pAlgo->getEvalRslt();
	RSLT_EVAL rslt = pMain->pAlgo->eval_rslt;
	int col = 1;
	//"True Positive"
	HWND hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_EVALRSLT);
	CString value = generateText("",rslt.ac1_pre1,"");
	mgr.SetCell(m_hWnd,value,0,col);
	//"False Positive"
	value = generateText("",rslt.ac0_pre1,"");
	mgr.SetCell(m_hWnd,value,1,col);
	//"True Negative"
	value = generateText("",rslt.ac0_pre0,"");
	mgr.SetCell(m_hWnd, value,2,col);
	//"False Negative"
	value = generateText("",rslt.ac1_pre0,"");
	mgr.SetCell(m_hWnd, value,3,col);

	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,4,col);

	//accuracy precision
	//"Train Accuracy"
	value = generateText("",rslt.train_accuracy,"");
	mgr.SetCell(m_hWnd, value,5,col);
	//"Test Accuracy"
	value = generateText("",rslt.test_accuracy,"");
	mgr.SetCell(m_hWnd, value,6,col);
	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,7,col);

	//"Precision"
	value = generateText("",rslt.precision,"");
	mgr.SetCell(m_hWnd, value,8,col);
	//recall 
	value = generateText("",rslt.recall,"");
	mgr.SetCell(m_hWnd, value,9,col);
	//"PR"
	value = generateText("",rslt.PR,"");
	mgr.SetCell(m_hWnd, value,10,col);

	//null row
	value = _T("");
	mgr.SetCell(m_hWnd, value,11,col);
	//cost
	//"Jtrain"
	value = generateText("",rslt.Jtrain,"");
	mgr.SetCell(m_hWnd, value,12,col);
	//"Jcv"
	value = generateText("",rslt.Jcv,"");
	mgr.SetCell(m_hWnd, value,13,col);
	//"Jtest"
	value = generateText("",rslt.Jtest,"");
	mgr.SetCell(m_hWnd, value,14,col);
}


void DlgRealTimeMonitor::OnLvnItemchangedListEvalrslt(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
