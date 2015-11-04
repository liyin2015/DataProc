#pragma once
#include "stdafx.h"
class ListControlMgr
{
public:
	ListControlMgr();
	~ListControlMgr();

	void setEditControlId(int id);
	void setListControlId(int id);
	void SetCell(HWND hwnd, CString value, int row, int col);

	CString GetItemText(HWND hwnd, int nItem,int nSubItem)const;

	//void OnClickList(NMHDR* pNmhdr, LRESULT* pResult,HWND hwnd);
	void OnClickList(HWND& hwnd, LPNMITEMACTIVATE& pNMItemActivate);
	
public:
	int nItem;
	int nSubItem;
	int edit_control_id;
	int list_control_id;
};
