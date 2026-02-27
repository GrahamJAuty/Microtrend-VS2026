//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetPebbleExportPayment.h"
//**********************************************************************
#include "SQLRepositoryPebbleExportPayment.h"
//**********************************************************************

CSQLRepositoryPebbleExportPayment::CSQLRepositoryPebbleExportPayment()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportPayment::SelectRow(CSQLRowPebbleExportPayment& RowPEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportPayment RecordSet(pDatabase, 
			RSParams_PebbleExportPayment_NormalByFullParams{
				SimpleFormatInt64Value(RowPEP.GetUserID()),
				RowPEP.GetDate(),
				RowPEP.GetTime(),
				RowPEP.GetCCNo()
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
				RecordSet.SaveToPebbleExportPaymentRow(RowPEP);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportPayment::UpdateRow(CSQLRowPebbleExportPayment& RowPEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportPayment RecordSet(pDatabase,
			RSParams_PebbleExportPayment_NormalByFullParams{
				SimpleFormatInt64Value(RowPEP.GetUserID()),
				RowPEP.GetDate(),
				RowPEP.GetTime(),
				RowPEP.GetCCNo()
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
				RecordSet.LoadFromPebbleExportPaymentRow(RowPEP);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportPayment::InsertRow(CSQLRowPebbleExportPayment& RowPEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportPayment RecordSet(pDatabase,
			RSParams_PebbleExportPayment_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebbleExportPaymentRow(RowPEP);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportPayment::UpsertRow(CSQLRowPebbleExportPayment& RowPEP, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowPEP.GetUserID(), RowPEP.GetDate(), RowPEP.GetTime(), RowPEP.GetCCNo(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowPEP, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowPEP, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportPayment::DeleteRow(CSQLRowPebbleExportPayment& RowPEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetPebbleExportPayment RecordSet(pDatabase,
			RSParams_PebbleExportPayment_NormalByFullParams{
				SimpleFormatInt64Value(RowPEP.GetUserID()),
				RowPEP.GetDate(),
				RowPEP.GetTime(),
				RowPEP.GetCCNo()
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

CSQLResultInfo CSQLRepositoryPebbleExportPayment::RowExists(__int64 nUserID, CString strDate, CString strTime, int nCCNo, CDatabase* pDatabase)
{
	CSQLRecordSetPebbleExportPayment RecordSet(pDatabase,
		RSParams_PebbleExportPayment_NormalByFullParams{
			SimpleFormatInt64Value(nUserID),
			strDate,
			strTime,
			nCCNo 
		}, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

