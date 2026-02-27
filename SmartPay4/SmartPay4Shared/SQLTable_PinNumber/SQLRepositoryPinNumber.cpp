//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SQLConnectionPool.h"
#include "..\SQLDefines.h"
#include "..\SQLDb.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRepositoryPINNumber.h"
//**********************************************************************

CSQLRepositoryPINNumber::CSQLRepositoryPINNumber()
{
}				

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::SelectRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetPINNumber RecordSet(pDatabase,
			RSParams_PINNumber_NormalByPINNumber{ RowPIN.GetPINNumber() });

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
				RecordSet.SaveToPINNumberRow(RowPIN);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::SelectRowByUserID(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
		{
			CSQLResultInfo Result;

			CSQLRecordSetPINNumber RecordSet(pDatabase,
				RSParams_PINNumber_NormalByUserID{ RowPIN.GetUserID() });

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
					RecordSet.SaveToPINNumberRow(RowPIN);
					Result.SetSQLError(SQLCRUD_ERR_NONE);
				}

				RecordSet.Close();
			}

			return Result;
		};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::UpdateRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetPINNumber RecordSet(pDatabase,
			RSParams_PINNumber_NormalByPINNumber{ RowPIN.GetPINNumber() });

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
				RecordSet.LoadFromPINNumberRow(RowPIN);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}

			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::InsertRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetPINNumber RecordSet(pDatabase, RSParams_PINNumber_NormalNoParams{});
		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromPINNumberRow(RowPIN);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}

		return Result;
	};

	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::DeleteRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;

		CSQLRecordSetPINNumber RecordSet(pDatabase,
			RSParams_PINNumber_NormalByPINNumber{ RowPIN.GetPINNumber() });

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

CSQLResultInfo CSQLRepositoryPINNumber::RowExists(CString strPINNumber, CDatabase* pDatabase)
{
	CSQLRecordSetPINNumber RecordSet(pDatabase,
		RSParams_PINNumber_NormalByPINNumber{ strPINNumber },
		TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::CountRows(CDatabase* pDatabase)
{
	return CountRowsByRange("0000", "9999", pDatabase);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryPINNumber::CountRowsByRange(CString strPINNumberFrom, CString strPINNumberTo, CDatabase* pDatabase)
{
	CSQLRecordSetPINNumber RecordSet(pDatabase,
		RSParams_PINNumber_CountRowsByRange{ strPINNumberFrom, strPINNumberTo });

	return RecordSet.RSCountRowsByRange();
}

//**********************************************************************

bool CSQLRepositoryPINNumber::LinkAccountIDToPINNumber(CString strPINNumber, __int64 nUserID, CDatabase* pDatabase)
{
	int nConnection = -1;

	if (pDatabase == NULL)
	{
		pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (pDatabase == NULL)
		{
			return FALSE;
		}
	}

	bool bResult = LinkAccountIDToPINNumberInternal(strPINNumber, nUserID, pDatabase);

	if (nConnection != -1)
	{
		SQLConnectionPool.FreeConnection(nConnection);
	}

	return bResult;
}

//**********************************************************************

bool CSQLRepositoryPINNumber::LinkAccountIDToPINNumberInternal(CString strPINNumber, __int64 nUserID, CDatabase* pDatabase)
{
	bool bRandomPINNumberGenerated = FALSE;

	if (strPINNumber == "")
	{
		CString strError = "";
		strPINNumber = GetNewPINNumber(strError, pDatabase);
		bRandomPINNumberGenerated = TRUE;
	}

	if (CSQLRowPINNumber::IsValidPINNumber(strPINNumber) == FALSE)
	{
		return (bRandomPINNumberGenerated) ? FALSE : TRUE;
	}

	//WE ALREADY HAVE DATABASE POINTER, SO CAN GO STRAIGHT TO INTERNAL FUNCTION
	if (RemoveAccountIDLinkInternal(nUserID, pDatabase) == FALSE)
	{
		return FALSE;
	}

	CString strUsername = "";
	CSQLRepositoryAccount repoAccount;
	CSQLRowAccountFull rowAccount;
	rowAccount.SetUserID(nUserID);
	if ( repoAccount.SelectRow(rowAccount, pDatabase).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		strUsername = rowAccount.GetUsername();
	}

	CSQLRepositoryPINNumber repoPINNumber;
	CSQLRowPINNumber rowPINNumber;
	rowPINNumber.SetPINNumber(strPINNumber);
	rowPINNumber.SetUserID(FormatInt64Value(nUserID));
	rowPINNumber.SetUsername(strUsername);
	rowPINNumber.SetFirstDate("");
	rowPINNumber.SetFirstTime("");
	rowPINNumber.SetLastDate("");
	rowPINNumber.SetLastTime("");
	rowPINNumber.SetUseCounter(0);
	if ( repoPINNumber.UpdateRow(rowPINNumber, pDatabase).GetSQLError() != SQLCRUD_ERR_NONE )
	{
		return FALSE;
	}
	
	return TRUE;
}

//**********************************************************************

CString CSQLRepositoryPINNumber::GetNewPINNumber(CString& strError, CDatabase* pDatabase)
{
	int nConnection = -1;

	if (pDatabase == NULL)
	{
		pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (pDatabase == NULL)
		{
			return FALSE;
		}
	}

	CString strResult = GetNewPINNumberInternal(strError, pDatabase);

	if (nConnection != -1)
	{
		SQLConnectionPool.FreeConnection(nConnection);
	}

	return strResult;
}

//**********************************************************************

CString CSQLRepositoryPINNumber::GetNewPINNumberInternal(CString& strError, CDatabase* pDatabase)
{
	strError = "";

	CSQLRecordSetConnectionPool rs(pDatabase);

	CString strSQL = "";
	strSQL.Format("SELECT TOP 1 %s FROM %s WHERE %s IS NULL ORDER BY NEWID()",
		PINNumber::PINNumber.Label,
		SQLTableNames::PINNumber,
		PINNumber::UserID.Label);

	if (!rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly))
	{
		strError = "Unable to access the PIN number table.";
		return 0;
	}

	if (rs.IsEOF())
	{
		strError = "All PIN numbers are already in use.\n\nNo new PIN can be assigned.";
		rs.Close();
		return "";
	}

	short nIndex = 0;
	CString strResult = "";
	rs.GetFieldValue(nIndex, strResult);
	rs.Close();

	if ( CSQLRowPINNumber::IsValidPINNumber(strResult) == FALSE )
	{
		strError = "Invalid PIN number retrieved from the database.";
	}

	return strResult;
}

//**********************************************************************

bool CSQLRepositoryPINNumber::RemoveAccountIDLink(__int64 nUserID, CDatabase* pDatabase)
{
	int nConnection = -1;
	if (pDatabase == NULL)
	{
		pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);
		if (pDatabase == NULL)
		{
			return FALSE;
		}
	}

	bool bResult = RemoveAccountIDLinkInternal(nUserID, pDatabase);
	if (nConnection != -1)
	{
		SQLConnectionPool.FreeConnection(nConnection);
	}

	return bResult;
}

//**********************************************************************

bool CSQLRepositoryPINNumber::RemoveAccountIDLinkInternal(__int64 nUserID, CDatabase* pDatabase)
{
	CString strSQL = "";
	strSQL.Format("UPDATE %s SET %s = NULL, %s = NULL, %s = NULL, %s = NULL, %s = NULL, %s = NULL, %s = 0 WHERE %s = %I64d",
		(const char*)SQLTableNames::PINNumber,
		(const char*)PINNumber::UserID.Label,
		(const char*)Account::Username.Label,
		(const char*)PINNumber::FirstDate.Label,
		(const char*)PINNumber::FirstTime.Label,
		(const char*)PINNumber::LastDate.Label,
		(const char*)PINNumber::LastTime.Label,
		(const char*)PINNumber::UseCounter.Label,
		(const char*)Account::UserID.Label,
		nUserID);

	if (CSQLHelpers::ExecuteSQL(pDatabase, strSQL) != SQLCRUD_ERR_NONE)
	{
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************

void CSQLRepositoryPINNumber::UpdatePINNumberUsage(CString strPINNumber, CDatabase* pDatabase)
{
	int nConnection = -1;
	if (pDatabase == NULL)
	{
		pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);
		if (pDatabase == NULL)
		{
			return;
		}
	}

	UpdatePINNumberUsageInternal(strPINNumber, pDatabase);
	
	if (nConnection != -1)
	{
		SQLConnectionPool.FreeConnection(nConnection);
	}
}

//**********************************************************************

void CSQLRepositoryPINNumber::UpdatePINNumberUsageInternal(CString strPINNumber, CDatabase* pDatabase)
{
	COleDateTime currentDateTime = COleDateTime::GetCurrentTime();

	CString strDateNow = "";
	strDateNow.Format("%4.4d%2.2d%2.2d",
		currentDateTime.GetYear(),
		currentDateTime.GetMonth(),
		currentDateTime.GetDay());

	CString strTimeNow = "";
	strTimeNow.Format("%2.2d%2.2d%2.2d",
		currentDateTime.GetHour(),
		currentDateTime.GetMinute(),
		currentDateTime.GetSecond());

	CString strSQL;
	strSQL.Format(
		"UPDATE %s SET "
		"%s = %s + 1, "
		"%s = '%s', "
		"%s = '%s', "
		"%s = CASE WHEN %s IS NULL THEN '%s' ELSE %s END, "
		"%s = CASE WHEN %s IS NULL THEN '%s' ELSE %s END "
		"WHERE %s = '%s'",
		(const char*)SQLTableNames::PINNumber,
		(const char*)PINNumber::UseCounter.Label, (const char*)PINNumber::UseCounter.Label,
		(const char*)PINNumber::LastDate.Label, (const char*)strDateNow,
		(const char*)PINNumber::LastTime.Label, (const char*)strTimeNow,
		(const char*)PINNumber::FirstDate.Label, (const char*)PINNumber::FirstDate.Label, (const char*)strDateNow, (const char*)PINNumber::FirstDate.Label,
		(const char*)PINNumber::FirstTime.Label, (const char*)PINNumber::FirstTime.Label, (const char*)strTimeNow, (const char*)PINNumber::FirstTime.Label,
		(const char*)PINNumber::PINNumber.Label, (const char*)strPINNumber
	);

	CSQLHelpers::ExecuteSQL(pDatabase, strSQL);
}

//**********************************************************************
