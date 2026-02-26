#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SubUnitEdit.h"
#include "DelUnitSelector.h"
//include "MySSListCtrl.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockEditOpeningDlg : public CSSDialog
{
public:
	CStockEditOpeningDlg( CSSListCtrlVirtual& m_StockList, CStockFilterArray* pFilterArray, bool bStocktake, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockEditOpeningDlg)
	enum { IDD = IDD_STOCK_EDIT_OPENING };
	CSSComboBoxEx	m_comboDelUnit;
	int		m_nAction;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockEditOpeningDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CStockEditOpeningDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultButton();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectDeliveryUnit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SelectStockRecord();
	void PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos );
	void SetEditFocus();
	void SpinPrevious();
	void SpinNext();
	void ProcessLevels();
	
private:
	void UpdateTitle();

private:
	CEdit* GetEditStockQty();
	
private:
	double m_dQuantity;
	__int64 m_nTotalSubUnits;
	
private:
	bool m_bStocktake;
	int m_nSpIdx;
	int m_nSelectedDelUnit;
	CDelUnitSelector m_DelUnitSelector;
	CSubUnitEdit m_editStockSub;
	CStockFilterArray* m_pFilterArray;

private:
	CSSListCtrlVirtual& m_StockList;
};

/**********************************************************************/
#endif
/**********************************************************************/
