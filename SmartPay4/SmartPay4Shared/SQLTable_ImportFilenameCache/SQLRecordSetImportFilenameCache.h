#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowImportFilenameCache.h"
//**********************************************************************

struct RSParams_ImportFilenameCache_NormalNoParams
{
public:
	RSParams_ImportFilenameCache_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_ImportFilenameCache_NormalByTypeAndFilename
{
public:
	RSParams_ImportFilenameCache_NormalByTypeAndFilename(CString str1, CString str2)
		: strType(str1), strFilename(str2) {}

public:
	CString strType;
	CString strFilename;
};

//**********************************************************************

struct RSParams_ImportFilenameCache_NormalByLineId
{
public:
	RSParams_ImportFilenameCache_NormalByLineId(CString str)
		: strLineId(str) {}

public:
	CString strLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataImportFilenameCache : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataImportFilenameCache(bool bForExistenceCheck = FALSE);
	void AddParamsByLineID(const CString strLineID);
	void AddParamsByTypeAndFilename(const CString strType, const CString strFilename);

public:
	__int64 m_nLineID;
	int m_nType;
	CString m_strFilename;
	CString m_strDateTime;
};

//**********************************************************************

class CSQLRecordSetImportFilenameCache : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetImportFilenameCache(CDatabase* pDatabase, RSParams_ImportFilenameCache_NormalNoParams params);
	CSQLRecordSetImportFilenameCache(CDatabase* pDatabase, RSParams_ImportFilenameCache_NormalByTypeAndFilename params);
	CSQLRecordSetImportFilenameCache(CDatabase* pDatabase, RSParams_ImportFilenameCache_NormalByLineId params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromImportFilenameCacheRow(CSQLRowImportFilenameCache& RowBC);
	void SaveToImportFilenameCacheRow(CSQLRowImportFilenameCache& RowBC);

public:
	bool StepSelectAll(CSQLRowImportFilenameCache& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataImportFilenameCache m_MD;
};

//**********************************************************************
