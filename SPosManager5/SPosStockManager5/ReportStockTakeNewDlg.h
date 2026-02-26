#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategoryComboHelpers.h"
#include "CategorySelectorNew.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PresetReportBase.h"
#include "StockReportSortOrderCSVArray.h"
#include "StockTreeReportNewEstimateAllowance.h"
#include "StockTreeReportNewReconcile.h"
#include "StockTreeReportNewReconcileCash.h"
#include "StockTreeReportNewReconcileCategory.h"
#include "StockTreeReportNewStockSheet.h"
#include "StockTreeReportNewTransferClosing.h"
#include "StockTreeReportNewValuationClosing.h"
#include "StockTreeReportNewVariance.h"
/**********************************************************************/

class CReportStocktakeNewDlg : public CSSDialog
{
public:
	CReportStocktakeNewDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CReportStocktakeNewDlg)
	CSSListCtrl		m_listReports;

	CStatic			m_staticPreset;
	CButton			m_buttonPreset;
	CSSComboBox		m_comboPreset;
	
	CButton			m_buttonSortOrder;
	CSSComboBox		m_comboSortOrder;
	CButton			m_checkShowItems;
	
	CSSComboBox		m_comboStocktake;
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

	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CReportStocktakeNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportStocktakeNewDlg)

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
	void SetDefaultSelection ( int nDefSelectIdx, int nDefSelectType, int nDefStocktake, bool bFixStockpoint );

private:
	void SelectReport();
	void FillSortOrderCombo( int nType, int nSelection );

	void FillPresetCombo( int nSelection );
	
	void FillSupplierCombo();
	void FillCustomFieldCombos();
	
	void SetEnables();
	
	void PrintClosingValuationReport();
	void PrintVarianceReport();
	void PrintReconcileReport();
	void PrintEstimateAllowanceReport();
	void PrintReconcileCategoryReport();
	void PrintReconcileCashReport();
	void PrintTransferDetailReport();
	void PrintStockSheet();
	
	bool GetSupplierIdxArray( CWordArray& wArray );

	int GetSortOrderTypeFromReportType( int nReportType );

private:
	void InitialiseCustomFieldCombos();

public:
	void CreatePreset( CPresetReportInfo& base ); 
	
private:
	int m_nReportType;

	int m_nReportTimeType;
	int m_nCategoryDbIdx;
	int m_nCustomFieldDbIdx;
	
	bool m_bSelectShowItem;
	bool m_bSelectDrawLines;
	bool m_bSelectSuppRef;
	bool m_bSelectStockUnit;
	bool m_bSelectExcludeInactive;

	CWordArray m_wSuppSelectArray;
	int m_nDefSelectIdx;
	int m_nDefSelectType;
	int m_nDefStocktake;

private:
	CCategorySelectorNew m_CategorySelector;
	CCategoryComboHelpers m_CategoryComboHelpers;
	CLocationSelector m_LocationSelector;
	CLocationSelectorEntity m_LocSelEntityOld;

private:
	CStockLevelTreeNew m_StockLevelTree;
	CStockTreeReportNewValuationClosing m_StockTreeReportValuationClosing;
	CStockTreeReportNewEstimateAllowance m_StockTreeReportEstimatedAllowance;
	CStockTreeReportNewReconcile m_StockTreeReportReconcile;
	CStockTreeReportNewReconcileCash m_StockTreeReportReconcileCash;
	CStockTreeReportNewReconcileCategory m_StockTreeReportReconcileCategory;
	CStockTreeReportNewVariance m_StockTreeReportVariance;
	CStockTreeReportNewTransferClosing m_StockTreeReportTransferClosing;
	CStockTreeReportNewStockSheet m_StockTreeReportStockSheet;
	
private:
	CSalesHistoryCSVRecordStock m_chartinfoSales;
	CSalesHistoryCSVRecordStock m_chartinfoApparent;

private:
	CStockReportSortOrderCSVArray m_arraySortOrder;
	
private:
	WORD m_IDStaticCustomField[5];
	WORD m_IDComboCustomField[5];

private:
	bool m_bFixStockpoint;
};

/**********************************************************************/
#endif
/**********************************************************************/
