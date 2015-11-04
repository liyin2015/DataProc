#pragma once
#include "ChartCtrl_source\ChartCtrl.h"

// DlgModelSelect 对话框

class DlgModelSelect : public CDialogEx
{
	DECLARE_DYNAMIC(DlgModelSelect)

public:
	CChartCtrl m_chart_learncurve;
	DlgModelSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgModelSelect();

// 对话框数据
	enum { IDD = IDD_DIALOG_SELECT_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
