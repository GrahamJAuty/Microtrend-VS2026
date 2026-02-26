#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/
#include "EposSelectArray.h"
/**********************************************************************/

class CCustomerTrackTraceDlg : public CSSDialog
{
public:
	CCustomerTrackTraceDlg( int nLocIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomerTrackTraceDlg)
	enum { IDD = IDD_CUSTOMER_TRACKTRACE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomerTrackTraceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CEdit m_editName;
	CEdit m_editPhone;

protected:
	//{{AFX_MSG(CCustomerTrackTraceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonApply();
	afx_msg void OnButtonFind();
	afx_msg void OnButtonEdit();
	//}}AFX_MSG
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	int GetArraySize();
	int GetArrayIdxFromListIdx( int nListIdx );
	void SelectListLine( int nIndex );

private:
	CEdit* GetEditID();

private:
	int m_nLocIdx;
	CString m_strID;
	CString m_strName;
	CString m_strPhone;
	int m_nPhoneColumnType;

private:
	CSSListCtrlVirtual m_listCustomers;
	CListDataDlgChangeFlag m_WarnCancelAfterChange;
	CArray<int,int> m_arrayNameTableIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
