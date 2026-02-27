//**********************************************************************
#include "GlobalState.h"
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
//**********************************************************************
#include "SQLTable_Account/SQLRepositoryAccount.h"
//**********************************************************************
#include "SQLHelpers.h"
//**********************************************************************

int CSQLHelpers::CallbackDummy(void* udp, int c_num, char** c_vals, char** c_names)
{
	return 0;
}

//**********************************************************************

CString CSQLHelpers::IncrementUserID(const char* szUserID, const char* szMaxID)
{
	return IncrementUserID(_atoi64(szUserID), _atoi64(szMaxID));
}

//**********************************************************************

CString CSQLHelpers::IncrementUserID(__int64 nUserID, __int64 nMaxID)
{
	CString strUserID = "";

	if (nUserID < nMaxID)							// see if less than  9999999999
	{
		strUserID.Format("%I64d", nUserID + 1);		// inc user ID number
	}

	return strUserID;
}

//**********************************************************************

bool CSQLHelpers::IsValidUserID(CString strInputCardNo, CString& strOutputCardNo, int nMaxLength, bool bAcceptZero)
{
	strOutputCardNo = strInputCardNo;
	strOutputCardNo.TrimLeft('0');							// lose leading 0's

	bool bResult = FALSE;

	if ( ( strOutputCardNo.GetLength() <= nMaxLength ) && (::TestNumeric(strOutputCardNo) == TRUE) )
	{
		if ((strOutputCardNo == "") && (TRUE == bAcceptZero))
		{
			strOutputCardNo = "0";
		}

		bResult = (strOutputCardNo.GetLength() > 0);
	}

	return bResult;
}

//**********************************************************************

CString CSQLHelpers::GetCommandAlterColumnDecimal(CString strTable, CString strColumn, int nWidth, int nDPs)
{
	CString strResult = "";

	strResult.Format("ALTER TABLE %s ALTER COLUMN %s decimal(%d,%d) NOT NULL",
		(const char*)strTable,
		(const char*)strColumn,
		nWidth,
		nDPs);

	return strResult;
}

//**********************************************************************

CString CSQLHelpers::GetCommandDropColumn(CString strTable, CString strColumn)
{
	CString strResult = "";

	strResult.Format("ALTER TABLE %s DROP COLUMN %s",
		(const char*)strTable,
		(const char*)strColumn);

	return strResult;
}

//**********************************************************************

CString CSQLHelpers::GetCommandAddColumn(CString strTable, CString strColumn)
{
	CString strResult = "";

	strResult.Format("ALTER TABLE %s ADD %s",
		(const char*)strTable,
		(const char*)strColumn);

	return strResult;
}

//**********************************************************************

int CSQLHelpers::SetLogFileSize( int& nFailCommand )
{
	int nResult = -1;
	nFailCommand = 0;

	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

	if (nConnection != -1)
	{
		CStringArray arrayCommands;
		arrayCommands.Add("ALTER DATABASE SPOS_SmartPayDb MODIFY FILE(name = SPOS_SmartPayDb_data, maxsize = unlimited)");
		arrayCommands.Add("ALTER DATABASE SPOS_SmartPayDb MODIFY FILE(name = SPOS_SmartPayDb_data, filegrowth = 64mb)");
		arrayCommands.Add("ALTER DATABASE SPOS_SmartPayDb MODIFY FILE(name = SPOS_SmartPayDb_Log, maxsize = unlimited)");
		arrayCommands.Add("ALTER DATABASE SPOS_SmartPayDb MODIFY FILE(name = SPOS_SmartPayDb_Log, filegrowth = 64mb)");

		for (int n = 0; n < arrayCommands.GetSize(); n++)
		{
			nResult = ExecuteSQL(pDatabase, arrayCommands.GetAt(n));
			if (nResult != SQLCRUD_ERR_NONE)
			{
				nFailCommand = n + 1;
				break;
			}
		}

		SQLConnectionPool.FreeConnection(nConnection);
	}

	return nResult;
}

//**********************************************************************

int CSQLHelpers::ShrinkLogFile()
{
	int nResult = -1;

	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

	if (nConnection != -1)
	{
		nResult = ExecuteSQL(pDatabase, "DBCC SHRINKFILE(SPOS_SmartPayDb_Log,20);");
		SQLConnectionPool.FreeConnection(nConnection);
	}

	return nResult;
}

/**********************************************************************/

int CSQLHelpers::ExecuteSQL( CDatabase* pDatabase, CString strSQL)
{
	int nResult = SQLCRUD_ERR_NONE;
	g_GlobalState.SetExecuteSQLLastError("");

	try
	{
		pDatabase->ExecuteSQL(strSQL);
	}
	catch (CDBException* pe)
	{
		nResult = SQLCRUD_ERR_EXCEPT_DB;
		LogSQLException(strSQL, pe->m_strError);
		g_GlobalState.SetExecuteSQLLastError(pe->m_strError);
		pe->Delete();
	}
	catch (...)
	{
		nResult = SQLCRUD_ERR_EXCEPT_MISC;
		CString strError = "General Exception";
		LogSQLException(strSQL, strError);
		g_GlobalState.SetExecuteSQLLastError(strError);
	}

	return nResult;
}

/**********************************************************************/

void CSQLHelpers::LogSQLException(CString strSQL, CString strError)
{
	CString strFilename = Filenames.GetSQLExceptionsLogFilenameGeneral();

	CSSFile fileLog;
	if (fileLog.Open(strFilename, "ab") == TRUE)
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		CString strTime = "";
		strTime.Format("%2.2d/%2.2d/%4.4d  %2.2d:%2.2d:%2.2d",
			timeNow.GetDay(),
			timeNow.GetMonth(),
			timeNow.GetYear(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond());

		fileLog.WriteLine(strTime);
		fileLog.WriteLine(strSQL);
		fileLog.WriteLine(strError);
		fileLog.WriteLine("********************");
	}
}

/**********************************************************************/

bool CSQLHelpers::UpdateAccountWithVersionCheck(CSQLRowAccountFull& RowAccount, CString& strError)
{
	strError = "";
	bool bResult = FALSE;

	CSQLRowAccountFull RowCheck;
	RowCheck.SetUserID(RowAccount.GetUserID());

	CSQLRepositoryAccount RepoAccount;

	if (RepoAccount.SelectRow(RowCheck, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		strError = "Unable to save changes.\n\nUnable to check for changes by another user.";
	}
	else if (RowCheck.CompareRowVersion(RowAccount) == FALSE)
	{
		strError = "Unable to save changes.\n\nAccount has already been modified by another user.";
	}
	else if ( RepoAccount.UpdateRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE )
	{
		strError = "Unable to save changes.\n\nSQL update request failed.";
	}
	else
	{
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CSQLHelpers::DeleteTableRows(const CString strTableName, CDatabase* pDatabase)
{
	CString strCommand = "";
	strCommand += "DELETE FROM ";
	strCommand += strTableName;
	return (ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
}

/**********************************************************************/