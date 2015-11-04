#pragma once


// DlgMain 对话框

class DlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMain)

public:
	DlgMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgMain();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
