#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportAllowanceDlg : public CSSDialog
{
public:
	CStockReportAllowanceDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportAllowanceDlg)
	enum { IDD = IDD_STOCK_REPORT_ALLOWANCE };
	//}}AFX_DATA
	CStatic m_staticTax;
	CStatic m_staticTax2;

	//{{AFX_VIRTUAL(CStockReportAllowanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	double m_dAllowance[9];
	double m_dAllowanceTotal;
	double m_dAllowanceTax;
	double m_dCash;
	double m_dCashTax;

private:
	int m_nAllowanceID[9];
	int m_nAllowanceNameID[9];
	CString m_strAllowanceName[9];
	
protected:
	//{{AFX_MSG(CStockReportAllowanceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusAllowance();
	afx_msg void OnKillFocusCash();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CEdit* GetEditAllowance( int n );
	CEdit* GetEditAllowanceTotal();
	CEdit* GetEditAllowanceTax();
	CEdit* GetEditCash();
	CEdit* GetEditCashTax();
};

/**********************************************************************/
#endif
/**********************************************************************/

