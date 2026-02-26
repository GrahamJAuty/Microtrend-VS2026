#pragma once
/**********************************************************************/
#include "SQLTranBase.h"
/**********************************************************************/
#define IMPORT_METHOD_ALL 0
#define IMPORT_METHOD_NEWONLY 1
#define IMPORT_METHOD_EXISTONLY 2
/**********************************************************************/

class CSQLTranImportDatabase : public CSQLTranBase
{
public:
	void DoWorkManagerImport(bool bBackgroundMode, bool bForUpgrade, int nImportMethod, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID);
	void DoWorkServerImport(int nImportMethod, CSSFile& ImportFile, CArray<int, int>& arrayHeader, int nIndexUserID);

public:
	int GetRead() { return m_nRead; }
	int GetUsed() { return m_nUsed; }
	int GetAdded() { return m_nAdded; }
	int GetInvalid() { return m_nInvalid; }
	int GetIgnored() { return m_nIgnored; }

private:
	bool ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine);
	bool ImportLineForUpgrade(CArray<int, int>& arrayHeader, const char* szImportLine);
	bool ImportFields(CArray<int, int>& arrayHeader, CCSV* pCsvData, CSQLRowAccountFull& RowAccount);
	void ApplyRevaluationBonus(CSQLRowAccountFull& RowAccount, double dRevalueAmount,int nPurse);

private:
	int m_nImportMethod;
	int m_nIndexUserID;
	int m_nSourceType;

private:
	int m_nRead;
	int m_nInvalid;
	int m_nIgnored;
	int m_nAdded;
	int m_nUsed;
};

/**********************************************************************/
