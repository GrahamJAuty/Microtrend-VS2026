#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportSupplierProfitTickDlg : public CDialog
{
public:
	CStockReportSupplierProfitTickDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportSupplierTickDlg)
	enum { IDD = IDD_STOCK_REPORT_SUPPLIER_PROFIT_TICK };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportSupplierProfitTickDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

public:
	BOOL m_bAlphaSort;

protected:
	//{{AFX_MSG(CStockReportSupplierProfitTickDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/
