#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DepartmentSelector.h"
#include "DeptAndPluComboHelpers.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "SalesHistoryCSVArrayCustomer.h"
#include "SalesHistoryReportCustomer.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CSalesHistoryReportCustomerDlg : public CSSDialog
{
public:
	CSalesHistoryReportCustomerDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL);
	~CSalesHistoryReportCustomerDlg();

	void CreatePreset( CPresetReportInfo& base );
	int GetPresetError(){ return m_nPresetError; }

	//{{AFX_DATA(CSalesHistoryReportCustomerDlg)
	enum { IDD = IDD_SALESHISTORY_CUSTOMER };
	CStatic	m_staticPreset;
	CButton	m_buttonPreset;
	CSSComboBox	m_comboPreset;
	CSSComboBox	m_comboDeptFilterType;
	CButton m_buttonTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CSSTabbedComboBox m_comboPriceLevel;
	CSSComboBox	m_comboRound;
	CButton	m_buttonSelect;
	CSSComboBox	m_comboSession;
	CSSComboBox m_comboTimeSlice;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CButton	m_checkSpecify;
	CSSComboBox	m_comboReportType;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CSSComboBoxEx	m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	CButton m_checkSales;
	CButton m_checkTax;
	CButton m_checkRA;
	CButton m_checkCharges;
	CButton m_checkPayMeth;
	CButton m_checkCustomerList;
	CButton m_checkCustomerPage;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryReportCustomerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryReportCustomerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDate();
	afx_msg void OnSelectLocation();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();
	afx_msg void OnKillFocusPluFrom();
	afx_msg void OnKillFocusPluTo();
	afx_msg void OnTogglePluRange();
	afx_msg void OnToggleActivity();
	afx_msg void OnSelectReportType();
	afx_msg void OnSelectDepartment();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonTaxBand();
	afx_msg void OnButtonPriceLevel();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();

private:
	void FillReportCombo( int nSelection );
	void FillTimeSliceCombo( int nSelection );
	void FillPresetCombo( int nSelection );
	void SelectReportByItemData( int n );

private:
	void FillDepartmentComboForLocation();
	void FillTaxBandComboForLocation();
	void FillPriceLevelCombo();

private:
	void UpdateCustomTaxString();
	void UpdateCustomPriceLevelString();

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	
private:
	bool m_bSystemReport;

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bAutoRunPreset;
	
private:
	CSalesHistoryReportCustomer m_HistoryReport;
	CSalesHistoryCSVRecordCustomer m_ReportRecord;
	
private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	
private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CDepartmentSelector m_DepartmentSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CPluFilterArray m_FilterArray;

private:
	CString m_strCustomerFilter;
	int m_nLocationComboDbIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
