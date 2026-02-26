#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CProcessedOrderReportDlg : public CDialog
{
public:
	CProcessedOrderReportDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CProcessedOrderReportDlg)
	enum { IDD = IDD_ORDER_REPORT };
	BOOL m_bConsolidate;
	BOOL m_bSort;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderReportDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/
