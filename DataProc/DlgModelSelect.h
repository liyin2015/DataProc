#pragma once
#include "ChartCtrl_source\ChartCtrl.h"

// DlgModelSelect �Ի���

class DlgModelSelect : public CDialogEx
{
	DECLARE_DYNAMIC(DlgModelSelect)

public:
	CChartCtrl m_chart_learncurve;
	DlgModelSelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgModelSelect();

// �Ի�������
	enum { IDD = IDD_DIALOG_SELECT_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
