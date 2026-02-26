#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluRangeStatusDlg : public CSSDialog
{
public:
	CPluRangeStatusDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangeStatusDlg)
	enum { IDD = IDD_PLU_RANGE_STATUS };
	CSSComboBox	m_comboStatusOld;
	CSSComboBox	m_comboStatusNew;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	int		m_nMethod;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeStatusDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int UpdateStatus( int nOldComboStatus, int nNewComboStatus );

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

