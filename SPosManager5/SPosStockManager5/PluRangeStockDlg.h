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

class CPluRangeStockDlg : public CSSDialog
{
public:
	CPluRangeStockDlg( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangeStockDlg)
	enum { IDD = IDD_PLU_RANGE_STOCK };
	CEdit	m_editDeptNo;
	CButton	m_checkCreateStock;
	CButton	m_checkPluNumbers;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	BOOL	m_bCreateStock;
	BOOL	m_bDepartment;
	BOOL	m_bDescription;
	BOOL	m_bPluNumbers;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeStockDlg)
	afx_msg void OnSelchangeComboDeptname();
	afx_msg void OnKillFocusDeptNumber();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnToggleCreateStock();
	afx_msg void OnStaticRange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateStock ( int& nStockCreated, int& nStockModified );

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

