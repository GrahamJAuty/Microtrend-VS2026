#pragma once
/**********************************************************************/
#include "DateSelectorRange.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "SalesHistoryReportPlu.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CSalesHistoryReportBritanniaDlg : public CSSDialog
{
public:
	CSalesHistoryReportBritanniaDlg(CWnd* pParent = NULL);
	~CSalesHistoryReportBritanniaDlg();

	//{{AFX_DATA(CSalesHistoryReportBritanniaDlg)
	enum { IDD = IDD_SALESHISTORY_BRITANNIA };
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboSession;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CSSComboBoxEx	m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	CButton m_checkGuest;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryReportBritanniaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryReportBritanniaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDate();
	afx_msg void OnSelectLocation();
	afx_msg void OnButtonDisplay();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDeptName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();
	
private:
	bool m_bSystemReport;
	
private:
	CSalesHistoryReportPlu m_HistoryReport;
	
private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	
private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CDepartmentSelector m_DepartmentSelector;
	
private:
	int m_nLocationComboDbIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
