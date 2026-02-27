//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLTable_AuditPeriod\SQLRecordSetAuditPeriod.h"
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
			RSParams_AuditPeriod_NormalByPeriodId{RowAP.GetPeriodID()});
		
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
			RSParams_AuditPeriod_NormalByPeriodId{ RowAP.GetPeriodID() });

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
			RSParams_AuditPeriod_NormalByPeriodId{ RowAP.GetPeriodID() });

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
		RSParams_AuditPeriod_NormalByPeriodId{ nPeriodID }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

