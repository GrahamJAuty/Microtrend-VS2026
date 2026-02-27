#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/
#include "SQLTranImportHelpers.h"
/**********************************************************************/

class CSQLTranImportUpgrade : public CSQLTranBase
{
public:
	CSQLTranImportUpgrade();

public:
	void DoWork(int nImportAppNo, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID);
	
private:
	bool ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine);
	
private:
	int m_nIndexUserID;
	int m_nImportMethod;
	int m_nImportAppNo;
	CString m_strExceptionPrefix;
	
private:
	CSQLTranImportHelpers m_ImportHelpers;
	CSQLImportCounters m_ImportCounters;
};

/**********************************************************************/

