#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockDeleteDlg : public CDialog
{
public:
	CStockDeleteDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockDeleteDlg)
	enum { IDD = IDD_STOCK_DELETE };
	CSSComboBoxEx	m_comboDatabase;
	BOOL	m_bKeepApparent;
	BOOL	m_bKeepOnOrder;
	BOOL	m_bKeepSales;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockDeleteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	int DeleteStockInternal();
	
protected:
	//{{AFX_MSG(CStockDeleteDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CLocationSelector m_LocationSelector;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/


