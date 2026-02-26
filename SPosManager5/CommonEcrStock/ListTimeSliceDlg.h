#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "TimeSliceCSVArray.h"
/**********************************************************************/

class CListTimeSliceDlg : public CDialog
{
public:
	CListTimeSliceDlg( const char* szName, CWnd* pParent = NULL);  

	//{{AFX_DATA(CListTimeSliceDlg)
	enum { IDD = IDD_LIST_TIMESLICE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CListTimeSliceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListTimeSliceDlg)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	bool IsModified() { return m_bIsModified; }
	int GetListSelection() { return m_nListSelection; }	
	void SelectLine( int nIndex );

	bool CheckCreateMap( const char* szType );

protected:
	int GetArraySize();
	bool PrepareLine( int nIndex );
		
protected:
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );

protected:
	CSSListCtrlVirtual m_listData;
	CString m_strDisplayLine;
	bool m_bIsModified;
	int m_nListSelection;

public:
	CString m_strName;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
