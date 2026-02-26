#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStockTray\DepartmentSelector.h"
#include "..\CommonEcrStockTray\SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#include "..\SPosEcrManager5\EcrmanSupplierSelector.h"
/**********************************************************************/

class CPluDefineFilterDlg : public CSSDialog
{
public:
	CPluDefineFilterDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   
	~CPluDefineFilterDlg();

	void SetExternalTitle(const char* sz) { m_strExternalTitle = sz; }

	//{{AFX_DATA(CPluDefineFilterDlg)
	enum { IDD = IDD_PLU_DEFINE_FILTER };

	CButton m_checkMatchCase;
	/*****/
	CStatic	m_staticStockGroup;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	CStatic	m_staticSupplier;
	CEdit	m_editSupplier;
	CSSComboBox	m_comboSupplier;
	CStatic m_staticStockCode;
	CEdit	m_editStockCode;
	/*****/
	CButton	m_checkSpecify;
	CButton	m_buttonBrowseTo;
	CButton	m_buttonBrowseFrom;
	CSSComboBoxEx m_comboKeyboardLoc;
	CComboBox m_comboKeyboardNum;
	/*****/
	CButton	m_checkActive;
	CButton	m_checkInactive;
	CButton	m_checkNonStock;
	CButton m_checkUnlinked;
	CButton m_checkLinked;
	CButton m_checkRecipe;
	/*****/
	CString	m_strSearchText;
	int	m_nSearchType;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDefineFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	void InitStockFields();
	void SaveStockFields();
	void KillFocusSupplier();
	void SelectSupplier();
	
protected:
	//{{AFX_MSG(CPluDefineFilterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDepartment();
	afx_msg void OnSelectSupplier();
	afx_msg void OnKillFocusSupplier();
	afx_msg void OnSelectKeyboardLoc();
	/*****/
	afx_msg void OnTogglePluRange();
	afx_msg void OnKillFocusPluFrom();
	afx_msg void OnButtonBrowseFrom();
	afx_msg void OnKillFocusPluTo();
	afx_msg void OnButtonBrowseTo();
	/*****/
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_PluFilterArray;
	CKeyboardFilterSelector m_KeyboardFilterSelector;
	CString m_strExternalTitle;

#ifndef STOCKMAN_SYSTEM
	CEcrmanSupplierSelector m_EcrmanSupplierSelector;
#endif
};

/**********************************************************************/
#endif
/**********************************************************************/

