#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportSalesAuditDlg : public CDialog
{
public:
	CStockReportSalesAuditDlg( bool bExceptions, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportSalesAuditDlg)
	enum { IDD = IDD_STOCK_REPORT_SALES_AUDIT };
	CButton	m_checkManualSales;
	BOOL	m_bE1Sales;
	BOOL	m_bImportSales;
	BOOL	m_bManualSales;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportSalesAuditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportSalesAuditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bExceptions;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/
