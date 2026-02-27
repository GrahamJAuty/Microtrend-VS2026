#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "AuditReportConfig.h"
#include "DateSelectorRange.h"
#include "CommentHandler.h"
#include "ReportLocationControls.h"
//$$******************************************************************

class CAuditReportDlg : public CSSAutoShutdownDialog
{
public:
	CAuditReportDlg( CAuditReportConfig& Config, CWnd* pParent = NULL);
	
private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
public:
// Dialog Data
	enum { IDD = IDD_REPORT_AUDIT };

	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//time
	CSSComboBox m_comboTimeRange;
	CDateTimeCtrl m_TimePickerFrom;
	CDateTimeCtrl m_TimePickerTo;
	CButton m_checkContinuous;
	//accounts
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CEdit m_editCardNoFrom;
	CEdit m_editCardNoTo;
	CButton m_buttonList;
	CButton m_checkNewPage;
	CButton m_checkSortByCardNo;
	CButton m_checkSortByGroup;
	CSSTabbedComboBox m_tabcomboGroup;
	//terminals
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	//options
	CButton m_checkShowPcLines;
	CButton m_checkShowPosLines;
	CButton m_checkShowServerLines;
	CButton m_checkShowDeletedLines;
	CButton m_checkShowVendingLines;
	CButton m_checkShowExternalLines;
	CButton m_checkWebPayment;
	CButton m_checkCardLink;
	CButton m_checkShowEODLines;
	CButton m_checkShowEcrText;
	CButton m_checkPassword;
	CButton m_checkAction;
	CButton m_buttonAction;
	//request types
	CButton m_checkLookupScan;
	CButton m_checkLookupBio;
	CButton m_checkLookupManual;
	CButton m_checkLookupDefault;
	CButton m_checkLookupOther;
	CButton m_checkLookupInternal;
	CButton m_checkLookupPIN;
	CButton m_checkLookupUnknown;
	//altkey
	CButton m_buttonAltKey1;
	CButton m_buttonAltKey2;

private:
	CDateSelectorRange m_DateSelectorRange;
		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
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
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	//options
	afx_msg void OnTogglePassword();
	afx_msg void OnToggleAction();
	afx_msg void OnButtonAction();
	//altkey
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonAltKey2();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPassword();

private:
	void SetRangeOptions();
	void SetCardRangeType(int n);

private:
	CAuditReportConfig& m_ReportConfig;
	CAuditReportConfig m_ReportConfigAction;
	CReportLocationControls m_ReportLocationControls;
};

//$$******************************************************************
