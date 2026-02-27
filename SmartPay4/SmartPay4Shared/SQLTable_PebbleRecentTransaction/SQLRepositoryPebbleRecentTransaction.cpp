//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetPebbleRecentTransaction.h"
//**********************************************************************
#include "SQLRepositoryPebbleRecentTransaction.h"
//**********************************************************************

CSQLRepositoryPebbleRecentTransaction::CSQLRepositoryPebbleRecentTransaction()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleRecentTransaction::SelectRow(CSQLRowPebbleRecentTransaction& RowPRT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleRecentTransaction RecordSet(pDatabase, 
			RSParams_PebbleRecentTransaction_NormalByTransactionID{ RowPRT.GetTransactionID() });

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
				RecordSet.SaveToPebbleRecentTransactionRow(RowPRT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleRecentTransaction::UpdateRow(CSQLRowPebbleRecentTransaction& RowPRT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleRecentTransaction RecordSet(pDatabase,
			RSParams_PebbleRecentTransaction_NormalByTransactionID{ RowPRT.GetTransactionID() });

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
				RecordSet.LoadFromPebbleRecentTransactionRow(RowPRT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleRecentTransaction::InsertRow(CSQLRowPebbleRecentTransaction& RowPRT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleRecentTransaction RecordSet(pDatabase,
			RSParams_PebbleRecentTransaction_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebbleRecentTransactionRow(RowPRT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleRecentTransaction::UpsertRow(CSQLRowPebbleRecentTransaction& RowPRT, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPRT.GetTransactionID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPRT, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPRT, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleRecentTransaction::DeleteRow(CSQLRowPebbleRecentTransaction& RowPRT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleRecentTransaction RecordSet(pDatabase,
			RSParams_PebbleRecentTransaction_NormalByTransactionID{ RowPRT.GetTransactionID() });

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

CSQLResultInfo CSQLRepositoryPebbleRecentTransaction::RowExists(CString strTransactionID, CDatabase* pDatabase)
{
	CSQLRecordSetPebbleRecentTransaction RecordSet(pDatabase,
		RSParams_PebbleRecentTransaction_NormalByTransactionID{ strTransactionID }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

