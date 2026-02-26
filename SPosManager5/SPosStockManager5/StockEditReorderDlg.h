#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSDialog.h"
//include "MySSListCtrl.h"
#include "StockFilterArray.h"
#include "SubUnitEdit.h"
/**********************************************************************/

class CStockEditReorderDlg : public CSSDialog
{
public:
	CStockEditReorderDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockEditReorderDlg)
	enum { IDD = IDD_STOCK_EDIT_REORDER };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockEditReorderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CStatic m_staticLevels;
	CStatic m_staticMinimum;
	CStatic m_staticReorder;
	CStatic m_staticMaximum;
	CSpinButtonCtrl m_SpinButton;
	CButton m_buttonDefault;
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	// Generated message map functions
	//{{AFX_MSG(CStockEditReorderDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultButton();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SpinPrevious();
	void SpinNext();
	void ProcessLevels();
	void SelectStockRecord();
	void PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos );
	void SetEditFocus();

private:
	CEdit* GetEditStockQtyMinimum();
	CEdit* GetEditStockQtyReorder();
	CEdit* GetEditStockQtyMaximum();
	CSubUnitEdit m_editStockSubMinimum;
	CSubUnitEdit m_editStockSubReorder;
	CSubUnitEdit m_editStockSubMaximum;
	
private:
	bool m_bAllowReorder;
	int m_nSpIdx;
	CStockFilterArray* m_pFilterArray;

private:
	double m_dQuantityMinimum;
	__int64 m_nTotalSubUnitsMinimum;
	double m_dQuantityReorder;
	__int64 m_nTotalSubUnitsReorder;
	double m_dQuantityMaximum;
	__int64 m_nTotalSubUnitsMaximum;
	
private:
	CSSListCtrlVirtual& m_StockList;
};

/**********************************************************************/
#endif
/**********************************************************************/
