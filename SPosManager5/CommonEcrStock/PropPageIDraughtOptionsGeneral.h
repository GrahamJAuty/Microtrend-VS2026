#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
class CPropPageIDraughtOptionsGeneral : public CSSPropertyPage
{
public:
	CPropPageIDraughtOptionsGeneral();
	~CPropPageIDraughtOptionsGeneral();

	//{{AFX_DATA(CPropPageIDraughtOptionsGeneral)
	enum { IDD = IDD_PROPPAGE_IDRAUGHT_OPTIONS_GENERAL };
	CEdit m_editPassword;
	CButton m_buttonPassword;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageIDraughtOptionsGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageIDraughtOptionsGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPassword();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void GetRecordData();

private:
	CString m_strServer;
	CString m_strUserName;
	CString m_strNewPassword;
	bool m_bGotOldPassword;
	int m_nExportDelayTime;
	int m_nConnectRetryTime;
};

/**********************************************************************/
#endif
/**********************************************************************/
