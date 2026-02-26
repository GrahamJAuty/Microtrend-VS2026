//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRepositoryGroupTopupBonus.h"
//**********************************************************************

CSQLRepositoryGroupTopupBonus::CSQLRepositoryGroupTopupBonus()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroupTopupBonus::SelectRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupTopupBonus RecordSet(pDatabase, 
			RSParams_GroupTopupBonus_NormalByFullKey{
				SQLRowGroup.GetGroupNo(),
				SQLRowGroup.GetPurseNo(),
				SQLRowGroup.GetTopupNo()
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
				RecordSet.SaveToGroupTopupBonusRow(SQLRowGroup);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroupTopupBonus::UpdateRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupTopupBonus RecordSet(pDatabase,
			RSParams_GroupTopupBonus_NormalByFullKey{
				SQLRowGroup.GetGroupNo(),
				SQLRowGroup.GetPurseNo(),
				SQLRowGroup.GetTopupNo()
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
				RecordSet.LoadFromGroupTopupBonusRow(SQLRowGroup);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroupTopupBonus::InsertRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupTopupBonus RecordSet(pDatabase, RSParams_GroupTopupBonus_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromGroupTopupBonusRow(SQLRowGroup);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroupTopupBonus::UpsertRow(CSQLRowGroupTopupBonus& RowGroup, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowGroup.GetGroupNo(), RowGroup.GetPurseNo(), RowGroup.GetTopupNo(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0)
		{
			Result = InsertRow(RowGroup, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowGroup, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroupTopupBonus::DeleteRow(CSQLRowGroupTopupBonus& SQLRowGroup, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupTopupBonus RecordSet(pDatabase,
			RSParams_GroupTopupBonus_NormalByFullKey{
				SQLRowGroup.GetGroupNo(),
				SQLRowGroup.GetPurseNo(),
				SQLRowGroup.GetTopupNo()
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

CSQLResultInfo CSQLRepositoryGroupTopupBonus::RowExists(int nGroupNo, int nPurseNo, int nTopupNo, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLRecordSetGroupTopupBonus RecordSet(pDatabase, 
			RSParams_GroupTopupBonus_NormalByFullKey{
				nGroupNo,
				nPurseNo,
				nTopupNo
			},
			TRUE);

		return RecordSet.CheckRowExists();
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

