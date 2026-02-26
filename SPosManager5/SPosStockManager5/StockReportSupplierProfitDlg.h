#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/
#include "StockReportSupplierDlgHelpers.h"
/**********************************************************************/

class CStockReportSupplierProfitDlg : public CDialog
{
public:
	CStockReportSupplierProfitDlg( CWordArray& wSuppSelectArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportSupplierDlg)
	enum { IDD = IDD_STOCK_REPORT_SUPPLIER_PROFIT };
	CSSComboBox	m_comboSupplier;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportSupplierProfitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

public:
	BOOL m_bAlphaSort;

protected:
	//{{AFX_MSG(CStockReportSupplierProfitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAll();
	virtual void OnOK();
	afx_msg void OnButtonNone();
	afx_msg void OnSelectSupplierSet();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	CStockReportSupplierDlgHelpers m_Helpers;

private:
	CSSListTaggedSelectCtrl m_listSuppliers;
	CByteArray m_SupplierFlags;
	CWordArray& m_wSuppSelectArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
