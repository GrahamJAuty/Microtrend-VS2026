#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockDefineFilterDlg : public CSSDialog
{
public:
	CStockDefineFilterDlg( CStockFilterArray& FilterArray, CWnd* pParent = NULL); 
	void FillSupplierCombo();
	void SelectSupplierFromNumber();

	//{{AFX_DATA(CStockDefineFilterDlg)
	enum { IDD = IDD_STOCK_DEFINE_FILTER };
	CSSComboBox	m_comboSupplier;
	CSSComboBox	m_comboCategory;
	CSSComboBox	m_comboMinMax;
	BOOL	m_bMatchCase;
	CString	m_strSearchText;
	BOOL	m_bMatchDescription;
	BOOL	m_bMatchStockCode;
	BOOL	m_bMatchSuppRef;
	BOOL	m_bPluMulti;
	BOOL	m_bPluNone;
	BOOL	m_bPluSingle;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDefineFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
	CStockFilterArray& m_StockFilterArray;

protected:
	//{{AFX_MSG(CStockDefineFilterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectCategory();
	afx_msg void OnSelectSupplier();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillCategoryCombo();

private:
	CCategorySelector m_CategorySelector;
	int m_nSearchType;
	int m_nCategoryFilter;
	int m_nSupplierFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/
