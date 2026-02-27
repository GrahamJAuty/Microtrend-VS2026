#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\PaymentAuditRecord.h"
//$$******************************************************************
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "ReportLocationControls.h"
#include "SmartPay4Manager.h"
#include "TerminalListHandler.h"
//$$******************************************************************

class CPaymentAuditDlg : public CSSAutoShutdownDialog
{
public:
	CPaymentAuditDlg(const char* szLabel, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaymentAuditDlg();

// Dialog Data
	enum { IDD = IDD_REPORT_PAYMENTAUDIT };
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
	CSSComboBox m_comboPayments;
	CButton m_radioPaymentAll;
	CButton m_radioPaymentList;
	CButton m_checkSummary;
	CSSComboBox m_comboOrderBy;
	CButton m_checkEODLines;
	CButton m_buttonAltKey1;
	CButton m_buttonAltKey2;
	CButton m_buttonAltKey3;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnRadioPaymentAll();
	afx_msg void OnRadioPaymentList();
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonAltKey2();
	afx_msg void OnButtonAltKey3();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CString GetReportTitle() { return m_strReportTitle; }
	CString GetReportFilename()	{ return m_strReportFilename; }
	CString GetParamsFilename() { return m_strParamsFilename; }
	CString GetReportKey() { return  m_strReportKey; }
	CString GetDateString();
	bool IsValid ( CPaymentAuditRecord* atcRecord );

private:
	void SetDisplay();
	void AddPayments();

private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
	CTerminalListHandler m_TerminalListHandler;

private:
	CString m_strReportLabel;
	CString m_strReportTitle;
	CString m_strReportFilename;
	CString m_strParamsFilename;
	CString m_strReportKey;

	CCommentHandler m_PaymentList;
	CUIntArray m_arrayPayment;

private:
	bool m_bTimeRangeSet;
	int m_nPaymentListType;
	int m_nTerminalListType;
	CSSDate m_dateTo;
	CSSTime m_timeStart;
	CSSTime m_timeEnd;
	bool m_bContinuous;
	bool m_bEODLines;

public:
	CSSDate m_dateFrom;
	CSSDate m_dateAllFrom;
	int m_nOrderBy;
	bool m_bSummaryReqd;
};

//$$******************************************************************
