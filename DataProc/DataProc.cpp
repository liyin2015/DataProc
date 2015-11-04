
// DataProc.cpp : ����Ӧ�ó��������Ϊ��
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


// cMyDataProcApp ����

cMyDataProcApp::cMyDataProcApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	pSpvise = NULL;
	pCluster = NULL;
	dlg_type = SUPERVISE_TYPE;
	pOtherAlgo = NULL;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
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


// Ψһ��һ�� cMyDataProcApp ����

cMyDataProcApp theApp;


// cMyDataProcApp ��ʼ��

BOOL cMyDataProcApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		AfxMessageBox(TEXT("ok12"));
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}


	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


//used for mainfraim response
void cMyDataProcApp::OnHelpAbout()
{
	// TODO: �ڴ���������������
	AboutDlg dlgAbout;
	dlgAbout.DoModal();

}



void cMyDataProcApp::OnCategoryClustering()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	AfxMessageBox(TEXT("The settings in INPUT and SETTING RATES areas shoube be kept same if it is for multi-label situation."));
}



