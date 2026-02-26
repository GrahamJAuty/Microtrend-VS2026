#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DelUnitSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "StockFilterArray.h"
#include "StockItemKey.h"
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CReturnsEditDlg : public CSSDialog
{
public:
	CReturnsEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, CReturnsEditDlg** pParentToThis, int nStkTakeNo, CWnd* pParent = NULL);   
	CReturnsEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, int nStkTakeNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CReturnsEditDlg)
	enum { IDD = IDD_RETURNS_EDIT };
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
	double	m_dCost;
	double	m_dQty;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReturnsEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CReturnsEditDlg)
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

	void UpdateItemKey( bool bFromControl, bool bPluNo, bool bStockCode, bool bSuppRef );

private:
	bool LookUpSuppRef( int& nStockIdx, int& nStockSuppIdx );
	bool LookUpPluNo( int& nStockIdx, int& nStockSuppIdx );
	bool LookUpStockCode( int& nStockIdx, int& nStockSuppIdx );
	void FillDeliveryUnitCombo( int nStockIdx, int nStockSuppIdx );

private:
	void BrowsePluInternal();
	void BrowseStockInternal();

private:
	CEdit* GetEditQty();
	CEdit* GetEditCost();
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
	CReturnsEditDlg** m_pParentToThis;

private:
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif
/**********************************************************************/


