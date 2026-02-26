#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/

class CPropPagePosTrayOptionsFTP : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsFTP();
	~CPropPagePosTrayOptionsFTP();

	//{{AFX_DATA(CPropPagePosTrayOptionsFTP)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_FTP };
	//}}AFX_DATA
	CEdit m_editServer;
	CEdit m_editUserName;
	CEdit m_editPassword;
	CButton m_buttonPassword;
	CButton m_checkUnixMode;
	
	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsFTP)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButtonPassword();
		
protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsFTP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void GetRecordData();

private:
	CString m_strNewPassword;
	bool m_bGotOldPassword;
};

/**********************************************************************/
#endif
/**********************************************************************/
