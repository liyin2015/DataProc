// DlgModelSelect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgModelSelect.h"
#include "afxdialogex.h"


// DlgModelSelect �Ի���

IMPLEMENT_DYNAMIC(DlgModelSelect, CDialogEx)

DlgModelSelect::DlgModelSelect(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgModelSelect::IDD, pParent)
{

}

DlgModelSelect::~DlgModelSelect()
{
}

void DlgModelSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_LEARNCURVE, m_chart_learncurve);
	
}


BEGIN_MESSAGE_MAP(DlgModelSelect, CDialogEx)
END_MESSAGE_MAP()


// DlgModelSelect ��Ϣ�������
