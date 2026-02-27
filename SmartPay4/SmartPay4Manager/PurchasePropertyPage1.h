#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//**********************************************************************
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "ReportLocationControls.h"
#include "ReportPurchaseHelpers.h"
#include "SmartPay4Manager.h"
//************************************************************
// PurchasePropertyPage1 dialog

class CPurchasePropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CPurchasePropertyPage1(CReportPurchaseHelpers& ReportPurchaseHelpers);
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_PURCHASE1 };
	CButton m_checkNewPage;
	CButton m_checkContinuous;
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CEdit m_editFromCardNo;
	CEdit m_editToCardNo;
	CButton m_buttonList;
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_datePickerStart;
	CDateTimeCtrl m_datePickerEnd;
	CDateTimeCtrl m_timePickerFrom;
	CDateTimeCtrl m_timePickerTo;
	CSSComboBox m_comboTimeRange;
	CSSTabbedComboBox m_tabcomboGroup;
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	CSSComboBox m_comboSummary;
	CButton m_checkExcludeZeroPrice;
	CButton m_checkShowAccountName;
	CSSComboBox m_comboInfoText;
	CSSComboBox m_comboInfoFilter;
	CButton m_buttonAltKey1;
	CButton m_buttonAltKey2;
	CButton m_buttonAltKey3;
	CButton m_buttonSort;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnSelectDateRange();
	afx_msg void OnSelectTimeRange();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnDoubleClickRadioSingle();
	afx_msg void OnSelectSummary();
	afx_msg void OnButtonSort();
	afx_msg void OnSelectInfo();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonAltKey2();
	afx_msg void OnButtonAltKey3();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	
private:
	void FillReportCombo ( int nCurrentReportType );
	void SetRangeOptions();
	void AddInfoSelections();

private:
	CCommentHandler m_InfoList;
	CString m_strLastInfoFile;
	CUIntArray m_arrayReportReqd;
	
private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
	CReportPurchaseHelpers& m_ReportPurchaseHelpers;
};

//************************************************************
