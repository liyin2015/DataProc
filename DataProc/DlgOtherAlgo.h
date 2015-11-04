#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "ml\otherAlgo\apriori.hpp"
#include "ml\otherAlgo_Base.hpp"

#define DEBUG_OTHERALGO m_other_info.AddString
// DlgOtherAlgo 对话框
typedef enum
{
	APRIORI_TYPE = 0,
}OTHERALGO_TYPE;
class DlgOtherAlgo : public CDialogEx
{
	DECLARE_DYNAMIC(DlgOtherAlgo)

public:
	DlgOtherAlgo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgOtherAlgo();
	void ResetAllParas();
	virtual BOOL OnInitDialog();
	void OnShowWindow(BOOL bShow,UINT nFlag) 
	{      
		this->MoveWindow(100,100,100,100); //this is to make sure the supervise dlg and the cluster dialog show at the same place
	}
	//functions that used to support
	int getAlgo();
	int loadData();

	void getPara();
	int runMl();
// 对话框数据
	enum { IDD = IDD_DIALOG_OTHERALGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	
	
public:
	bool bDataLoad;
	OTHERALGO_TYPE algo_type;

	OTHER_ALGO_PARA para;

	OtherAlgo_Base* pOtherAlgo;

	CComboBox m_combo_algo;
	HICON m_hIcon;
	CMFCEditBrowseCtrl m_editbrowse_input;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRunAlgo();
	CListBox m_other_info;
	CEdit m_edit_min;
	afx_msg void OnBnClickedButtonReset();
};
