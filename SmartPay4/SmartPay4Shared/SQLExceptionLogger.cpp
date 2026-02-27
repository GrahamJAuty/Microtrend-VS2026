//*******************************************************************
#include "SQLExceptionLogger.h"
//*******************************************************************

void CSQLExceptionLogger::CheckMaxLogFileSize( CString& strNew, CString& strOld )
{
	CFileStatus Status;
	if ( CFile::GetStatus ( strNew, Status ) == TRUE )
	{
		if ( Status.m_size > 0x200000 )	
		{
			CopyFile( strNew, strOld, FALSE );
			CFileRecycler FileRecycler;
			FileRecycler.SendToRecycleBin( strNew );
		}
	}
}

/**********************************************************************/

void CSQLExceptionLogger::LogSQLExceptionMessage(CDBException* pe)
{
	char c[2000];
	pe->GetErrorMessage(c, 2000);
	CString str = c;

	if (str.Right(1) == "\n")
	{
		str = str.Left(str.GetLength() - 1);
	}

	LogSQLExceptionMessage(str);
}

/**********************************************************************/

void CSQLExceptionLogger::LogSQLExceptionMessage(CString str)
{
	CString strNew = Filenames.GetSQLExceptionsLogFilenameBromCom();
	CString strOld = Filenames.GetSQLExceptionsOldFilenameBromCom();
	CheckMaxLogFileSize(strNew, strOld);

	if (::FileExists(strNew) == FALSE)
	{
		CReportFile ReportFile;
		ReportFile.Open(strNew);
		ReportFile.SetStyle1("SQL Exceptions Log");
		ReportFile.AddColumn("Date", TA_LEFT, 250);
		ReportFile.AddColumn("Time", TA_LEFT, 250);
		ReportFile.AddColumn("PC ID", TA_LEFT, 300);
		ReportFile.AddColumn("Info", TA_LEFT, 2500);
		ReportFile.WriteHeader();
		ReportFile.Close();
	}

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strDate;
	strDate.Format("%2.2d/%2.2d/%4.4d",
		timeNow.GetDay(),
		timeNow.GetMonth(),
		timeNow.GetYear());

	CString strTime;
	strTime.Format("%2.2d:%2.2d:%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	CString strPCIdClient = "";
	CString strPCIdHost = "";
	strPCIdClient.Format("%10.10d", Sysset.GetPCIdClient());
	strPCIdHost.Format("%10.10d", Sysset.GetPCIdHost());

	CString strPCId = strPCIdClient;
	if (strPCIdClient != strPCIdHost)
	{
		strPCId += CString("/") + strPCIdHost;
	}

	CCSV csv('\t');
	csv.Add(strDate);
	csv.Add(strTime);
	csv.Add(strPCId);
	csv.Add(str);

	CSSFile fileLog;
	if (fileLog.Open(strNew, "ab") == TRUE)
	{
		fileLog.WriteLine(csv.GetLine());
	}
}

//*******************************************************************
