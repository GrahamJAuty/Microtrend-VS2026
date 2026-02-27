#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebbleConfig.h"
//**********************************************************************

struct CPebbleVATInfoRS
{
public:
	CPebbleVATInfoRS()
	{
		m_nVATCode = 0;
		m_strVATRate = "0.00";
	}

public:
	int m_nVATCode;
	CString m_strVATRate;
};

//**********************************************************************

struct RSParams_PebbleConfig_NormalNoParams
{
public:
	RSParams_PebbleConfig_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebbleConfig_NormalByLineId
{
public:
	RSParams_PebbleConfig_NormalByLineId(int n)
		: nLineId(n){}

public:
	int nLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataPebbleConfig : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebbleConfig(bool bForExistenceCheck = FALSE);
	void AddParamsByLineID(const int nLineID);
	
public:
	int m_nLineID;
	CString m_strInstallationID;
	CString m_strOrganisationID;
	int m_nMISInfoIndex;
	int m_nUPNInfoIndex;
	CString m_strLastImportDateTopup;
	CString m_strLastImportTimeTopup;
	CString m_strLastImportDateSale;
	CString m_strLastImportTimeSale;
	BOOL m_bFirstImportTopup;
	BOOL m_bFirstImportSale;
	int m_nRecentCacheDays;
	int m_nPaymentCacheDays;
	CPebbleVATInfoRS m_VATInfo[12];
	__int64 m_nOfficeMealPluNo;
	int m_nOfficeMealVATBand;
};

//**********************************************************************

class CSQLRecordSetPebbleConfig : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebbleConfig(CDatabase* pDatabase, RSParams_PebbleConfig_NormalNoParams params);
	CSQLRecordSetPebbleConfig(CDatabase* pDatabase, RSParams_PebbleConfig_NormalByLineId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebbleConfigRow(CSQLRowPebbleConfig& RowPB);
	void SaveToPebbleConfigRow(CSQLRowPebbleConfig& RowPB);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPebbleConfig m_MD;
};

//**********************************************************************

