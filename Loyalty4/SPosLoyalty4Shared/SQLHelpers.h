#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "SQLDefines.h"
#include "SQLResultInfo.h"
//**********************************************************************

class CSQLHelpers
{
public:
	static CString IncrementUserID(const char* szUserID, const char* szMaxID);
	static CString IncrementUserID(__int64 nUserID, __int64 nMaxID);
	static bool IsValidUserID(const char* szCardNo, CString& strCardNo, int nMaxLength);
	static CString GetCommandAlterColumnDecimal(CString strTable, CString strColumn, int nWidth, int nDPs);
	static CString GetCommandDropColumn(CString strTable, CString strColumn);
	static CString GetCommandAddColumn(CString strTable, CString strColumn);
	static int SetLogFileSize(int& nFailCommand);
	static int ShrinkLogFile();
	static int ExecuteSQL(CDatabase* pDatabase, CString strSQL);
	static void LogSQLException(CString strSQL, CString strError);
	static bool DeleteTableRows(const CString strTableName, CDatabase* pDatabase);

public:
	template<typename Operation>
	static CSQLResultInfo ExecuteWithExceptionHandling(Operation&& operation)
	{
		CSQLResultInfo Result;

		try
		{
			Result = operation();
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
};

//**********************************************************************

