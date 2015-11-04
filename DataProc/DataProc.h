
// DataProc.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "DataProcDlg.h"
#include "DlgOtherAlgo.h"

// cMyDataProcApp:
// 有关此类的实现，请参阅 DataProc.cpp
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
// 重写
public:
	virtual BOOL InitInstance();

// 实现

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