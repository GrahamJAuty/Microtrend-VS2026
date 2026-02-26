#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "CategoryComboHelpers.h"
#include "CategorySelectorNew.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PresetReportBase.h"
#include "StockTreeReportNewAdjustment.h"
#include "StockTreeReportNewApparent.h"
#include "StockTreeReportNewApparentDU.h"
#include "StockTreeReportNewCostSummary.h"
#include "StockTreeReportNewDelivery.h"
#include "StockTreeReportNewEstimatedProfit.h"
#include "StockTreeReportNewPriceList.h"
#include "StockTreeReportNewReorder.h"
#include "StockTreeReportNewRetailSummary.h"
#include "StockTreeReportNewReturns.h"
#include "StockTreeReportNewSales.h"
#include "StockTreeReportNewStockSheet.h"
#include "StockTreeReportNewSupplier.h"
#include "StockTreeReportNewTop100Sales.h"
#include "StockTreeReportNewTop100Apparent.h"
#include "StockTreeReportNewTransferPeriod.h"
#include "StockTreeReportNewValuationOpening.h"
#include "StockTreeReportNewValuationApparent.h"
#include "StockTreeReportNewValuationTransfer.h"
#include "StockTreeReportNewYtdSales.h"
/**********************************************************************/

class CReportStockNewDlg : public CSSDialog
{
public:
	CReportStockNewDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL);   

	void CreatePreset( CPresetReportInfo& preset );
	int GetPresetError(){ return m_nPresetError; }

	//{{AFX_DATA(CReportStockNewDlg)
	CSSListCtrl		m_listReports;

	CStatic			m_staticPreset;
	CButton			m_buttonPreset;
	CSSComboBox		m_comboPreset;
	
	CButton			m_buttonSortOrder;
	CSSComboBox		m_comboSortOrder;
	CButton			m_checkShowItems;
	
	CSSComboBox		m_comboPeriod;
	CSSComboBoxEx	m_comboConLevel;
	
	CButton			m_checkSuppName;
	CButton			m_checkSuppRef;
	CButton			m_checkStockUnit;
	CButton			m_checkDrawLines;
	CButton			m_checkAlphaSort;
	
	CSSComboBox		m_comboSupplier;
	CSSComboBox		m_comboCategory;
	CStatic			m_staticCustomField[5];
	CSSComboBox		m_comboCustomField[5];
	CButton			m_checkStockCodes;
	CEdit			m_editStockCodeTo;
	CEdit			m_editStockCodeFrom;
	CButton			m_buttonBrowseTo;
	CButton			m_buttonBrowseFrom;
	CButton			m_checkExcludeInactive;
	CButton			m_checkItemFilter;
	
	BOOL			m_bShowItems;
	
	BOOL			m_bSuppName;
	BOOL			m_bSuppRef;
	BOOL			m_bStockUnit;
	BOOL			m_bDrawLines;
	BOOL			m_bAlphaSort;
	
	BOOL			m_bStockCodes;
	CString			m_strStockCodeFrom;
	CString			m_strStockCodeTo;
	BOOL			m_bExcludeInactive;
	BOOL			m_bItemFilter;

	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CReportStockNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportStockNewDlg)

	virtual BOOL OnInitDialog();
	
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	
	afx_msg void OnButtonSortOrder();
	
	afx_msg void OnSelectConLevel();
	
	afx_msg void OnSelectCategory();
	afx_msg void OnToggleAllStock();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();

	afx_msg void OnButtonPrint();
	afx_msg long OnReportMessage ( WPARAM wIndex, LPARAM lParam );

	afx_msg void OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDefaultSelection ( int nDefSelectIdx, int nDefSelectType );

private:
	void SelectReport();
	void FillSortOrderCombo( int nType, int nSelection );

	void FillPresetCombo( int nSelection );
	
	void FillSupplierCombo();
	void FillCustomFieldCombos();
	
	void SetEnables();
	
	void PrintSalesReport();
	void PrintTop100ReportSales();
	void PrintTop100ReportApparent();
	void PrintEstimatedProfitReport();
	void PrintApparentQtyReport();
	void PrintApparentSummaryReport();
	void PrintApparentQtyDUReport();
	void PrintApparentSummaryDUReport();
	void PrintRetailSummaryReport();
	void PrintCostSummaryReport();
	void PrintOpeningValuationReport();
	void PrintApparentValuationReport();
	void PrintOverstockValuationReport();
	void PrintTransferValuationReport();
	void PrintTransferPeriodReport();
	void PrintAdjustmentReport();
	void PrintDeliveryReport();
	void PrintReturnsReport();
	void PrintSupplierReport();
	void PrintReorderReport();
	void PrintOnOrderReportCategory();
	void PrintOnOrderReportSupplier();
	void PrintStockSheet();
	void PrintStockPriceList();

	void OpenStockLevels();

	bool GetSupplierIdxArray( CWordArray& wArray );

	int GetSortOrderTypeFromReportType( int nReportType );
	
private:
	void InitialiseCustomFieldCombos();
	
private:
	int m_nReportType;

	int m_nReportTimeType;
	int m_nCategoryDbIdx;
	int m_nCustomFieldDbIdx;
	
	bool m_bSelectShowItem;
	bool m_bSelectTimeType;
	bool m_bSelectDrawLines;
	bool m_bSelectSuppRef;
	bool m_bSelectStockUnit;
	bool m_bSelectExcludeInactive;
	bool m_bSelectConLevel;

	CWordArray m_wSuppSelectArray;
	int m_nDefSelectIdx;
	int m_nDefSelectType;

private:
	CCategorySelectorNew m_CategorySelector;
	CCategoryComboHelpers m_CategoryComboHelpers;
	CLocationSelector m_LocationSelector;

private:
	CStockLevelTreeNew m_StockLevelTree;
	CStockTreeReportNewAdjustment m_StockTreeReportAdjustment;
	CStockTreeReportNewApparent m_StockTreeReportApparent;
	CStockTreeReportNewApparent m_StockTreeReportSummary;
	CStockTreeReportNewApparentDU m_StockTreeReportApparentDU;
	CStockTreeReportNewApparentDU m_StockTreeReportSummaryDU;
	CStockTreeReportNewCostSummary m_StockTreeReportCostSummary;
	CStockTreeReportNewDelivery m_StockTreeReportDelivery;
	CStockTreeReportNewEstimatedProfit m_StockTreeReportEstimatedProfit;
	CStockTreeReportNewPriceList m_StockTreeReportPriceList;
	CStockTreeReportNewReorder m_StockTreeReportReorder;
	CStockTreeReportNewRetailSummary m_StockTreeReportRetailSummary;
	CStockTreeReportNewReturns m_StockTreeReportReturns;
	CStockTreeReportNewSales m_StockTreeReportSales;
	CStockTreeReportNewStockSheet m_StockTreeReportStockSheet;
	CStockTreeReportNewSupplier m_StockTreeReportSupplier;
	CStockTreeReportNewTop100Apparent m_StockTreeReportTop100Apparent;
	CStockTreeReportNewTop100Sales m_StockTreeReportTop100Sales;
	CStockTreeReportNewValuationOpening m_StockTreeReportValuationOpening;
	CStockTreeReportNewValuationApparent m_StockTreeReportValuationApparent;
	CStockTreeReportNewValuationApparent m_StockTreeReportValuationOverStock;
	CStockTreeReportNewValuationTransfer m_StockTreeReportValuationTransfer;
	CStockTreeReportNewTransferPeriod m_StockTreeReportTransferPeriod;
	CStockTreeReportNewYtdSales m_StockTreeReportYtdSales;

private:
	CSalesHistoryCSVRecordStock m_chartinfoSales;
	CSalesHistoryCSVRecordStock m_chartinfoApparent;

private:
	CStockReportSortOrderCSVArray m_arraySortOrder;
	
private:
	WORD m_IDStaticCustomField[5];
	WORD m_IDComboCustomField[5];

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bAutoRunPreset;
};

/**********************************************************************/
#endif
/**********************************************************************/
