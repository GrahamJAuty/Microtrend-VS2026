#pragma once
//**********************************************************************
#include "SquidData.h"
//**********************************************************************

class CSSSquid
{
public:
	CSSSquid ( CSquidData* pData );

	CString RetrieveCardDetails ( const char* szLoginReply, const char* szLinkReference1, const char* szLinkReference2 );
	CString DisassociateCard ( const char* szLoginReply, const char* szSquidCardNo );

	CString GetSquidAccountNo() { return m_strSquidAccountNo; }
	CString GetSquidCardNo() { return m_strSquidCardNo; }
	CString GetSquidCVV() { return m_strSquidCVV; }

	CString GetPayments (  const char* szLoginReply, const char* szCsvFilename, const char* szHeader );
	bool MoreRecordsAvailable() { return m_bMoreRecordsAvailable; }

	CString UploadPaymentAcks ( const char* szSource, const char* szLoginReply );
	CString UploadTransactions ( const char* szSourceFile, const char* szLoginReply );

private:
	void AddLoginCredentials ( CSSSoap* pSoap );
	CString ValidateLoginReply ( CSSSoap* s, const char* szError );

	CString UploadData ( int nType, const char* szSource, const char* szLoginReply );
	CString SendTransactions ( const char* szSourceFile, const char* szLoginReply, int nTxCount );
	CString SendPaymentAcks ( const char* szSource, const char* szLoginReply, int  nTxCount );

	CString ExtractPurseAccountNumber ( CSSSoap* pSoap );

private:
	CSquidData* m_pData;
	CString m_strIPAddress;
	CString m_strIssuerID;
	CString m_strPassword;
	CString m_strMerchantID;

	CString m_strSquidAccountNo;
	CString m_strSquidCardNo;
	CString m_strSquidCVV;

	bool m_bUseHttps;
	bool m_bMoreRecordsAvailable;
};

/**********************************************************************/
