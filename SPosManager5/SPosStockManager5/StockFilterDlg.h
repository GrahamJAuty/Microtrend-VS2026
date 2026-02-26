#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelectorNew.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockFilterDlg : public CSSDialog
{
public:
	CStockFilterDlg( CStockFilterArray& FilterArray, CWnd* pParent = NULL);
	CStockFilterDlg( CStockFilterArray& FilterArray, CArray<int,int>* pArrayStockIdx, CWnd* pParent = NULL);

	//{{AFX_DATA(CStockFilterDlg)
	enum { IDD = IDD_STOCK_FILTER };
	CSSComboBox	m_comboSupplier;
	CSSComboBox	m_comboCategory;
	CString	m_strSearchText;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetStockIdx() { return m_nStockIdx; }
	void SetSimpleSupplierMode() { m_bSimpleSupplier = TRUE; }

public:
	void SetSpSelect ( int nSpSelect ) { m_nSpSelect = nSpSelect; }
	int GetSpSelect() { return m_nSpSelect; }

private:
	void FillCategoryCombo();
	void SelectCategoryFromNumber();
	void FillSupplierCombo();
	void SelectSupplierFromNumber();

	void RunSearch( bool bDoSearch, bool bUpdateDisplay );
	void UpdateTitle();
	
private:
	CSSListCtrlVirtual m_listStock;
	CStockFilterArray& m_StockFilterArray;

protected:
	//{{AFX_MSG(CStockFilterDlg)
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnSelectSearchText();
	afx_msg void OnSelectCategory();
	afx_msg void OnSelectSupplier();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCategorySelectorNew m_CategorySelector;
	int m_nCategoryFilter;
	int m_nSupplierFilter;
	int m_nSpSelect;
	bool m_bSimpleSupplier;
	CString m_strTitle;

private:
	int m_nStockIdx;
	CArray<int,int>* m_pArrayStockIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
