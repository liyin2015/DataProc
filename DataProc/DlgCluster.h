#pragma once
#include "afxwin.h"
#include "ml/cluster_base.h"
#include "ml/clustering/level_cluster.hpp"
#include "common.hpp"
#include "ChartCtrl_source\draw_lib.h"

#define DEBUG_CLUSTER m_cluster_info.AddString


// DlgCluster 对话框

class DlgCluster : public CDialogEx
{
	DECLARE_DYNAMIC(DlgCluster)

public:
	DlgCluster(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgCluster();
	void ResetAllParas();
	virtual BOOL OnInitDialog();
	void OnShowWindow(BOOL bShow,UINT nFlag) 
	{      
		this->MoveWindow(100,100,100,100); //this is to make sure the supervise dlg and the cluster dialog show at the same place
	}

	int getAlgo();

	int loadData();
	void getTrainPara();

	int runMl();

	void draw_input(CChartCtrl& m_chart);

public:


	bool bDataLoad;
	Cluster_Base* pCluster;
	ALGO_TYPE algo_type;
	CLUSTER_TRAIN_PARA para;

// 对话框数据
	enum { IDD = IDD_DIALOG_CLUSTER };

protected:
	inputDlg* pDlgChart;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HICON m_hIcon;

	CComboBox m_combo_cluster;
	afx_msg void OnBnClickedButtonLoad();
	CListBox m_cluster_info;
	afx_msg void OnBnClickedButtonVisual();
	afx_msg void OnBnClickedButtonRunAlgo();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnLbnSelchangeInfo();
};
