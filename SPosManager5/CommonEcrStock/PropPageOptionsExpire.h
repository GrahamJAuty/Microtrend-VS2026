#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsExpire : public CSSPropertyPage
{
public:
	CPropPageOptionsExpire();
	~CPropPageOptionsExpire();

	//{{AFX_DATA(CPropPageOptionsExpire)
	enum { IDD = IDD_PROPPAGE_OPTIONS_EXPIRE };
	CStatic	m_staticMonth5;
	CStatic	m_staticMonth6;
	CButton	m_checkExpire2;
	CButton	m_checkExpire3;
	CButton	m_checkExpire4;
	CButton	m_checkExpire5;
	CButton	m_checkExpire6;
	BOOL	m_bExpire1;
	BOOL	m_bExpire2;
	BOOL	m_bExpire3;
	BOOL	m_bExpire4;
	BOOL	m_bExpire5;
	BOOL	m_bExpire6;
	CString	m_strMonths1;
	CString	m_strMonths2;
	CString	m_strMonths3;
	CString	m_strMonths4;
	CString	m_strMonths5;
	CString	m_strMonths6;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsExpire)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageOptionsExpire)
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleAutoExpire();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetExpireEdit1();
	CEdit* GetExpireEdit2();
	CEdit* GetExpireEdit3();
	CEdit* GetExpireEdit4();
	CEdit* GetExpireEdit5();
	CEdit* GetExpireEdit6();
	
private:
	void SaveRecord();
	void GetRecordData();
};

/**********************************************************************/
#endif
/**********************************************************************/
