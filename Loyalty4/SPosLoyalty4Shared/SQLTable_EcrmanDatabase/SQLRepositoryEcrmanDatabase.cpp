//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetEcrmanDatabase.h"
//**********************************************************************
#include "SQLRepositoryEcrmanDatabase.h"
//**********************************************************************
		
CSQLRepositoryEcrmanDatabase::CSQLRepositoryEcrmanDatabase()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEcrmanDatabase::SelectRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEcrmanDatabase RecordSet(pDatabase, 
			RSParams_EcrmanDatabase_NormalByDbNo{SQLRowOS.GetDbNo()});
			
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
				RecordSet.SaveToEcrmanDatabaseRow(SQLRowOS);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEcrmanDatabase::UpdateRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEcrmanDatabase RecordSet(pDatabase,
			RSParams_EcrmanDatabase_NormalByDbNo{ SQLRowOS.GetDbNo() });

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
			RecordSet.LoadFromEcrmanDatabaseRow(SQLRowOS);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEcrmanDatabase::InsertRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEcrmanDatabase RecordSet(pDatabase, RSParams_EcrmanDatabase_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromEcrmanDatabaseRow(SQLRowOS);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEcrmanDatabase::DeleteRow(CSQLRowEcrmanDatabase& SQLRowOS, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEcrmanDatabase RecordSet(pDatabase,
			RSParams_EcrmanDatabase_NormalByDbNo{ SQLRowOS.GetDbNo() });

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
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

void CSQLRepositoryEcrmanDatabase::ImportEcrmanDatabase(CString strFilename)
{
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
	Columns.Add("DBNO");
	Columns.Add("DBNAME");
	
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
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, "DELETE FROM EcrmanDatabase") == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strEcrmanDatabase = "";
		while (fileImport.ReadString(strEcrmanDatabase) == TRUE)
		{
			CCSV csv(strEcrmanDatabase);

			int nDbNo = csv.GetInt(0);
			if ((nDbNo < EcrmanDatabase::DbNo.Min) || (nDbNo > EcrmanDatabase::DbNo.Max))
			{
				continue;
			}

			CSQLRowEcrmanDatabase RowEcrmanDatabase;
			RowEcrmanDatabase.SetDbNo(nDbNo);
			RowEcrmanDatabase.SetDbName(csv.GetString(1));

			InsertRow(RowEcrmanDatabase, NULL);
		}
	}
}

//**********************************************************************

void CSQLRepositoryEcrmanDatabase::GetDatabaseNameList(CStringArray& arrayDbNames, CDatabase* pDatabase)
{
	arrayDbNames.RemoveAll();

	for (int nDbNo = 1; nDbNo <= PLU_DBNO_COUNT; nDbNo++)
	{
		CString strName = "";
		strName.Format("Database %d", nDbNo);
		arrayDbNames.Add(strName);
	}

	CSQLRecordSetEcrmanDatabase RecordSet(NULL, RSParams_EcrmanDatabase_NormalNoParams{});

	CSQLRowEcrmanDatabase RowDb;
	while (RecordSet.StepSelectAll(RowDb) == TRUE)
	{
		int nDbNo = RowDb.GetDbNo();
		if ((nDbNo >= 1) && (nDbNo < arrayDbNames.GetSize()))
		{
			CString strName = RowDb.GetDbName();
			TrimSpacesFromString(strName, FALSE);

			if (strName != "")
			{
				arrayDbNames.SetAt(nDbNo - 1, strName);
			}
		}
	}

}

//**********************************************************************
