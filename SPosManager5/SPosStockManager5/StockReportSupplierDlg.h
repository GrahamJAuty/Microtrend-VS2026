#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockReportSupplierDlgHelpers.h"
/**********************************************************************/

class CStockReportSupplierDlg : public CDialog
{
public:
	CStockReportSupplierDlg( CWordArray& wSuppSelectArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportSupplierDlg)
	enum { IDD = IDD_STOCK_REPORT_SUPPLIER };
	CSSComboBox	m_comboSupplier;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportSupplierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportSupplierDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAll();
	virtual void OnOK();
	afx_msg void OnButtonNone();
	afx_msg void OnSelectSupplierSet();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
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
