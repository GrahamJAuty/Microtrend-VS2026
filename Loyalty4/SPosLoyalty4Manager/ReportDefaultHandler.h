#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CReportDefaultHandler
{
public:
	CReportDefaultHandler() {}
	bool Read ( const char* szLabel, CString& strLine );
	void Save ( const char* szLabel, CCSV* pCsv );
};

//**************************************************************/

