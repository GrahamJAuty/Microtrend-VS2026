#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSalesHistoryPriceLevelDlg : public CDialog
{
public:
	CSalesHistoryPriceLevelDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CSalesHistoryPriceLevelDlg)
	enum { IDD = IDD_SALESHISTORY_PRICELEVEL };
	CButton m_checkPriceLevel1;
	CButton m_checkPriceLevel2;
	CButton m_checkPriceLevel3;
	CButton m_checkPriceLevel4;
	CButton m_checkPriceLevel5;
	CButton m_checkPriceLevel6;
	CButton m_checkPriceLevel7;
	CButton m_checkPriceLevel8;
	CButton m_checkPriceLevel9;
	CButton m_checkPriceLevel10;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryPriceLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryPriceLevelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
