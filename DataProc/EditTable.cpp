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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	POSITION pos;
	BOOL bSelected = FALSE;
	if( m_bEditing ==TRUE)
	goto defalt_session;	// ����Ƿ���Item��ѡ�У�û��ʱ������༭
	pos = GetFirstSelectedItemPosition();
	if( pos )
	{
		// �õ��������Item
		LVHITTESTINFO testinfo;
		testinfo.pt.x = point.x;
		testinfo.pt.y = point.y;            //���ʱ�����λ��
		testinfo.flags = LVHT_ONITEMLABEL;    
		if( SubItemHitTest(&testinfo)<0 )    //����ı����Ǳ���
			goto defalt_session;            //û�е�����Ч���򣬲�����༭
		m_nItem = testinfo.iItem;            //�����������к�
		m_nSubItem = testinfo.iSubItem;    //�����������к�
		if(m_nSubItem == 0)
		{
		goto defalt_session;            //ѡ�е�һ�У����༭
		}
		// ���ñ����Ƿ�ѡ�У�û��ѡ�в�����༭
		while( pos )
			if( m_nItem==GetNextSelectedItem(pos) )
			{
				bSelected = TRUE;
				break;
			}
			if( bSelected==FALSE )
				goto defalt_session;            //û�е�����Ч���򣬲��༭
			// ��ʼ�༭
			m_bEditing = BeginEdit();
			return;
	}
defalt_session:
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

BOOL CEditTable::BeginEdit()
{
	// �õ����༭���������
	CRect rect;
	if( GetSubItemRect(m_nItem, m_nSubItem, LVIR_LABEL, rect)==FALSE )
		return FALSE;
	// �����༭�ؼ�
	int style =     WS_CHILD |
		WS_CLIPSIBLINGS |
		WS_EX_TOOLWINDOW |
		WS_BORDER;
	if( m_edit.Create(style, rect, this, 101)==FALSE )
		return FALSE;
	// ȡ���༭���������
	CString txtItem = GetItemText( m_nItem, m_nSubItem );
	// ȡ����������д���༭�ؼ�
	m_edit.SetWindowText( txtItem );
	m_edit.SetFocus();
	m_edit.SetSel( 0, -1 );
	m_edit.ShowWindow( SW_SHOW );
	return TRUE;
}

LRESULT CEditTable::EndEdit(WPARAM wParam,LPARAM lParam)
{
	
	// ���ٱ༭����
		CString txtItem;
		m_edit.GetWindowText( txtItem );
		SetItemText(m_nItem, m_nSubItem, txtItem);
	m_edit.DestroyWindow();
	m_bEditing = FALSE;
	  return 0;
}
