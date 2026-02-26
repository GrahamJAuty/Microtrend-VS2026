#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluRangeProfitDlg : public CSSDialog
{
public:
	CPluRangeProfitDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangeProfitDlg)
	enum { IDD = IDD_PLU_RANGE_PROFIT };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	int		m_nMethod;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeProfitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeProfitDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioGpPercent();
	afx_msg void OnRadioGpFixed();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int UpdateProfit();

private:
	CEdit* GetEditGPPercent();
	CEdit* GetEditGPFixed();
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

public:
	CString m_strPluFrom;
	CString m_strPluTo;

private:
	CDepartmentSelector m_DepartmentSelector;
	CPluFilterArray& m_FilterArray;

	double m_dGPFixed;
	double m_dGPPercent;
};

/**********************************************************************/
#endif
/**********************************************************************/
