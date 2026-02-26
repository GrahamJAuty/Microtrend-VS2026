#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
//include "MyComboBox.h"
/**********************************************************************/
//ECRMAN USES THIS FOR MANUAL EPOS SALES REPORT
/**********************************************************************/

class CProcessedOrderPrintDlg : public CDialog
{
public:
	CProcessedOrderPrintDlg( const char* szTitle, bool bAllDates, CWnd* pParent = NULL);   

	//{{AFX_DATA(CProcessedOrderPrintDlg)
	enum { IDD = IDD_ORDER_PRINT };
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessedOrderPrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessedOrderPrintDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSetDates();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CDateSelectorRange m_DateSelectorRange;
	CString m_strDateFrom;
	CString m_strDateTo;
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;

private:
	CString m_strTitle;
	bool m_bAllDates;
};

/**********************************************************************/
