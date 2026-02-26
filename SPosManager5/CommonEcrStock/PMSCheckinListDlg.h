#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSAccountFindDlg.h"
#include "PMSBookingCSVArray.h"
#include "PMSCustomerCSVArray.h"
/**********************************************************************/

class CPMSCheckInListDlg : public CSSDialog
{
public:
	CPMSCheckInListDlg( int nRoomNo, CWnd* pParent = NULL);   

	int GetSelectedRoomIdx(){ return m_nSelectedRoomIdx; }
	
	//{{AFX_DATA(CPMSCheckInListDlg)
	enum { IDD = IDD_PMS_CHECKIN_LIST };
	//}}AFX_DATA
	CButton m_checkTraining;
	CSSListCtrlVirtual m_listData;
	BOOL m_bTraining;

	//{{AFX_VIRTUAL(CPMSCheckInListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusReference();

private:
	CEdit* GetEditReference();

protected:
	//{{AFX_MSG(CPMSCheckInListDlg)
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
	CString m_strReference;

private:
	CString m_strDisplayLine;
	int m_nSelectedRoomIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
