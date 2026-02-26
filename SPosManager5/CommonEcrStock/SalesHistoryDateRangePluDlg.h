#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "DeptAndPluComboHelpers.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "SalesHistoryDateRangePlu.h"
/**********************************************************************/

class CSalesHistoryDateRangePluDlg : public CSSDialog
{
public:
	CSalesHistoryDateRangePluDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CSalesHistoryDateRangePluDlg)
	enum { IDD = IDD_SALESHISTORY_PLU_DATERANGE };
	CSSComboBoxEx	m_comboLocation;
	CSSComboBox	m_comboZeroSkip;
	CSSComboBox	m_comboReport;
	CButton m_buttonTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CSSComboBox m_comboSession;
	CButton	m_buttonGraph;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CSSComboBox m_comboPluTime;
	CButton	m_checkSpecify;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryDateRangePluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryDateRangePluDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();
	afx_msg void OnKillFocusPluFrom();
	afx_msg void OnKillFocusPluTo();
	afx_msg void OnTogglePluRange();
	afx_msg void OnSelectDepartment();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnButtonGraph();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonTaxBand();
	afx_msg void OnSelectReport();
	afx_msg void OnSelectLocation();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
	
private:
	void ReloadDatabase();
	void FillTaxBandComboForLocation();
	bool GetAdhocReportSession();
	
private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	
private:
	void CreateReport( bool bGraph );

private:
	int m_nLocationComboDbIdx;
		
private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CSalesHistoryDateRangePlu m_Report;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CDepartmentSelector m_DepartmentSelector;

	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	
private:
	CLocationSelector m_LocationSelector;
	CSSListTaggedSelectCtrl m_listDateRange;
	CStringArray m_arrayDateRangeNames;
	CPluFilterArray m_FilterArray;
};

/**********************************************************************/

