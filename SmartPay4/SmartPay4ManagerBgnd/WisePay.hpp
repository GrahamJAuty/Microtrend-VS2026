#pragma once
//*********************************************************************
#include "WisePayData.h"
//*********************************************************************

class CSSWisePay
{
public:
	CSSWisePay ( CWisePayData* pData );
	CString GetChildAccounts (  const char* szLoginReply, const char* szFilename );
	CString GetPayments		 ( int nAccountNo, const char* szLoginReply, const char* szCsvFilename );
	CString SendTransactions ( const char* szSource, const char* szLoginReply );
	CString SendTransactions2 ( const char* szSource, const char* szLoginReply );
//	CString SendBalances	 ( const char* szSource, const char* szLoginReply );

private:
	CWisePayData* m_pData;
	CString m_strError;
};

//**********************************************************************
