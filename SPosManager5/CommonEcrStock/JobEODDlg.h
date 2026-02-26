#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CJobEODDlg : public CDialog
{
public:
	CJobEODDlg( CWnd* pParent = NULL );  

	int GetDateType(){ return m_nType; }

	//{{AFX_DATA(CJobEODDlg)
	enum { IDD = IDD_JOB_EOD };
	CStatic m_staticReason;
	CSSComboBox	m_comboDates;
	CButton m_checkReason;
	CButton m_checkPlu;
	CButton m_checkCustomer;
	BOOL m_bPlu;
	BOOL m_bReason;
	BOOL m_bCustomer;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CJobEODDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnCheckReason();

public:
	CString m_strDate;

protected:
	//{{AFX_MSG(CJobEODDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectDate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetDates();

private:
	int m_nType;
};

/**********************************************************************/
