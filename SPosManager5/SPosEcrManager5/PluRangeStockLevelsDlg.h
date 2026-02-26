#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluRangeStockLevelsDlg : public CSSDialog
{
public:
	CPluRangeStockLevelsDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangeStockLevelsDlg)
	enum { IDD = IDD_PLU_RANGE_STOCKLEVELS };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeStockLevelsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeStockLevelsDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

public:
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;
};

/**********************************************************************/
#endif			
/**********************************************************************/
