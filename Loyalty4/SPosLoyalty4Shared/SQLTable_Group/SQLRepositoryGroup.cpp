//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRepositoryGroup.h"
//**********************************************************************
		
CSQLRepositoryGroup::CSQLRepositoryGroup() : m_pTransaction(nullptr)
{
}

//**********************************************************************

CSQLRepositoryGroup::CSQLRepositoryGroup(CSQLTranBase* pTransaction) : m_pTransaction(pTransaction)
{
}

//**********************************************************************

CDatabase* CSQLRepositoryGroup::GetLegacyDatabase(CDatabase* pDatabase) const
{
	// Priority:
	// 1. Explicit parameter
	// 2. Transaction's database
	// 3. NULL (pooling)

	if (pDatabase != nullptr)
	{
		return pDatabase;
	}

	if (m_pTransaction != nullptr)
	{
		return m_pTransaction->GetDatabase();
	}

	return nullptr;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::SelectRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroup RecordSet(GetLegacyDatabase(pDatabase), RSParams_Group_NormalByGroupNo{SQLRowGroup.GetGroupNo()});
		
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
				RecordSet.SaveToGroupRow(SQLRowGroup);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::UpdateRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroup RecordSet(GetLegacyDatabase(pDatabase), RSParams_Group_NormalByGroupNo{ SQLRowGroup.GetGroupNo() });

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
				RecordSet.LoadFromGroupRow(SQLRowGroup);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::InsertRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroup RecordSet(GetLegacyDatabase(pDatabase), RSParams_Group_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromGroupRow(SQLRowGroup);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::DeleteRow(CSQLRowGroup& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroup RecordSet(GetLegacyDatabase(pDatabase), RSParams_Group_NormalByGroupNo{ SQLRowGroup.GetGroupNo() });

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

CSQLResultInfo CSQLRepositoryGroup::DeleteRow_Modern(CSQLRowGroup& row)
{
	IDatabase* pDb = GetModernDatabase();
	if (!pDb)
	{
		// Fallback to legacy for now
		return DeleteRow(row, NULL);
	}

	// In future: use modern backend
	// For now, this is just a placeholder
	return DeleteRow(row, NULL);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::RowExists(int nGroupNo, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLRecordSetGroup RecordSet(GetLegacyDatabase(pDatabase), 
			RSParams_Group_NormalByGroupNo{ nGroupNo }, 
			TRUE);

		return RecordSet.CheckRowExists();
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

