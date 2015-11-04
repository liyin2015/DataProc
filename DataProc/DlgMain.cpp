// DlgMain.cpp : 实现文件
//

#include "stdafx.h"
#include "DataProc.h"
#include "DlgMain.h"
#include "afxdialogex.h"


// DlgMain 对话框

IMPLEMENT_DYNAMIC(DlgMain, CDialogEx)

DlgMain::DlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgMain::IDD, pParent)
{

}

DlgMain::~DlgMain()
{
}

void DlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgMain, CDialogEx)
END_MESSAGE_MAP()


// DlgMain 消息处理程序
