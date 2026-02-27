#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRowExternalAccount.h"
//$$******************************************************************

class CEditRecordPropertyPageExternal : public CSSAutoShutdownPropertyPage
{
public:
	CEditRecordPropertyPageExternal(CSQLRowAccountFull& Row, CSQLRowExternalAccount& RowExternal);
	virtual ~CEditRecordPropertyPageExternal();
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_EDITRECORD_EXTERNAL };
	CSSListCtrlVirtual m_list;
	CEdit m_editUserName;
	CEdit m_editPassword;
	CButton m_buttonPassword;
	CButton m_buttonCheck;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPassword();
	afx_msg void OnButtonCheckPassword();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnQueryCancel();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	
private:
	CSQLRowAccountFull& m_SQLRowAccount;
	CSQLRowExternalAccount& m_SQLRowExternal;
	CSQLRowExternalAccount m_SQLRowExternalOld;
	CString m_strNewPassword;
};

//$$******************************************************************
