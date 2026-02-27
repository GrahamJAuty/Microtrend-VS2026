//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryGroup.h"
//**********************************************************************

CSQLRepositoryGroup::CSQLRepositoryGroup()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::SelectRow(CSQLRowGroupFull& RowGF, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupFull RecordSet(pDatabase, 
			RSParams_Group_NormalByGroupNo{ RowGF.GetGroupNo() });

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
				RecordSet.SaveToGroupRow(RowGF);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::UpdateRow(CSQLRowGroupFull& RowGF, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupFull RecordSet(pDatabase,
			RSParams_Group_NormalByGroupNo{ RowGF.GetGroupNo() });

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
				RecordSet.LoadFromGroupRow(RowGF);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::InsertRow(CSQLRowGroupFull& RowGF, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupFull RecordSet(pDatabase,
			RSParams_Group_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromGroupRow(RowGF);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::DeleteRow(CSQLRowGroupFull& RowGF, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetGroupFull RecordSet(pDatabase,
			RSParams_Group_NormalByGroupNo{ RowGF.GetGroupNo() });

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

CSQLResultInfo CSQLRepositoryGroup::RowExists(int nGroupNo, CDatabase* pDatabase)
{
	CSQLRecordSetGroupFull RecordSet(pDatabase,
		RSParams_Group_NormalByGroupNo{ nGroupNo }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::CountRows(CDatabase* pDatabase)
{
	return CountRowsByRange(Group::GroupNo.Min, Group::GroupNo.Max, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryGroup::CountRowsByRange(int nGroupNoFrom, int nGroupNoTo, CDatabase* pDatabase)
{
	CSQLRecordSetGroupFull RecordSet(pDatabase,
		RSParams_Group_CountRowsByRange{ nGroupNoFrom, nGroupNoTo });

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************