#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Consolidation.h"
#include "DateSelectorRange.h"
//include "MySSListCtrl.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CIDraughtReportDlg : public CDialog
{
public:
	CIDraughtReportDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CIDraughtReportDlg)
	enum { IDD = IDD_IDRAUGHT_REPORT };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIDraughtReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonCheck();
	afx_msg void OnButtonLog();

protected:
	//{{AFX_MSG(CIDraughtReportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ShowReport();

private:
	void GetReportDates();

private:
	CReportConsolidationArray<CSortedStringCounter> m_arrayDates;

private:
	COleDateTime m_OleDateFrom;
	COleDateTime m_OleDateTo;

private:
	CDateSelectorRange m_DateSelectorRange;

private:
	CSSListCtrl m_listReports;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_DatePickerFrom;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
