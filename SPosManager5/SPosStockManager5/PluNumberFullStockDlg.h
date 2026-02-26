#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "PluAddOrCopyInfo.h"
/**********************************************************************/

class CPluNumberFullStockDlg : public CSSDialog
{
public:
	CPluNumberFullStockDlg( bool bFullEdit, CPluAddOrCopyInfo& PluInfo, CWnd* pParent = NULL);
	
	//{{AFX_DATA(CPluNumberFullStockDlg)
	CStatic m_staticPluNumber;
	CStatic m_staticDepartment;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CStatic m_staticStock;
	CEdit m_editStockCode;
	CButton	m_buttonBrowse;
	CStatic m_staticSupplier;
	CSSComboBox	m_comboSupplier;
	CButton m_buttonSupplier;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluNumberFullStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluNumberFullStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnKillFocusStockCode();
	afx_msg void OnButtonBrowse();
	afx_msg void OnSelectSupplier();
	afx_msg void OnButtonSupplier();
	//}}AFX_MSG
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluNumber();
	
private:
	void UpdateStockCode();
	void FillSupplierCombo();
	void UpdateSupplierEnables();

private:
	CDepartmentSelector m_DepartmentSelector;
	
private:
	CPluAddOrCopyInfo& m_PluInfo;
	bool m_bFullEdit;
	bool m_bShowSupplier;
};

/**********************************************************************/
#endif				
/**********************************************************************/
#endif				
/**********************************************************************/
