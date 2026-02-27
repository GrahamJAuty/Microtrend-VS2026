#pragma once
//************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//************************************************************
#include "..\SmartPay4ManagerBgnd\CardPicker.h"
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//************************************************************
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "GroupPicker.h"
#include "ReportLocationControls.h"
#include "ReportPurchaseHelpers.h"
#include "SmartPay4Manager.h"
//************************************************************

class CPurchasePropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CPurchasePropertyPage2(CReportPurchaseHelpers& ReportPurchaseHelpers);
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_PURCHASE2 };
	CButton m_checkContinuous;
	CButton m_radioRange;
	CButton m_radioSingle;
	CButton m_radioList;
	CButton m_checkNewPage;
	CButton m_checkExcludeZeroPrice;
	CButton m_checkSkipOnBlank;
	CButton m_checkSortByGroup;
	CEdit m_editFromCardNo;
	CEdit m_editToCardNo;
	CDateTimeCtrl m_datePickerStart;
	CDateTimeCtrl m_datePickerEnd;
	CDateTimeCtrl m_timePickerFrom;
	CDateTimeCtrl m_timePickerTo;
	CSSComboBox m_comboDateRange;
	CSSComboBox m_comboTimeRange;
	CButton m_radioAllGroups;
	CButton m_radioListGroups;
	CSSComboBox m_comboInfoText;
	CStatic m_staticFolderSet;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	CButton m_buttonAltKey1;
	CButton m_buttonList;
	CButton m_buttonList2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioRange();
	afx_msg void OnRadioSingle();
	afx_msg void OnRadioList();
	afx_msg void OnButtonList();
	afx_msg void OnSelectDateRange();
	afx_msg void OnSelectTimeRange();
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnDoubleClickRadioSingle();
	afx_msg void OnSelectInfo();
	afx_msg void OnSelectGroup();
	afx_msg void OnButtonList2();
	afx_msg void OnRadioGroupAll();
	afx_msg void OnRadioGroupList();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
		
private:
	void SetDisplay();
	void SetRangeOptions();
	
private:
	CDateSelectorRange m_DateSelectorRange;
	CReportLocationControls m_ReportLocationControls;
	CReportPurchaseHelpers& m_ReportPurchaseHelpers;
};

//************************************************************
