#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDefEditCustomerDateDlg : public CDialog
{
public:
	CDefEditCustomerDateDlg( int nLocIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditCustomerDateDlg)
	enum { IDD = IDD_DEF_EDIT_CUSTOMER_DATE };
	CButton m_buttonAdmin;
	CDateTimeCtrl	m_DatePickerDate;
	CDateTimeCtrl	m_DatePickerTime;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CDefEditCustomerDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditCustomerDateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonAdmin();
	DECLARE_MESSAGE_MAP()

public:
	bool GetAdminOptionsFlag(){ return m_bAdminOptions; }

private:
	void GetData();
	void SaveData();

private:
	int m_nLocIdx;
	bool m_bRightClicked;
	bool m_bAdminOptions;
};

/**********************************************************************/
#endif
/**********************************************************************/
