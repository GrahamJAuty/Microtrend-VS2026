/**********************************************************************/
#pragma once
/**********************************************************************/

#include "SyssetEdit.h"
#include "sysset3.h"

/**********************************************************************/

class CSyssetDlg1 : public CSSDialog
{
public:
	CSyssetDlg1 ( CSysset* pSysset, const char* szRandom, const char* szTitle, const char* szGroup, const char* szText, const char* szComment1, const char* szComment2, int nCodeLen, CWnd* pParent );

	enum { IDD = IDD_SYSSET_DLG1 };
	CStatic	m_staticText;
	CStatic	m_staticRandom;
	CButton	m_staticGroup3;
	CButton	m_staticGroup2;
	CButton	m_staticGroup1;
	CStatic	m_staticComment1;
	CStatic	m_staticComment2;
	CUserEdit m_editUserPassword;
	CButton m_buttonOK;
	CString	m_strUserAdd1;
	CString	m_strUserAdd2;
	CString	m_strUserAdd3;
	CString	m_strUserAdd4;
	CString	m_strUserAdd5;
	CString	m_strUserName;
	CString	m_strComment1;
	CString	m_strComment2;
	CString	m_strCode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSecret();
	afx_msg void OnButtonRegisterOnline();
	DECLARE_MESSAGE_MAP()

public:
	bool m_bSecretMode;

private:
	CSysset* m_pSysset;
	CString m_strTitle;
	CString m_strRandom;
	CString m_strGroup;
	CString m_strText;
	int m_nCodeLen;
	CButton m_buttonRegisterOnline;
};

/**********************************************************************/
