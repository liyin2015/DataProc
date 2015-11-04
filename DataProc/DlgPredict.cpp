// DlgPredict.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgPredict.h"
#include "afxdialogex.h"


// DlgPredict 对话框

IMPLEMENT_DYNAMIC(DlgPredict, CDialogEx)

DlgPredict::DlgPredict(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgPredict::IDD, pParent)
{

}

DlgPredict::~DlgPredict()
{
}

void DlgPredict::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL_PREDICT, m_chart_predict);
}


BEGIN_MESSAGE_MAP(DlgPredict, CDialogEx)
END_MESSAGE_MAP()


// DlgPredict 消息处理程序
