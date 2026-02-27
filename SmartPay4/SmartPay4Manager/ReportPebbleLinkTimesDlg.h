//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "DateSelectorRange.h"
//$$******************************************************************

struct CReportPebbleLinkInfo
{
public:
	CString GetDateString();

public:
	CSSDate m_dateFrom = CSSDate(CSSDate::GetCurrentDate());
	CSSDate m_dateTo = CSSDate(CSSDate::GetCurrentDate());
	bool m_bTimeRangeSet = FALSE;
	CSSTime m_timeStart = CSSTime("00:00");
	CSSTime m_timeEnd = CSSTime("23:59");
	bool m_bContinuous = FALSE;
	int m_nSingleGroupNo = 0;
	int m_nReportRangeType = 0;
	CString m_strCardNoFrom = "";
	CString m_strCardNoTo = "";
	bool m_bSortByDate = FALSE;
	bool m_bSortByGroup = FALSE;
	bool m_bSortByID = FALSE;
	CString m_strReportLabel = "";
	CString m_strReportTitle = "";
	CString m_strReportFilename = "";
	CString m_strParamsFilename = "";
	CString m_strReportKey = "";
};

//$$******************************************************************

class CReportPebbleLinkTimesDlg : public CSSAutoShutdownDialog
{
public:
	CReportPebbleLinkTimesDlg( CReportPebbleLinkInfo& ReportInfo, CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportPebbleLinkTimesDlg();
	CString GetDateString();

// Dialog Data
	enum { IDD = IDD_REPORT_PEBBLELINK };
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerStart;
	CDateTimeCtrl m_TimePickerEnd;
	CButton m_checkContinuous;
	CButton m_checkSortDate;
	CButton m_checkSortGroup;
	CButton m_checkSortID;
	/*****/
	CButton m_radioRange;
	CButton m_radioSingle;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CSSTabbedComboBox m_tabcomboGroup;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectTimeRange();
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnDoubleClickRadioSingle();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetCardRangeType(int n);
	void SetRangeOptions();

private:
	CDateSelectorRange m_DateSelectorRange;
	CReportPebbleLinkInfo& m_ReportInfo;
};

//$$******************************************************************
