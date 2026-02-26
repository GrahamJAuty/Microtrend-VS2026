#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportVarianceDlg : public CDialog
{
public:
	CStockReportVarianceDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportVarianceDlg)
	enum { IDD = IDD_STOCK_REPORT_VARIANCE };
	BOOL	m_bZeroSkip;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportVarianceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportVarianceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
