#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CProcessedOrderAutoPrintBasicDlg : public CDialog
{
public:
	CProcessedOrderAutoPrintBasicDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CProcessedOrderAutoPrintBasicDlg)
	enum { IDD = IDD_ORDER_AUTOPRINT_BASIC };
	BOOL	m_bPurchase;
	BOOL	m_bReports;
	BOOL	m_bEmail;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderAutoPrintBasicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderAutoPrintBasicDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
};

/**********************************************************************/
#endif
/**********************************************************************/

