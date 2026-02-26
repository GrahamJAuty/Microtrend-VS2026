#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CSalesXXXDlg : public CDialog
{
public:
	CSalesXXXDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CSalesXXXDlg)
	enum { IDD = IDD_SALESXXX };
	CSSComboBoxEx	m_comboLocation;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesXXXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesXXXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetDates();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonView();
	afx_msg void OnButtonProcess();
	afx_msg void OnButtonDiscard();
	afx_msg void OnButtonAuto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetSpIdxArray( CWordArray& array );
	void GetDateInfo( CString& strDate, COleDateTime& oleDateFrom, COleDateTime& oleDateTo );

public:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;

private:
	CString m_strDateFrom;
	CString m_strDateTo;

private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
};

/**********************************************************************/
#endif
/**********************************************************************/

