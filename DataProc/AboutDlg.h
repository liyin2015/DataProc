#pragma once


// AboutDlg 对话框

class AboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AboutDlg)

public:
	AboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
