#pragma once


// AboutDlg �Ի���

class AboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AboutDlg)

public:
	AboutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
