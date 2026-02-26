#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageOptionsEmail : public CSSPropertyPage
{
public:
	CPropPageOptionsEmail();
	~CPropPageOptionsEmail();

	//{{AFX_DATA(CPropPageOptionsEmail)
	enum { IDD = IDD_PROPPAGE_OPTIONS_EMAIL };
	CStatic m_staticServer;
	CStatic m_staticUsername;
	CStatic m_staticPassword;
	CStatic m_staticPort;
	CStatic m_staticTo1;
	CStatic m_staticTo2;
	CButton m_buttonMethod1;
	CButton m_buttonMethod2;
	CButton m_buttonMethod3;
	CButton m_buttonPassword;
	CEdit m_editServer;
	CEdit m_editUsername;
	CEdit m_editPassword;
	CButton m_checkAddressFrom;
	CEdit m_editAddressFrom;
	CButton m_checkReplyTo;
	CEdit m_editReplyTo;
	CSSComboBox m_comboEmail;
	CButton m_buttonEmail;
	CButton m_checkSSL;
	CButton m_buttonTest;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageOptionsEmail)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL
	afx_msg void OnButtonMethod1();
	afx_msg void OnButtonMethod2();
	afx_msg void OnButtonMethod3();
	afx_msg void OnButtonPassword();
	afx_msg void OnButtonEmailList();
	afx_msg void OnToggleFrom();
	afx_msg void OnToggleReplyTo();
	afx_msg void OnButtonTest();

protected:
	//{{AFX_MSG(CPropPageOptionsEmail)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void GetRecordData();
	void SelectMethod(int n);

private:
	CEdit* GetEditPort();
	void FillEmailCombo();
	void TestInternal();

private:
	CString m_strNewPassword;
	bool m_bGotOldPassword;
	int m_nMethod;
	int m_nPortNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
