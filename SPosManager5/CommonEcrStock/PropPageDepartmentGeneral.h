#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "GroupSelector.h"
/**********************************************************************/

class CPropPageDepartmentGeneral : public CSSPropertyPage
{
public:
	CPropPageDepartmentGeneral();
	~CPropPageDepartmentGeneral();

	//{{AFX_DATA(CPropPageDepartmentGeneral)
	enum { IDD = IDD_PROPPAGE_DEPARTMENT_GENERAL };
	
	CEdit	m_editEposText;
	CEdit	m_editGroupEpos;
	CSSComboBox	m_comboGroupEpos;

	CEdit	m_editReportText;
	CEdit	m_editGroupReport;
	CSSComboBox	m_comboGroupReport;
	CEdit	m_editGroupConsol;
	CSSComboBox	m_comboGroupConsol;
	
	CButton m_checkEnable;
	CButton m_checkIDraught;
	CStatic m_staticSptBook;
	CButton m_checkSptBook;
	
	CString	m_strEposText;
	CString	m_strReportText;

	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageDepartmentGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageDepartmentGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectGroupEpos();
	afx_msg void OnKillFocusGroupNoEpos();
	afx_msg void OnSelectGroupReport();
	afx_msg void OnKillFocusGroupNoReport();
	afx_msg void OnSelectGroupConsol();
	afx_msg void OnKillFocusGroupNoConsol();
	afx_msg void OnButtonSetEpos();
	afx_msg void OnButtonSetReport();
	afx_msg void OnButtonSetConsol();
	afx_msg void OnTickSportsBooker();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDepartmentRecord ( CDepartmentCSVRecord* pDeptRecord ) { m_pDeptRecord = pDeptRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	void UpdateConsolGroupFromReportGroup();
	void LinkReportGroupToConsolGroup();
	
private:
	CDepartmentCSVRecord* m_pDeptRecord;

private:
	CGroupSelector m_GroupSelectorEpos;
	CGroupSelector m_GroupSelectorReport;
	CGroupSelector m_GroupSelectorConsol;
};

/**********************************************************************/
#endif
/**********************************************************************/

