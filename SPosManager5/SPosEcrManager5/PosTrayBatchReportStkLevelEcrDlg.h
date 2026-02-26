#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\DeptAndPluComboHelpers.h"
#include "EcrmanSupplierSelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "PosTrayTask.h"
/**********************************************************************/

class CPosTrayBatchReportStkLevelEcrDlg : public CSSDialog
{
public:
	CPosTrayBatchReportStkLevelEcrDlg( CPosTrayTask& BatchEmail, CWnd* pParent = NULL);
	~CPosTrayBatchReportStkLevelEcrDlg();

	//{{AFX_DATA(CPosTrayBatchReportStkLevelEcrDlg)
	enum { IDD = IDD_POSTRAY_BATCH_REPORT_STKLEVEL_ECR };

	CTabCtrl m_TabFilter;

	CStatic m_staticName;
	CEdit m_editName;

	CStatic m_staticDate;
	CStatic m_staticDate1;
	CSSComboBox	m_comboDates;
	
	CStatic m_staticDatabase;
	CStatic m_staticDatabase1;
	CSSComboBoxEx	m_comboLocation;
	
	CStatic m_staticFilter;
	CStatic m_staticFilter1;
	CButton	m_checkSpecify;
	CButton	m_buttonBrowseFrom;
	CButton	m_buttonBrowseTo;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CStatic m_staticFilter2;
	CEdit m_editSupplier;
	CSSComboBox m_comboSupplier;
	CButton m_checkZeroSkip;
	
	CStatic m_staticSort;
	CButton	m_checkDeptSort;
	CButton	m_checkSupplierSort;
	
	CSSListTaggedSelectCtrl m_listReport;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayBatchReportStkLevelEcrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

protected:
	//{{AFX_MSG(CPosTrayBatchReportStkLevelEcrDlg)
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
	afx_msg void OnSelectSupplier();
	afx_msg void OnKillFocusSupplier();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	
private:
	void SelectTabFilter();
	
private:
	void FillReportList();
	void AddReportToList( const char* szName, int nEntityID );
	
private:
	void LoadEmailBatchSettings();
	void SaveEmailBatchSettings();
				
private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CLocationSelector m_LocationSelector;
	CDepartmentSelector m_DepartmentSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CPluFilterArray m_FilterArray;
	CEcrmanSupplierSelector m_EcrmanSupplierSelector;
	
private:
	CPosTrayTask& m_BatchEmail;
	CStringArray m_arrayReportEntityIDs;

private:
	int m_nTabPageFilter;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
