#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "CategorySelector.h"
/**********************************************************************/

class CStockDeleteRangeSetDlg : public CSSDialog
{
public:
	CStockDeleteRangeSetDlg( CWnd* pParent = NULL );   

	//{{AFX_DATA(CStockDeleteRangeSetDlg)
	enum { IDD = IDD_STOCK_DELETE_RANGE_SET };
	CSSComboBox	m_comboCategory;
	int		m_nDelete;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDeleteRangeSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockDeleteRangeSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusStockCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	void FillCategoryCombo();

public:
	CCategorySelector m_CategorySelector;
	CString m_strStockCodeFrom;
	CString m_strStockCodeTo;
	int m_nCategoryFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/

