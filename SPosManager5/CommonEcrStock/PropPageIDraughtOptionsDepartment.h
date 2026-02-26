#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Consolidation.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPropPageIDraughtOptionsDepartment : public CSSPropertyPage
{
public:
	CPropPageIDraughtOptionsDepartment();
	~CPropPageIDraughtOptionsDepartment();

	//{{AFX_DATA(CPropPageIDraughtOptionsDepartment)
	enum { IDD = IDD_PROPPAGE_IDRAUGHT_OPTIONS_DEPARTMENT };
	//}}AFX_DATA
		
public:
	//{{AFX_VIRTUAL(CPropPageIDraughtOptionsDepartment)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnToggleLines();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnDoubleClickListSelected(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickListAvailable(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:
	//{{AFX_MSG(CPropPageIDraughtOptionsDepartment)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelectDatabase();
	DECLARE_MESSAGE_MAP()

private:
	void LoadDatabaseDepartments( int nDbIdx );
	void SaveDatabaseDepartments( int nDbIdx );
	void MoveDepartment( int nSel, CSSListCtrl& listSource, CSSListCtrl& listDest, CReportConsolidationArray<CSortedIntItem>& arraySource, CReportConsolidationArray<CSortedIntItem>& arrayDept );
	void AddList();

private:
	void SaveRecord();

private:
	CSSListCtrl m_listSelected;
	CSSListCtrl m_listAvailable;
	CReportConsolidationArray<CSortedIntItem> m_arraySelected;
	CReportConsolidationArray<CSortedIntItem> m_arrayAvailable;

private:
	CSSComboBoxEx m_comboDatabase;
	CLocationSelector m_LocationSelector;
	int m_nCurrentDbIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
