#include "StdAfx.h"
#include "ItemEdit.h"

CItemEdit::CItemEdit(void)
{
}

CItemEdit::~CItemEdit(void)
{
}


BEGIN_MESSAGE_MAP(CItemEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

BOOL CItemEdit::PreTranslateMessage(MSG* pMsg)
{
	return CEdit::PreTranslateMessage(pMsg);
}

void CItemEdit::OnKillFocus(CWnd* pNewWnd)
{
		
	CEdit::OnKillFocus(pNewWnd);
	CWnd* pParent = this->GetParent();
	::PostMessage(pParent->GetSafeHwnd(),WM_USER_EDIT_END,0,0);

	// TODO: 在此处添加消息处理程序代码
}
