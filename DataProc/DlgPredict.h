#pragma once
#include "ChartCtrl_source\ChartCtrl.h"


// DlgPredict 对话框

class DlgPredict : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPredict)

public:
	CChartCtrl m_chart_predict;
	DlgPredict(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgPredict();

// 对话框数据
	enum { IDD = IDD_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
