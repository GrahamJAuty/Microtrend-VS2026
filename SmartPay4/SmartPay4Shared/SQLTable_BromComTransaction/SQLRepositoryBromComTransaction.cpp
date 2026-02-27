//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryBromComTransaction.h"
//**********************************************************************

CSQLRepositoryBromComTransaction::CSQLRepositoryBromComTransaction()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComTransaction::SelectRow(CSQLRowBromComTransaction& RowBCT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComTransaction RecordSet(pDatabase, 
			RSParams_BromComTransaction_NormalByLineId{SimpleFormatInt64Value(RowBCT.GetLineID())});

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
				RecordSet.SaveToBromComTransactionRow(RowBCT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComTransaction::UpdateRow(CSQLRowBromComTransaction& RowBCT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComTransaction RecordSet(pDatabase,
			RSParams_BromComTransaction_NormalByLineId{ SimpleFormatInt64Value(RowBCT.GetLineID()) });

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
				RecordSet.LoadFromBromComTransactionRow(RowBCT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComTransaction::InsertRow(CSQLRowBromComTransaction& RowBCT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComTransaction RecordSet(pDatabase,
			RSParams_BromComTransaction_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromBromComTransactionRow(RowBCT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComTransaction::DeleteRow(CSQLRowBromComTransaction& RowBCT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComTransaction RecordSet(pDatabase,
			RSParams_BromComTransaction_NormalByLineId{ SimpleFormatInt64Value(RowBCT.GetLineID()) });

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

CSQLResultInfo CSQLRepositoryBromComTransaction::RowExists(__int64 nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetBromComTransaction RecordSet(pDatabase,
		RSParams_BromComTransaction_NormalByLineId{ SimpleFormatInt64Value(nLineID) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************
