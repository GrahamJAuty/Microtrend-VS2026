//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\FilenameHandler.h"
#include "..\ServerData.h"
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLRecordSetEposTerminal.h"
//**********************************************************************
#include "SQLRepositoryEposTerminal.h"
//**********************************************************************

CSQLRepositoryEposTerminal::CSQLRepositoryEposTerminal()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEposTerminal::SelectRow(CSQLRowEposTerminal& RowET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEposTerminal RecordSet(pDatabase, 
			RSParams_EposTerminal_NormalByTerminalNo{ RowET.GetTerminalNo() });

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
				RecordSet.SaveToEposTerminalRow(RowET);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEposTerminal::UpdateRow(CSQLRowEposTerminal& RowET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEposTerminal RecordSet(pDatabase,
			RSParams_EposTerminal_NormalByTerminalNo{ RowET.GetTerminalNo() });

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
				RecordSet.LoadFromEposTerminalRow(RowET);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEposTerminal::InsertRow(CSQLRowEposTerminal& RowET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEposTerminal RecordSet(pDatabase,
			RSParams_EposTerminal_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromEposTerminalRow(RowET);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEposTerminal::UpsertRow(CSQLRowEposTerminal& RowET, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowET.GetTerminalNo(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowET, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowET, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryEposTerminal::DeleteRow(CSQLRowEposTerminal& RowET, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetEposTerminal RecordSet(pDatabase,
			RSParams_EposTerminal_NormalByTerminalNo{ RowET.GetTerminalNo() });

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

CSQLResultInfo CSQLRepositoryEposTerminal::RowExists(int nTerminalNo, CDatabase* pDatabase)
{
	CSQLRecordSetEposTerminal RecordSet(pDatabase,
		RSParams_EposTerminal_NormalByTerminalNo{ nTerminalNo }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryEposTerminal::ImportTerminalInfo(bool& bDeleteFile, bool& bCommitChanges, CDatabase* pDatabase)
{
	bDeleteFile = FALSE;
	bCommitChanges = FALSE;

	CSSFile fileImport;
	if (fileImport.Open(Server.GetEcrManagerImportEposTerminalFilename(), "rb") == FALSE)
	{
		return;
	}

	CString strHeader = "";
	if (fileImport.ReadString(strHeader) == FALSE)
	{
		bDeleteFile = TRUE;
		return;
	}

	CStringArray Columns;
	Columns.Add("TERMNO");
	Columns.Add("TERMNAME");
	
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
		bDeleteFile = TRUE;
		return;
	}

	bool bDoneDelete = FALSE;

	CString strCommand = "";
	strCommand.Format("DELETE FROM %s", SQLTableNames::EposTerminals);
	if (CSQLHelpers::ExecuteSQL(pDatabase, strCommand) != SQLCRUD_ERR_NONE)
	{
		return;
	}

	CString strEPInfo = "";
	while (fileImport.ReadString(strEPInfo) == TRUE)
	{
		CCSV csv(strEPInfo);
		CSQLRowEposTerminal RowET;
		RowET.SetTerminalNo(csv.GetInt(0));

		switch (SelectRow(RowET, pDatabase).GetSQLResult())
		{
		case SQLCRUD_ERR_NONE:
		case SQLCRUD_ERR_NOMATCH:
			break;

		default:
			return;
		}

		RowET.SetTerminalName(csv.GetString(1));

		if (UpsertRow(RowET, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}
	}

	bDeleteFile = TRUE;
	bCommitChanges = TRUE;
}

//**********************************************************************

bool CSQLRepositoryEposTerminal::UpgradeLegacyData(CDatabase* pDatabase, bool bQuitOnFail)
{
	CString strFilename = Filenames.GetLegacyEposTerminalFilename();

	CSSFile fileImport;
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return TRUE;
	}

	CString strBuffer = "";
	if (fileImport.ReadString(strBuffer) == FALSE)
	{
		CCSV csvLine(strBuffer);

		CSQLRowEposTerminal RowET;
		RowET.SetTerminalType(csvLine.GetInt(0));
		RowET.SetTerminalNo(csvLine.GetInt(1));
		RowET.SetLastUsedDate(csvLine.GetString(2));
		RowET.SetLastUsedTime(csvLine.GetString(3));
		RowET.SetCCNo(csvLine.GetInt(4));

		{
			CString strUserID = csvLine.GetString(5);
			if (strUserID.GetLength() > 0)
			{
				RowET.SetUserID(_atoi64(strUserID));
			}
			else
			{
				RowET.SetUserID(-1);
			}
		}

		RowET.SetCardValue(csvLine.GetDouble(6));
		RowET.SetTotalCounter(csvLine.GetInt(7));
		RowET.SetTotalInDrawer(csvLine.GetDouble(8));

		int nFlags = csvLine.GetInt(9);

		RowET.SetDisableFlag((nFlags & 0x01) != 0);
		RowET.SetDoorOpenFlag((nFlags & 0x02) != 0);
		RowET.SetTerminalOnFlag((nFlags & 0x04) != 0);
		RowET.SetInServiceFlag((nFlags & 0x08) != 0);

		if (UpsertRow(RowET, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			if (TRUE == bQuitOnFail)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//**********************************************************************

