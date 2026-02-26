#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DelUnitSelector.h"
//include "MyComboBoxEx.h"
//include "MySSListCtrl.h"
#include "StockFilterArray.h"
#include "SubUnitEdit.h"
/**********************************************************************/

class CStockEditTransferDlg : public CSSDialog
{
public:
	CStockEditTransferDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockEditTransferDlg)
	enum { IDD = IDD_STOCK_EDIT_TRANSFER };
	CEdit	m_editTo;
	CEdit	m_editFrom;
	//}}AFX_DATA
	CSSComboBoxEx	m_comboDelUnit;
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockEditTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CStockEditTransferDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultButton();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectDeliveryUnit();
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
	CEdit* GetEditStockQty();

private:
	CSubUnitEdit m_editStockSub;
	int m_nSpIdx;
	int m_nSpIdx2;
	CStockFilterArray* m_pFilterArray;
	
private:
	double m_dQuantity;
	__int64 m_nTotalSubUnits;

private:
	int m_nSelectedDelUnit;
	CDelUnitSelector m_DelUnitSelector;
	
private:
	CSSListCtrlVirtual& m_StockList;
};

/**********************************************************************/
#endif
/**********************************************************************/
