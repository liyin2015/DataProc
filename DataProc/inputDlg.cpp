// inputDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataProc.h"
#include "inputDlg.h"
#include "afxdialogex.h"
#include "ChartCtrl_source\ChartTitle.h"

// inputDlg �Ի���

IMPLEMENT_DYNAMIC(inputDlg, CDialogEx)

inputDlg::inputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(inputDlg::IDD, pParent)
{

}

inputDlg::~inputDlg()
{

}

void inputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_CHARTCTRL,m_chart);
}
BOOL inputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return true;
}


BEGIN_MESSAGE_MAP(inputDlg, CDialogEx)
	//ON_BN_CLICKED(IDOK, &inputDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON1, &inputDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// inputDlg ��Ϣ�������


/*void inputDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
   
}*/

