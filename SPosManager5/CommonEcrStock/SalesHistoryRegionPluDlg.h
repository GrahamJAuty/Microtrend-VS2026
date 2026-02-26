#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DepartmentSelector.h"
#include "DeptAndPluComboHelpers.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "ReportConsolidationArray.h"
#include "SalesHistoryRegionPlu.h"
#include "SessionCSVArray.h"
/**********************************************************************/

struct CSalesHistoryRegionListItem
{
public:
	CSalesHistoryRegionListItem();
	void Reset();

public:
	int Compare ( CSalesHistoryRegionListItem& source, int nHint = 0 );
	void Add	( CSalesHistoryRegionListItem& source );
	
public:
	int m_nDbIdx;
	int m_nLineNo;
	int m_nConType;
	int m_nEntityIdx;

public:
	bool m_bSelected;
};

/**********************************************************************/

class CSalesHistoryRegionPluDlg : public CSSDialog
{
public:
	CSalesHistoryRegionPluDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CSalesHistoryRegionPluDlg)
	enum { IDD = IDD_SALESHISTORY_PLU_REGION };
	CButton	m_checkLocation;
	CSSComboBox	m_comboReport;
	CButton m_buttonTaxBand;
	CSSTabbedComboBox m_comboTaxBand;
	CSSComboBox	m_comboRound;
	CSSComboBox m_comboZeroSkip;
	CSSComboBox	m_comboSession;
	CButton	m_buttonGraph;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CSSComboBox m_comboPluTime;
	CButton	m_checkSpecify;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CStatic m_staticDatabase;
	CSSComboBoxEx m_comboDatabase;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesHistoryRegionPluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryRegionPluDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDate();
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
	afx_msg void OnSelectDatabase();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();
	
private:
	void UpdateRegionList();
	void ReloadDatabase();
	void FillDepartmentComboForLocation();
	void FillTaxBandComboForLocation();

private:
	void RegionArrayAddDatabases( int& nLineNo );
	void RegionArrayAddLocationSets( int& nLineNo );
	void RegionArrayAddLocations( int& nLineNo );
	void RegionArrayAddLocationSet( int& nLineNo, int nDbIdx, int nLsIdx );
	void RegionArrayAddLocation( int& nLineNo, int nDbIdx, int nLocIdx );
	
private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();
	
private:
	void CreateReport( bool bGraph );
		
private:
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	COleDateTime m_OleDateFrom;
	COleDateTime m_OleDateTo;

private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CSalesHistoryRegionPlu m_Report;
	CDateSelectorRange m_DateSelectorRange;
	CDepartmentSelector m_DepartmentSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CLocationSelector m_LocationSelector;
	int m_nRegionListStartIdx;
	
private:
	CReportConsolidationArray<CSalesHistoryRegionListItem> m_arrayRegionInfo;

private:
	CSSListCtrl m_listLocations;
	CImageList m_ImageList;
	CPluFilterArray m_FilterArray;
};

/**********************************************************************/
