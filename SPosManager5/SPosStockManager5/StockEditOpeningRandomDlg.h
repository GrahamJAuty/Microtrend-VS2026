#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SubUnitEdit.h"
#include "DelUnitSelector.h"
//include "MyComboBox.h"
//include "MySSListCtrl.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CStockEditOpeningRandomDlg : public CSSDialog
{
public:
	CStockEditOpeningRandomDlg( CStockFilterArray* pStockFilterArray, bool bStocktake, int nStocktakeNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockEditOpeningRandomDlg)
	enum { IDD = IDD_STOCK_EDIT_OPENING_RANDOM };
	CButton	m_radioStockCode;
	CButton	m_radioPluNumber;
	CButton	m_buttonUndo;
	CSSComboBox	m_comboAction;
	CButton	m_staticControls;
	CSSComboBoxEx	m_comboDeliveryUnit;
	CEdit	m_editDescription;
	CButton	m_buttonAdd;
	CEdit	m_editStockCode;
	int m_nStockQty;
	int		m_nAction;
	CString	m_strPluNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockEditOpeningRandomDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	bool m_bModified;

protected:
	//{{AFX_MSG(CStockEditOpeningRandomDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonAdd();
	virtual void OnCancel();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnSelectDeliveryUnit();
	virtual void OnOK();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnButtonBrowseStock();
	afx_msg void OnButtonUndo();
	afx_msg void OnRadioPluNumber();
	afx_msg void OnRadioStockCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FindStockCodeFromPlu();
	void FindPluFromStockCode();
	void CheckStockCode();
	void Undo( bool bUpdateList );
	void PrepareEditBoxes( CStockCSVRecord& StockRecord, int nUnitPos );
	int HandleAdd();

private:
	CEdit* GetEditStockQty();
	CEdit* GetEditPluNo();

private:
	double m_dQuantity;
	__int64 m_nTotalSubUnits;
	int m_nSelectedDelUnit;

private:
	CStockFilterArray m_StockFilterArray;
	CPluFilterArray m_PluFilterArray;
	CDelUnitSelector m_DelUnitSelector;
	CString m_strStockCode;
	CString m_strDescription;
	CSSListCtrl m_listOpening;
	CSubUnitEdit m_editStockSub;
	CStringArray m_OpeningArray;
	int m_nEntryMode;
	int m_nStockIdx;
	bool m_bStocktake;
	int m_nStocktakeNo;
	int m_nSpNo;
	int m_nDbNo;
};

/**********************************************************************/
#endif
/**********************************************************************/

