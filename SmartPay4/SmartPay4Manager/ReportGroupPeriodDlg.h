#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
//$$******************************************************************
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "SmartPay4Manager.h"
#include "ReportLocationControls.h"
#include "TerminalListHandler.h"
//$$******************************************************************
#define nREPORT_BYGROUP	0
#define nREPORT_BYGROUPSET 1
//$$******************************************************************

class CReportGroupPeriodDlg : public CSSAutoShutdownDialog
{
public:
	CReportGroupPeriodDlg(const char* szLabel, CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportGroupPeriodDlg();

// Dialog Data
	enum { IDD = IDD_REPORT_PERIOD };
	CSSComboBox	m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_TimePickerStart;
	CDateTimeCtrl m_TimePickerEnd;
	CButton m_checkContinuous;
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	CButton m_radioGroupNo;
	CButton m_radioGroupSetNo;
	CButton m_checkSummary;
	CButton m_buttonAltKey1;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
	CTerminalListHandler m_TerminalListHandler;

public:
	CString GetReportLabel() { return m_strReportLabel; }
	CString GetReportKey() { return  m_strReportKey; }
	CString GetDateString();
	
public:
	bool IsValidDate(CSQLAuditRecord* atcRecord);
	bool IsValidTerminalNo(int nFolderSet, int nTNo);

private:
	CString m_strReportLabel;
	CString m_strReportKey;

private:
	bool m_bTimeRangeSet;
	CSSDate m_dateTo;
	CSSTime m_timeStart;
	CSSTime m_timeEnd;
	bool m_bContinuous;

public:
	CSSDate m_dateFrom;
	CSSDate m_dateAllFrom;
	bool m_bSummaryReqd;
	int m_nReportBy;
};

//$$******************************************************************
