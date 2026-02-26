#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategoryComboHelpers.h"
#include "CategorySelectorNew.h"
#include "DateSelectorRange.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "SalesHistoryCSVArrayStock.h"
#include "SalesHistoryReportStock.h"
#include "SessionCSVArray.h"
#include "..\CommonEcrStock\StockReportTop100Dlg.h"
/**********************************************************************/

class CSalesHistoryReportStockNewDlg : public CSSDialog
{
public:
	CSalesHistoryReportStockNewDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL); 
	~CSalesHistoryReportStockNewDlg();
	
	void CreatePreset( CPresetReportInfo& base );
	int GetPresetError(){ return m_nPresetError; }

	//{{AFX_DATA(CSalesHistoryReportStockNewDlg)
	enum { IDD = IDD_SALESHISTORY_STOCK_NEW };

	CSSComboBox		m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CSSComboBox		m_comboRound;
	CSSComboBox		m_comboSession;
	
	CSSComboBoxEx	m_comboLocation;
	CButton			m_checkEposSales;
	CButton			m_checkManualSales;
	CButton			m_checkImportSales;
	
	CSSComboBox		m_comboSupplier;
	CSSComboBox		m_comboCategory;
	CSSComboBox		m_comboCatFilterType;
	CStatic			m_staticCustomField[5];
	CSSComboBox		m_comboCustomField[5];
	CButton			m_checkStockFilter;
	CEdit			m_editStockCodeFrom;
	CButton			m_buttonBrowseFrom;
	CEdit			m_editStockCodeTo;
	CButton			m_buttonBrowseTo;
	
	CButton			m_buttonPreset;
	CStatic			m_staticPreset;
	CSSComboBox		m_comboPreset;
	CButton			m_buttonSelect;
	CSSComboBox		m_comboReportType;
	CButton			m_checkByMonth;
	CButton			m_checkCatSort;
	CButton			m_checkPercent;
	CButton			m_checkEstimatedProfit;
	CButton			m_checkApparent;

	CButton			m_buttonGraph;

	BOOL			m_bEposSales;
	BOOL			m_bManualSales;
	BOOL			m_bImportSales;

	CString			m_strStockCodeFrom;
	CString			m_strStockCodeTo;
	
	BOOL			m_bByMonth;
	BOOL			m_bCatSort;
	BOOL			m_bPercent;
	BOOL			m_bEstimatedProfit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryReportStockNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryReportStockNewDlg)
	virtual BOOL OnInitDialog();

	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnSelectLocation();
	
	afx_msg void OnToggleStockRange();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();

	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	afx_msg void OnButtonSelect();
	afx_msg void OnSelectReportType();
	
	afx_msg void OnButtonGraph();
	afx_msg void OnButtonDisplay();
	//}}AFX_MSG

	afx_msg long OnReportMessage ( WPARAM wIndex, LPARAM lParam );
	
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();
	
	void FillSupplierCombo();
	void InitialiseCustomFieldCombos();
	bool GetSupplierArray( CWordArray& wSuppSelectArray );

	void FillPresetCombo( int nSelection );
	void FillReportCombo( int nSelection );
	void SelectReportByItemData( int n );

private:
	void HandleTop100Report( CStockReportTop100Dlg* pDlg );
		
private:
	bool m_bSystemReport;

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bAutoRunPreset;
	
private:
	CSalesHistoryReportStock m_HistoryReport;
	CSalesHistoryCSVRecordStock m_ReportRecord;
	
private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;

private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CCategorySelectorNew m_CategorySelector;
	CCategoryComboHelpers m_CategoryComboHelpers;
	CPluFilterArray m_FilterArray;
	CWordArray m_wSuppSelectArray;

private:
	int m_nCategoryDbIdx;
	int m_nCustomFieldDbIdx;
	bool m_bChangeTop100;

private:
	bool m_bReportShowsPercent;
	bool m_bReportShowsProfit;
	bool m_bReportShowsApparent;
	
private:
	WORD m_IDStaticCustomField[5];
	WORD m_IDComboCustomField[5];
};

/**********************************************************************/
#endif
/**********************************************************************/
