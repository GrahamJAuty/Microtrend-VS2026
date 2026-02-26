#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSalesHistoryCSVRecordAddDlg : public CDialog
{
public:
	CSalesHistoryCSVRecordAddDlg( CWnd* pParent = NULL);   

// Dialog Data
	//{{AFX_DATA(CSalesHistoryCSVRecordAddDlg)
	enum { IDD = IDD_SALESHISTORY_ADD };
	CSSComboBox	m_comboReport;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryCSVRecordAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryCSVRecordAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nReportType;

protected:
	virtual void PrepareComboAndTitle() = 0;

protected:
	CWordArray m_arrayReportTypes;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
