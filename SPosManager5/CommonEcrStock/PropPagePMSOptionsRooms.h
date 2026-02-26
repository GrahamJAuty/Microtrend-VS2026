#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPagePMSOptionsRooms : public CPropertyPage
{
public:
	CPropPagePMSOptionsRooms();
	~CPropPagePMSOptionsRooms();

	//{{AFX_DATA(CPropPagePMSOptionsRooms)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_ROOMS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePMSOptionsRooms)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPagePMSOptionsRooms)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

public:
	void SelectLine( int nIndex );
	
private:
	int GetArraySize();
	bool PrepareLine( int nIndex );
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );

private:
	CArray<bool,bool> m_arrayOccupied;
	CSSListCtrlVirtual m_listData;
	CString m_strDisplayLine;
	bool m_bIsModified;
	int m_nListSelection;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
