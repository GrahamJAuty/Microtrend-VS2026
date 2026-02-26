#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockRangeDlg : public CSSDialog
{
public:
	CStockRangeDlg( const char* szTitle, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockRangeDlg)
	enum { IDD = IDD_STOCK_RANGE };
	CString	m_strStockCodeFrom;
	CString	m_strStockCodeTo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockRangeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusStockCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int UpdateProgram();

private:
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
