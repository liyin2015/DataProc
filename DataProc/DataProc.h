
// DataProc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DataProcDlg.h"
#include "DlgOtherAlgo.h"

// cMyDataProcApp:
// �йش����ʵ�֣������ DataProc.cpp
//
typedef enum
{
	SUPERVISE_TYPE = 0,
	CLUSTER_TYPE = 1,
	OTHER_ALGO_TYPE =2,
}MAINDLG_TYPE;

class cMyDataProcApp : public CWinApp
{
public:
	cMyDataProcApp();
	~cMyDataProcApp();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelpAbout();

public:
	//CMyDataProcDlg* pMyDataDlg;
	CMyDataProcDlg* pSpvise;;
	DlgCluster* pCluster;
	DlgOtherAlgo* pOtherAlgo;
	MAINDLG_TYPE dlg_type;
	afx_msg void OnCategoryClustering();
	afx_msg void OnCategorySupervise();
	afx_msg void OnHelpCheckhelpdocuments();
	afx_msg void OnCategoryOtheralgorithms();
};

extern cMyDataProcApp theApp;