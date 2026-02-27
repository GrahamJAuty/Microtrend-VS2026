#include "SchoolcommsData.h"

//**********************************************************************

class CSSSchoolcomms
{
public:
	CSSSchoolcomms ( CSchoolcommsData* pData );

	CString GetPayments ( const char* szLoginReply, const char* szCsvFilename, const char* szHeader );
	CString SendTransactions ( const char* szSourceFile, const char* szLoginReply, int nMaxTxCount );

	bool MoreRecordsAvailable() { return m_bMoreRecordsAvailable; }

private:
	CString ValidateLoginReply ( CSSSoap* s, const char* szError );

private:
	CSchoolcommsData* m_pData;
	bool m_bMoreRecordsAvailable;
};

//**********************************************************************
