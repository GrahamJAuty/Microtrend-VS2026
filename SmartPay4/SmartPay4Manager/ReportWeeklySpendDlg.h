#pragma once
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//**********************************************************************
#include "..\SmartPay4Shared\Defines.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "SmartPay4Manager.h"
#include "ReportLocationControls.h"
#include "TerminalListHandler.h"
//**********************************************************************
#define nSORTON_SURNAME		0
#define nSORTON_USERID		1
#define nSORTON_USERNAME	2
//**********************************************************************

class CReportWeeklySpendDlg : public CSSAutoShutdownDialog
{
public:
	CReportWeeklySpendDlg(const char* szLabel, CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportWeeklySpendDlg();

	// Dialog Data
	enum { IDD = IDD_REPORT_WEEKLYSPEND };
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_TimePickerStart;
	CDateTimeCtrl m_TimePickerEnd;
	CButton m_checkContinuous;
	CButton m_checkIncludePurse1;
	CButton m_checkIncludePurse2;
	CButton m_checkIncludePurse3;
	CButton m_checkIncludeCash;
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CButton m_buttonList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CSSComboBox m_comboDateRange;
	CSSComboBox m_comboTimeRange;
	CSSTabbedComboBox m_tabcomboGroup;
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	CSSComboBox m_comboSortOn;
	CButton m_checkShowDeletedLines;
	CButton m_checkShowUnusedLines;
	CButton m_checkOnlyShowMonFri;
	CButton m_buttonAltKey1;

private:
	CDateSelectorRange m_DateSelectorRange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectTimeRange();
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnDoubleClickRadioSingle();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	const char* GetReportFilename() { return m_strReportName; }
	const char* GetParamsFilename() { return m_strParamsName; }
	const char* GetReportKey() { return m_strReportKey; }
	CString GetReportTitle();

	void SetRangeOptions();
	bool IsValidTerminalNo(int nFolderSet, int nTNo);

	bool IsValid(CSQLAuditRecord* pAtc);
	bool IsValid(CSQLRowAccountFull& RowAccount);
	double GetSpendValue(CSQLAuditRecord* pAtc);

private:
	void SetCardRangeType(int n);

private:
	CString m_strTitle;
	CString m_strReportLabel;
	CString m_strReportList;
	CString m_strParamsName;
	CString m_strReportName;
	CString m_strReportKey;

	CCardPicker m_Picker;
	CReportLocationControls m_ReportLocationControls;
	CTerminalListHandler m_TerminalListHandler;

private:
	int m_nReportRangeType;
	bool m_bTimeRangeSet;
	CSSDate m_dateTo;
	CSSTime m_timeStart;
	CSSTime m_timeEnd;
	bool m_bContinuous;
	bool m_bIncludePurse1;
	bool m_bIncludePurse2;
	bool m_bIncludePurse3;
	bool m_bIncludeCash;
	__int64 m_nRangeCardNoFrom;
	__int64 m_nRangeCardNoTo;
	int m_nGroupIndex;
	int m_nSingleGroupNo;

public:
	CSSDate m_dateFrom;
	CString m_strSingleGroupName;
	int m_nSortOn;
	bool m_bShowDeletedLines;
	bool m_bOnlyShowMonFri;
	bool m_bShowUnusedLines;
};

//**********************************************************************
