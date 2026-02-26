#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockDeletePrintDlg : public CDialog
{
public:
	CStockDeletePrintDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockDeletePrintDlg)
	enum { IDD = IDD_STOCK_DELETE_PRINT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDeletePrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

public:
	BOOL m_bLines;
	BOOL m_bMarked;
	BOOL m_bOnOrder;
	BOOL m_bApparent;
	BOOL m_bSales;
	BOOL m_bValue;
	BOOL m_bEmpty;
	
protected:
	//{{AFX_MSG(CStockDeletePrintDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnButtonSave();
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/


