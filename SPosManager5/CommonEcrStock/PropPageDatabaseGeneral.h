#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/

class CPropPageDatabaseGeneral : public CPropertyPage
{
public:
	CPropPageDatabaseGeneral();
	~CPropPageDatabaseGeneral();

	//{{AFX_DATA(CPropPageDatabaseGeneral)
	enum { IDD = IDD_PROPPAGE_DATABASE_GENERAL };
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

	//{{AFX_VIRTUAL(CPropPageDatabaseGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageDatabaseGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDatabaseBuffer( CDatabaseCSVArray* pBuffer ) { m_pDatabaseBuffer = pBuffer; }

private:
	void GetRecordData();
	void SaveRecord();

private:
	CDatabaseCSVArray* m_pDatabaseBuffer;
};

/**********************************************************************/
#endif // __PROPPAGEDATABASEGENERAL_H__
/**********************************************************************/
