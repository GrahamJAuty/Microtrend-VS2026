#pragma once
//******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//******************************************************************

class CSquidData  
{
public:
	CSquidData();
	void Read();
	void Save();
	bool IsSet();

	CString GetXmlLoginPath ( const char* szFilename );

	void SetSquidCardNo ( CSQLRowAccountFull& RowAccount, const char* szSquidAccountNo );
	void SetSquidCVV ( CSQLRowAccountFull& RowAccount, const char* szSquidCVV );

	CString GetSquidLinkReference1 (CSQLRowAccountFull& RowAccount);
	CString GetSquidLinkReference2 (CSQLRowAccountFull& RowAccount);
	CString GetSquidCardNo (CSQLRowAccountFull& RowAccount);

	CString GetWebPaymentListFilename()		 { return System.GetWebPaymentPath ( "Sqlist.dat",   nWEBPAYMENT_SQUID ); }
	CString GetSquidLinkListFilename()		 { return System.GetWebPaymentPath ( "Linklog.dat",  nWEBPAYMENT_SQUID ); }
	CString GetWebPaymentExceptionFilename() { return System.GetWebPaymentPath ( "SqFile01.dat", nWEBPAYMENT_SQUID ); }
	CString GetNewUpdatesFilename()			 { return System.GetWebPaymentPath ( "SqFile02.dat", nWEBPAYMENT_SQUID ); }
	CString GetUpdatesFilename()			 { return System.GetWebPaymentPath ( "SqFile03.dat", nWEBPAYMENT_SQUID ); }
	CString GetTransRefFilename()			 { return System.GetWebPaymentPath ( "SqFile04.dat", nWEBPAYMENT_SQUID ); }
	CString GetDisassociateFilename()		 { return System.GetWebPaymentPath ( "SqFile05.dat", nWEBPAYMENT_SQUID ); }

//*******************************************************************

public:
	bool m_bKeepXmlLogin;
	bool m_bKeepXmlFiles;
	bool m_bKeepLinkLog;
	CString m_strSoapVersion;
	CString m_strIPAddress;
	CString m_strMerchantID;
	CString m_strIssuerID;
	CString m_strPassword;
	int m_nAccountPurse;
	int m_nMaxTransactionLines;
	int m_nSquidCardHolderField;
	int m_nSquidExtRefField;
	int m_nSquidCardField;
	int m_nSquidCVVField;

	CString m_strUploadHeader;

private:
	CString m_strFilename;
};
