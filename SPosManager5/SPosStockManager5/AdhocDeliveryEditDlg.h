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
#define PLULOOKUP_NOPLU 0
#define PLULOOKUP_NOLINK 1
#define PLULOOKUP_GOTLINK 2
/**********************************************************************/

class CAdhocDeliveryEditDlg : public CSSDialog
{
public:
	CAdhocDeliveryEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, int& nNewItemDeptNo, CAdhocDeliveryEditDlg** pParentToThis, int nStkTakeNo, CWnd* pParent = NULL);   
	CAdhocDeliveryEditDlg( int nSuppIdx, int nSpIdx, CSuppRefFinder& SuppRefFinder, int& nNewItemDeptNo, int nStkTakeNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CAdhocDeliveryEditDlg)
	CButton	m_buttonNew;
	CEdit	m_editDescriptionPlu;
	CButton	m_buttonEdit;
	CStatic	m_staticGPType;
	CEdit	m_editPrice1;
	CEdit	m_editGP;
	CSSComboBox m_comboRetail;
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
	BOOL	m_bFreeStock;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdhocDeliveryEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CAdhocDeliveryEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseStock();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnButtonFreeStock();
	afx_msg void OnKillFocusQty();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKillFocusSuppRef();
	afx_msg void OnRadioPluNumber();
	afx_msg void OnRadioStockCode();
	afx_msg void OnRadioSuppRef();
	afx_msg void OnSelectRetailPlu();
	afx_msg void OnButtonEdit();
	afx_msg void OnSelectDelUnit();
	afx_msg void OnButtonEditDeliveryUnit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DataInit( int nSuppIdx, int nSpIdx );
	void UpdateStockCode( int nStockIdx, int nStockSuppIdx );
	void FillRetailCombo( int nStockIdx );
	void UpdateRetailPlu();
	int HandleAdd();

	void UpdateItemKey( bool bFromControl, bool bPluNo, bool bStockCode, bool bSuppRef );

private:
	bool CreatePluRecord( __int64 nPluNo );
	bool CreatePluRecordInternal( __int64 nPluNo );
	bool LinkPluRecord( __int64 nPluNo );
	bool LinkPluRecordInternal( __int64 nPluNo );

private:
	void BrowsePluInternal();
	void BrowseStockInternal();
	void HandlePluInternal();
	void HandleEditInternal();

private:
	bool LookUpSuppRef( int& nStockIdx, int& nStockSuppIdx );
	int LookUpPluNo( int& nStockIdx, int& nStockSuppIdx );
	bool LookUpStockCode( int& nStockIdx, int& nStockSuppIdx );
	void FillDeliveryUnitCombo( int nStockIdx, int nStockSuppIdx );

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
	int m_nStkTakeNo;

private:
	CAdhocDeliveryEditDlg** m_pParentToThis;
	int& m_nNewPluDeptNo;
};

/**********************************************************************/
#endif
/**********************************************************************/


