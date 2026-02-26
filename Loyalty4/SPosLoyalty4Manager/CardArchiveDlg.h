//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AuditReportConfig.h"
//$$******************************************************************

class CCardArchiveDlg : public CSSDialog
{
public:
	CCardArchiveDlg( CAuditReportConfig& ReportConfig, CWnd* pParent);
	virtual ~CCardArchiveDlg();

public:
	enum { IDD = IDD_CARDARCHIVE };
	CEdit	m_editCardNo;
	CButton	m_buttonAll;
	CTabCtrl m_tabSearch;
	CEdit m_editText;
	CButton m_buttonBrowse;
	CSSListTaggedSelectCtrl m_listFiles;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAll();
	afx_msg void OnButtonBrowse();
	afx_msg void OnTcnSelchangeTabSearch(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	void SetTabs();
	void SetAllButtonText();

private:
	CAuditReportConfig& m_ReportConfig;
	int m_nAllReqdFlag;
};

//$$******************************************************************
