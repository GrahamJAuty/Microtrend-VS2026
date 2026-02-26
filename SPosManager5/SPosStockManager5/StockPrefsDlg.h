#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockPrefsDlg : public CDialog
{
public:
	CStockPrefsDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockPrefsDlg)
	enum { IDD = IDD_STOCK_PREFERENCES };
	BOOL	m_bCreateStock;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockPrefsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockPrefsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nImportMethod;
};

/**********************************************************************/
#endif
/**********************************************************************/
