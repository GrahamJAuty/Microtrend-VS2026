/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MaxLengths.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "FNBDatabaseOptions.h"
/**********************************************************************/

CFNBDatabaseOptions::CFNBDatabaseOptions()
{
	m_Enable.Create( "EnableExport", FALSE );
	m_LocSelType.Create( "LocSelType", 0, 9999, 0 );
	m_LocSelNwkNo.Create( "LocSelNwkNo", 0, 9999, 0 );
	m_LocSelEntityNo.Create( "LocSelEntityNo", 0, 9999, 0 );
	m_FTPServer.Create( "FTPServer", MAX_LENGTH_FTP_SERVER, "ftp.fnbshop.com" );
	m_FTPUserName.Create( "FTPUserName", MAX_LENGTH_FTP_USERNAME, "" );
	m_FTPPassword.Create( "FTPPassword", MAX_LENGTH_FTP_PASSWORD, "" );
	m_FTPFolder.Create( "FTPFolder", MAX_LENGTH_FTP_FOLDER, "" );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CFNBDatabaseOptions::CopyFrom( CFNBDatabaseOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CFNBDatabaseOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadBool( m_Enable );
	iniFile.ReadInt( m_LocSelType );
	iniFile.ReadInt( m_LocSelNwkNo );
	iniFile.ReadInt( m_LocSelEntityNo );
	iniFile.ReadString( m_FTPServer );
	iniFile.ReadString( m_FTPUserName );
	iniFile.ReadString( m_FTPPassword );
	iniFile.ReadString( m_FTPFolder );
}

/**********************************************************************/

void CFNBDatabaseOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteBool( m_Enable );
	iniFile.WriteInt( m_LocSelType );
	iniFile.WriteInt( m_LocSelNwkNo );
	iniFile.WriteInt( m_LocSelEntityNo );
	iniFile.WriteString( m_FTPServer );
	iniFile.WriteString( m_FTPUserName );
	iniFile.WriteString( m_FTPPassword );
	iniFile.WriteString( m_FTPFolder );
}

/**********************************************************************/

void CFNBDatabaseOptions::Reset()
{
	m_Enable.Reset();
	m_LocSelType.Reset();
	m_LocSelNwkNo.Reset();
	m_LocSelEntityNo.Reset();
	m_FTPServer.Reset();
	m_FTPUserName.Reset();
	m_FTPPassword.Reset();
	m_FTPFolder.Reset();
}

/**********************************************************************/

bool CFNBDatabaseOptions::Read( int nDbIdx, bool bTemp )
{
	CString strFilename = GetFilename( nDbIdx, bTemp );
	
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

bool CFNBDatabaseOptions::Write( int nDbIdx, bool bTemp  )
{
	bool bResult = FALSE;

	CString strFilename = GetFilename( nDbIdx, bTemp );

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

const char* CFNBDatabaseOptions::GetFilename( int nDbIdx, bool bTemp )
{
	if ( FALSE == bTemp )
	{
		CFilenameUpdater FnUp( SysFiles::FNBOptions, nDbIdx );
		m_strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::FNBOptionsTemp, nDbIdx );
		m_strFilename = FnUp.GetFilenameToUse();
	}

	return m_strFilename;
}

/**********************************************************************/

void CFNBDatabaseOptions::SetLastExportDate( int nDbIdx, COleDateTime& date, bool bTemp )
{
	if ( date.m_status == COleDateTime::valid )
	{
		CString strDate;
		strDate.Format( "%4.4d%2.2d%2.2d",
			date.GetYear(),
			date.GetMonth(),
			date.GetDay() );

		CString strFNBFolder = dbDatabase.GetFolderPathSysset( nDbIdx );
		strFNBFolder += "\\FNB";
		::CreateSubdirectory( strFNBFolder );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( nDbIdx, bTemp );
		if ( fileDate.Open( strFilename, "wb" ) == TRUE )
			fileDate.WriteLine( strDate );
	}
}

/**********************************************************************/

void CFNBDatabaseOptions::SetLastExportDate( int nDbIdx, const char* sz, bool bTemp )
{
	CString str = sz;

	bool bValid = FALSE;

	if ( str == "" )
		bValid = TRUE;
	else if ( ( str.GetLength() == 8 ) && ( ::TestNumeric( str ) == TRUE ) )
		bValid = TRUE;

	if ( TRUE == bValid )
	{
		CString strFNBFolder = dbDatabase.GetFolderPathSysset( nDbIdx );
		strFNBFolder += "\\FNB";
		::CreateSubdirectory( strFNBFolder );

		CSSFile fileDate;
		CString strFilename = GetDateFilename( nDbIdx, bTemp );
		if ( fileDate.Open( strFilename, "wb" ) == TRUE )
			fileDate.WriteLine( str );
	}
}

/**********************************************************************/

const char* CFNBDatabaseOptions::GetLastExportDate( int nDbIdx, bool bTemp )
{
	m_strLastExportDate = "";

	CSSFile fileDate;
	CString strFilename = GetDateFilename( nDbIdx, bTemp );
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

bool CFNBDatabaseOptions::GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate )
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
	oleNextExportTime += COleDateTimeSpan( 0, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute(), 0 );
	oleNextExportTime += COleDateTimeSpan( 0, PosTrayOptions.GetFNBDelayTime(), 0, 0 );

	return TRUE;
}

/**********************************************************************/

const char* CFNBDatabaseOptions::GetDateFilename( int nDbIdx, bool bTemp )
{
	m_strDateFilename = "";

	if ( FALSE == bTemp )
	{
		CFilenameUpdater FnUp( SysFiles::FNBExportDate, nDbIdx );
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::FNBExportDateTemp, nDbIdx );
		m_strDateFilename = FnUp.GetFilenameToUse();
	}
		
	return m_strDateFilename;
}

/**********************************************************************/

