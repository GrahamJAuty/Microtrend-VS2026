#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************

class CHotlistTraceDlg : public CSSAutoShutdownDialog
{
public:
	CHotlistTraceDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_HOTLISTTRACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnNMDblclkListTrace(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CString m_strRequestedCardNo;

private:
	CString CreateDisplayLine ( int nLine, const char* szCardNo, const char* szDate = "", const char* szTime = "" );

private:
	CSQLRowAccountFull& m_RowAccount;
	CSSListCtrl	m_list;
};

//*******************************************************************
