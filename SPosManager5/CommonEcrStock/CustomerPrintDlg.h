#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
/**********************************************************************/

class CCustomerPrintDlg : public CSSDialog
{
public:
	CCustomerPrintDlg( int nLocIdx, bool bOneOff, CWnd* pParent = NULL); 

public:
	void SetTitle( const char* szTitle ){ m_strTitle = szTitle; }
	void SetRangeText( const char* szText ){ m_strRangeText = szText; }

	//{{AFX_DATA(CCustomerPrintDlg)
	enum { IDD = IDD_CUSTOMER_PRINT };
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CStatic	m_staticRange;
	CButton m_checkSPos;
	CButton m_checkEcrman;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomerPrintDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CCustomerPrintDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToggleDateTick();
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDateSelectorRange m_DateSelectorRange;
	CString m_strTitle;
	CString m_strRangeText;
	int m_nLocIdx;
	bool m_bOneOff;
public:
	afx_msg void OnBnClickedStaticRange();
};

/**********************************************************************/
#endif
/**********************************************************************/
