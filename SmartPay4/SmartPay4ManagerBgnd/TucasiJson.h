#pragma once
/**********************************************************************/
#include "TucasiJSONData.h"
/**********************************************************************/

class CSSTucasiJson
{
public:
	CSSTucasiJson ( CTucasiJsonData* pData );

	CString GetPayments ( const char* szCsvFilename, const char* szResponseFilename );
	CString SendUpdates ( const char* szCsvDataFile, const char* szResponseFilename );

	bool LogResponse ( const char* szResponseFile );

private:
	CTucasiJsonData* m_pData;
};

/**********************************************************************/
