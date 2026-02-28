//**********************************************************************
#include "..\SQLTable_AuditPeriod\SQLRecordSetAuditPeriod.h"
#include "..\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryAuditPeriod.h"
//**********************************************************************

CSQLRepositoryAuditPeriod::CSQLRepositoryAuditPeriod()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAuditPeriod::SelectRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAuditPeriod RecordSet(pDatabase, 
			RSParams_AuditPeriod_NormalById{ RowAP.GetPeriodID() });

		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.SaveToAuditPeriodRow(RowAP);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAuditPeriod::UpdateRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAuditPeriod RecordSet(pDatabase,
			RSParams_AuditPeriod_NormalById{ RowAP.GetPeriodID() });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.Edit();
				RecordSet.LoadFromAuditPeriodRow(RowAP);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAuditPeriod::InsertRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAuditPeriod RecordSet(pDatabase,
			RSParams_AuditPeriod_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromAuditPeriodRow(RowAP);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAuditPeriod::UpsertRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowAP.GetPeriodID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowAP, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowAP, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAuditPeriod::DeleteRow(CSQLRowAuditPeriod& RowAP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetAuditPeriod RecordSet(pDatabase,
			RSParams_AuditPeriod_NormalById{ RowAP.GetPeriodID() });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == FALSE)
			{
				RecordSet.Delete();
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			else
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryAuditPeriod::RowExists(int nPeriodID, CDatabase* pDatabase)
{
	CSQLRecordSetAuditPeriod RecordSet(pDatabase,
		RSParams_AuditPeriod_NormalById{ nPeriodID }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

bool CSQLRepositoryAuditPeriod::ImportLegacyAuditPeriod(CDatabase* pDatabase)
{
	int nPeriod = 0;
	SQLRowSetAuditPeriod.LoadPeriods(pDatabase);

	bool bResult = FALSE;

	CString strFilename = Filenames.GetLegacyPeriodDataFilename();

	if (SolutionGlobalFunctions::FileExists(strFilename) == FALSE)
	{
		bResult = TRUE;
	}
	else
	{
		CSSFile filePeriods;
		if (filePeriods.Open(Filenames.GetLegacyPeriodDataFilename(), "rb") == TRUE)
		{
			bResult = TRUE;

			CString strBuffer = "";
			while ((nPeriod < nMAXPERIODS) && (filePeriods.ReadString(strBuffer) == TRUE))
			{
				CCSV csv(strBuffer);
				int nOldTime = csv.GetInt(1);
				int nNewTime = ((nOldTime / 100) * 60) + (nOldTime % 100);
				SQLRowSetAuditPeriod.SetPeriodName(nPeriod, csv.GetString(0));
				SQLRowSetAuditPeriod.SetStartTime(nPeriod, nNewTime);
				nPeriod++;
			}
		}
	}

	SQLRowSetAuditPeriod.SavePeriods(pDatabase);

	return bResult;
}

//**********************************************************************
