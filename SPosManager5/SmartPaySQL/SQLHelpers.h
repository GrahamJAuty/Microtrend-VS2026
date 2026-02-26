#pragma once
/**********************************************************************/
#include "afxdb.h"
/**********************************************************************/

class CSQLHelpers
{
public:
	static int CallbackDummy(void* udp, int c_num, char** c_vals, char** c_names);
	static CString IncrementUserID(const char* szUserID, const char* szMaxID);
	static CString IncrementUserID(__int64 nUserID, __int64 nMaxID);
	static bool IsValidUserID(CString strInputCardNo, CString& strOutputCardNo, int nMaxLength, bool bAcceptZero = FALSE);
	static CString GetCommandAlterColumnDecimal(CString strTable, CString strColumn, int nWidth, int nDPs);
	static CString GetCommandDropColumn(CString strTable, CString strColumn);
	static CString GetCommandAddColumn(CString strTable, CString strColumn);
	static int SetLogFileSize(int& nFailCommand);
	static int ShrinkLogFile();
	static int ExecuteSQL(CDatabase* pDatabase, CString strSQL);
	static void LogSQLException(CString strSQL, CString strError);
};

//**********************************************************************

