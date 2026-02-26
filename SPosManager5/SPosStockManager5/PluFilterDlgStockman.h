#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluFilterDlgStockman : public CDialog
{
public:
	CPluFilterDlgStockman( CPluFilterArray& FilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluFilterDlgStockman)
	enum { IDD = IDD_PLU_FILTER_STOCKMAN };
	CStatic	m_staticStockCode;
	CEdit	m_editStockCode;
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSTabbedComboBox m_comboCategory;
	CString	m_strSearchText;
	CString	m_strStockCode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluFilterDlgStockman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetPluIdx() { return m_nPluIdx; }
	int GetRecordCount() { return m_PluFilterArray.GetSize(); }

private:
	void RunSearch( bool bDoSearch, bool bUpdateDisplay );
	void UpdateTitle();

private:
	CSSListCtrlVirtual m_listPlu;
	CPluFilterArray& m_PluFilterArray;
	int m_nPluIdx;

protected:
	//{{AFX_MSG(CPluFilterDlgStockman)
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDepartment();
	afx_msg void OnSelectCategory();
	afx_msg void OnSelectStockCode();
	afx_msg void OnSelectSearchText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDepartmentSelector m_DepartmentSelector;
	int m_nDepartmentFilter;
	int m_nCategoryFilter;
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/

