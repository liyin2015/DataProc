#pragma once

#include "ChartCtrl_source\ChartCtrl.h"
// inputDlg �Ի���

class inputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(inputDlg)

public:
	//chartctrl
	CChartCtrl m_chart;
	CChartCtrl chart_decision_bound;
	//CChartCtrl m_chart2;

	inputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~inputDlg();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

};
