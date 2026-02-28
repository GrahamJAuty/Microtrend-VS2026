//**********************************************************************
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLNamespaces.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetScheme.h"
//**********************************************************************
#include "SQLRepositoryScheme.h"
//**********************************************************************

CSQLRepositoryScheme::CSQLRepositoryScheme()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryScheme::SelectRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetScheme RecordSet(pDatabase, RSParams_Scheme_NormalBySchemeNo{SQLRowSC.GetSchemeNo()});
		
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
				RecordSet.SaveToSchemeRow(SQLRowSC);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryScheme::UpdateRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetScheme RecordSet(pDatabase, RSParams_Scheme_NormalBySchemeNo{ SQLRowSC.GetSchemeNo() });

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
				RecordSet.LoadFromSchemeRow(SQLRowSC);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryScheme::InsertRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetScheme RecordSet(pDatabase, RSParams_Scheme_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromSchemeRow(SQLRowSC);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryScheme::DeleteRow(CSQLRowScheme& SQLRowSC, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetScheme RecordSet(pDatabase, RSParams_Scheme_NormalBySchemeNo{ SQLRowSC.GetSchemeNo() });

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

CSQLResultInfo CSQLRepositoryScheme::RowExists(int nSchemeNo, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLRecordSetScheme RecordSet(pDatabase, 
			RSParams_Scheme_NormalBySchemeNo{ nSchemeNo }, 
			TRUE);

		return RecordSet.CheckRowExists();
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

void CSQLRepositoryScheme::ImportScheme(CString strFilename)
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
	Columns.Add("SCHEMENO");
	Columns.Add("SCHEMENAME");
	
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
			strCommand.Format("DELETE FROM %s",
				SQLTableNames::Schemes);
				
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strScheme = "";
		while (fileImport.ReadString(strScheme) == TRUE)
		{
			CCSV csv(strScheme);

			int nSchemeNo = csv.GetInt(0);
			if ((nSchemeNo < Scheme::SchemeNo.Min) || (nSchemeNo > Scheme::SchemeNo.Max))
			{
				continue;
			}

			CSQLRowScheme RowScheme;
			RowScheme.SetSchemeNo(nSchemeNo);
			RowScheme.SetSchemeName(csv.GetString(1));

			InsertRow(RowScheme, NULL);
		}
	}
}

//**********************************************************************

void CSQLRepositoryScheme::GetSchemeNameList(CReportConsolidationArray<CSortedStringByInt>& arrayNames, int nCurrentNo, bool bAutoFirst10, CDatabase* pDatabase)
{
	arrayNames.RemoveAll();

	if (TRUE == bAutoFirst10)
	{
		for (int nSchemeNo = 1; nSchemeNo <= 10; nSchemeNo++)
		{
			CSortedStringByInt item;
			item.m_nItem = nSchemeNo;
			item.m_strItem.Format("Scheme %d", nSchemeNo);
			arrayNames.DirectAdd(item);
		}
	}

	CSQLRecordSetScheme RecordSet(pDatabase, RSParams_Scheme_NormalNoParams{});

	CSQLRowScheme RowSC;
	while (RecordSet.StepSelectAll(RowSC) == TRUE)
	{
		CSortedStringByInt item;
		item.m_nItem = RowSC.GetSchemeNo();
		item.m_strItem = RowSC.GetSchemeName();
		
		SolutionGlobalFunctions::TrimSpacesFromString(item.m_strItem, FALSE);

		if (item.m_strItem == "" )
		{
			item.m_strItem.Format("Scheme %d", item.m_nItem);
		}

		int nPos = 0;
		if (arrayNames.Find(item, nPos) == FALSE)
		{
			arrayNames.InsertAt(nPos, item);
		}
		else
		{
			arrayNames.SetAt(nPos, item);
		}
	}

	if ((nCurrentNo > 0) && (nCurrentNo <= Scheme::SchemeNo.Max))
	{
		CSortedStringByInt item;
		item.m_nItem = nCurrentNo;
		
		int nPos = 0;
		if (arrayNames.Find(item, nPos) == FALSE)
		{		
			item.m_strItem.Format("Scheme %d", nCurrentNo);
			arrayNames.InsertAt(nPos, item);
		}
	}
}

//**********************************************************************

void CSQLRepositoryScheme::SaveSchemeNameList(CReportConsolidationArray<CSortedStringByInt>& arrayNames)
{
	bool bDoneDelete = FALSE;

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CString strCommand = "";
			strCommand.Format("DELETE FROM %s",
				SQLTableNames::Schemes);

			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		for (int n = 0; n < arrayNames.GetSize(); n++)
		{
			CSortedStringByInt item;
			arrayNames.GetAt(n, item);
			CSQLRowScheme RowScheme;
			RowScheme.SetSchemeNo(item.m_nItem);
			RowScheme.SetSchemeName(item.m_strItem);
			InsertRow(RowScheme, NULL);
		}
	}
}

//**********************************************************************
