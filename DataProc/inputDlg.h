#pragma once

#include "ChartCtrl_source\ChartCtrl.h"
// inputDlg 对话框

class inputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(inputDlg)

public:
	//chartctrl
	CChartCtrl m_chart;
	CChartCtrl chart_decision_bound;
	//CChartCtrl m_chart2;

	inputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~inputDlg();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

};
