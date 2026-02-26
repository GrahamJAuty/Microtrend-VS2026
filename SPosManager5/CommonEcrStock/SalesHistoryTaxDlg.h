#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSalesHistoryTaxDlg : public CSSDialog
{
public:
	CSalesHistoryTaxDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CSalesHistoryTaxDlg)
	enum { IDD = IDD_SALESHISTORY_TAX };
	WORD m_IDTaxStatic[MAX_TAX_BANDS + 1];
	WORD m_IDTaxCheck[MAX_TAX_BANDS + 1];
	CStatic m_staticTaxBand[MAX_TAX_BANDS + 1];
	CButton m_checkTaxBand[MAX_TAX_BANDS + 1];
	CButton m_buttonAll;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryTaxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryTaxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAll();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bAllButton;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/

