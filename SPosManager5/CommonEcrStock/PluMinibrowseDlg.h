#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSelector.h"
//include "MySSListCtrl.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/
#define ADDPLU_MESSAGE 9999
/**********************************************************************/

class CPluMiniBrowseDlg : public CDialog
{
public:
	CPluMiniBrowseDlg( int nNodeType, CPluFilterArray& FilterArray, CWnd* pParent = NULL);

	//{{AFX_DATA(CPluMiniBrowseDlg)
	enum { IDD = IDD_PLU_MINIBROWSE };
	CEdit	m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CString	m_strSearchText;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluMiniBrowseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetPluIdx() { return m_nPluIdx; }
	int GetRecordCount() { return m_PluFilterArray.GetSize(); }

private:
	void RunSearch( bool bDoSearch, bool bUpdateDisplay );

private:
	CSSListCtrlVirtual m_listPlu;
	int m_nPluIdx;
	CPluFilterArray& m_PluFilterArray;
	
protected:
	//{{AFX_MSG(CPluMiniBrowseDlg)
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDepartment();
	afx_msg void OnSelectSearchText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDepartmentSelector m_DepartmentSelector;
	int m_nDepartmentFilter;

private:
	int m_nNodeType;
	CWnd* m_pMessageTarget;
};

/**********************************************************************/
