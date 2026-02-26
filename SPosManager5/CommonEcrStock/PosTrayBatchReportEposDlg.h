#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DeptAndPluComboHelpers.h"
#include "DepartmentSelector.h"
#include "EposReports.h"
#include "EposSelectArray.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "PosTrayTask.h"
#include "PosTrayEposConsolParamsArray.h"
#include "ServerSelectionList.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CPosTrayBatchReportEposDlg : public CSSDialog
{
public:
	CPosTrayBatchReportEposDlg( CPosTrayTask& BatchEmail, bool bModifiedBatch, CWnd* pParent = NULL);
	~CPosTrayBatchReportEposDlg();

	//{{AFX_DATA(CPosTrayBatchReportEposDlg)
	enum { IDD = IDD_POSTRAY_BATCH_REPORT_EPOS };

	CTabCtrl m_TabFilter;

	CStatic m_staticName;
	CEdit m_editName;

	CStatic m_staticDate;
	CStatic m_staticDate1;
	CStatic m_staticDate2;
	CStatic m_staticDate3;
	CSSComboBox	m_comboDates;
	CSSComboBox	m_comboSession;
	CSSComboBox	m_comboPluTime;

	CStatic m_staticDatabase;
	CStatic m_staticDatabase1;
	CStatic m_staticDatabase2;
	CSSComboBoxEx m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	CSSTabbedComboBox	m_comboTermDate;
	CButton	m_checkReportCombined;
	CButton	m_checkReportTerminal;
	CButton	m_checkReportLocation;
	CButton	m_checkReportDatabase;
	
	CStatic m_staticPMS;
	CButton m_radioPMSConsolidate1;
	CButton m_radioPMSConsolidate2;
	CButton m_radioPMSConsolidate3;
	CButton m_checkPending;
	CButton m_checkActive;
	CButton m_checkComplete;
	CButton m_checkCancelled;

	CStatic m_staticLocation;
	CButton m_radioConsolDefault;
	CButton m_radioConsolSaved;
	CButton m_radioConsolSystem;
	CButton m_checkOverride;
	
	CStatic m_staticFilter;
	CStatic m_staticFilter1;
	CStatic m_staticFilter2;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSComboBox	m_comboPluFilter;
	
	CStatic m_staticMisc;
	CButton	m_checkItemServer;
	CButton	m_checkReportTime;

	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayBatchReportEposDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	virtual void OnOK();

protected:
	//{{AFX_MSG(CPosTrayBatchReportEposDlg)

	virtual BOOL OnInitDialog();
	afx_msg void OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioPMSConsolidate1();
	afx_msg void OnRadioPMSConsolidate2();
	afx_msg void OnRadioPMSConsolidate3();
	afx_msg void OnSelectLocation();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDeptName();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
	
private:
	void SelectTabFilter();
	int GetSelectedReportType();
	void LoadEmailBatchSettings( CPosTrayTask& batch, bool bModifiedBatch );
	void SaveEmailBatchSettings( CPosTrayTask& batch );
	void HandlePMSRadioConsolidate( int n );
	
private:
	CSSListTaggedSelectCtrl m_listReport;
	CDepartmentSelector m_DepartmentSelector;
	CLocationSelector m_LocationSelector;

private:
	CPosTrayTask& m_BatchEmail;
	bool m_bModifiedBatch;

private:
	CWordArray m_arrayReportTypes;
	int m_nTabPageFilter;	

private:
	int m_nPMSConsolidationMode;

private:
	CEposReportSelect m_EposReportSelect;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CPosTrayEposConsolParamsArray m_ParamsArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
