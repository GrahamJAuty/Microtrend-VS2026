/**********************************************************************/
#include "..\SmartPay4Shared\FileRemover.h"
#include "..\SmartPay4Shared\ReportFileWithEndText.h"
/**********************************************************************/
#include "EmailLogger.h"
/**********************************************************************/

CEmailLoggerInfo::CEmailLoggerInfo()
{
	m_strEmail = "";
	m_strFilename = "";
	m_strSubject = "";
	m_strResult = "";
	m_strDate = "";
	m_strTime = "";
	m_nHideLevel = 0;
}

/**********************************************************************/

CEmailLogger::CEmailLogger() 
{
	m_strLastEmailMessage = "";
}

/**********************************************************************/

void CEmailLogger::Initialise()
{
	m_LogFileEmail.SetFilename( Filenames.GetEmailLogAllFilename() );
}

/**********************************************************************/

void CEmailLogger::CheckMaxLogSysFileSize( CString& strNew, CString& strOld )
{
	CFileStatus Status;
	if ( CFile::GetStatus ( strNew, Status ) == TRUE )
	{
		if ( Status.m_size > 0x200000 )	
		{
			CopyFile( strNew, strOld, FALSE );
			CFileRemover FileRemover( strNew );
		}
	}
}

/**********************************************************************/

void CEmailLogger::LogEmailSystem( CEmailLoggerInfo& info )
{
	CString strNew = Filenames.GetEmailLogAllFilename();
	CString strOld = Filenames.GetEmailLogOldFilename();

	CheckMaxLogSysFileSize( strNew, strOld );
	
	bool bNewFile = FALSE;
	if ( ::FileExists ( strNew ) == FALSE )
	{
		CReportFileWithEndText ReportFile;
		ReportFile.Open ( strNew );
		ReportFile.SetStyle1 ( "Email Log" );
		ReportFile.AddColumn ( "Date",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Time",TA_LEFT, 250 );
		ReportFile.AddColumn ( "Filename",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Subject",TA_LEFT, 1500 );
		ReportFile.AddColumn ( "Address",TA_LEFT, 800 );
		ReportFile.AddColumn ( "Result",TA_LEFT, 700 );
		ReportFile.WriteHeader();
		ReportFile.Close();
		bNewFile = TRUE;
	}

	CString strLine = GetEmailLogLine( info );
	m_LogFileEmail.WriteLine( strLine );
	m_strLastEmailMessage = strLine;
}

/**********************************************************************/

const char* CEmailLogger::GetEmailLogLine(CEmailLoggerInfo& info)
{
	CCSV csv('\t');

	csv.Add(info.m_strDate);
	csv.Add(info.m_strTime);
	csv.Add(info.m_strFilename);
	csv.Add(info.m_strSubject);
	csv.Add(info.m_strEmail);
	csv.Add(info.m_strResult);

	if (info.m_nHideLevel > 0)
	{
		m_strEmailLogLine.Format("<HIDE%1.1d>%s",
			info.m_nHideLevel,
			(const char*) csv.GetLine());
	}
	else
	{
		m_strEmailLogLine = csv.GetLine();
	}

	return m_strEmailLogLine;
}	

/**********************************************************************/

