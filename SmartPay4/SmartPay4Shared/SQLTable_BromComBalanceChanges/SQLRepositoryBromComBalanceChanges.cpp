//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryBromComBalanceChanges.h"
//**********************************************************************

CSQLRepositoryBromComBalanceChanges::CSQLRepositoryBromComBalanceChanges()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComBalanceChanges::SelectRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComBalanceChanges RecordSet(pDatabase,
			RSParams_BromComBalanceChanges_NormalByPersonId{ RowBC.GetPersonID() });

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
				RecordSet.SaveToBromComBalanceChangesRow(RowBC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComBalanceChanges::UpdateRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComBalanceChanges RecordSet(pDatabase,
			RSParams_BromComBalanceChanges_NormalByPersonId{ RowBC.GetPersonID() });

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
				RecordSet.LoadFromBromComBalanceChangesRow(RowBC);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComBalanceChanges::InsertRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComBalanceChanges RecordSet(pDatabase, RSParams_BromComBalanceChanges_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromBromComBalanceChangesRow(RowBC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComBalanceChanges::DeleteRow(CSQLRowBromComBalanceChanges& RowBC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetBromComBalanceChanges RecordSet(pDatabase,
			RSParams_BromComBalanceChanges_NormalByPersonId{ RowBC.GetPersonID() });

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

CSQLResultInfo CSQLRepositoryBromComBalanceChanges::RowExists(int nPersonID, CDatabase* pDatabase)
{
	CSQLRecordSetBromComBalanceChanges RecordSet(pDatabase,
		RSParams_BromComBalanceChanges_NormalByPersonId{ nPersonID }, 
		TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBromComBalanceChanges::CountRows(CDatabase* pDatabase)
{
	CSQLRecordSetBromComBalanceChanges RecordSet(pDatabase,
		RSParams_BromComBalanceChanges_CountRowsByRange{
			BromComBalanceChanges::PersonID.Min,
			BromComBalanceChanges::PersonID.Max });
	
	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************