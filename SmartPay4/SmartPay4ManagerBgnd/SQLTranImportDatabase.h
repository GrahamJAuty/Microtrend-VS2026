#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/
#include "SQLTranImportHelpers.h"
/**********************************************************************/

class CSQLTranImportDatabase : public CSQLTranBase
{
public:
	CSQLTranImportDatabase( int nCacheType = -1, CString strCacheFilename = "");

public:
	void DoWork(int nImportMethod, int nImportAppNo, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID);
	void GetImportCounters(CSQLImportCounters& Counters) { Counters = m_ImportCounters; }

private:
	bool ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine);

private:
	int m_nIndexUserID;
	int m_nImportMethod;
	int m_nImportAppNo;
	CString m_strExceptionPrefix;

private:
	int m_nCacheType;
	CString m_strCacheFilename;

private:
	CSQLTranImportHelpers m_ImportHelpers;
	CSQLImportCounters m_ImportCounters;
};

/**********************************************************************/

