#pragma once
/**********************************************************************/
#include "TucasiSOAPData.h"
/**********************************************************************/

class CSSTucasiSoap
{
public:
	CSSTucasiSoap ( CTucasiSoapData* pData );

	CString GetPayments ( const char* szCsvFilename, const char* szLoginReply, const char* szHeader );
	CString SendBalances ( const char* szCsvBalances, const char* szLoginReply );
	CString SendTransactions ( const char* szSourceFile, const char* szLoginReply );

//	CSSTucasi ( const char* szUsername, const char* szPassword, const char* szSchoolID, const char* szSoapVersion );
//	CString GetPupilBalanceV1a ( const char* szPupilID, const char* szFilename );
//	CString GetPupilBalancesV1a ( const char* szFilename );


private:
	CTucasiSoapData* m_pData;
	bool m_bDemoSystem;
};

/**********************************************************************/
