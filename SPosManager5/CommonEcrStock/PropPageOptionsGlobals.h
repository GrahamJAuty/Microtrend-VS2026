#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsGlobals : public CPropertyPage
{
public:
	CPropPageOptionsGlobals();
	~CPropPageOptionsGlobals();

	//{{AFX_DATA(CPropPageOptionsGlobals)
	enum { IDD = IDD_PROPPAGE_OPTIONS_GLOBALS };
	CButton m_checkTax;
	CButton m_checkModifier;
	CButton m_checkAnalysis;
	CButton m_checkPaidIO;
	CButton m_checkPayment;
	CButton m_checkPaymentGroup;
	CButton m_checkDepartment;
	CButton m_checkDeptGroupReport;
	CButton m_checkDeptGroupConsol;
	CButton m_checkDeptGroupEpos;
	CButton	m_checkCategory;
	CButton m_checkCustomField;
	CButton	m_checkAllowance;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsGlobals)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnToggleGlobalDept();
	afx_msg void OnToggleGlobalPayment();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageOptionsGlobals)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void SetRecordControls();
};

/**********************************************************************/
#endif
/**********************************************************************/
