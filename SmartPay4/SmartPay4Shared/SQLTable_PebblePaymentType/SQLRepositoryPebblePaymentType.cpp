//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetPebblePaymentType.h"
//**********************************************************************
#include "SQLRepositoryPebblePaymentType.h"
//**********************************************************************

CSQLRepositoryPebblePaymentType::CSQLRepositoryPebblePaymentType()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePaymentType::SelectRow(CSQLRowPebblePaymentType& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePaymentType RecordSet(pDatabase, 
			RSParams_PebblePaymentType_NormalByPaymentType{ RowPPT.GetSPOSPaymentType() });

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
				RecordSet.SaveToPebblePaymentTypeRow(RowPPT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePaymentType::UpdateRow(CSQLRowPebblePaymentType& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePaymentType RecordSet(pDatabase,
			RSParams_PebblePaymentType_NormalByPaymentType{ RowPPT.GetSPOSPaymentType() });

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
				RecordSet.LoadFromPebblePaymentTypeRow(RowPPT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePaymentType::InsertRow(CSQLRowPebblePaymentType& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePaymentType RecordSet(pDatabase,
			RSParams_PebblePaymentType_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebblePaymentTypeRow(RowPPT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePaymentType::UpsertRow(CSQLRowPebblePaymentType& RowPPT, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPPT.GetSPOSPaymentType(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPPT, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPPT, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebblePaymentType::DeleteRow(CSQLRowPebblePaymentType& RowPPT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebblePaymentType RecordSet(pDatabase,
			RSParams_PebblePaymentType_NormalByPaymentType{ RowPPT.GetSPOSPaymentType() });

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

CSQLResultInfo CSQLRepositoryPebblePaymentType::RowExists(int nPaymentNo, CDatabase* pDatabase)
{
	CSQLRecordSetPebblePaymentType RecordSet(pDatabase,
		RSParams_PebblePaymentType_NormalByPaymentType{ nPaymentNo }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************
