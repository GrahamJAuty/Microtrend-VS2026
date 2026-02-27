//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLExceptionLogger.h"
//**********************************************************************
#include "SQLRepositoryBromComPendingItem.h"
//**********************************************************************

CSQLRepositoryBromComPendingItem::CSQLRepositoryBromComPendingItem()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingItem::SelectRow(CSQLRowBromComPendingItem& RowBCP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingItem RecordSet(pDatabase, 
			RSParams_BromComPendingItem_NormalByOrderIdPersonId{
				RowBCP.GetOrderID(),
				RowBCP.GetPersonID()
			});

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
				RecordSet.SaveToBromComPendingItemRow(RowBCP);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingItem::UpdateRow(CSQLRowBromComPendingItem& RowBCP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingItem RecordSet(pDatabase,
			RSParams_BromComPendingItem_NormalByOrderIdPersonId{
				RowBCP.GetOrderID(),
				RowBCP.GetPersonID()
			});

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
				RecordSet.LoadFromBromComPendingItemRow(RowBCP);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingItem::InsertRow(CSQLRowBromComPendingItem& RowBCP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingItem RecordSet(pDatabase,
			RSParams_BromComPendingItem_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromBromComPendingItemRow(RowBCP);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingItem::DeleteRow(CSQLRowBromComPendingItem& RowBCP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingItem RecordSet(pDatabase,
			RSParams_BromComPendingItem_NormalByOrderIdPersonId{
				RowBCP.GetOrderID(),
				RowBCP.GetPersonID()
			});

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

CSQLResultInfo CSQLRepositoryBromComPendingItem::DeletePendingItemOrder(int nOrderID, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		
		int nConnection = -1;

		CString strCommand = "";
		strCommand.Format("DELETE FROM %s WHERE OrderID = %d",
			SQLTableNames::BromComPendingItems,
			nOrderID);

		if (NULL == pDatabase)
		{
			pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);
		}

		pDatabase->ExecuteSQL(strCommand);
		Result.SetSQLError(SQLCRUD_ERR_NONE);

		if (nConnection != -1)
		{
			SQLConnectionPool.FreeConnection(nConnection);
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingItem::RowExists(int nOrderID, int nPersonID, CDatabase* pDatabase)
{
	CSQLRecordSetBromComPendingItem RecordSet(pDatabase,
		RSParams_BromComPendingItem_NormalByOrderIdPersonId{ nOrderID, nPersonID }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************