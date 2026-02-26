#pragma once
/**********************************************************************/
#include "sslibset.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockFilterArray.h"
#include "SubUnitFractionEdit.h"
/**********************************************************************/

class CRecipeItemDlg : public CSSDialog
{
public:
	CRecipeItemDlg( const char* szTitle, bool bEditStockCode, bool bApparentStock, CWnd* pParent = NULL);   

	//{{AFX_DATA(CRecipeItemDlg)
	enum { IDD = IDD_RECIPE_ITEM };
	CButton	m_buttonOK;
	CEdit	m_editDescription;
	CStatic	m_staticWeight;
	CButton	m_buttonBrowse;
	CEdit	m_editStockCode;
	CString	m_strStockCode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CRecipeItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRecipeItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowse();
	afx_msg void OnKillFocusStockQty();
	afx_msg void OnKillFocusStockSub();
	afx_msg void OnKillFocusWeight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetStockRecordInfo();
	void UpdateControls();
	void UpdateFocus();
	void SetDefaultQuantityFocus();

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
	
	double GetStockQty();
	double GetWeight();
	double GetSubUnits();

public:
	double m_dQuantity;

private:
	CString m_strStockDescription;
	double m_dQtyToWeightFactor;
	double m_dStockUnitWeight;
	int m_nSubUnitsPerStockItem;

private:
	CStockFilterArray m_StockFilterArray;
	CString m_strTitle;
	bool m_bEditStockCode;

private:
	CSubUnitFractionEdit m_editStockSub;

private:
	bool m_bSubUnits;
	bool m_bInitialised;
};

/**********************************************************************/
