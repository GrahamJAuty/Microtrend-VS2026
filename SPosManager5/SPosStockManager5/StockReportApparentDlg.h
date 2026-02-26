#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportApparentDlg : public CSSDialog
{
public:
	CStockReportApparentDlg( int nType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportApparentDlg)
	enum { IDD = IDD_STOCK_REPORT_APPARENT };
	BOOL	m_bHideAboveMin;
	BOOL	m_bPositive;
	BOOL	m_bZero;
	BOOL	m_bNegative;
	BOOL	m_bZeroCost;
	CButton m_checkHideAboveMin;
	CStatic m_staticPositive;
	CButton m_checkPositive;
	CStatic m_staticZero;
	CButton m_checkZero;
	CStatic m_staticNegative;
	CButton m_checkNegative;
	CButton m_checkZeroCost;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportApparentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnToggleHideAboveMin();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockReportApparentDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nType;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
