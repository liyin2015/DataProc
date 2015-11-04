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
	CItemEdit  m_edit; //�����к��е��������
	int m_nItem;   //���༭������к�
	int m_nSubItem; //�к�
	BOOL m_bEditing;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL BeginEdit();
	afx_msg LRESULT EndEdit(WPARAM wParam,LPARAM lParam);
};
