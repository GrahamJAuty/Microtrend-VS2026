#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AuditReportConfig.h"
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "ReportLocationControls.h"
//$$******************************************************************

class CAuditReportDlg : public CSSDialog
{
public:
	CAuditReportDlg( CAuditReportConfig& config, CWnd* pParent);
	~CAuditReportDlg();

public:
	enum { IDD = IDD_REPORT_AUDIT };
	
	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	//accounts
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CButton m_checkNewPage;
	CButton m_checkSortByCardNo;
	CSSTabbedComboBox m_comboGroup;
	CButton m_checkSortByGroupNo;
	CButton m_checkAction;
	CButton m_buttonAction;
	//terminals
	CSSTabbedComboBox m_comboDatabase;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	//options
	CButton m_checkShowPcLines;
	CButton m_checkShowExtLines;
	CButton m_checkShowPosLines;
	CButton m_checkShowServerLines;
	CButton m_checkShowDeletedLines;
	CButton m_checkShowEcrText;
	CButton m_checkShowNonPurse;
	//altkey
	CButton m_buttonAltKey1;

private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//date
	afx_msg void OnSelectDateRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//time
	afx_msg void OnSelectTimeRange();
	//account selection
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnDoubleClickRadioSingle();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnToggleSortByCardNo();
	//terminals
	afx_msg void OnSelectDatabase();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	//options
	afx_msg void OnToggleAction();
	afx_msg void OnButtonAction();
	//altkey
	afx_msg void OnButtonAltKey1();
	
	DECLARE_MESSAGE_MAP()

private:
	void SetRangeOptions();
	void SetCardRangeType(int n);

private:
	CAuditReportConfig& m_ReportConfig;
	CAuditReportConfig m_ReportConfigAction;
};

//$$******************************************************************

