#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
#include "CategorySetCSVArray.h"
/**********************************************************************/

class CStocktakeCategorySetDlg : public CDialog
{
public:
	CStocktakeCategorySetDlg( CCategorySetCSVRecord& CategorySet, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStocktakeCategorySetDlg)
	enum { IDD = IDD_STOCKTAKE_CATEGORYSET };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStocktakeCategorySetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStocktakeCategorySetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAll();
	virtual void OnOK();
	afx_msg void OnButtonNone();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddList();

private:
	CSSListTaggedSelectCtrl m_listCategories;
	CCategorySetCSVRecord& m_CategorySet;
};

/**********************************************************************/
#endif
/**********************************************************************/

