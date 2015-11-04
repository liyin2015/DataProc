#pragma once
#include "ChartCtrl_source\ChartCtrl.h"
#include "afxwin.h"
#include "ChartCtrl_source\ChartCtrl.h"
#include "afxcmn.h"
#include "dlgComm.h"

#define DEBUG_MONITOR  m_list_eval.AddString

// DlgRealTimeMonitor �Ի���

class DlgRealTimeMonitor : public CDialogEx
{
	DECLARE_DYNAMIC(DlgRealTimeMonitor)

public:
	CChartCtrl m_chart;
	DlgRealTimeMonitor(CWnd* pParent=NULL);   // ��׼���캯��
	virtual ~DlgRealTimeMonitor();
	virtual BOOL OnInitDialog();

	void drawCostCurve();

// �Ի�������
	enum { IDD = IDD_DLG_MONITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDrawCost();
	afx_msg void OnBnClickedMfcbuttonHelp();
	afx_msg void OnBnClickedMfcbuttonEval();
	CListCtrl m_list_evalrslt;
	afx_msg void OnLvnItemchangedListEvalrslt(NMHDR *pNMHDR, LRESULT *pResult);
	ListControlMgr mgr;
};
