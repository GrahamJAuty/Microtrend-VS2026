//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryPebbleExportTransaction.h"
//**********************************************************************

CSQLRepositoryPebbleExportTransaction::CSQLRepositoryPebbleExportTransaction()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransaction::SelectRow(CSQLRowPebbleExportTransaction& RowPET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportTransaction RecordSet(pDatabase, 
			RSParams_PebbleExportTransaction_NormalByLineId{ SimpleFormatInt64Value(RowPET.GetLineID()) });

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
				RecordSet.SaveToPebbleExportTransactionRow(RowPET);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransaction::UpdateRow(CSQLRowPebbleExportTransaction& RowPET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportTransaction RecordSet(pDatabase,
			RSParams_PebbleExportTransaction_NormalByLineId{ SimpleFormatInt64Value(RowPET.GetLineID()) });

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
				RecordSet.LoadFromPebbleExportTransactionRow(RowPET);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransaction::InsertRow(CSQLRowPebbleExportTransaction& RowPET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportTransaction RecordSet(pDatabase,
			RSParams_PebbleExportTransaction_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebbleExportTransactionRow(RowPET);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransaction::DeleteRow(CSQLRowPebbleExportTransaction& RowPET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportTransaction RecordSet(pDatabase,
			RSParams_PebbleExportTransaction_NormalByLineId{ SimpleFormatInt64Value(RowPET.GetLineID()) });

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

CSQLResultInfo CSQLRepositoryPebbleExportTransaction::RowExists(__int64 nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetPebbleExportTransaction RecordSet(pDatabase,
		RSParams_PebbleExportTransaction_NormalByLineId{ SimpleFormatInt64Value(nLineID) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************
