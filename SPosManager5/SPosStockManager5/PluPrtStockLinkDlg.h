#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluPrtStockLinkDlg : public CSSDialog
{
public:
	CPluPrtStockLinkDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPluPrtStockLinkDlg)
	enum { IDD = IDD_PLU_PRT_STOCKLINK };
	CButton	m_checkSeparate;
	CEdit	m_editDeptNo;
	CButton	m_checkDeptSort;
	CSSComboBox	m_comboDeptName;
	BOOL	m_bSortByDept;
	BOOL	m_bLinked;
	BOOL	m_bRecipe;
	BOOL	m_bUnlinked;
	BOOL	m_bSeparate;
	BOOL	m_bZeroPriced;
	BOOL	m_bProfit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluPrtStockLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluPrtStockLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusEditDeptNumber();
	afx_msg void OnKillFocusPluNo();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	afx_msg void OnStaticRange();
	afx_msg void OnToggleRecipeProfit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetCSVLine();

private:
	void SetDefaults();

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

public:
	int m_nDepartmentFilter;
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;
	CString m_strFieldSelect;
};

/**********************************************************************/

class CCSVPluPrtStockLinkKey : public CCSV
{
public:
	CCSVPluPrtStockLinkKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	bool PrintUnlinked() { return GetBool(0); }
	bool PrintLinked() { return GetBool(1); }
	bool PrintRecipe() { return GetBool(2); }
	//FIDDLE - REMOVED PRINT INHIBITED
	bool PrintZeroPriced() { return GetBool(4); }
	bool PrintSeparate() { return GetBool(5); }
	bool PrintProfit() { return GetBool(6); }
};

/**********************************************************************/
#endif				
/**********************************************************************/