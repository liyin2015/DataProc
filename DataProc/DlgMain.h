#pragma once


// DlgMain �Ի���

class DlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMain)

public:
	DlgMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgMain();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
