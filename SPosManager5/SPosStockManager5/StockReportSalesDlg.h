#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockReportSalesDlg : public CDialog
{
public:
	CStockReportSalesDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportSalesDlg)
	enum { IDD = IDD_STOCK_REPORT_SALES };
	CButton m_checkEpos[10];
	BOOL m_bEpos[10];
	CButton	m_checkManual;
	CButton	m_checkImport;
	CButton	m_checkRecipe;
	CButton	m_checkZeroSkip;
	CButton	m_checkAverage;
	CButton m_checkTax;
	BOOL	m_bManual;
	BOOL	m_bImport;
	BOOL	m_bRecipe;
	BOOL	m_bZeroSkip;
	BOOL	m_bAverage;
	BOOL	m_bTax;
	int		m_nSalesReportType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportSalesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CStockReportSalesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonSave();
	afx_msg void OnRadioReportType();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetCSVLine();
	const char* GetReportFields();

private:
	void RestoreDefaults();
	void UpdateEnables();

private:
	CString m_strFieldSelect;

private:
	int m_checkEposID[10];
};

/**********************************************************************/
#endif		//__STOCKREPORTSALESDLG_H__
/**********************************************************************/
