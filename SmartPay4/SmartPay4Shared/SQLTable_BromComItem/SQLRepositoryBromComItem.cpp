//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryBromComItem.h"
//**********************************************************************

CSQLRepositoryBromComItem::CSQLRepositoryBromComItem()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComItem::SelectRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComItem RecordSet(pDatabase, 
			RSParams_BromComItem_NormalByOrderIdPersonId{
				RowBC.GetOrderID(),
				RowBC.GetPersonID()
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
				RecordSet.SaveToBromComItemRow(RowBC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComItem::UpdateRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComItem RecordSet(pDatabase,
			RSParams_BromComItem_NormalByOrderIdPersonId{
				RowBC.GetOrderID(),
				RowBC.GetPersonID()
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
				RecordSet.LoadFromBromComItemRow(RowBC);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComItem::InsertRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComItem RecordSet(pDatabase,
			RSParams_BromComItem_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromBromComItemRow(RowBC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComItem::UpsertRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowBC.GetOrderID(), RowBC.GetPersonID(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowBC, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowBC, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComItem::DeleteRow(CSQLRowBromComItem& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComItem RecordSet(pDatabase,
			RSParams_BromComItem_NormalByOrderIdPersonId{
				RowBC.GetOrderID(),
				RowBC.GetPersonID()
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

CSQLResultInfo CSQLRepositoryBromComItem::RowExists(int nOrderID, int nPersonID, CDatabase* pDatabase)
{
	CSQLRecordSetBromComItem RecordSet(pDatabase,
		RSParams_BromComItem_NormalByOrderIdPersonId{ nOrderID, nPersonID }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************