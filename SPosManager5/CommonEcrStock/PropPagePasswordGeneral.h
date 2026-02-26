#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "Password.h"
/**********************************************************************/

class CPropPagePasswordGeneral : public CPropertyPage
{
public:
	CPropPagePasswordGeneral();
	~CPropPagePasswordGeneral();

	//{{AFX_DATA(CPropPagePasswordGeneral)
	enum { IDD = IDD_PROPPAGE_PASSWORD_GENERAL };
	CString	m_strPassword;
	CString	m_strUserName;
	CButton m_checkLocation;
	CSSComboBoxEx m_comboLocation;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePasswordGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnToggleLocation();

protected:
	//{{AFX_MSG(CPropPagePasswordGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetPasswordRecord ( CPasswordCSVRecord* pPasswordRecord ) { m_pPasswordRecord = pPasswordRecord; }
	void Refresh();
	bool UpdateRecord();

private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();
	
private:
	CLocationSelector m_LocationSelector;
	CPasswordCSVRecord* m_pPasswordRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/
