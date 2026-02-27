//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
//**********************************************************************
#include "SQLRepositoryPebbleExportTransactionSent.h"
//**********************************************************************

CSQLRepositoryPebbleExportTransactionSent::CSQLRepositoryPebbleExportTransactionSent()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransactionSent::SelectRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetPebbleExportTransactionSent RecordSet(pDatabase,
			RSParams_PebbleTransactionSent_NormalByLineId(SimpleFormatInt64Value(SQLRowBC.GetLineID())));

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
				RecordSet.SaveToPebbleExportTransactionSentRow(SQLRowBC);

				RecordSet.MoveNext();
				if (RecordSet.IsEOF() == FALSE)
				{
					Result.SetSQLError(SQLCRUD_ERR_DUPLICATE);
				}
			}

			RecordSet.Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransactionSent::UpdateRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetPebbleExportTransactionSent RecordSet(pDatabase,
			RSParams_PebbleTransactionSent_NormalByLineId(SimpleFormatInt64Value(SQLRowBC.GetLineID())));

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else if (RecordSet.IsEOF() == TRUE)
		{
			Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
		}
		else
		{
			RecordSet.Edit();
			RecordSet.LoadFromPebbleExportTransactionSentRow(SQLRowBC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransactionSent::InsertRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetPebbleExportTransactionSent RecordSet(pDatabase, RSParams_PebbleTransactionSent_NormalNoParams());
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPebbleExportTransactionSentRow(SQLRowBC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransactionSent::DeleteRow(CSQLRowPebbleExportTransactionSent& SQLRowBC, CDatabase* pDatabase)
{
	CSQLResultInfo Result;

	try
	{
		CSQLRecordSetPebbleExportTransactionSent RecordSet(pDatabase,
			RSParams_PebbleTransactionSent_NormalByLineId(SimpleFormatInt64Value(SQLRowBC.GetLineID())));

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else if (RecordSet.IsEOF() == FALSE)
		{
			RecordSet.Delete();
			RecordSet.Close();
			Result.SetSQLError(SQLCRUD_ERR_NONE);
		}
		else
		{
			Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
		}
	}
	catch (CDBException* pe)
	{
		pe->Delete();
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_DB);
	}
	catch (...)
	{
		Result.SetSQLError(SQLCRUD_ERR_EXCEPT_MISC);
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPebbleExportTransactionSent::RowExists(__int64 nLineID, CDatabase* pDatabase)
{
	CSQLRecordSetPebbleExportTransactionSent RecordSet(pDatabase,
		RSParams_PebbleTransactionSent_NormalByLineId{ SimpleFormatInt64Value(nLineID) },
		TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************
