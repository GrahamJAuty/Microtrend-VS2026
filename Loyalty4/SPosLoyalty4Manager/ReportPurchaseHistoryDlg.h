#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\Defines.h"
//$$******************************************************************
#include "CardPicker.h"
#include "CommentHandler.h"
#include "DateSelectorRange.h"
#include "DbReportHandler.h"
#include "ReportInfoPurchaseHistory.h"
#include "ReportLocationControls.h"
//$$******************************************************************

class CReportPurchaseHistoryDlg : public CSSDialog
{
public:
	CReportPurchaseHistoryDlg( CReportInfoPurchaseHistory& ReportInfo, CWnd* pParent = NULL);   
	virtual ~CReportPurchaseHistoryDlg();

// Dialog Data
	enum { IDD = IDD_REPORT_PURCHASE };
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
	CButton m_radioList;
	CButton m_radioSingle;
	CEdit m_editFromCardNo;
	CEdit m_editToCardNo;
	CButton m_buttonList;
	CButton m_checkNewPage;
	CSSTabbedComboBox m_comboGroup;
	//terminals
	CSSTabbedComboBox m_comboDatabase;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
	//report options
	CButton m_checkSummary;
	CButton m_radioSalesAll;
	CButton m_radioSalesItem;
	CButton m_radioSalesRenewal;
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
	afx_msg void OnDoubleClickRadioList();
	afx_msg void OnButtonList();
	//terminals
	afx_msg void OnSelectDatabase();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	//options
	afx_msg void OnRadioSalesAll();
	afx_msg void OnRadioSalesItem();
	afx_msg void OnRadioSalesRenewal();
	//altkey
	afx_msg void OnButtonAltKey1();

	DECLARE_MESSAGE_MAP()

private:
	void SetRangeOptions();
	void SetCardRangeType(int n);
	void SetSalesType(int n);
		
private:
	CReportInfoPurchaseHistory& m_ReportInfo;
	int m_nSalesType;
};

//$$******************************************************************
