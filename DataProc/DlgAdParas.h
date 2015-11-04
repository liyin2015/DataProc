#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EditTable.h"


// DlgAdParas 对话框

class DlgAdParas : public CDialogEx
{
	DECLARE_DYNAMIC(DlgAdParas)

public:
	DlgAdParas(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgAdParas();

// 对话框数据
	enum { IDD = IDD_DLG_PARAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	//void InsertItems();


	DECLARE_MESSAGE_MAP()
public:
	CEditTable m_list_paras;
	CEdit m_edit_table;
	afx_msg void OnBnClickedOk();
	CComboBox m_combo_transfunc;
	CComboBox m_combo_method;
	//CEdit m_edit1;
	int hiddenLayers;
	int tempC;
	int tempR;
	//enter
	afx_msg void OnBnClickedMfcbuttonTotal();
};
