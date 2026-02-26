/**********************************************************************/
#include "PosTrayOptions.h"
/**********************************************************************/
#include "LIPAExportDate.h"
/**********************************************************************/

CLIPAExportDate::CLIPAExportDate()
{
}

/**********************************************************************/

void CLIPAExportDate::SetLastExportDate( COleDateTime& date, bool bTemp )
{
	if ( date.m_status == COleDateTime::valid )
	{
		CString strDate;
		strDate.Format( "%4.4d%2.2d%2.2d",
			date.GetYear(),
			date.GetMonth(),
			date.GetDay() );

		CString strLIPAFolder = "LIPA";
		GetSyssetProgramPath( strLIPAFolder );
		::CreateSubdirectory( strLIPAFolder );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( bTemp );
		if (fileDate.Open(strFilename, "wb") == TRUE)
		{
			fileDate.WriteLine(strDate);
		}
	}
}

/**********************************************************************/

void CLIPAExportDate::SetLastExportDate( const char* sz, bool bTemp )
{
	CString str = sz;

	bool bValid = FALSE;

	if (str == "")
	{
		bValid = TRUE;
	}
	else if ((str.GetLength() == 8) && (::TestNumeric(str) == TRUE))
	{
		bValid = TRUE;
	}

	if ( TRUE == bValid )
	{
		CString strLIPAFolder = "LIPA";		
		GetSyssetProgramPath( strLIPAFolder );
		::CreateSubdirectory( strLIPAFolder );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( bTemp );
		if (fileDate.Open(strFilename, "wb") == TRUE)
		{
			fileDate.WriteLine(str);
		}
	}
}

/**********************************************************************/

const char* CLIPAExportDate::GetLastExportDate( bool bTemp )
{
	m_strLastExportDate = "";

	CSSFile fileDate;
	CString strFilename = GetDateFilename( bTemp );
	if ( fileDate.Open( strFilename, "rb" ) == TRUE )
	{
		CString strDate;
		fileDate.ReadString( strDate );

		bool bValid = FALSE;

		if (strDate == "")
		{
			bValid = TRUE;
		}
		else if ((strDate.GetLength() == 8) && (::TestNumeric(strDate) == TRUE))
		{
			bValid = TRUE;
		}

		if (TRUE == bValid)
		{
			m_strLastExportDate = strDate;
		}
	}

	return m_strLastExportDate;
}

/**********************************************************************/

bool CLIPAExportDate::GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate )
{
	COleDateTime dateLastExport;
	bool bGotLastExportDate = FALSE;

	{
		CString str = strLastExportDate;
	
		if ( ( str.GetLength() == 8 ) && ( ::TestNumeric( str ) == TRUE ) )
		{
			dateLastExport = COleDateTime( atoi( str.Left(4) ), atoi( str.Mid(4,2) ), atoi( str.Right(2) ), 0, 0, 0 );
			bGotLastExportDate = ( dateLastExport.m_status == COleDateTime::valid );
		}
	}

	if ( FALSE == bGotLastExportDate )
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		dateLastExport = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
		dateLastExport -= COleDateTimeSpan( 1, 0, 0, 0 );
	}

	oleNextDayToExport = dateLastExport;
	oleNextDayToExport += COleDateTimeSpan( 1, 0, 0, 0 );

	oleNextExportTime = dateLastExport;
	oleNextExportTime += COleDateTimeSpan( 2, 0, 0, 0 );

	int nExportHour = PosTrayOptions.GetLIPAExportHour();
	oleNextExportTime += COleDateTimeSpan( 0, nExportHour, 0, 0 );

	int nSODHour = EcrmanOptions.GetReportsDayStartHour();
	int nSODMinute = EcrmanOptions.GetReportsDayStartMinute();
	
	if ((nExportHour < nSODHour) || ((nExportHour == nSODHour) && (nSODMinute != 0)))
	{
		oleNextExportTime += COleDateTimeSpan(1, 0, 0, 0);
	}

	return TRUE;
}

/**********************************************************************/

const char* CLIPAExportDate::GetDateFilename( bool bTemp )
{
	m_strDateFilename = "";

	if ( FALSE == bTemp )
	{
		CFilenameUpdater FnUp( SysFiles::LIPAExportDate );
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::LIPAExportDateTemp );
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
		
	return m_strDateFilename;
}

/**********************************************************************/

