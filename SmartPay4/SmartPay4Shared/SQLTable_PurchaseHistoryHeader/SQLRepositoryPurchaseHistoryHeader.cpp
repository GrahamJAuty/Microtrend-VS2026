//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLRecordsetConnectionPool.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "SQLRepositoryPurchaseHistoryHeader.h"
//**********************************************************************

CSQLRepositoryPurchaseHistoryHeader::CSQLRepositoryPurchaseHistoryHeader()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseHistoryHeader::SelectRow(CSQLRowPurchaseHistoryHeader& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseHistoryHeader RecordSet(pDatabase,
			RSParams_PurchaseHistoryHeader_NormalByLineId{ SimpleFormatInt64Value(RowPBT.GetLineID()) });

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
				RecordSet.SaveToPurchaseHistoryHeaderRow(RowPBT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseHistoryHeader::UpdateRow(CSQLRowPurchaseHistoryHeader& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseHistoryHeader RecordSet(pDatabase,
			RSParams_PurchaseHistoryHeader_NormalByLineId{ SimpleFormatInt64Value(RowPBT.GetLineID()) });

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
				RecordSet.LoadFromPurchaseHistoryHeaderRow(RowPBT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseHistoryHeader::InsertRow(CSQLRowPurchaseHistoryHeader& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseHistoryHeader RecordSet(pDatabase,
			RSParams_PurchaseHistoryHeader_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPurchaseHistoryHeaderRow(RowPBT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};
		
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseHistoryHeader::InsertRowAndGetID(CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		Result.SetSQLError(SQLCRUD_ERR_RESET);
		Result.SetSQLResult(0);

		int nConnection = -1;
		pDatabase = SQLConnectionPool.GetPooledConnection(nConnection, pDatabase);

		if (pDatabase != NULL)
		{
			CString strInsert = "";
			strInsert += "INSERT INTO ";
			strInsert += SQLTableNames::PurchaseHistoryHeader;
			strInsert += " DEFAULT VALUES";
			pDatabase->ExecuteSQL(strInsert);

			CSQLRecordSetPurchaseHistoryHeader rsID(pDatabase, RSParams_PurchaseHistoryHeader_GetLastId{});

			CString strSelect = "";
			strSelect += "SELECT SCOPE_IDENTITY() ";
			strSelect += PurchaseHistoryHeader::LineID.Label;

			if (rsID.Open(CRecordset::forwardOnly, strSelect, CRecordset::executeDirect) == TRUE)
			{
				CSQLRowPurchaseHistoryHeader RowHeader;
				while (rsID.StepSelectAll(RowHeader) == TRUE)
				{
					Result.SetSQLError(SQLCRUD_ERR_NONE);
					Result.SetSQLResult(RowHeader.GetLineID());
				}

				rsID.Close();
			}
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPurchaseHistoryHeader::DeleteRow(CSQLRowPurchaseHistoryHeader& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPurchaseHistoryHeader RecordSet(pDatabase,
			RSParams_PurchaseHistoryHeader_NormalByLineId{ SimpleFormatInt64Value(RowPBT.GetLineID()) });

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

CSQLResultInfo CSQLRepositoryPurchaseHistoryHeader::RowExists(__int64 nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetPurchaseHistoryHeader RecordSet(pDatabase,
		RSParams_PurchaseHistoryHeader_NormalByLineId{ SimpleFormatInt64Value(nLineID) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************


