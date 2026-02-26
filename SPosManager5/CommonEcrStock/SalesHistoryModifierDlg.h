#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSalesHistoryModifierDlg : public CDialog
{
public:
	CSalesHistoryModifierDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CSalesHistoryModifierDlg)
	enum { IDD = IDD_SALESHISTORY_MODIFIER };
	CButton	m_checkMod0;
	CButton	m_checkMod1;
	CButton	m_checkMod2;
	CButton	m_checkMod3;
	CButton	m_checkMod4;
	CButton	m_checkMod5;
	CButton	m_checkMod6;
	CButton	m_checkMod7;
	CButton	m_checkMod8;
	CButton	m_checkMod9;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryModifierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryModifierDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/

