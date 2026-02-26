#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
/**********************************************************************/

class CPluDeleteRangeSetDlg : public CSSDialog
{
public:
	CPluDeleteRangeSetDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluDeleteRangeSetDlg)
	enum { IDD = IDD_PLU_DELETE_RANGE_SET };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	int		m_nDelete;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDeleteRangeSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluDeleteRangeSetDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int UpdateProfit();

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

public:
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CDepartmentSelector m_DepartmentSelector;

public:
	int m_nDepartmentFilter;
};

/**********************************************************************/
#endif		
/**********************************************************************/
