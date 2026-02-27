#pragma once
//****************************************************************************
#include "Cardlink.h"
#include "Defines.h"
#include "UserIDRecord.h"
//****************************************************************************

class CGetAdminDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CGetAdminDlg)

public:
	CGetAdminDlg( CUserIDRecord* pRecord, bool bServerOn, CWnd* pParent); 
	
	enum { IDD = IDD_GETADMINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTopup();
	afx_msg void OnButtonSetup();
	afx_msg void OnButtonEmptyCash();

	DECLARE_MESSAGE_MAP()

public:
	int GetClosingState() { return m_nClosingState; }

private:
	bool CheckPassword();

private:
	CSSColourButton m_buttonCancel;
	CSSColourButton m_buttonTopup;
	CSSColourButton m_buttonAdmin;
	CSSColourButton m_buttonEmptyCash;

private:
	CUserIDRecord* m_pRecord = NULL;
	bool m_bOnline = FALSE;
	int m_nClosingState = nGETADMIN_CANCEL;
};

//****************************************************************************
