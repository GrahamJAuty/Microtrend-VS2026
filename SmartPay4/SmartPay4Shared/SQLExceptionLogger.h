#pragma once
//*******************************************************************
#include "afxdb.h"
//*******************************************************************

class CSQLExceptionLogger
{
public:
	static void LogSQLExceptionMessage(CDBException* pe);
	static void LogSQLExceptionMessage(CString str);
	
private:
	static void CheckMaxLogFileSize( CString& strNew, CString& strOld );
};

//*******************************************************************
