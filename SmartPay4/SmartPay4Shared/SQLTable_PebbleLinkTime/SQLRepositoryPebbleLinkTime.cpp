//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLWhereBuilder.h"
//**********************************************************************
#include "SQLRepositoryPebbleLinkTime.h"
//**********************************************************************

CSQLRepositoryPebbleLinkTime::CSQLRepositoryPebbleLinkTime()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleLinkTime::SelectRow(CSQLRowPebbleLinkTime& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleLinkTime RecordSet(pDatabase,
			RSParams_PebbleLinkTime_NormalByLineId{ SimpleFormatInt64Value(RowPBT.GetLineID()) });

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
				RecordSet.SaveToPebbleLinkTimeRow(RowPBT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleLinkTime::UpdateRow(CSQLRowPebbleLinkTime& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleLinkTime RecordSet(pDatabase,
			RSParams_PebbleLinkTime_NormalByLineId{ SimpleFormatInt64Value(RowPBT.GetLineID()) });

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
				RecordSet.LoadFromPebbleLinkTimeRow(RowPBT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleLinkTime::InsertRow(CSQLRowPebbleLinkTime& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleLinkTime RecordSet(pDatabase,
			RSParams_PebbleLinkTime_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebbleLinkTimeRow(RowPBT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleLinkTime::DeleteRow(CSQLRowPebbleLinkTime& RowPBT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleLinkTime RecordSet(pDatabase,
			RSParams_PebbleLinkTime_NormalByLineId{ SimpleFormatInt64Value(RowPBT.GetLineID()) });

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

CSQLResultInfo CSQLRepositoryPebbleLinkTime::RowExists(__int64 nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetPebbleLinkTime RecordSet(pDatabase,
		RSParams_PebbleLinkTime_NormalByLineId{ SimpleFormatInt64Value(nLineID) }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

