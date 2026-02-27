#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\SQLTranImportHelpers.h"
/**********************************************************************/

class CSQLTranImportTopup : public CSQLTranBase
{
public:
	CSQLTranImportTopup(int nCacheType = -1, CString strCacheFilename = "");

public:
	void DoWork(int nImportAppNo, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nLocateOnIndex, CString strLocateOnLabel, int nIndexUserID);
	void GetImportCounters(CSQLImportCounters& Counters) { Counters = m_ImportCounters; }

private:
	bool ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine);
	void ImportTopupFields(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData);

private:
	int m_nIndexUserID;
	int m_nImportMethod;
	int m_nImportAppNo;
	CString m_strExceptionPrefix;
	int m_nIndexLocateOn;
	CString m_strLocateOnLabel;

private:
	int m_nCacheType;
	CString m_strCacheFilename;

private:
	CSQLTranImportHelpers m_ImportHelpers;
	CSQLImportCounters m_ImportCounters;
};

/**********************************************************************/

