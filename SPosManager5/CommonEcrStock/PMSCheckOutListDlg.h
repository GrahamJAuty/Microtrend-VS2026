#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSCustomerCSVArray.h"
/**********************************************************************/

class CPMSCheckOutListDlg : public CSSDialog
{
public:
	CPMSCheckOutListDlg( int nRoomNo, CWnd* pParent = NULL);   

	int GetSelectedRoomIdx(){ return m_nSelectedRoomIdx; }
	
	//{{AFX_DATA(CPMSCheckOutListDlg)
	enum { IDD = IDD_PMS_CHECKOUT_LIST };
	//}}AFX_DATA
	CSSListCtrlVirtual m_listData;
	CButton m_checkTraining;
	BOOL m_bAllAccounts;
	BOOL m_bTraining;

	//{{AFX_VIRTUAL(CPMSCheckOutListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleToday();

protected:
	//{{AFX_MSG(CPMSCheckOutListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonPrint();
	DECLARE_MESSAGE_MAP()

private:
	int GetArraySize();
	bool PrepareLine( int nIndex );
	void UpdateList();

private:
	CArray<int,int> m_FilterArray;
	CArray<CPMSAccountFindInfo,CPMSAccountFindInfo> m_arrayBookingInfo;

private:
	CString m_strName;
	CString m_strRoomNo;

private:
	CString m_strDisplayLine;
	int m_nSelectedRoomIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
