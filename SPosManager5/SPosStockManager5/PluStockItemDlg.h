#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockFilterArray.h"
#include "SubUnitFractionEdit.h"
/**********************************************************************/

class CPluStockItemDlg : public CSSDialog
{
public:
	CPluStockItemDlg( CRecipeCSVRecord& RecipeRecord, const char* szTitle, bool bEditStockCode, bool bEditSpecialFlags, bool bApparentStock, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluStockItemDlg)
	enum { IDD = IDD_PLU_STOCKITEM };
	CEdit m_editStockCode;
	CButton	m_buttonBrowse;
	CEdit m_editDescription;
	CStatic	m_staticWeight;
	CStatic m_staticStockpoint;
	CSSComboBox m_comboStockpoint;
	CStatic m_staticSalesAction;
	CSSTabbedComboBox m_comboSalesAction;
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluStockItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluStockItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowse();
	afx_msg void OnKillFocusStockQty();
	afx_msg void OnKillFocusStockSub();
	afx_msg void OnKillFocusWeight();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateStockRecordInfo();
	void UpdateFocus();
	void SetDefaultQuantityFocus();
	void FillStockpointCombo();
	void FillSalesActionCombo();

private:
	void EnableSubUnitEntry( bool bEnable );
	void EnableWeightEntry ( bool bEnable );

private:
	CEdit* GetEditStockQty();
	CEdit* GetEditWeight();

private:
	void PutStockQty( double dValue );
	void PutWeight( double dValue );
	void PutSubUnits( double dQuantity );

private:
	double GetStockQty();
	double GetWeight();
	double GetSubUnits();

private:
	double m_dQtyToWeightFactor;
	double m_dStockUnitWeight;
	int m_nSubUnitsPerStockItem;

private:
	bool m_bEditStockCode;
	bool m_bEditSpecialFlags;
	CRecipeCSVRecord& m_RecipeRecord;
	CStockFilterArray m_StockFilterArray;
	CString m_strTitle;
	
private:
	CSubUnitFractionEdit m_editStockSub;
};

/**********************************************************************/
#endif
/**********************************************************************/
