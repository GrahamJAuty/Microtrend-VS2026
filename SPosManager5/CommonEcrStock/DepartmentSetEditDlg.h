#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentSetCSVArray.h"
/**********************************************************************/

class CDepartmentSetEditDlg : public CSSDialog
{
public:
	CDepartmentSetEditDlg( bool bSystem, CDepartmentSetCSVRecord& BatchRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDepartmentSetEditDlg)
	enum { IDD = IDD_DEPARTMENTSET_EDIT };
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDepartmentSetEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDepartmentSetEditDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void AddList();

private:
	CSSListTaggedSelectCtrl m_listDepartment;
	CButton m_checkBritannia;
	CDepartmentSetCSVRecord& m_DepartmentSetRecord;

private:
	bool m_bSystem;
	CDepartmentCSVArray* m_pDeptArray;
};

/**********************************************************************/
