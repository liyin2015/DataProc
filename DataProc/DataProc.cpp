
// DataProc.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DataProc.h"
#include "DataProcDlg.h"
#include "AboutDlg.h"
#include "DlgCluster.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// cMyDataProcApp

BEGIN_MESSAGE_MAP(cMyDataProcApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_HELP_ABOUT, &cMyDataProcApp::OnHelpAbout)
	ON_COMMAND(ID_CATEGORY_CLUSTERING, &cMyDataProcApp::OnCategoryClustering)
	ON_COMMAND(ID_CATEGORY_SUPERVISE, &cMyDataProcApp::OnCategorySupervise)
	ON_COMMAND(ID_HELP_CHECKHELPDOCUMENTS, &cMyDataProcApp::OnHelpCheckhelpdocuments)
	ON_COMMAND(ID_CATEGORY_OTHERALGORITHMS, &cMyDataProcApp::OnCategoryOtheralgorithms)
END_MESSAGE_MAP()


// cMyDataProcApp 构造

cMyDataProcApp::cMyDataProcApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	pSpvise = NULL;
	pCluster = NULL;
	dlg_type = SUPERVISE_TYPE;
	pOtherAlgo = NULL;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}
cMyDataProcApp::~cMyDataProcApp()
{
	if(pSpvise!=NULL)
	{			
		delete pSpvise;
		pSpvise = NULL;
	}
	if(pCluster!=NULL)
	{			
		delete pCluster;
		pCluster = NULL;
	}
	if(pOtherAlgo!=NULL)
	{
		delete pOtherAlgo;
		pOtherAlgo=NULL;
	}
}


// 唯一的一个 cMyDataProcApp 对象

cMyDataProcApp theApp;


// cMyDataProcApp 初始化

BOOL cMyDataProcApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	init_log();//used to init the log setting in model ML

	pSpvise = new CMyDataProcDlg;
	dlg_type =SUPERVISE_TYPE;
	
	m_pMainWnd = pSpvise;

	
	//pCluster->DoModal();
	//pCluster->ShowWindow(SW_HIDE);
	INT_PTR nResponse = pSpvise->DoModal();
	if (nResponse == IDOK)
	{
		AfxMessageBox(TEXT("ok1"));
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		AfxMessageBox(TEXT("ok12"));
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}


	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


//used for mainfraim response
void cMyDataProcApp::OnHelpAbout()
{
	// TODO: 在此添加命令处理程序代码
	AboutDlg dlgAbout;
	dlgAbout.DoModal();

}



void cMyDataProcApp::OnCategoryClustering()
{
	// TODO: 在此添加命令处理程序代码
	//pMyDataDlg->ShowWindow(SW_HIDE);
	if(dlg_type == CLUSTER_TYPE)
	{
		return;
	}
	//hide former
	if(dlg_type==OTHER_ALGO_TYPE && pOtherAlgo!=NULL)
	{
		pOtherAlgo->ShowWindow(SW_HIDE);
	}
	else if(dlg_type == SUPERVISE_TYPE && pSpvise!=NULL)
	{
		pSpvise->ShowWindow(SW_HIDE);
	}
	if(pCluster == NULL)
	{
		dlg_type = CLUSTER_TYPE;
		pCluster = new DlgCluster;
		m_pMainWnd = pCluster;
	
		pCluster->DoModal();
		
	}
	else
	{
		dlg_type = CLUSTER_TYPE;
		m_pMainWnd = pCluster;
		pCluster ->ShowWindow(SW_SHOW);	
	}	
	return;
}


void cMyDataProcApp::OnCategorySupervise()
{
	// TODO: 在此添加命令处理程序代码
	if(dlg_type == SUPERVISE_TYPE)
	{
		return;
	}
	//hide former
	if(dlg_type==OTHER_ALGO_TYPE && pOtherAlgo!=NULL)
	{
		pOtherAlgo->ShowWindow(SW_HIDE);
	}
	else if(dlg_type == CLUSTER_TYPE&&pCluster!=NULL )
	{
		pCluster->ShowWindow(SW_HIDE);
	}

	if(pSpvise != NULL)
	{
		dlg_type = SUPERVISE_TYPE;
		m_pMainWnd = pSpvise;
		pSpvise->ShowWindow(SW_SHOW);
	}
}
void cMyDataProcApp::OnCategoryOtheralgorithms()
{
	// TODO: 在此添加命令处理程序代码
	if(dlg_type == OTHER_ALGO_TYPE)
	{
		return;
	}
		//hide the first
	if(dlg_type==SUPERVISE_TYPE && pSpvise!=NULL)
	{
		pSpvise->ShowWindow(SW_HIDE);
	}
	else if(dlg_type == CLUSTER_TYPE && pCluster!=NULL)
	{
		pCluster->ShowWindow(SW_HIDE);
	}

	if(pOtherAlgo == NULL)
	{
		dlg_type = OTHER_ALGO_TYPE;
		pOtherAlgo = new DlgOtherAlgo;
		m_pMainWnd = pOtherAlgo;
	
		pOtherAlgo->DoModal();
		
	}
	else
	{
		dlg_type = OTHER_ALGO_TYPE;
		m_pMainWnd = pOtherAlgo;
		pOtherAlgo ->ShowWindow(SW_SHOW);
	}
}


void cMyDataProcApp::OnHelpCheckhelpdocuments()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(TEXT("The settings in INPUT and SETTING RATES areas shoube be kept same if it is for multi-label situation."));
}



