#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
/**********************************************************************/

class CStockReportReorderDateDlg : public CDialog
{
public:
	CStockReportReorderDateDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CStockReportReorderDateDlg)
	enum { IDD = IDD_STOCK_REPORT_REORDERDATE };
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	BOOL	m_bReorderItems;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockReportReorderDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CString m_strDateFrom;
	CString m_strDateTo;
	
protected:
	//{{AFX_MSG(CStockReportReorderDateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CDateSelectorRange m_DateSelectorRange;
};

/**********************************************************************/
#endif
/**********************************************************************/
