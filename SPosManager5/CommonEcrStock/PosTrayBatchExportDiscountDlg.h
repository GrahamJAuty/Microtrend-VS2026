#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DeptAndPluComboHelpers.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "PosTrayExportFieldDlg.h"
#include "PosTrayExportFieldNames.h"
#include "PosTrayTaskEditHelpers.h"
#include "TableReportHelpers.h"
/**********************************************************************/

class CPosTrayBatchExportDiscountDlg : public CSSDialog
{
public:
	CPosTrayBatchExportDiscountDlg( CPosTrayTask& BatchEmail, CWnd* pParent = NULL);
	~CPosTrayBatchExportDiscountDlg();

	//{{AFX_DATA(CPosTrayBatchExportDiscountDlg)
	enum { IDD = IDD_POSTRAY_BATCH_EXPORT_DISCOUNT };

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
	CSSComboBoxEx m_comboLocation;
	CSSTabbedComboBox m_comboTerminal;
	CStatic m_staticTable;
	CSSTabbedComboBox	m_comboTable;
	CButton	m_checkEposSales;
	CButton	m_checkManualSales;
	CButton	m_checkImportSales;

	CStatic m_staticFilter;
	CStatic m_staticFilter1;
	CStatic m_staticFilter2;
	CButton	m_checkSpecify;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSComboBox	m_comboPluFilter;
	CButton m_buttonModifier;
	CSSTabbedComboBox	m_comboModifier;
	CButton m_buttonTaxBand;
	CSSTabbedComboBox	m_comboTaxBand;
	CButton m_buttonPriceLevel;
	CSSTabbedComboBox m_comboPriceLevel;
	
	CStatic m_staticMisc;
	CButton m_checkGuest;
	CButton m_checkCustomer;
	CButton m_checkVIPSales;

	CStatic m_staticOffer;
	CStatic m_staticOffer1;
	CButton m_checkOffer[9];

	CSSListCtrlVirtual m_listFields;

	CButton m_buttonOK;
	CButton m_buttonCancel;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayBatchExportDiscountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

protected:
	//{{AFX_MSG(CPosTrayBatchExportDiscountDlg)
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
	afx_msg void OnButtonTaxBand();
	afx_msg void OnButtonPriceLevel();
	afx_msg void OnButtonModifier();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFieldAdd();
	afx_msg void OnButtonFieldChange();
	afx_msg void OnButtonFieldRemove();
	afx_msg void OnButtonFieldTidy();
	afx_msg void OnButtonFieldMoveUp();
	afx_msg void OnButtonFieldMoveDown();
	afx_msg long OnAddField( WPARAM wIndex, LPARAM lParam );
	//}}AFX_MSG
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	
private:
	void SelectTabFilter();

private:
	void FillTimeSliceCombo();
		
private:
	void LoadEmailBatchSettings();
	void SaveEmailBatchSettings();
	
private:
	bool PrepareLine( int nIndex );
	void CreateFieldDisplayLine( int nIndex, CPosTrayExportFieldName& Name );
	bool SelectExistingItem( int nFieldType );

public:
	void AddField( int nFieldType );
				
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
	CArray<int,int> m_arrayFieldTypes;
	int m_nTabPageFilter;

private:
	CString m_strDisplayLine;
	bool m_bShowVIPTick;

private:
	CPosTrayExportFieldDlg* m_pFieldsDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
