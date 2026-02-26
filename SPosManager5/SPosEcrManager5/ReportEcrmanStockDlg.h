#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\DeptAndPluComboHelpers.h"
#include "EcrmanSupplierSelector.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\CommonEcrStock\PresetReportBase.h"
#include "..\CommonEcrTray\ReportPluStockLevel.h"
/**********************************************************************/

class CReportEcrmanStockDlg : public CSSDialog
{
public:
	CReportEcrmanStockDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent = NULL);
	~CReportEcrmanStockDlg();

	//{{AFX_DATA(CReportEcrmanStockDlg)
	enum { IDD = IDD_REPORT_ECRMAN_STOCK };

	CSSComboBox		m_comboDates;
	CDateTimeCtrl	m_DatePickerFrom;
	CStatic			m_staticTo;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_TimePicker;

	CButton			m_checkSpecify;
	CButton			m_buttonBrowseTo;
	CButton			m_buttonBrowseFrom;
	CEdit			m_editDeptNo;
	CSSComboBox		m_comboDeptName;
	CEdit			m_editSupplier;
	CSSComboBox		m_comboSupplier;
	CButton			m_checkZeroSkip;
	
	CSSComboBoxEx	m_comboDatabase;
	
	CStatic			m_staticPreset;
	CButton			m_buttonPreset;
	CSSComboBox		m_comboPreset;
	CSSComboBox		m_comboReport;
	CButton			m_checkDeptSort;
	CButton			m_checkSupplierSort;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReportEcrmanStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportEcrmanStockDlg)
	virtual BOOL OnInitDialog();

	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnTogglePluRange();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnButtonBrowseTo();
	afx_msg void OnKillFocusPluFrom();
	afx_msg void OnKillFocusPluTo();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusSupplier();
	afx_msg void OnSelectSupplier();

	afx_msg void OnSelectLocation();
	
	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	afx_msg void OnSelectReport();
	
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddToReportCombo( const char* szLabel, int nType );
	void FillDepartmentCombo();
	void BuildZeroDateMap( int nDbIdx, COleDateTime& dateFrom, COleDateTime& timeFrom );

public:
	void CreatePreset( CPresetReportInfo& base );

private:
	void FillPresetCombo( int nSelection );

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

private:
	CString m_strPluNoFrom;
	CString m_strPluNoTo;

private:
	CArray<CPluStockLevelBuffer,CPluStockLevelBuffer> m_StockLevelBuffer;
	CPluFilterArray m_FilterArray;
	CEcrmanSupplierSelector m_EcrmanSupplierSelector;
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;
	CDepartmentSelector m_DepartmentSelector;
	int m_nDepartmentFilter;
	int m_nSuppNo;

private:
	int m_nLastDbIdx;
	COleDateTime m_lastDateFrom;
	COleDateTime m_lastTimeFrom;

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bAutoRunPreset;
};

/**********************************************************************/
#endif
/**********************************************************************/

