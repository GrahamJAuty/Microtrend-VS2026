#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
//include "MySSListCtrl.h"
/**********************************************************************/

class CEposReportSelectNewDlg : public CSSDialog
{
public:
	CEposReportSelectNewDlg( CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CEposReportSelectNewDlg)
	enum { IDD = IDD_EPOS_REPORT_SELECT_NEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportSelectNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEposReportSelectNewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonResetOrder();
	afx_msg void OnButtonCreate();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	void MoveReport( CSSListCtrl& listSource, CSSListCtrl& listDest, CArray<int,int>& arraySource, CArray<int,int>& arrayDest, bool bRemove );
	void MoveReports( bool bFromAvailable );
	void RestoreSelectedSortOrder();
	
private:
	CSSListCtrlVirtual* GetReportList( bool bAvailable );
	CArray<CSortedIntWithFlag,CSortedIntWithFlag>* GetReportArray( bool bAvailable );
	void GetSelectedItemArray( bool bAvailable, CReportConsolidationArray<CSortedIntItem>& arrayItems );
	void SelectSingleItem( bool bAvailable );
	void SelectLine( bool bAvailable, int nIndex );
	void ClearSelection( bool bAvailable );
	void MarkSelection( bool bAvailable );
	void RestoreSelection( bool bAvailable );
	
private:
	CEposReportSelect m_EposReportSelect;
	
private:
	CSSListCtrlVirtual m_listSelected;
	CSSListCtrlVirtual m_listAvailable;
	CArray<CSortedIntWithFlag,CSortedIntWithFlag> m_arraySelected;
	CArray<CSortedIntWithFlag,CSortedIntWithFlag> m_arrayAvailable;
};

/**********************************************************************/
#endif
/**********************************************************************/
