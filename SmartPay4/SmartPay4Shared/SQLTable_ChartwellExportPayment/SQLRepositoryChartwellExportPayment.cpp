//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetChartwellExportPayment.h"
//**********************************************************************
#include "SQLRepositoryChartwellExportPayment.h"
//**********************************************************************

CSQLRepositoryChartwellExportPayment::CSQLRepositoryChartwellExportPayment()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryChartwellExportPayment::SelectRow(CSQLRowChartwellExportPayment& RowCEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetChartwellExportPayment RecordSet(pDatabase, 
			RSParams_ChartwellExportPayment_NormalByTranDetail{
				SimpleFormatInt64Value(RowCEP.GetUserID()),
				SimpleFormatInt64Value(RowCEP.GetTransactionID()),
				RowCEP.GetTerminalNo(),
				RowCEP.GetDate()
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
				RecordSet.SaveToChartwellExportPaymentRow(RowCEP);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryChartwellExportPayment::UpdateRow(CSQLRowChartwellExportPayment& RowCEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetChartwellExportPayment RecordSet(pDatabase,
			RSParams_ChartwellExportPayment_NormalByTranDetail{
				SimpleFormatInt64Value(RowCEP.GetUserID()),
				SimpleFormatInt64Value(RowCEP.GetTransactionID()),
				RowCEP.GetTerminalNo(),
				RowCEP.GetDate()
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
				RecordSet.LoadFromChartwellExportPaymentRow(RowCEP);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryChartwellExportPayment::InsertRow(CSQLRowChartwellExportPayment& RowCEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetChartwellExportPayment RecordSet(pDatabase,
			RSParams_ChartwellExportPayment_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromChartwellExportPaymentRow(RowCEP);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryChartwellExportPayment::UpsertRow(CSQLRowChartwellExportPayment& RowCEP, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowCEP.GetUserID(), 
		RowCEP.GetTransactionID(), 
		RowCEP.GetTerminalNo(),
		RowCEP.GetDate(),
		pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowCEP, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowCEP, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryChartwellExportPayment::DeleteRow(CSQLRowChartwellExportPayment& RowCEP, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetChartwellExportPayment RecordSet(pDatabase,
			RSParams_ChartwellExportPayment_NormalByTranDetail{
				SimpleFormatInt64Value(RowCEP.GetUserID()),
				SimpleFormatInt64Value(RowCEP.GetTransactionID()),
				RowCEP.GetTerminalNo(),
				RowCEP.GetDate()
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

CSQLResultInfo CSQLRepositoryChartwellExportPayment::RowExists(__int64 nUserID, __int64 nTransactionID, int nTerminalNo, CString strDate, CDatabase* pDatabase)
{
	CSQLRecordSetChartwellExportPayment RecordSet(pDatabase,
		RSParams_ChartwellExportPayment_NormalByTranDetail{
			SimpleFormatInt64Value(nUserID),
			SimpleFormatInt64Value(nTransactionID),
			nTerminalNo,
			strDate
		}, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

