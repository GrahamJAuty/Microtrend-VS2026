#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSDialog.h"
#include "SubUnitEdit.h"
/**********************************************************************/

class CStockEditInitialDlg : public CSSDialog
{
public:
	CStockEditInitialDlg( CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockEditInitialDlg)
	enum { IDD = IDD_STOCK_EDIT_INITIAL };
	CStatic	m_staticOpening;
	CStatic	m_staticOpeningUnit;
	CStatic	m_staticMinimum;
	CStatic	m_staticMinimumUnit;
	CStatic	m_staticReorder;
	CStatic	m_staticReorderUnit;
	CStatic	m_staticMaximum;
	CStatic	m_staticMaximumUnit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockEditInitialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CStockEditInitialDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CEdit* GetEditQtyOpening();
	CEdit* GetEditQtyMinimum();
	CEdit* GetEditQtyReorder();
	CEdit* GetEditQtyMaximum();

private:
	CSubUnitEdit m_editStockSubOpening;
	CSubUnitEdit m_editStockSubMinimum;
	CSubUnitEdit m_editStockSubReorder;
	CSubUnitEdit m_editStockSubMaximum;
	
private:
	double m_dOldQuantityOpening;

	double m_dQuantityOpening;
	__int64 m_nTotalSubUnitsOpening;
	double m_dQuantityMinimum;
	__int64 m_nTotalSubUnitsMinimum;
	double m_dQuantityReorder;
	__int64 m_nTotalSubUnitsReorder;
	double m_dQuantityMaximum;
	__int64 m_nTotalSubUnitsMaximum;

private:
	CStockCSVRecord& m_StockRecord;
	CStockLevelsCSVRecord& m_StockLevels;

private:
	bool m_bEditOpening;
	bool m_bEditMinimum;
	bool m_bEditMaximum;
	int m_nSubUnits;
};

/**********************************************************************/
#endif
/**********************************************************************/
