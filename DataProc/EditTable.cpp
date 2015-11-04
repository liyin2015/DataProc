#include "StdAfx.h"
#include "EditTable.h"

CEditTable::CEditTable(void)
{
}

CEditTable::~CEditTable(void)
{
}
BEGIN_MESSAGE_MAP(CEditTable, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_USER_EDIT_END,EndEdit)
END_MESSAGE_MAP()

void CEditTable::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	POSITION pos;
	BOOL bSelected = FALSE;
	if( m_bEditing ==TRUE)
	goto defalt_session;	// 检查是否有Item被选中，没有时不进入编辑
	pos = GetFirstSelectedItemPosition();
	if( pos )
	{
		// 得到被点击的Item
		LVHITTESTINFO testinfo;
		testinfo.pt.x = point.x;
		testinfo.pt.y = point.y;            //点击时的鼠标位置
		testinfo.flags = LVHT_ONITEMLABEL;    
		if( SubItemHitTest(&testinfo)<0 )    //点击的必须是标题
			goto defalt_session;            //没有点在有效区域，不进入编辑
		m_nItem = testinfo.iItem;            //被点击表项的行号
		m_nSubItem = testinfo.iSubItem;    //被点击表项的列号
		if(m_nSubItem == 0)
		{
		goto defalt_session;            //选中第一列，不编辑
		}
		// 检查该表项是否被选中，没被选中不进入编辑
		while( pos )
			if( m_nItem==GetNextSelectedItem(pos) )
			{
				bSelected = TRUE;
				break;
			}
			if( bSelected==FALSE )
				goto defalt_session;            //没有点在有效区域，不编辑
			// 开始编辑
			m_bEditing = BeginEdit();
			return;
	}
defalt_session:
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CEditTable::BeginEdit()
{
	// 得到被编辑表项的区域
	CRect rect;
	if( GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rect)==FALSE )
		return FALSE;
	// 创建编辑控件
	int style =     WS_CHILD |
		WS_CLIPSIBLINGS |
		WS_EX_TOOLWINDOW |
		WS_BORDER;
	if( m_edit.Create(style, rect, this, 101)==FALSE )
		return FALSE;
	// 取被编辑表项的文字
	CString txtItem = GetItemText( m_nItem, m_nSubItem );
	// 取出的文字填写到编辑控件
	m_edit.SetWindowText( txtItem );
	m_edit.SetFocus();
	m_edit.SetSel( 0, -1 );
	m_edit.ShowWindow( SW_SHOW );
	return TRUE;
}

LRESULT CEditTable::EndEdit(WPARAM wParam,LPARAM lParam)
{
	
	// 销毁编辑窗口
		CString txtItem;
		m_edit.GetWindowText( txtItem );
		SetItemText(m_nItem, m_nSubItem, txtItem);
	m_edit.DestroyWindow();
	m_bEditing = FALSE;
	  return 0;
}
