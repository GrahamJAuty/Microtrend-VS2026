#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportAdjustmentDlg : public CDialog
{
public:
	CStockReportAdjustmentDlg( int nTimeType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportAdjustmentDlg)
	enum { IDD = IDD_STOCK_REPORT_ADJUSTMENT };
	CButton	m_checkWasteAuto;
	BOOL	m_bAdjust;
	BOOL	m_bSales;
	BOOL	m_bWasteAuto;
	BOOL	m_bWasteEcr;
	BOOL	m_bBreakdown;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportAdjustmentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportAdjustmentDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nTimeType;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/

