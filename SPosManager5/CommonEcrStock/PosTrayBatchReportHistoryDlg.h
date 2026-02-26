#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "DeptAndPluComboHelpers.h"
#include "PosTrayTask.h"
#include "PosTrayTaskEditHelpers.h"
#include "SalesHistoryReportPlu.h"
#include "StockReportTop100Dlg.h"
#include "TableReportHelpers.h"
/**********************************************************************/

class CPosTrayBatchReportHistoryDlg : public CSSDialog
{
public:
	CPosTrayBatchReportHistoryDlg( CPosTrayTask& BatchEmail, CWnd* pParent = NULL);
	~CPosTrayBatchReportHistoryDlg();

	//{{AFX_DATA(CPosTrayBatchReportHistoryDlg)
	enum { IDD = IDD_POSTRAY_BATCH_REPORT_HISTORY };

	CTabCtrl m_TabFilter;

	CStatic m_staticName;
	CEdit m_editName;

	CStatic m_staticDate;
	CStatic m_staticDate1;
	CStatic m_staticDate2;
	CStatic m_staticDate3;
	CStatic m_staticDate4;
	CStatic m_staticDate5;
	CSSComboBox	m_comboDates;
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboSession;
	CSSComboBox m_comboTimeSlice;
	CSSComboBox	m_comboPluTime;

	CStatic m_staticDatabase;
	CStatic m_staticDatabase1;
	CStatic m_staticDatabase2;
	CSSComboBoxEx	m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	CSSTabbedComboBox	m_comboTermDate;
	CStatic m_staticTable;
	CSSTabbedComboBox	m_comboTable;
	CButton	m_checkEposSales;
	CButton	m_checkManualSales;
	CButton	m_checkImportSales;

	CStatic m_staticFilter;
	CStatic m_staticFilter1;
	CStatic m_staticFilter2;
	CButton	m_checkSpecify;
	CButton	m_buttonBrowseFrom;
	CButton	m_buttonBrowseTo;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSComboBox	m_comboDeptFilterType;
	CSSComboBox	m_comboPluFilter;
	CButton m_buttonTaxBand;
	CSSTabbedComboBox	m_comboTaxBand;
	CButton m_buttonPriceLevel;
	CSSTabbedComboBox m_comboPriceLevel;
	CButton m_buttonModifier;
	CSSTabbedComboBox m_comboModifier;
	
	CStatic m_staticMisc;
	CButton m_checkGuest;
	CButton	m_checkByMonth;
	CButton	m_checkDeptSort;
	CButton	m_checkPercent;
	CButton	m_checkEstimatedProfit;
	CButton m_checkCustomer;
	CButton m_checkVIPSales;
	CButton m_buttonChart;
	CEdit m_editChart;

	CSSListTaggedSelectCtrl m_listReport;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayBatchReportHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

protected:
	//{{AFX_MSG(CPosTrayBatchReportHistoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectLocation();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();
	afx_msg void OnKillFocusPluFrom();
	afx_msg void OnKillFocusPluTo();
	afx_msg void OnTogglePluRange();
	afx_msg void OnSelectDepartment();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnButtonModifier();
	afx_msg void OnButtonTaxBand();
	afx_msg void OnButtonPriceLevel();
	afx_msg void OnButtonChart();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	
private:
	void SelectTabFilter();
	void FillTimeSliceCombo();
	
private:
	void FillReportList();
	
private:
	void LoadEmailBatchSettings();
	void SaveEmailBatchSettings();
			
private:
	CSessionCSVRecord m_ReportSession;
	
private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CLocationSelector m_LocationSelector;
	CDepartmentSelector m_DepartmentSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CTableReportHelpers m_TableReportHelpers;
	CPluFilterArray m_FilterArray;
	
private:
	CPosTrayTask& m_BatchEmail;
	CPosTrayTaskEditHelpers m_EditHelpers;
	CStringArray m_arrayReportEntityIDs;

private:
	int m_nTabPageFilter;
	bool m_bShowVIPTick;
};

/**********************************************************************/
#endif
/**********************************************************************/
