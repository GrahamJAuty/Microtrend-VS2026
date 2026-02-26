#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPagePMSOptionsAddress : public CPropertyPage
{
public:
	CPropPagePMSOptionsAddress();
	~CPropPagePMSOptionsAddress();

	//{{AFX_DATA(CPropPagePMSOptionsAddress)
	enum { IDD = IDD_PROPPAGE_PMS_OPTIONS_ADDRESS };
	CString	m_strAddress1;
	CString	m_strAddress2;
	CString	m_strAddress3;
	CString	m_strName;
	CString	m_strPhone;
	CString	m_strPostcode;
	CString	m_strEmail;
	CString	m_strFax;
	CStatic m_staticTaxNo;
	CString	m_strTaxNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPagePMSOptionsAddress)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	virtual BOOL OnKillActive();

private:
	void SaveRecord();
	
protected:
	//{{AFX_MSG(CPropPagePMSOptionsAddress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
