//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
#include "..\SmartPay4Shared\PaymentAuditRecord.h"
//$$******************************************************************
#include "DateSelectorRange.h"
//$$******************************************************************

class CReportKioskAuditDlg : public CSSAutoShutdownDialog
{
public:
	CReportKioskAuditDlg( const char* szLabel, CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportKioskAuditDlg();
	bool IsValid ( CSQLAuditRecord* pAtc );

	const char* GetReportFilename() { return m_strReportFilename; }
	const char* GetParamsFilename() { return m_strParamsFilename; }
	const char* GetReportKey()		{ return m_strReportKey; }
	const char* GetReportTitle()	{ return m_strReportTitle; }
	CString GetDateString();

// Dialog Data
	enum { IDD = IDD_REPORT_KIOSKAUDIT };
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerStart;
	CDateTimeCtrl m_TimePickerEnd;
	CButton m_checkNewPage;
	CButton m_checkSummaryReport;
	CButton m_checkSingleKiosk;
	CButton m_checkContinuous;
	int m_nKioskNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectTimeRange();
	afx_msg void OnToggleSingle();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditKioskNo();

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CDateSelectorRange m_DateSelectorRange;

private:
	CString m_strReportLabel;
	CString m_strReportList;
	CString m_strReportTitle;
	CString m_strReportFilename;
	CString m_strParamsFilename;
	CString m_strReportKey;

private:
	CSSDate m_dateTo;
	bool m_bTimeRangeSet;
	CSSTime m_timeStart;
	CSSTime m_timeEnd;
	
public:
	CSSDate m_dateFrom;
	bool m_bContinuous;
	bool m_bSummaryReport;
	bool m_bNewPage;
	bool m_bSingleKiosk;
};

//$$******************************************************************
