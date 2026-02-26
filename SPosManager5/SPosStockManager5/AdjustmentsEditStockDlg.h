#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
#include "StockFilterArray.h"
#include "StocktakeCSVArray.h"
#include "SubUnitEdit.h"
/**********************************************************************/

class CAdjustmentsEditStockDlg : public CSSDialog
{
public:
	CAdjustmentsEditStockDlg( int nSpIdx, CString& strLastAdjustment, CAdjustmentsEditStockDlg** pParentToThis, int nStkTakeNo, CWnd* pParent = NULL);   
	CAdjustmentsEditStockDlg( int nSpIdx, CString& strDummy, int nStkTakeNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CAdjustmentsEditStockDlg)
	CSSComboBox m_comboReason;
	CButton	m_buttonCancel;
	CButton	m_buttonBrowsePlu;
	CButton	m_buttonBrowseStock;
	CButton	m_radioStockCode;
	CButton	m_radioPluNumber;
	CButton	m_buttonOK;
	CEdit	m_editStockCode;
	CStatic m_staticUnitText;
	double	m_dCost;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdjustmentsEditStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CString m_strInputLine;
	CString m_strOutputLine;

protected:
	//{{AFX_MSG(CAdjustmentsEditStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowseStock();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnButtonBrowsePlu();
	afx_msg void OnKillFocusStockQty();
	afx_msg void OnKillFocusStockSub();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRadioPluNumber();
	afx_msg void OnRadioStockCode();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DataInit( int nSpIdx );
	void UpdateStockCode( int nStockIdx );
	void PrepareEditBoxes( CStockCSVRecord& StockRecord );
	void SetEditFocus();
	int HandleAdd();

private:
	bool LookUpPluNo( __int64 nPluNo, CString& strStockCode, int& nStockIdx );
	bool LookUpStockCode( __int64& nPluNo, CString& strStockCode, int& nStockIdx );

private:
	void BrowsePluInternal();
	void BrowseStockInternal();

private:
	CEdit* GetEditStockQty();
	CEdit* GetEditPluNo();
	CEdit* GetEditCost();
	
private:
	bool m_bAdd;
	CString m_strAction;
	int m_nEntryMode;

private:	
	CPluFilterArray m_FilterArray;	
	CStockFilterArray m_StockFilterArray;

private:
	CString m_strStockCode;
	CString m_strDescription;
	CString m_strPluNo;
	CString& m_strLastAdjustment;
	int m_nSpIdx;
	int m_nSpNo;

private:
	CSubUnitEdit m_editStockSub;
	CAdjustmentsEditStockDlg** m_pParentToThis;

private:
	double m_dQuantity;
	__int64 m_nTotalSubUnits;

private:
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif
/**********************************************************************/

