#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonStockTray\RecipeCSVArray.h"
/**********************************************************************/

class CPluStockActionDlg : public CSSDialog
{
public:
	CPluStockActionDlg( CRecipeCSVRecord& RecipeRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluStockActionDlg)
	enum { IDD = IDD_PLU_STOCKACTION };
	CSSComboBox m_comboStockpoint;
	CSSTabbedComboBox m_comboSalesAction;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluStockActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluStockActionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillStockpointCombo();
	void FillSalesActionCombo();

private:
	CRecipeCSVRecord& m_RecipeRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/
