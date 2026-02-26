#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockImportAdjustmentReasonDlg : public CDialog
{
public:
	CStockImportAdjustmentReasonDlg( CString& strLastAdjustmentr, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CStockImportAdjustmentReasonDlg)
	enum { IDD = IDD_STOCK_IMPORT_ADJUSTMENT_REASON };
	CButton	m_buttonSet;
	CSSComboBox	m_comboReason;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockImportAdjustmentReasonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CStockImportAdjustmentReasonDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSet();
//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool CheckReason();

private:
	CString& m_strLastAdjustment;
};

/**********************************************************************/
#endif
/**********************************************************************/
