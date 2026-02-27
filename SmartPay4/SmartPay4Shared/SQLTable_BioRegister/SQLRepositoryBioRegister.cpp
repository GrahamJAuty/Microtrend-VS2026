//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryBioRegister.h"
//**********************************************************************

CSQLRepositoryBioRegister::CSQLRepositoryBioRegister()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBioRegister::SelectRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetBioRegister RecordSet(pDatabase,
			RSParams_BioRegister_NormalByUserId{ FormatInt64Value(RowBio.GetUserID()) });

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
				RecordSet.SaveToBioRegisterRow(RowBio);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBioRegister::UpdateRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetBioRegister RecordSet(pDatabase,
			RSParams_BioRegister_NormalByUserId{ FormatInt64Value(RowBio.GetUserID()) });

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
				RecordSet.LoadFromBioRegisterRow(RowBio);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBioRegister::InsertRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetBioRegister RecordSet(pDatabase, RSParams_BioRegister_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromBioRegisterRow(RowBio);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBioRegister::DeleteRow(CSQLRowBioRegister& RowBio, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetBioRegister RecordSet(pDatabase,
			RSParams_BioRegister_NormalByUserId{ FormatInt64Value(RowBio.GetUserID()) });

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

CSQLResultInfo CSQLRepositoryBioRegister::RowExists(__int64 nUserID, CDatabase* pDatabase)
{
	CSQLRecordSetBioRegister RecordSet(pDatabase,
		RSParams_BioRegister_NormalByUserId{ FormatInt64Value(nUserID) },
		TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryBioRegister::GetSortedUserIDList(CReportConsolidationArray<CSortedInt64Item>& arrayUserIDs, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		arrayUserIDs.RemoveAll();

		CSQLRecordSetBioRegister RecordSet(pDatabase, RSParams_BioRegister_NormalNoParams{});

		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			while (RecordSet.IsEOF() == FALSE)
			{
				CSortedInt64Item item;
				item.m_nItem = RecordSet.m_MD.m_nUserID;
				arrayUserIDs.DirectAdd(item);
				RecordSet.MoveNext();
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************
