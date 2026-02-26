#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "StockCustomListFields.h"
#include "StockFilterArray.h"
#include "StockLevelTreeNew.h"
#include "StockTreeReportNewCustomList.h"
/**********************************************************************/

class CStockCustomListDlg : public CSSDialog
{
public:
	CStockCustomListDlg( CStockFilterArray& StockFilter, int nRepNum, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockCustomListDlg)
	enum { IDD = IDD_STOCK_CUSTOM_LIST };
	CButton	m_checkDrawLines;
	CSSComboBox	m_comboSortOrder;
	CSSComboBox	m_comboSupplier;
	CButton	m_checkExcludeInactive;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CEdit	m_editStockCodeTo;
	CEdit	m_editStockCodeFrom;
	CStatic	m_staticCategory;
	CSSComboBox	m_comboCategory;
	CSSComboBoxEx	m_comboConLevel;
	int		m_nCategorySelect;
	BOOL m_bSpecifyStockCodes;
	CString	m_strStockCodeFrom;
	CString	m_strStockCodeTo;
	BOOL	m_bExcludeInactive;
	BOOL	m_bDrawLines;
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockCustomListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockCustomListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonPrint();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnToggleAllStock();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonResetOrder();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillCategoryCombo();
	void FillSupplierCombo();
	void FillSortOrderCombo();

	bool GetSupplierIdxArray( CWordArray& wArray );

	void CreateAvailableFieldList();
	void AddAvailableField( CustomStockField Field,  const char* szCustomText = "" );
	void AddFieldToList( const char* szField, int nData, int nTagState );
	void AppendFieldTicks ( CCSV& csv );

	void CreateSavedSettingsLine();
	
private:
	int m_nCategoryDbIdx;
	int m_nRepNum;
	CWordArray m_wSuppSelectArray;

	CString m_strFilenameSettings;
	CString m_strSavedSettingsLine;

	CStockFilterArray& m_StockFilter;

private:
	CCategorySelector m_CategorySelector;
	CLocationSelector m_LocationSelector;
	CSSListTaggedSelectCtrl m_listFields;
	
private:
	CStockLevelTreeNew m_StockLevelTree;
	CStockTreeReportNewCustomList m_StockTreeReportCustomList;
	bool m_bFirstTime;

private:
	CArray<int,int> m_arrayDefaultFields;
	CReportConsolidationArray<CSortedStringByInt> m_arrayAvailableFields;
};

/**********************************************************************/
#endif
/**********************************************************************/

