//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetPebblePendingTransaction.h"
//**********************************************************************
#include <memory>
//**********************************************************************
#include "SQLRepositoryPebblePendingTransaction.h"
//**********************************************************************

CSQLRepositoryPebblePendingTransaction::CSQLRepositoryPebblePendingTransaction(bool bUnmatched)
{
	m_bUnmatched = bUnmatched;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePendingTransaction::SelectRow(CSQLRowPebblePendingTransaction& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePendingTransaction RecordSet(pDatabase, 
			RSParams_PebblePendingTransaction_NormalByTransactionId{ RowPPT.GetTransactionID() },
			m_bUnmatched);

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
				RecordSet.SaveToPebblePendingTransactionRow(RowPPT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePendingTransaction::UpdateRow(CSQLRowPebblePendingTransaction& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePendingTransaction RecordSet(pDatabase,
			RSParams_PebblePendingTransaction_NormalByTransactionId{ RowPPT.GetTransactionID() },
			m_bUnmatched);

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
				RecordSet.LoadFromPebblePendingTransactionRow(RowPPT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePendingTransaction::InsertRow(CSQLRowPebblePendingTransaction& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePendingTransaction RecordSet(pDatabase,
			RSParams_PebblePendingTransaction_NormalNoParams{},
			m_bUnmatched);

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebblePendingTransactionRow(RowPPT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePendingTransaction::UpsertRow(CSQLRowPebblePendingTransaction& RowPPT, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPPT.GetTransactionID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPPT, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPPT, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePendingTransaction::DeleteRow(CSQLRowPebblePendingTransaction& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePendingTransaction RecordSet(pDatabase, 
			RSParams_PebblePendingTransaction_NormalByTransactionId{ RowPPT.GetTransactionID() },
			m_bUnmatched);

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

CSQLResultInfo CSQLRepositoryPebblePendingTransaction::RowExists(CString strTransactionID, CDatabase* pDatabase)
{
	CSQLRecordSetPebblePendingTransaction RecordSet(pDatabase,
		RSParams_PebblePendingTransaction_NormalByTransactionId{ strTransactionID },
		m_bUnmatched, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************
