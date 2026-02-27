#pragma once
/**********************************************************************/
#include "ParentMailData.h"
/**********************************************************************/

class CSSParentMail
{
public:
	CSSParentMail ( CParentMailData* pData );
	CString GetPayments ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader );
	CString SendUpdates ( const char* szSourceFile, const char* szLoginReply );
	int GetTxCount() { return m_nTxCount; }

	CString GetLastProcessedSnapShotID ( const char* szLoginReply );
	__int64 LastSnapshotID() { return m_nLastTransactionID; }

	CString LinkNewAccount ( const char* szLoginReply, const char* szForename, const char* szSurname, const char* szXMLDoB, const char* szCardNo, const char* szAdditionalID, const char* szFormName, const char* szAccountType );
	CString GetAccountID() { return m_strAccountID; }

	CString RequestLinkedAccounts ( const char* strLoginReply, const char* strCsvFilename, const char* szHeader );

private:
	CString ValidateReply ( CSSSoap* pSoap );

private:
	CParentMailData* m_pData;
	CString m_strIPAddress;
	CString m_strAccountID;
	__int64 m_nLastTransactionID;

	int m_nTxCount;
};

/**********************************************************************/
