#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DepartmentSelector.h"
#include "DeptAndPluComboHelpers.h"
#include "EposSelectArray.h"
#include "SalesHistoryCSVArrayPlu.h"
#include "SalesHistoryReportPlu.h"
#include "SessionCSVArray.h"
#include "StockReportTop100Dlg.h"
#include "TableReportHelpers.h"
/**********************************************************************/

class CSalesHistoryReportPluDlg : public CSSDialog
{
public:
	CSalesHistoryReportPluDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL);
	~CSalesHistoryReportPluDlg();

	void CreatePreset( CPresetReportInfo& base );
	int GetPresetError(){ return m_nPresetError; }

	//{{AFX_DATA(CSalesHistoryReportPluDlg)
	enum { IDD = IDD_SALESHISTORY_PLU };
	
	//FIRST DATE RANGE
	CSSComboBox	m_comboDates;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_DatePickerFrom;
	CSSComboBox	m_comboRound;
	//TIME
	CSSComboBox	m_comboSession;
	CSSComboBox m_comboTimeSlice;
	//PLU SALE TIME
	CSSComboBox m_comboPluTime;
	//SECOND DATE RANGE
	CButton m_checkDateCompare;
	CSSComboBox m_comboAutoDate2;
	CSSComboBox	m_comboDates2;
	CDateTimeCtrl m_DatePickerTo2;
	CDateTimeCtrl m_DatePickerFrom2;
	CSSComboBox	m_comboRound2;
	//PLU RANGE
	CButton	m_checkSpecify;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CSSComboBox	m_comboPluFilter;
	//OTHER ITEM FILTERS
	CSSTabbedComboBox m_comboModifier;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSComboBox	m_comboDeptFilterType;
	CButton m_buttonTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CSSTabbedComboBox m_comboPriceLevel;
	//LOCATION
	CSSComboBoxEx m_comboLocation;
	CSSTabbedComboBox m_comboTerminal;
	CSSTabbedComboBox m_comboTermDate;
	CStatic m_staticTable;
	CSSTabbedComboBox m_comboTable;
	CButton m_checkGuest;
	CButton	m_checkEposSales;
	CButton	m_checkManualSales;
	CButton	m_checkImportSales;
	//REPORT TYPE
	CStatic	m_staticPreset;
	CButton	m_buttonPreset;
	CSSComboBox	m_comboPreset;
	CButton	m_buttonSelect;
	CSSComboBox	m_comboReportType;
	CButton	m_checkByMonth;
	CButton	m_checkDeptSort;
	CButton	m_checkPercent;
	CButton	m_checkEstimatedProfit;
	CButton m_checkCustomer;
	CButton m_checkVIPSales;
	//GRAPH BUTTON
	CButton	m_buttonGraph;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryReportPluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryReportPluDlg)
	virtual BOOL OnInitDialog();
	//FIRST DATE RANGE
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//SECOND DATE RANGE
	afx_msg void OnToggleDateCompare();
	afx_msg void OnSelectAutoDate2();
	afx_msg void OnSelectDate2();
	afx_msg void OnChangeDateFrom2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo2(NMHDR* pNMHDR, LRESULT* pResult);
	//PLU RANGE
	afx_msg void OnTogglePluRange();
	afx_msg void OnKillFocusPluFrom();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnKillFocusPluTo();
	afx_msg void OnButtonBrowseTo();
	//OTHER ITEM FILTERS
	afx_msg void OnButtonModifier();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDepartment();
	afx_msg void OnButtonTaxBand();
	afx_msg void OnButtonPriceLevel();
	//LOCATION
	afx_msg void OnSelectLocation();
	//REPORT TYPE
	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	afx_msg void OnButtonSelect();
	afx_msg void OnSelectReportType();
	//ACTIONS
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonGraph();
	//}}AFX_MSG
	afx_msg long OnReportMessage ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void HandleTop100Report( CStockReportTop100Dlg* pDlg );
	void GetReportDates();
	void GetReportDates( int nRoundType, CDateTimeCtrl& DatePickerFrom, CDateTimeCtrl& DatePickerTo, COleDateTime& DateFrom, COleDateTime& DateTo );
	void AutoUpdateDate2();
	bool GetAdhocReportSession();

private:
	void FillReportCombo( int nSelection );
	void FillTimeSliceCombo( int nSelection );
	void FillPresetCombo( int nPresetIdxSel );
	void SelectReportByItemData( int n );

private:
	void FillDatabaseSpecificCombos();
	
private:
	void UpdateCustomTaxString();
	void UpdateCustomPriceLevelString();
	void UpdateCustomModifierString();
	
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
	CSalesHistoryReportPlu m_HistoryReport;
	CSalesHistoryCSVRecordPlu m_ReportRecord;
	
private:
	COleDateTime m_oleReportDateFrom;
	COleDateTime m_oleReportDateTo;
	COleDateTime m_oleReportDateFrom2;
	COleDateTime m_oleReportDateTo2;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	
private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CDateSelectorRange m_DateSelectorRange;
	CDateSelectorRange m_DateSelectorRange2;
	CLocationSelector m_LocationSelector;
	CDepartmentSelector m_DepartmentSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CTableReportHelpers m_TableReportHelpers;
	CPluFilterArray m_FilterArray;

private:
	bool m_bChangeTop100;

private:
	bool m_bReportShowsPercent;
	bool m_bReportShowsProfit;
};

/**********************************************************************/
#endif
/**********************************************************************/
