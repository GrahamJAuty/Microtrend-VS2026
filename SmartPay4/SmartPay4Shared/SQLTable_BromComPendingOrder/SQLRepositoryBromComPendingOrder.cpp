//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryBromComPendingOrder.h"
//**********************************************************************

CSQLRepositoryBromComPendingOrder::CSQLRepositoryBromComPendingOrder()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::SelectRow(CSQLRowBromComPendingOrder& RowBCPO, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingOrder RecordSet(pDatabase, 
			RSParams_BromComPendingOrder_NormalByOrderId{ RowBCPO.GetOrderID() });

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
				RecordSet.SaveToBromComPendingOrderRow(RowBCPO);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::UpdateRow(CSQLRowBromComPendingOrder& RowBCPO, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingOrder RecordSet(pDatabase,
			RSParams_BromComPendingOrder_NormalByOrderId{ RowBCPO.GetOrderID() });

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
				RecordSet.LoadFromBromComPendingOrderRow(RowBCPO);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::InsertRow(CSQLRowBromComPendingOrder& RowBCPO, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingOrder RecordSet(pDatabase,
			RSParams_BromComPendingOrder_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromBromComPendingOrderRow(RowBCPO);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::UpsertRow(CSQLRowBromComPendingOrder& RowBCPO, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowBCPO.GetOrderID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowBCPO, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowBCPO, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::DeleteRow(CSQLRowBromComPendingOrder& RowBCPO, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComPendingOrder RecordSet(pDatabase,
			RSParams_BromComPendingOrder_NormalByOrderId{ RowBCPO.GetOrderID() });

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

CSQLResultInfo CSQLRepositoryBromComPendingOrder::RowExists(int nOrderID, CDatabase* pDatabase)
{
	CSQLRecordSetBromComPendingOrder RecordSet(pDatabase,
		RSParams_BromComPendingOrder_NormalByOrderId{ nOrderID }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::CountRows(CDatabase* pDatabase)
{
	return CountRowsByRange(BromComPendingOrder::OrderID.Min, BromComPendingOrder::OrderID.Max, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComPendingOrder::CountRowsByRange(int nFrom, int nTo, CDatabase* pDatabase)
{
	CSQLRecordSetBromComPendingOrder RecordSet(pDatabase,
		RSParams_BromComPendingOrder_CountRowsByRange{ nFrom, nTo });

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************