
// DataProcDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "inputDlg.h"
#include "ml\algo_basic.h"
#include "ml\supervise\linear_regression.hpp"
#include "ml\supervise\logistic_regression.hpp"
#include "ml\supervise\neural_network.hpp"
#include "ml\supervise\svm.hpp"
#include "ml\io_manager.hpp"
#include "common.hpp"
#include <afx.h>
#include "DlgAdParas.h"
#include "DlgCluster.h"
#include "DlgRealTimeMonitor.h"
#include "DlgPredict.h"

#define DEBUG_INFO m_listbox_info.AddString




// CMyDataProcDlg 对话框
class CMyDataProcDlg : public CDialogEx
{
// 构造
public:
	CMyDataProcDlg(CWnd* pParent = NULL);	// 标准构造函数
	void ResetAllParas();
	~CMyDataProcDlg();

// 对话框数据
	enum { IDD = IDD_MYDATAPROC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	

//used to take the m l
	int trainSigLabel(int index);

	int runMl();
	int predictMl();

    //include data preprecessing and train parameters
	void getTrainPara();
	int loadData();


	void draw_input(CChartCtrl& );//use the same m_chart since they both use input data
	void draw_data_with_decision_boundary(CChartCtrl& m_chart);
	void draw_output(CChartCtrl&, Matrix y);
	//void execute_algo
	int getAlgo();

	Algo_Basic* getPAlgo();


public:
	//PALOG palog;
	ALGO_TYPE algo_type;

	bool bRand;

	RATE_PARA rate;

	//template<typename T>
	Algo_Basic* pAlgo;

	//train para
	SUPERVISE_PARA paras;
	PRE_PROCESS_PARA pre_para;

	//int index;

	XY_PARA predict_rslt;
	XY_PARA inputdata;
	
	//data processing
	bool bScale;
	bool bNorm;
	int dim;
	int num_label;
	int index;
	//
	bool bDataLoad;
	bool preProcessSeted;

// 实现
public:
	HICON m_hIcon;
	inputDlg* new_dialog;
	DlgPredict* dlgPredict;
	DlgRealTimeMonitor* pMonitor;
	DlgAdParas* para_set_dlg;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	void OnShowWindow(BOOL bShow,UINT nFlag) 
	{      
		this->MoveWindow(100,100,100,100);
	}
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeMfceditbrowse1();
	afx_msg void OnBnClickedButtonRunAlgo();

	//CString algo;
	//afx_msg void OnLbnDblclkListAlgorithm();
	//inputDlg* new_dialog;
	CListBox m_listbox_info;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnLbnSelchangeInfo();
	afx_msg void OnEnChangeInputfile();
	afx_msg void OnEnChangePredictfile();

	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonPredict();
	afx_msg void OnCbnSelchangeCombo1();//combo box
	//afx_msg void OnBnClickedMfcbutton1();
	afx_msg void OnEnChangePredictrslt();
	afx_msg void OnBnClickedButtonVisual();
	CEdit m_index;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_commo_algo;
	CComboBox m_combo_norm;
	CComboBox m_combo_scale;
	afx_msg void OnBnClickedButtonPara();

	POINT oldWin;
	afx_msg void OnBnClickedButtonMonitor();
	afx_msg void OnBnClickedButtonStopMon();
	afx_msg void OnBnClickedButtonDrawDecisionBound();
	CComboBox m_combo_rand;
	afx_msg void OnBnClickedButtonDrawOutput();
	CComboBox m_combo_method;
	afx_msg void OnBnClickedButtonRate();
	afx_msg void OnBnClickedMfcbuttonPre();
};
