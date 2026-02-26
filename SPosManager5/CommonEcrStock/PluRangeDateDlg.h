#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluRangeDateDlg : public CSSDialog
{
public:
	CPluRangeDateDlg( const char* szDate, CWnd* pParent = NULL);
	
	//{{AFX_DATA(CPluRangeDateDlg)
	enum { IDD = IDD_PLU_RANGE_DATE };
	CDateTimeCtrl m_DatePicker;
	//}}AFX_DATA
	CButton	m_radioNoChange;
	CButton	m_radioDate;
	
	//{{AFX_VIRTUAL(CPluRangeDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeDateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioNoChange();
	afx_msg void OnRadioDate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_strDate;
};

/**********************************************************************/


