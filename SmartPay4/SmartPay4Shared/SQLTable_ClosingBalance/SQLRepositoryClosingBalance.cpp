//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "..\DateRangeHelpers.h"
#include "..\SQLConnectionPool.h"
//**********************************************************************
#include "SQLRecordSetClosingBalance.h"
//**********************************************************************
#include "SQLRepositoryClosingBalance.h"
//**********************************************************************

CSQLRepositoryClosingBalance::CSQLRepositoryClosingBalance()
{
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetClosingBalance RecordSet(pDatabase, 
			RSParams_ClosingBalance_NormalByDateSingle{ RowCB.GetKeyDate() });

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
				RecordSet.SaveToClosingBalanceRow(RowCB);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectFirstRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	CString strSQL = "";
	strSQL.Format("SELECT TOP 1 %s FROM %s ORDER BY %s",
		(const char*)GetAllFieldNames(),
		SQLTableNames::ClosingBalances,
		ClosingBalance::KeyDate.Label);

	return SelectInternal(strSQL, RowCB, pDatabase);
}

//**********************************************************************

CString CSQLRepositoryClosingBalance::GetAllFieldNames()
{
	CCSV csv;
	csv.Add(ClosingBalance::KeyDate.Label);
	csv.Add(ClosingBalance::Date.Label);
	csv.Add(ClosingBalance::Time.Label);
	csv.Add(ClosingBalance::ArchiveDate.Label);
	csv.Add(ClosingBalance::ArchiveTime.Label);
	csv.Add(ClosingBalance::Purse1LiabilityAudit.Label);
	csv.Add(ClosingBalance::Purse1CreditAudit.Label);
	csv.Add(ClosingBalance::Purse2BalanceAudit.Label);
	csv.Add(ClosingBalance::Purse3LiabilityAudit.Label);
	csv.Add(ClosingBalance::Purse3CreditAudit.Label);
	csv.Add(ClosingBalance::Purse1LiabilityAdjust.Label);
	csv.Add(ClosingBalance::Purse1CreditAdjust.Label);
	csv.Add(ClosingBalance::Purse3LiabilityAdjust.Label);
	csv.Add(ClosingBalance::Purse3CreditAdjust.Label);
	csv.Add(ClosingBalance::HaveAdjust.Label);
	return csv.GetLine();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectLastRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	CString strSQL = "";
	strSQL.Format("SELECT TOP 1 %s FROM %s ORDER BY %s DESC",
		(const char*)GetAllFieldNames(),
		SQLTableNames::ClosingBalances,
		ClosingBalance::KeyDate.Label);

	return SelectInternal(strSQL, RowCB, pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectNearestRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	CSQLResultInfo infoResult;
	infoResult.SetSQLError(SQLCRUD_ERR_FAIL);
	infoResult.SetSQLResult(0);

	CString strKeyDate = RowCB.GetKeyDate();

	if (::TestNumeric(strKeyDate) && (strKeyDate.GetLength() == 8))
	{
		COleDateTime dateEarliest = COleDateTime(
			atoi(strKeyDate.Left(4)),
			atoi(strKeyDate.Mid(4, 2)),
			atoi(strKeyDate.Right(2)),
			0, 0, 0);

		if (dateEarliest.m_status == COleDateTime::valid)
		{
			CDateRangeHelpers DateRangeHelpers;
			DateRangeHelpers.AddDaysToTime(dateEarliest, -31);

			CString strEarliestDate = "";
			strEarliestDate.Format("%4.4d%2.2d%2.2d",
				dateEarliest.GetYear(),
				dateEarliest.GetMonth(),
				dateEarliest.GetDay() );

			CString strSQL = "";
			strSQL.Format("SELECT TOP 1 %s FROM %s WHERE ( %s <= %s and %s >= %s ) ORDER BY %s DESC",
				(const char*)GetAllFieldNames(),
				SQLTableNames::ClosingBalances,
				(const char*) ClosingBalance::KeyDate.Label,
				(const char*) strKeyDate,
				(const char*) ClosingBalance::KeyDate.Label,
				(const char*) strEarliestDate,
				(const char*) ClosingBalance::KeyDate.Label);

			infoResult = SelectInternal(strSQL, RowCB, pDatabase);
		}
	}
	
	return infoResult;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectPreviousRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	CSQLResultInfo infoResult;
	infoResult.SetSQLError(SQLCRUD_ERR_FAIL);
	infoResult.SetSQLResult(0);

	CString strKeyDate = RowCB.GetKeyDate();

	if (::TestNumeric(strKeyDate) && (strKeyDate.GetLength() == 8))
	{
		CString strSQL = "";
		strSQL.Format("SELECT TOP 1 %s FROM %s WHERE %s < %s ORDER BY %s DESC",
			(const char*)GetAllFieldNames(),
			SQLTableNames::ClosingBalances,
			(const char*)ClosingBalance::KeyDate.Label,
			(const char*)strKeyDate,
			(const char*)ClosingBalance::KeyDate.Label);

		infoResult = SelectInternal(strSQL, RowCB, pDatabase);
	}

	return infoResult;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectNextRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	CSQLResultInfo infoResult;
	infoResult.SetSQLError(SQLCRUD_ERR_FAIL);
	infoResult.SetSQLResult(0);

	CString strKeyDate = RowCB.GetKeyDate();

	if (::TestNumeric(strKeyDate) && (strKeyDate.GetLength() == 8))
	{
		CString strSQL = "";
		strSQL.Format("SELECT TOP 1 %s FROM %s WHERE %s > %s ORDER BY %s",
			(const char*)GetAllFieldNames(),	
			SQLTableNames::ClosingBalances,
			(const char*)ClosingBalance::KeyDate.Label,
			(const char*) strKeyDate,
			ClosingBalance::KeyDate.Label);

		infoResult = SelectInternal(strSQL, RowCB, pDatabase);
	}

	return infoResult;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::SelectInternal(CString strSQL, CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetClosingBalance RecordSet(pDatabase, RSParams_ClosingBalance_NormalNoParams{});
		
		if (RecordSet.Open(CRecordset::forwardOnly, strSQL, CRecordset::executeDirect) == FALSE)
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
				RecordSet.SaveToClosingBalanceRow(RowCB);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::UpdateRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetClosingBalance RecordSet(pDatabase,
			RSParams_ClosingBalance_NormalByDateSingle{ RowCB.GetKeyDate() });

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
				RecordSet.LoadFromClosingBalanceRow(RowCB);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::InsertRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetClosingBalance RecordSet(pDatabase,
			RSParams_ClosingBalance_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromClosingBalanceRow(RowCB);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::UpsertRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowCB.GetKeyDate(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowCB, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowCB, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::DeleteRow(CSQLRowClosingBalance& RowCB, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetClosingBalance RecordSet(pDatabase,
			RSParams_ClosingBalance_NormalByDateSingle{ RowCB.GetKeyDate() });

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

CSQLResultInfo CSQLRepositoryClosingBalance::RowExists(CString strDate, CDatabase* pDatabase)
{
	CSQLRecordSetClosingBalance RecordSet(pDatabase, 
		RSParams_ClosingBalance_NormalByDateSingle{ strDate }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryClosingBalance::CountRows(CDatabase* pDatabase)
{
	CSQLRecordSetClosingBalance RecordSet(pDatabase,
		RSParams_ClosingBalance_CountRowsByRange{ "00000000", "99999999" });	

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************

void CSQLRepositoryClosingBalance::ImportClosingBalance(CString strFilename)
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
	Columns.Add("K1");
	Columns.Add("D1");
	Columns.Add("T1");
	Columns.Add("D2");
	Columns.Add("T2");
	Columns.Add("A1");
	Columns.Add("A2");
	Columns.Add("A3");
	Columns.Add("C1");
	Columns.Add("C3");

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

	if (nMatched < 8)
	{
		return;
	}

	bool bDoneDelete = FALSE;

	{
		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			bDoneDelete = (CSQLHelpers::ExecuteSQL(pDatabase, "DELETE FROM ClosingBalance") == SQLCRUD_ERR_NONE);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	if (TRUE == bDoneDelete)
	{
		CString strClosingBalance = "";
		while (fileImport.ReadString(strClosingBalance) == TRUE)
		{
			CCSV csv(strClosingBalance);
			CSQLRowClosingBalance RowCB;
			RowCB.SetKeyDate(csv.GetString(0));
			RowCB.SetDate(csv.GetString(1));
			RowCB.SetTime(csv.GetString(2));
			RowCB.SetArchiveDate(csv.GetString(3));
			RowCB.SetArchiveTime(csv.GetString(4));
			RowCB.SetPurse1LiabilityAudit(csv.GetDouble(5));
			RowCB.SetPurse2BalanceAudit(csv.GetDouble(6));
			RowCB.SetPurse3LiabilityAudit(csv.GetDouble(7));
			
			if (nMatched >= 10)
			{
				RowCB.SetPurse1CreditAudit(csv.GetDouble(8));
				RowCB.SetPurse3CreditAudit(csv.GetDouble(9));
			}

			InsertRow(RowCB, NULL);
		}
	}
}

//**********************************************************************
