#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "DateSelectorRange.h"
#include "ReportInfoPeriodSales.h"
#include "ReportLocationControls.h"
//$$******************************************************************

class CReportGroupPeriodSalesDlg : public CSSDialog
{
public:
	CReportGroupPeriodSalesDlg( CWnd* pParent = NULL);

public:
	void SetReportInfo( CReportInfoPeriodSales* pInfo ){ m_pReportInfo = pInfo; }

public:
	enum { IDD = IDD_REPORT_GROUPPERIOD };
	CStatic m_staticRange;
	//date
	CSSComboBox m_comboDateRange;
	CDateTimeCtrl m_DatePickerFrom;
	CDateTimeCtrl m_DatePickerTo;
	//close buttons
	CButton m_buttonOK;
	CButton m_buttonCancel;

	//options
	CButton m_checkSummary;
	CButton m_radioSalesAll;
	CButton m_radioSalesItem;
	CButton m_radioSalesRenewal;
	CButton m_checkManual;
	CButton m_checkDiscount;
	//terminals
	CSSTabbedComboBox m_comboDatabase;
	CSSTabbedComboBox m_comboFolderSet;
	CButton m_checkTerminals;
	CSSComboBox m_comboTerminals;
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
	//options
	afx_msg void OnRadioSalesAll();
	afx_msg void OnRadioSalesItem();
	afx_msg void OnRadioSalesRenewal();
	//terminals
	afx_msg void OnSelectDatabase();
	afx_msg void OnSelectFolderSet();
	afx_msg void OnToggleTerminals();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	void SetSalesType(int n);

private:
	CReportInfoPeriodSales* m_pReportInfo;

private:
	CString m_strReportLabel;
	int m_nSalesType;
};

//$$******************************************************************
