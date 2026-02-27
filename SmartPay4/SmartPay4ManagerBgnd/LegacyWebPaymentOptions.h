#pragma once
//$$******************************************************************

class CLegacyWebPaymentOptions
{
public:
	CLegacyWebPaymentOptions();
	void Read();

public:
	bool m_bPromptBeforeAccess = TRUE;
	int m_nPaymentDayFlags = 0x3E;
	CString m_strPaymentStart = "00:00";
	CString m_strPaymentEnd = "23:59";
	CString m_strDateLastPayment = "";
	CString m_strTimeLastPayment = "";
	bool m_bExclude1Reqd = FALSE;
	CString m_strExclude1Start = "00:00";
	CString m_strExclude1End = "00:00";
	bool m_bExclude2Reqd = FALSE;
	CString m_strExclude2Start = "00:00";
	CString m_strExclude2End = "00:00";
	bool m_bExclude3Reqd = FALSE;
	CString m_strExclude3Start = "00:00";
	CString m_strExclude3End = "00:00";
	bool m_bExclude4Reqd = FALSE;
	CString m_strExclude4Start = "00:00";
	CString m_strExclude4End = "00:00";
	CString m_strExcludeUploadStart = "00:00";
	CString m_strExcludeUploadEnd = "00:00";
	CString m_strSendEODAfter = "16:00";
	CString m_strDateLastTransaction = "";
	CString m_strTimeLastTransaction = "";
	CString m_strDateLastBalance = "";
	CString m_strTimeLastBalance = "";
	bool m_bEODAfterPayments = TRUE;
	bool m_bImportAccounts = TRUE;
	int m_nAccountLinkDayFlags = 0x3E;
	CString m_strAccountLinkStart = "00:00";
	CString m_strAccountLinkEnd = "23:59";
	CString m_strDateLastAccountLink = "";
	CString m_strTimeLastAccountLink = "";
	bool m_bPromptBeforeAccessFlag = TRUE;
	bool m_bImportAccountsFlag = TRUE;	

private:
	CString m_strFilename = "";
};

//$$******************************************************************
