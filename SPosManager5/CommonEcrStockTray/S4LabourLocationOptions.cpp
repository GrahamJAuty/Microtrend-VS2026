/**********************************************************************/
//#include "DataManager.h"
#include "DateRangeHelpers.h"
//#include "globalfunctions.h"
//#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "S4LabourLocationOptions.h"
/**********************************************************************/

CS4LabourLocationOptions::CS4LabourLocationOptions()
{
	m_Enable.Create( "EnableExport", FALSE );
	m_LocID.Create( "LocationID", MAXLENGTH_S4LABOUR_LOCID, ""  );
	m_UseTermID.Create( "UseTermID", TRUE  );
	
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CS4LabourLocationOptions::CopyFrom( CS4LabourLocationOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CS4LabourLocationOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadBool( m_Enable );
	iniFile.ReadString( m_LocID );
	iniFile.ReadBool( m_UseTermID );
}

/**********************************************************************/

void CS4LabourLocationOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteBool( m_Enable );
	iniFile.WriteString( m_LocID );
	iniFile.WriteBool( m_UseTermID );
}

/**********************************************************************/

void CS4LabourLocationOptions::Reset()
{
	m_Enable.Reset();
	m_LocID.Reset();
	m_UseTermID.Reset();
}

/**********************************************************************/

bool CS4LabourLocationOptions::Read( int nLocIdx, bool bTemp )
{
	CString strFilename = GetFilename( nLocIdx, bTemp );
	
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return TRUE;
}

/**********************************************************************/

bool CS4LabourLocationOptions::Write( int nLocIdx, bool bTemp  )
{
	bool bResult = FALSE;

	CString strFilename = GetFilename( nLocIdx, bTemp );

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		bResult = iniFile.Write( strFilename );

		DataManager.UnlockAllUsers();
	}	
	return bResult;
}

/**********************************************************************/

const char* CS4LabourLocationOptions::GetFilename( int nLocIdx, bool bTemp )
{
	if ( FALSE == bTemp )
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourOptions, nLocIdx );
		m_strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourOptionsTemp, nLocIdx );
		m_strFilename = FnUp.GetFilenameToUse();
	}

	return m_strFilename;
}

/**********************************************************************/

void CS4LabourLocationOptions::SetLastExportDate( int nLocIdx, COleDateTime& date, bool bTemp )
{
	if ( date.m_status == COleDateTime::valid )
	{
		CString strDate;
		strDate.Format( "%4.4d%2.2d%2.2d",
			date.GetYear(),
			date.GetMonth(),
			date.GetDay() );

		CString strS4LabourFolder = dbLocation.GetFolderPathSysset( nLocIdx );
		strS4LabourFolder += "\\S4Labour";
		::CreateSubdirectory( strS4LabourFolder );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( nLocIdx, bTemp );
		if ( fileDate.Open( strFilename, "wb" ) == TRUE )
			fileDate.WriteLine( strDate );
	}
}

/**********************************************************************/

void CS4LabourLocationOptions::SetLastExportDate( int nLocIdx, const char* sz, bool bTemp )
{
	CString str = sz;

	bool bValid = FALSE;

	if ( str == "" )
		bValid = TRUE;
	else if ( ( str.GetLength() == 8 ) && ( ::TestNumeric( str ) == TRUE ) )
		bValid = TRUE;

	if ( TRUE == bValid )
	{
		CString strS4LabourFolder = dbLocation.GetFolderPathSysset( nLocIdx );
		strS4LabourFolder += "\\S4LABOUR";
		::CreateSubdirectory( strS4LabourFolder );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( nLocIdx, bTemp );
		if ( fileDate.Open( strFilename, "wb" ) == TRUE )
			fileDate.WriteLine( str );
	}
}

/**********************************************************************/

const char* CS4LabourLocationOptions::GetLastExportDate( int nLocIdx, bool bTemp )
{
	m_strLastExportDate = "";

	CSSFile fileDate;
	CString strFilename = GetDateFilename( nLocIdx, bTemp );
	if ( fileDate.Open( strFilename, "rb" ) == TRUE )
	{
		CString strDate;
		fileDate.ReadString( strDate );

		bool bValid = FALSE;

		if ( strDate == "" )
			bValid = TRUE;
		else if ( ( strDate.GetLength() == 8 ) && ( ::TestNumeric( strDate ) == TRUE ) )
			bValid = TRUE;

		if ( TRUE == bValid )
			m_strLastExportDate = strDate;
	}

	return m_strLastExportDate;
}

/**********************************************************************/

bool CS4LabourLocationOptions::GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate )
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

	int nExportHour = PosTrayOptions.GetS4LabourExportHour();
	oleNextExportTime += COleDateTimeSpan( 0, nExportHour, 0, 0 );

	int nSODHour = EcrmanOptions.GetReportsDayStartHour();
	int nSODMinute = EcrmanOptions.GetReportsDayStartMinute();
	
	if ( ( nExportHour < nSODHour  ) || ( ( nExportHour == nSODHour ) && ( nSODMinute != 0 ) ) )
		oleNextExportTime += COleDateTimeSpan( 1, 0, 0, 0 );
	
	return TRUE;
}

/**********************************************************************/

const char* CS4LabourLocationOptions::GetDateFilename( int nLocIdx, bool bTemp )
{
	m_strDateFilename = "";

	if ( FALSE == bTemp )
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourExportDate, nLocIdx );
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::S4LabourExportDateTemp, nLocIdx );
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
		
	return m_strDateFilename;
}

/**********************************************************************/

COleDateTime CS4LabourLocationOptions::GetLatestExportableDate()
{
	COleDateTime dateTimeNow = COleDateTime::GetCurrentTime();
	COleDateTime dateNow = COleDateTime( dateTimeNow.GetYear(), dateTimeNow.GetMonth(), dateTimeNow.GetDay(), 0, 0, 0 );

	int nTimeNow = ( dateTimeNow.GetHour() * 60 ) + dateTimeNow.GetMinute();
	int nTimeSOD = ( EcrmanOptions.GetReportsDayStartHour() * 60 ) + EcrmanOptions.GetReportsDayStartMinute();
	int nTimeS4 = PosTrayOptions.GetS4LabourExportHour() * 60;

	int nDaysToSubtract = 1;

	if ( nTimeS4 < nTimeSOD )
		nDaysToSubtract++;

	if ( nTimeNow < nTimeS4 )
		nDaysToSubtract++;

	CDateRangeHelpers DateRangeHelpers;
	COleDateTime dateExport = dateNow;
	DateRangeHelpers.AddDaysToTime( dateExport, -nDaysToSubtract );
	return dateExport;
}

/**********************************************************************/

int CS4LabourLocationOptions::ValidateSettings( int nLocIdx )
{
	CString strLocID = m_LocID.GetValue();
	::TrimSpaces( strLocID, FALSE );
	
	if ( strLocID == "" )
		return S4LABOUR_VALIDATE_NOID;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo( nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		return S4LABOUR_VALIDATE_NODB;

	return S4LABOUR_VALIDATE_OK;
}

/**********************************************************************/
