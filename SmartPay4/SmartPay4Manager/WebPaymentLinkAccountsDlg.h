#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//**********************************************************************

class CWebPaymentLinkAccountsDlg : public CSSAutoShutdownDialog
{
public:
	CWebPaymentLinkAccountsDlg( CWnd* pParent);   // standard constructor

	enum { IDD = IDD_WEBPAYMENTLINKSQUID };
	CButton m_buttonImport2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnButtonImport();
	afx_msg void OnButtonImport2();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	int GetUnlinkedAccountsList (CArray<CSQLRowAccountList, CSQLRowAccountList>& arrayAccountList);
	bool LinkRequired(CSQLRowAccountFull& RowAccount);

private:
	CWnd* m_pParent;
};

//**********************************************************************
