#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/
#include "CustomerCSVArray.h"
//include "MySSListCtrl.h"
/**********************************************************************/

class CCustomerMoveDlg : public CDialog
{
public:
	CCustomerMoveDlg( int nLocIdx, bool bOneOff, CArray<int,int>& arrayCustomerIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomerMoveDlg)
	enum { IDD = IDD_CUSTOMER_MOVE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomerMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCustomerMoveDlg)
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectNone();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCustomerCSVArray* GetCustomerArray();
	void AddList();

private:
	CSSListCtrl m_listCustomers;
	CArray<int,int>& m_arrayCustomerIdx;
	bool m_bOneOff;
};

/**********************************************************************/
