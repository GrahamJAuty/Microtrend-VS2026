#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "DepartmentSelector.h"
/**********************************************************************/

struct CPluRangeStockpointInfo
{
public:
	CPluRangeStockpointInfo()
	{
		m_strSpName = "";
		m_nSpNo = 1;
	}

public:
	CString m_strSpName;
	int m_nSpNo;
};

/**********************************************************************/

class CPluRangeProgramDlg : public CSSDialog
{
public:
	CPluRangeProgramDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);  
	~CPluRangeProgramDlg();

	//{{AFX_DATA(CPluRangeProgramDlg)
	enum { IDD = IDD_PLU_RANGE_PROGRAM };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeProgramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	static bool IsRelevant();

protected:
	//{{AFX_MSG(CPluRangeProgramDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCellButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

private:
	void AddList();
	int UpdateProgram();
	
private:
	static bool CheckField( const char* sz );

private:
	void AddDeptNo();
	void AddPluType();
	void AddExternalProcess();
	void AddAnalysisCategory();
	void AddAllergen( const CustomPluField& Field );
	void AddPriceType();
	void AddAgeVerificationType();
	void AddFreeText();
	void AddStockpoint();
	void AddOPGroup();
	void AddPriority();
	void AddTax();
	void AddFirstMenu();
	void AddFirstQualifier();
	void AddFirstExtra();

#ifndef STOCKMAN_SYSTEM
	void AddEcrmanSupplier();
#endif

	void AddYesNoField( const CustomPluField& Field );
	void AddField( CCSV& csv );

	void UpdatePluProfit( CPluCSVRecord& PluRecord );
	void UpdateAllergen( CPluCSVRecord& PluRecord, int nAllergen, int nCellInt );

private:
	void AddKbdListNames( CellTypeInfoTextBuffer& arrayTexts );

public:
	CString m_strPluFrom;
	CString m_strPluTo;
	
private:
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl m_listSettings;

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;

	CArray<CPluRangeStockpointInfo,CPluRangeStockpointInfo> m_arrayStockpointInfo;
	CellTypeInfoTextBuffer m_arrayAllergenTexts;

private:
	CPluCSVRecord m_PluRecord;

private:
	CArray<CustomPluField> m_arrayOrderPrinterFields;
	CArray<CustomPluField> m_arrayAllergenFields;
};

/**********************************************************************/
#endif
/**********************************************************************/
