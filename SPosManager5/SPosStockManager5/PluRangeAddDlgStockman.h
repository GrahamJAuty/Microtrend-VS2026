#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
/**********************************************************************/
#include "PluAddOrCopyInfo.h"
/**********************************************************************/

class CPluRangeAddDlgStockman : public CSSDialog
{
public:
	CPluRangeAddDlgStockman( CPluAddOrCopyInfo& PluInfo, CWnd* pParent = NULL);  

	//{{AFX_DATA(CPluRangeAddDlgStockman)
	enum { IDD = IDD_PLU_RANGE_ADD_STOCKMAN };
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CButton	m_buttonSupplier;
	CSSComboBox	m_comboSupplier;
	CButton	m_checkCreate;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeAddDlgStockman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeAddDlgStockman)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDept();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnToggleCreateStock();
	afx_msg void OnButtonSupplier();
	afx_msg void OnSelectSupplier();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillSupplierCombo();

private:
	CDepartmentSelector m_DepartmentSelector;

private:
	CPluAddOrCopyInfo& m_PluInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
