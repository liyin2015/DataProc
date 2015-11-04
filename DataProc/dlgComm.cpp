#include "stdafx.h"
#include "dlgComm.h"
ListControlMgr::ListControlMgr()
{
	nItem = -2;
	nSubItem =-2;
	edit_control_id = -1;
	list_control_id = -1;
}
ListControlMgr::~ListControlMgr()
{
}
//used for list control to add one item
void ListControlMgr::SetCell(HWND hwnd,CString value, int row, int col)
{
	HWND hwndList = ::GetDlgItem(hwnd, list_control_id);
	TCHAR szString [256];
	wsprintf(szString,value ,0);
	//Fill the LVITEM structure with the
	//values given as parameters.
	LVITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = row;
	lvItem.pszText = szString;
	lvItem.iSubItem = col;
	if(col >0)
		//set the value of listItem
		::SendMessage(hwndList,LVM_SETITEM,
		(WPARAM)0,(WPARAM)&lvItem);
	else
		//Insert the value into List
		ListView_InsertItem(hwndList,&lvItem);
}
void ListControlMgr::setEditControlId(int id)
{
	edit_control_id = id;
}
void ListControlMgr::setListControlId(int id)
{
	list_control_id = id;
}
//throught row and col number to get the text
CString ListControlMgr::GetItemText(HWND hwnd, int nItem,int nSubItem)const
{
	LVITEM lvi;
	memset(&lvi,0,sizeof(LVITEM));
	lvi.iSubItem = nSubItem;
	CString str;
	int nLen = 150;
	int nRes;
	//do
	//{
		//nLen*=2;
		lvi.cchTextMax = nLen;
		lvi.pszText = str.GetBufferSetLength(nLen);
		nRes=(int)::SendMessageA(hwnd,LVM_GETITEMTEXT,(WPARAM)nItem,(LPARAM)&lvi);
		str.ReleaseBuffer();
		return str;
	//}while(1);
}


//when you click one place, there will appear an edit control to let users to set paras
void ListControlMgr::OnClickList(HWND& hwnd,LPNMITEMACTIVATE& pNMItemActivate)
{
	RECT rect;
	//get the row number
	NM_LISTVIEW* pNMLV = (NM_LISTVIEW*)pNMItemActivate;
	nItem = pNMLV->iItem;
	//get the column number
	nSubItem = pNMLV->iSubItem;
	if(nSubItem ==-1 || nItem==-1)
	{
		return;
	}
	CString strtemp;
	strtemp.Format(_T("the %d row, and %d col"),nItem,nSubItem); //used to transfer
	AfxMessageBox(strtemp);
	//get list control and edit control handler
	HWND hwndList = ::GetDlgItem(hwnd,list_control_id);
	HWND hwndEdit = ::GetDlgItem(hwnd,edit_control_id);

	//retrieve the text of the selected subItem from the list
	CString str = GetItemText(hwndList,nItem,nSubItem);
	RECT rect1,rect2;
	//this macro is used to retrieve the rectanle of the selected subItem
	ListView_GetSubItemRect(hwndList, nItem,nSubItem,LVIR_BOUNDS,&rect);

	strtemp.Format(_T("the %d r, and %d l, %d top, %d bottom"),rect.right,rect.left,rect.top,rect.bottom);
	AfxMessageBox(strtemp);
	//get the rectrange of the list control
	::GetWindowRect(pNMItemActivate->hdr.hwndFrom,&rect1);
	strtemp.Format(_T("the %d r, and %d l, %d top, %d bottom"),rect1.right,rect1.left,rect1.top,rect1.bottom);
	AfxMessageBox(strtemp);
	//get the rectange od the dialog
	::GetWindowRect(hwnd,&rect2);
	strtemp.Format(_T("the %d r, and %d l, %d top, %d bottom"),rect2.right,rect2.left,rect2.top,rect2.bottom);
	AfxMessageBox(strtemp);

	int x = rect1.left -rect2.left;
	int y = rect1.top -rect2.top;

	if(nItem!=-1)
	{
		//AfxMessageBox(TEXT("14"));
		::SetWindowPos(hwndEdit,HWND_BOTTOM,rect.left+x,y-rect.top,rect.right-x,rect.bottom+y,NULL);
		::ShowWindow(hwndEdit,SW_SHOW);
		::SetFocus(hwndEdit);
		//Draw a rectange around the subitem
		//::Rectangle(::GetDC(pNMItemActivate->hdr.hwndFrom),rect.left,rect.top-1,rect.right,rect.bottom);
		//set the listitem text in the editbox
		::SetWindowTextW(hwndEdit,str);
	}
}