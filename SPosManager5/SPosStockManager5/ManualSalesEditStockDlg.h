/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DelUnitSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "StockItemKey.h"
#include "StockFilterArray.h"
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CManualSalesEditStockDlg : public CSSDialog
{
public:
	CManualSalesEditStockDlg( int nSpIdx, CSuppRefFinder& SuppRefFinder, CManualSalesEditStockDlg** pParentToThis, int m_nStkTakeNo, CWnd* pParent = NULL);   
	CManualSalesEditStockDlg( int nSpIdx, CSuppRefFinder& SuppRefFinder, int m_nStkTakeNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CManualSalesEditStockDlg)
	enum { IDD = IDD_MANUAL_SALES_EDIT_STOCK };
	CButton	m_buttonCancel;
	CButton	m_buttonBrowsePlu;
	CButton	m_buttonBrowseStock;
	CButton	m_radioSuppRef;
	CButton	m_radioStockCode;
	CButton	m_radioPluNumber;
	CEdit	m_editSuppRef;
	CButton	m_buttonOK;
	CSSComboBoxEx	m_comboDeliveryUnit;
	CEdit	m_editStockCode;
	double	m_dRetail;
	double	m_dQty;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CManualSalesEditStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CManualSalesEditStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseStock();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnKillFocusQty();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKillFocusSuppRef();
	afx_msg void OnRadioPluNumber();
	afx_msg void OnRadioStockCode();
	afx_msg void OnRadioSuppRef();
	afx_msg void OnSelectDelUnit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DataInit( int nSuppIdx, int nSpIdx );
	void UpdateStockCode( int nStockIdx, int nStockSuppIdx );
	int HandleAdd();

private:
	bool LookUpSuppRef( int& nStockIdx, int& nStockSuppIdx );
	bool LookUpPluNo( int& nStockIdx, int& nStockSuppIdx );
	bool LookUpStockCode( int& nStockIdx, int& nStockSuppIdx );
	void FillDeliveryUnitCombo( int nStockIdx, int nStockSuppIdx );

	void UpdateItemKey( bool bFromControl, bool bPluNo, bool bStockCode, bool bSuppRef );

private:
	void BrowsePluInternal();
	void BrowseStockInternal();

private:
	CEdit* GetEditQty();
	CEdit* GetEditRetail();
	CEdit* GetEditPluNo();

private:
	bool m_bAdd;
	CString m_strAction;
	int m_nEntryMode;

private:
	CDelUnitSelector m_DelUnitSelector;
	CPluFilterArray m_FilterArray;
	CStockFilterArray m_StockFilterArray;
	CSuppRefFinder& m_SuppRefFinder;

private:
	CStockItemKey m_StockItemKey;
	CString m_strSupplier;
	CString m_strDescription;
	int m_nSuppNo;
	int m_nSuppIdx;
	int m_nSpIdx;
	int m_nSpNo;

private:
	CManualSalesEditStockDlg** m_pParentToThis;

private:
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif
/**********************************************************************/

