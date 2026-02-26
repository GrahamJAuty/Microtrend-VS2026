#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "EcrmanSupplierSelector.h"
#include "PluAddOrCopyInfo.h"
/**********************************************************************/

class CPluNumberEcrStockDlg : public CSSDialog
{
public:
	CPluNumberEcrStockDlg( CPluAddOrCopyInfo& PluInfo, CWnd* pParent = NULL);
	
	INT_PTR DoModalWrapper();
	
	//{{AFX_DATA(CPluNumberEcrStockDlg)
	enum { IDD = IDD_PLU_NUMBER_ECRSTOCK };
	CStatic m_staticPlu;
	CStatic m_staticPluNumber;
	CStatic m_staticDepartment;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CStatic m_staticStock;
	CStatic m_staticSupplier;
	CEdit m_editSupplier;
	CSSComboBox	m_comboSupplier;
	CButton m_buttonSupplier;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluNumberEcrStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluNumberEcrStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnKillFocusSupplier();
	afx_msg void OnSelectSupplier();
	afx_msg void OnButtonSupplier();
	//}}AFX_MSG
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluNumber();
	
private:
	bool m_bShowSupplier;
	CPluAddOrCopyInfo& m_PluInfo;

private:
	CEcrmanSupplierSelector m_EcrmanSupplierSelector;
	CDepartmentSelector m_DepartmentSelector;
};

/**********************************************************************/
#endif				
/**********************************************************************/
#endif				
/**********************************************************************/
