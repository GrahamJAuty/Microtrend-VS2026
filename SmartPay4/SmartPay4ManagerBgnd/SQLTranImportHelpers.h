#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "afxdb.h"
/**********************************************************************/

struct CSQLImportCounters
{
public:
	CSQLImportCounters();

public:
	int GetRead() { return m_nRead; }
	int GetUsed() { return m_nUsed; }
	int GetAdded() { return m_nAdded; }
	int GetInvalid() { return m_nInvalid; }
	int GetIgnored() { return m_nIgnored; }
	int GetDeleted() { return m_nDeleted; }

public:
	void IncRead() { m_nRead++; }
	void IncUsed() { m_nUsed++; }
	void IncAdded() { m_nAdded++; }
	void IncInvalid() { m_nInvalid++; }
	void IncIgnored() { m_nIgnored++; }
	void IncDeleted() { m_nDeleted++; }

private:
	int m_nIgnored;
	int m_nRead;
	int m_nUsed;
	int m_nInvalid;
	int m_nAdded;
	int m_nDeleted;
};

/**********************************************************************/

class CSQLTranImportHelpers
{
public:
	CSQLTranImportHelpers(CSQLAuditRecord_base& atc, CSQLImportCounters& ImportCounters);
	
public:
	void SetDatabase(CDatabase* pDatabase) { m_pDatabase = pDatabase; }
	void SetImportAppNo(int n) { m_nImportAppNo = n; }

public:
	void ImportFieldsStage1(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData, bool& bGotMaxSpendField, bool& bChangedGroup);
	void ImportFieldsStage2(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData);

public:
	bool ProcessNewRecord(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData);
	bool ProcessExistingRecord(CSQLRowAccountFull& RowAccount, CArray<int, int>& arrayHeader, CCSV* pCsvData);

public:
	void LogNoRecord(const char* szCardNo, bool bRecordExists);
	void LogException(const char* szPrefix, const char* szText);

private:
	bool GetBoolData(const char* szData);
	void SetAllergies(CSQLRowAccountFull& RowAccount, const char* szText);

private:
	bool m_bAllergyRead;
	CAllergyText m_allergy;
	int m_nImportAppNo;

private:
	CDatabase* m_pDatabase;
	CSQLAuditRecord_base& m_atc;
	CSQLImportCounters& m_ImportCounters;
};

/**********************************************************************/
