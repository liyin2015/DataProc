#pragma once
#include "ChartCtrl_source\ChartCtrl.h"


// DlgPredict �Ի���

class DlgPredict : public CDialogEx
{
	DECLARE_DYNAMIC(DlgPredict)

public:
	CChartCtrl m_chart_predict;
	DlgPredict(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgPredict();

// �Ի�������
	enum { IDD = IDD_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
