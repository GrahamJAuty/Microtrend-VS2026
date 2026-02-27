//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLRecordSetOperatorName.h"
//**********************************************************************
#include "SQLRepositoryOperatorName.h"
//**********************************************************************

CSQLRepositoryOperatorName::CSQLRepositoryOperatorName()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOperatorName::SelectRow(CSQLRowOperatorName& RowON, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOperatorName RecordSet(pDatabase, 
			RSParams_OperatorName_NormalByOperatorNo{ RowON.GetOperatorNo() });

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
				RecordSet.SaveToOperatorNameRow(RowON);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOperatorName::UpdateRow(CSQLRowOperatorName& RowON, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOperatorName RecordSet(pDatabase,
			RSParams_OperatorName_NormalByOperatorNo{ RowON.GetOperatorNo() });

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
				RecordSet.LoadFromOperatorNameRow(RowON);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOperatorName::InsertRow(CSQLRowOperatorName& RowON, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOperatorName RecordSet(pDatabase,
			RSParams_OperatorName_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromOperatorNameRow(RowON);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOperatorName::UpsertRow(CSQLRowOperatorName& RowON, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowON.GetOperatorNo(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowON, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowON, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryOperatorName::DeleteRow(CSQLRowOperatorName& RowON, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetOperatorName RecordSet(pDatabase,
			RSParams_OperatorName_NormalByOperatorNo{ RowON.GetOperatorNo() });

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

CSQLResultInfo CSQLRepositoryOperatorName::RowExists(int nOperatorNo, CDatabase* pDatabase)
{
	CSQLRecordSetOperatorName RecordSet(pDatabase,
		RSParams_OperatorName_NormalByOperatorNo{ nOperatorNo }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryOperatorName::ImportOperatorInfo(CString strFilename)
{
	/*
	CSSFile fileImport;
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return;
	}

	CString strHeader = "";
	if (fileImport.ReadString(strHeader) == FALSE)
	{
		return;
	}

	CStringArray Columns;
	Columns.Add("OPNO");
	Columns.Add("OPNAME");
	
	strHeader.MakeUpper();
	CCSV csv(strHeader);

	int nMatched = 0;
	for (; nMatched < Columns.GetSize() && nMatched < csv.GetSize(); nMatched++)
	{
		if (Columns.GetAt(nMatched) != csv.GetString(nMatched))
		{
			break;
		}
	}

	if (nMatched < 2)
	{
		return;
	}

	bool bDoneDelete = FALSE;

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CString strCommand = "";
			strCommand.Format("DELETE FROM %s", SQLTableNames::OperatorNames);
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strOPInfo = "";
		while (fileImport.ReadString(strOPInfo) == TRUE)
		{
			CCSV csv(strOPInfo);
			CSQLRowOperatorName RowON;
			RowON.SetOperatorNo(csv.GetInt(0));
			RowON.SetOperatorName(csv.GetString(1));
			
			InsertRow(RowON, NULL);
		}
	}
	*/
}

//**********************************************************************
