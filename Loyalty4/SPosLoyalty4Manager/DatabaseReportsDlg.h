#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "DbExportHandler.h"
//*******************************************************************

//*******************************************************************
// CDatabaseReportsDlg dialog

class CDatabaseReportsDlg : public CDialog
{
public:
	CDatabaseReportsDlg( CWnd* pParent = NULL);   // standard constructor
	virtual ~CDatabaseReportsDlg();

// Dialog Data
	enum { IDD = IDD_DATABASEREPORTS };
	CButton	m_buttonEdit;
	CButton	m_buttonDelete;
	CButton	m_buttonAdd;
	CListBox m_listboxReports;
	CString	m_strReportName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListDbReports();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonEdit();

	DECLARE_MESSAGE_MAP()

	public:
	int m_nDefaultOption;
	bool m_bEditReqd;

public:
	const char* GetReportLabel() { return m_strReportLabel; }

private:
	int GetLabel();

private:
	CDbExportHandler m_export;
	CString m_strReportLabel;
	CCSV m_csvReportList;
};

//*******************************************************************
