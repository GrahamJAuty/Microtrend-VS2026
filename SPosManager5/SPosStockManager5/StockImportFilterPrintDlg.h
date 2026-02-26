#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockImportFilterPrintDlg : public CDialog
{
public:
	CStockImportFilterPrintDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockImportFilterPrintDlg)
	enum { IDD = IDD_STOCK_IMPORT_FILTER_PRINT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockImportFilterPrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

public:
	BOOL m_bLines;
	BOOL m_bMarked;
	
protected:
	//{{AFX_MSG(CStockImportFilterPrintDlg)
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


