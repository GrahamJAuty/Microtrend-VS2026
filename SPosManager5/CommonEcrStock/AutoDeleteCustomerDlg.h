#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CAutoDeleteCustomerDlg : public CSSDialog
{
public:
	CAutoDeleteCustomerDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CAutoDeleteCustomerDlg)
	enum { IDD = IDD_CUSTOMER_AUTODELETE };
	CSSComboBoxEx m_comboLocation;
	//}}AFX_DATA
	CStatic m_staticOneOff;
	CStatic m_staticAccount;
	CButton m_checkDays1;
	CButton m_checkDays2;

	//{{AFX_VIRTUAL(CAutoDeleteCustomerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnToggleDays1();
	afx_msg void OnToggleDays2();
	afx_msg void OnButtonLog();
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditDays1();
	CEdit* GetEditDays2();

private:
	int CheckDeletionDates( CCustomerCSVArray& Customers, int nRetainDays );
	void LogMessage( int nLocIdx, bool bOneOff, const char* szMsg );
	void LogMessage( const char* szMsg );

protected:
	//{{AFX_MSG(CAutoDeleteCustomerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CLocationSelector m_LocationSelector;
};

/**********************************************************************/
