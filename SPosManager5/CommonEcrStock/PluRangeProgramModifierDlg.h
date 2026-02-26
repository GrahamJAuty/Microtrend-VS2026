#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluRangeProgramModifierDlg : public CSSDialog
{
public:
	CPluRangeProgramModifierDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);  
	~CPluRangeProgramModifierDlg();

	//{{AFX_DATA(CPluRangeProgramModifierDlg)
	enum { IDD = IDD_PLU_RANGE_PROGRAM_MODIFIER };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	CButton	m_checkModifier[10];
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeProgramModifierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	static bool IsRelevant();

protected:
	//{{AFX_MSG(CPluRangeProgramModifierDlg)
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
	bool CanChangeModifier( CPluCSVRecord& PluRecord, int nMod );
	static bool CheckField( const char* sz );
	static bool CheckEnable( int nMod );
	static bool CheckEposText( int nMod );
	static bool CheckKeyText( int nMod );
	static bool CheckDeptNo( int nMod );
	static bool CheckTaxBand( int nMod );
	static bool CheckOPGroup( int nMod );
	static bool CheckAnalysisCategory( int nMod );
	static bool CheckPluType( int nMod );
	static bool CheckFirstMenu( int nMod );
	static bool CheckFirstQualifier( int nMod );
	static bool CheckFirstExtra( int nMod );

private:
	void AddDeptNo();
	void AddOPGroup();
	void AddTax();
	void AddAnalysisCategory();
	void AddPluType();
	void AddFirstMenu();
	void AddFirstQualifier();
	void AddFirstExtra();

private:
	void AddKbdListNames( CellTypeInfoTextBuffer& arrayTexts );
	void AddYesNoField( const CustomPluField& Field );
	void AddField( CCSV& csv );
	
public:
	CString m_strPluFrom;
	CString m_strPluTo;
	
private:
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl m_listSettings;

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;
	
private:
	CPluCSVRecord m_PluRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/
