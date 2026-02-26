//**********************************************************************
#include "GlobalState.h"
#include "SQLConnectionPool.h"
#include "SQLDefines.h"
//**********************************************************************
#include "SQLHelpers.h"
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

bool CSQLHelpers::IsValidUserID(const char* szCardNo, CString& strCardNo, int nMaxLength)
{
	strCardNo = szCardNo;
	strCardNo.TrimLeft('0');							// loose leading 0's

	int nLen = strCardNo.GetLength();

	if (nLen > 0 && nLen <= nMaxLength)			// see if length ok
	{
		for (int i = 0; i < nLen; i++)
		{
			if (strCardNo.GetAt(i) < '0' || strCardNo.GetAt(i) > '9')
			{
				return FALSE;
			}
		}

		return (_atoi64(strCardNo) > 0) ? TRUE : FALSE;
	}
	return FALSE;
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

int CSQLHelpers::SetLogFileSize(int& nFailCommand)
{
	int nResult = -1;
	nFailCommand = 0;

	int nConnection = -1;
	CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

	if (nConnection != -1)
	{
		CStringArray arrayCommands;
		arrayCommands.Add("ALTER DATABASE SPOS_LoyaltyDb MODIFY FILE(name = SPOS_LoyaltyDb_data, maxsize = unlimited)");
		arrayCommands.Add("ALTER DATABASE SPOS_LoyaltyDb MODIFY FILE(name = SPOS_LoyaltyDb_data, filegrowth = 64mb)");
		arrayCommands.Add("ALTER DATABASE SPOS_LoyaltyDb MODIFY FILE(name = SPOS_LoyaltyDb_Log, maxsize = unlimited)");
		arrayCommands.Add("ALTER DATABASE SPOS_LoyaltyDb MODIFY FILE(name = SPOS_LoyaltyDb_Log, filegrowth = 64mb)");

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
		nResult = ExecuteSQL(pDatabase, "DBCC SHRINKFILE(SPOS_LoyaltyDb_Log,20);");
		SQLConnectionPool.FreeConnection(nConnection);
	}

	return nResult;
}

/**********************************************************************/

int CSQLHelpers::ExecuteSQL(CDatabase* pDatabase, CString strSQL)
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
	CString strFilename = Filenames.GetSQLExceptionsLogFilename();

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

bool CSQLHelpers::DeleteTableRows(const CString strTableName, CDatabase* pDatabase)
{
	CString strCommand = "";
	strCommand += "DELETE FROM ";
	strCommand += strTableName;
	return (ExecuteSQL(pDatabase, strCommand) == SQLCRUD_ERR_NONE);
}

/**********************************************************************/
