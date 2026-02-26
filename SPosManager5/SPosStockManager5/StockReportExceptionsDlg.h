#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "StockAuditExceptions.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportExceptionsDlg : public CDialog
{
public:
	CStockReportExceptionsDlg( CStockAuditExceptionTypes& Types, CWnd* pParent = NULL); 

	//{{AFX_DATA(CStockReportExceptionsDlg)
	enum { IDD = IDD_STOCK_REPORT_EXCEPTIONS };
	BOOL	m_bAdjustments;
	BOOL	m_bDelivery;
	BOOL	m_bOpening;
	BOOL	m_bReturns;
	BOOL	m_bStocktaking;
	BOOL	m_bTransfers;
	BOOL	m_bSales;
	BOOL	m_bHeaders;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportExceptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportExceptionsDlg)
	virtual void OnOK();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStockAuditExceptionTypes& m_Types;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/

