#pragma once
#include "afxcmn.h"
#include "ItemEdit.h"
//#include "PRIMDlg.h"
class CEditTable :
	public CListCtrl
{
public:
	CEditTable(void);
	~CEditTable(void);

public:
	DECLARE_MESSAGE_MAP()
	CItemEdit  m_edit; //便是行和列的坐标变量
	int m_nItem;   //被编辑表项的行号
	int m_nSubItem; //列号
	BOOL m_bEditing;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL BeginEdit();
	afx_msg LRESULT EndEdit(WPARAM wParam,LPARAM lParam);
};
