#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRecordSetPurchaseControlStamp.h"
//$$******************************************************************

class CPurchaseControlStampDeleteDlg : public CSSAutoShutdownDialog
{
public:
	CPurchaseControlStampDeleteDlg(bool bAllAccounts, __int64 nAccountNo, int nRuleID, CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	enum { IDD = IDD_PURCHASECONTROLSTAMP_DELETE };

public:
	static void AutoDeleteExpiredStamps();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	CButton m_checkAction1;
	CButton m_checkAction2;

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void DeleteExpiredStamps();
	void DeletePreviousEditionStamps();
	static void DeleteRecords(CSQLRecordSetPurchaseControlStamp& RecordSet, int nAppNo, int& nRows, int& nStamps);
	CString GetAccountFilter();

private:
	bool m_bAllAccounts;
	__int64 m_nUserID;
	int m_nRuleID;

private:
	CString m_strResults;
};

//$$******************************************************************

