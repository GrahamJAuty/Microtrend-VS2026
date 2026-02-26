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

class CStockRangeUnitsDlg : public CSSDialog
{
public:
	CStockRangeUnitsDlg( CStockFilterArray& FilterArray, CWnd* pParent = NULL);   
	~CStockRangeUnitsDlg();

	//{{AFX_DATA(CStockRangeUnitsDlg)
	enum { IDD = IDD_STOCK_RANGE_UNITS };
	CSSComboBox	m_comboSaleWeight;
	CSSComboBox	m_comboUnitWeight;
	CSSComboBox	m_comboCategory;
	CSSComboBox	m_comboSubUnit;
	CSSComboBox	m_comboStockUnit;
	CString	m_strStockCodeFrom;
	CString	m_strStockCodeTo;
	CString	m_strStockUnit;
	CString	m_strSubUnit;
	double	m_dWeight;
	BOOL	m_bChangeStockUnit;
	BOOL	m_bChangeSubUnit;
	BOOL	m_bChangeWastage;
	BOOL	m_bChangeWeight;
	BOOL	m_bChangeWeightUnit;
	BOOL	m_bChangeYield;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockRangeUnitsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	bool GetModifiedFlag() { return m_bModifiedStock; }

private:
	CEdit* GetEditYield();
	CEdit* GetEditWeight();
	CEdit* GetEditWastage();

protected:
	//{{AFX_MSG(CStockRangeUnitsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnCheckToggle();
	afx_msg void OnSelectUnitWeight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillCategoryCombo();
	void FillStockUnitCombo();
	void FillUnitWeightCombo();
	void SetEnables();
	void UpdateWeightCombos( int nUnitWeightType, int nSaleWeightType );
	void GetWeightTypes( int& nUnitWeightType, int& nSaleWeightType );
	int UpdateProgram();
	
private:
	CStockFilterArray& m_FilterArray;
	bool m_bModifiedStock;

private:
	CCategorySelector m_CategorySelector;
	double m_dWastePercent;
	int m_nSubUnits;
};

/**********************************************************************/
#endif
/**********************************************************************/

