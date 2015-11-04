#pragma once
#include "afxwin.h"
# define   WM_USER_EDIT_END WM_USER+1
class CItemEdit :
	public CEdit
{
public:
	CItemEdit(void);
	~CItemEdit(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	bool m_bInputValid;//表示编辑是否有效
};
