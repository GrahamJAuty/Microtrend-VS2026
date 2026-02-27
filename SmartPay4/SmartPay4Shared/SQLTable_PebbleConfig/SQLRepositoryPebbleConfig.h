#pragma once
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPebbleConfig.h"
//**********************************************************************
#define nPEBBLE_LOCATEBY_UPN			1		// by UPN
#define nPEBBLE_LOCATEBY_MIS			2		// by Person_id
//*******************************************************************

class CSQLRepositoryPebbleConfig
{
public:
	CSQLRepositoryPebbleConfig();

public:
	void LoadRow(CDatabase* pDatabase);
	__int64 SaveRow(CDatabase* pDatabase);
	bool IsSet();

public:
	CSQLResultInfo SelectRow(CSQLRowPebbleConfig& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPebbleConfig& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPebbleConfig& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPebbleConfig& SQLRowPB, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPebbleConfig& SQLRowPB, CDatabase* pDatabase);
	
public:
	CSQLResultInfo RowExists(int nLineID, CDatabase* pDatabase);

public:
	CString GetLocateByLabelMisID();
	CString GetLocateByLabelUPN();
	CString GetLocateByText(int n);
	CString GetSQLFilterDateTime(bool bSale);

public:
	CString GetUpdatesFilename() { return System.GetWebPaymentPath("PbFile03.dat", nWEBPAYMENT_PEBBLE); }
	CString GetBalanceDBListFilename() { return System.GetWebPaymentPath("PbFile04.dat", nWEBPAYMENT_PEBBLE); }

private:
	CString GetTextLabel(int n);

public:
	CSQLRowPebbleConfig m_Row;
};

//**********************************************************************
