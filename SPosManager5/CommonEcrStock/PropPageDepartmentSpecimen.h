#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPropPageDepartmentSpecimen : public CSSPropertyPage
{
public:
	CPropPageDepartmentSpecimen();
	~CPropPageDepartmentSpecimen();

	//{{AFX_DATA(CPropPageDepartmentSpecimen)
	enum { IDD = IDD_PROPPAGE_DEPARTMENT_SPECIMEN };
	
	CString m_strPluNumber;
	CSSComboBox m_comboModifier;
	CEdit	m_editDescription;
	CButton m_checkStockTemplate;
	CButton m_checkStockDescription;
	CButton m_checkStockCategory;
	CButton m_checkStockQty;
	
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageDepartmentSpecimen)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageDepartmentSpecimen)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnButtonBrowse();
	afx_msg void OnToggleStock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluNo();

public:
	void SetDepartmentRecord ( CDepartmentCSVRecord* pDeptRecord ) { m_pDeptRecord = pDeptRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	void LoadSQLPluNo( __int64 nEposPluNo );

private:
	CPluFilterArray m_PluFilterArray;
	__int64 m_nBasePluNo;

private:
	CDepartmentCSVRecord* m_pDeptRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/

