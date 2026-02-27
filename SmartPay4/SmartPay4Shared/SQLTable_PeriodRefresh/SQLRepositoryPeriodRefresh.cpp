//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryPeriodRefresh.h"
//**********************************************************************

CSQLRepositoryPeriodRefresh::CSQLRepositoryPeriodRefresh()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPeriodRefresh::SelectRow(CSQLRowPeriodRefresh& RowPR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPeriodRefresh RecordSet(pDatabase, 
			RSParams_PeriodRefresh_NormalByUserID{ FormatInt64Value(RowPR.GetUserID()) });

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
				RecordSet.SaveToPeriodRefreshRow(RowPR);
				Result.SetSQLError(SQLCRUD_ERR_NONE);				
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPeriodRefresh::UpdateRow(CSQLRowPeriodRefresh& RowPR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPeriodRefresh RecordSet(pDatabase,
			RSParams_PeriodRefresh_NormalByUserID{ FormatInt64Value(RowPR.GetUserID()) });

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
				RecordSet.LoadFromPeriodRefreshRow(RowPR);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPeriodRefresh::InsertRow(CSQLRowPeriodRefresh& RowPR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPeriodRefresh RecordSet(pDatabase,
			RSParams_PeriodRefresh_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPeriodRefreshRow(RowPR);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPeriodRefresh::UpsertRow(CSQLRowPeriodRefresh& RowPR, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPR.GetUserID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPR, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPR, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPeriodRefresh::DeleteRow(CSQLRowPeriodRefresh& RowPR, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPeriodRefresh RecordSet(pDatabase,
			RSParams_PeriodRefresh_NormalByUserID{ FormatInt64Value(RowPR.GetUserID()) });

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

CSQLResultInfo CSQLRepositoryPeriodRefresh::RowExists(__int64 nUserID, CDatabase* pDatabase)
{
	CSQLRecordSetPeriodRefresh RecordSet(pDatabase,
		RSParams_PeriodRefresh_NormalByUserID{ FormatInt64Value(nUserID) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************
