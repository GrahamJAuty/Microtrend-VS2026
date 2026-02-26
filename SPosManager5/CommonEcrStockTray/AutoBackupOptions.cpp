/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "AutoBackupOptions.h"
/**********************************************************************/

CAutoBackupOptions::CAutoBackupOptions()
{
	Reset();
	m_bFatalReadError = FALSE;
	m_strMemory = "";
}

/**********************************************************************/

const char* CAutoBackupOptions::GetCSVLine()
{
	CString strTimeNext;
	strTimeNext.Format("%4.4d%2.2d%2.2d%2.2d%2.2d",
		m_OleTimeNextBackup.GetYear(),
		m_OleTimeNextBackup.GetMonth(),
		m_OleTimeNextBackup.GetDay(),
		m_OleTimeNextBackup.GetHour(),
		m_OleTimeNextBackup.GetMinute());

	CString strTimeLast;
	strTimeLast.Format("%4.4d%2.2d%2.2d%2.2d%2.2d",
		m_OleTimeLastBackup.GetYear(),
		m_OleTimeLastBackup.GetMonth(),
		m_OleTimeLastBackup.GetDay(),
		m_OleTimeLastBackup.GetHour(),
		m_OleTimeLastBackup.GetMinute());

	CCSV csv;
	csv.Add(2);
	csv.Add(m_nAutoBackupType);
	csv.Add(m_nHourlyMinute);
	csv.Add(m_nDailyHour);
	csv.Add(m_nDailyMinute);
	csv.Add(m_nWeeklyHour);
	csv.Add(m_nWeeklyMinute);
	csv.Add(m_nWeeklyDay);
	csv.Add(m_nCustomMinute);
	csv.Add(m_nCustomMode);
	csv.Add(m_nMondayMask);
	csv.Add(m_nTuesdayMask);
	csv.Add(m_nWednesdayMask);
	csv.Add(m_nThursdayMask);
	csv.Add(m_nFridayMask);
	csv.Add(m_nSaturdayMask);
	csv.Add(m_nSundayMask);
	csv.Add(m_nMonFriMask);
	csv.Add(m_nMonSatMask);
	csv.Add(m_nEverydayMask);
	csv.Add(m_strPath);
	csv.Add(m_nArchiveCount);
	csv.Add(m_nHoursBeforePrompt);
	csv.Add(strTimeLast);
	csv.Add(strTimeNext);
	csv.Add(m_nMinutesBeforeOverdue);
	csv.Add(m_bAlwaysPrompt);
	csv.Add(m_bExcludeDailySales);
	csv.Add(m_bExcludeZeroSizeFiles);
	csv.Add(m_nCompressionLevel);

	m_strCSVLine = csv.GetLine();
	return m_strCSVLine;
}

/**********************************************************************/

void CAutoBackupOptions::SetDefaults()
{
	Reset();
}

/**********************************************************************/

void CAutoBackupOptions::Reset()
{
	m_nAutoBackupType = AUTOBACKUP_TYPE_NONE;
	m_nHourlyMinute = 0;
	m_nDailyHour = 0;
	m_nDailyMinute = 0;
	m_nWeeklyHour = 0;
	m_nWeeklyMinute = 0;
	m_nWeeklyDay = 0;
	m_nCustomMinute = 0;
	m_nCustomMode = 0;
	m_nMondayMask = 0xFFFFFF;
	m_nTuesdayMask = 0xFFFFFF;
	m_nWednesdayMask = 0xFFFFFF;
	m_nThursdayMask = 0xFFFFFF;
	m_nFridayMask = 0xFFFFFF;
	m_nSaturdayMask = 0xFFFFFF;
	m_nSundayMask = 0xFFFFFF;
	m_nMonFriMask = 0xFFFFFF;
	m_nMonSatMask = 0xFFFFFF;
	m_nEverydayMask = 0xFFFFFF;
	m_strPath = "";
	m_nArchiveCount = 0;
	m_nHoursBeforePrompt = 0;
	m_OleTimeLastBackup = COleDateTime(1980, 1, 1, 0, 0, 0);
	m_OleTimeNextBackup = COleDateTime(2008, 1, 1, 0, 0, 0);
	m_nMinutesBeforeOverdue = 0;
	m_bAlwaysPrompt = FALSE;
	m_bExcludeDailySales = FALSE;
	m_bExcludeZeroSizeFiles = FALSE;
	m_nCompressionLevel = 6;
}

/**********************************************************************/

bool CAutoBackupOptions::Read()
{
	::CreateSyssetSubdirectory( "SysFiles" );

	CFilenameUpdater FnUp( SysFiles::AutoBackupOptions );
		
	CSSFile fileOptions;
	if ( fileOptions.Open( FnUp.GetFilenameToUse(), "rb" ) == FALSE )
	{
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			m_bFatalReadError = TRUE;
		}
			
		return FALSE;
	}
	
	CString strBuffer = "";
	if (fileOptions.ReadString(strBuffer) == FALSE)
	{
		return TRUE;
	}

	ReloadFromString( strBuffer );
	return TRUE;
}

/**********************************************************************/

void CAutoBackupOptions::ReloadFromString(CString& strBuffer)
{
	CCSV csv(strBuffer);

	int nVer = csv.GetInt(0);

	switch (nVer)
	{
	case 1:
	case 2:
		SetAutoBackupType(csv.GetInt(1));
		SetHourlyMinute(csv.GetInt(2));
		SetDailyHour(csv.GetInt(3));
		SetDailyMinute(csv.GetInt(4));
		SetWeeklyHour(csv.GetInt(5));
		SetWeeklyMinute(csv.GetInt(6));
		SetWeeklyDay(csv.GetInt(7));
		SetCustomMinute(csv.GetInt(8));
		SetCustomMode(csv.GetInt(9));
		SetMondayMask(csv.GetInt(10));
		SetTuesdayMask(csv.GetInt(11));
		SetWednesdayMask(csv.GetInt(12));
		SetThursdayMask(csv.GetInt(13));
		SetFridayMask(csv.GetInt(14));
		SetSaturdayMask(csv.GetInt(15));
		SetSundayMask(csv.GetInt(16));
		SetMonFriMask(csv.GetInt(17));
		SetMonSatMask(csv.GetInt(18));
		SetEverydayMask(csv.GetInt(19));
		SetPath(csv.GetString(20));
		SetArchiveCount(csv.GetInt(21));
		SetHoursBeforePrompt(csv.GetInt(22));
		SetOleTimeLastBackup(csv.GetString(23));
		SetOleTimeNextBackup(csv.GetString(24));
		SetMinutesBeforeOverdue(csv.GetInt(25));
		SetAlwaysPromptFlag(csv.GetBool(26));
		SetExcludeDailySalesFlag(csv.GetBool(27));
		SetExcludeZeroSizeFilesFlag(csv.GetBool(28));
		break;
	}

	switch (nVer)
	{
	case 1:
		SetCompressionLevel(6);
		break;

	case 2:
		SetCompressionLevel(csv.GetInt(29));
		break;
	}
}
	
/**********************************************************************/

bool CAutoBackupOptions::HasScheduleChanged()
{
	CAutoBackupOptions OldSettings;
	OldSettings.ReloadFromString( m_strMemory );

	bool bResult = FALSE;

	if ( m_nAutoBackupType != OldSettings.m_nAutoBackupType )
		bResult = TRUE;
	else
	{
		switch( m_nAutoBackupType )
		{
		case AUTOBACKUP_TYPE_HOURLY:	
			bResult |= ( m_nHourlyMinute != OldSettings.m_nHourlyMinute );
			break;

		case AUTOBACKUP_TYPE_DAILY:
			bResult |= ( m_nDailyHour != OldSettings.m_nDailyHour );
			bResult |= ( m_nDailyMinute != OldSettings.m_nDailyMinute );
			break;

		case AUTOBACKUP_TYPE_WEEKLY:
			bResult |= ( m_nWeeklyDay != OldSettings.m_nWeeklyDay );
			bResult |= ( m_nWeeklyHour != OldSettings.m_nWeeklyHour );
			bResult |= ( m_nWeeklyMinute != OldSettings.m_nWeeklyMinute );
			break;

		case AUTOBACKUP_TYPE_CUSTOM:
			if ( m_nCustomMode != OldSettings.m_nCustomMode )
				bResult = TRUE;
			else
			{
				switch( m_nCustomMode )
				{
				case BACKUP_MODE_MON_FRI:
					bResult |= ( m_nMonFriMask != OldSettings.m_nMonFriMask );
					bResult |= ( m_nSaturdayMask != OldSettings.m_nSaturdayMask );
					bResult |= ( m_nSundayMask != OldSettings.m_nSundayMask );
					break;

				case BACKUP_MODE_MON_SAT:
					bResult |= ( m_nMonSatMask != OldSettings.m_nMonSatMask );
					bResult |= ( m_nSundayMask != OldSettings.m_nSundayMask );
					break;

				case BACKUP_MODE_EVERYDAY:
					bResult |= ( m_nEverydayMask != OldSettings.m_nEverydayMask );
					break;

				case BACKUP_MODE_INDIVIDUAL:
					bResult |= ( m_nMondayMask != OldSettings.m_nMondayMask );
					bResult |= ( m_nTuesdayMask != OldSettings.m_nTuesdayMask );
					bResult |= ( m_nWednesdayMask != OldSettings.m_nWednesdayMask );
					bResult |= ( m_nThursdayMask != OldSettings.m_nThursdayMask );
					bResult |= ( m_nFridayMask != OldSettings.m_nFridayMask );
					bResult |= ( m_nSaturdayMask != OldSettings.m_nSaturdayMask );
					bResult |= ( m_nSundayMask != OldSettings.m_nSundayMask );
					break;
				}
			}
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CAutoBackupOptions::SetAutoBackupType( int n )
{
	switch( n )
	{
	case AUTOBACKUP_TYPE_NONE:
	case AUTOBACKUP_TYPE_HOURLY:
	case AUTOBACKUP_TYPE_DAILY:
	case AUTOBACKUP_TYPE_WEEKLY:
	case AUTOBACKUP_TYPE_CUSTOM:
		m_nAutoBackupType = n;
		break;
	}
}

/**********************************************************************/

void CAutoBackupOptions::SetHourlyMinute( int n )
{
	if ( ( n >= 0 ) && ( n <= 59 ) )
		m_nHourlyMinute = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetDailyHour( int n )
{
	if ( ( n >= 0 ) && ( n <= 23 ) )
		m_nDailyHour = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetDailyMinute( int n )
{
	if ( ( n >= 0 ) && ( n <= 59 ) )
		m_nDailyMinute = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetWeeklyHour( int n )
{
	if ( ( n >= 0 ) && ( n <= 23 ) )
		m_nWeeklyHour = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetWeeklyMinute( int n )
{
	if ( ( n >= 0 ) && ( n <= 59 ) )
		m_nWeeklyMinute = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetWeeklyDay( int n )
{
	if ( ( n >= 0 ) && ( n <= 6 ) )
		m_nWeeklyDay = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetCustomMinute( int n )
{
	if ( ( n >= 0 ) && ( n <= 59 ) )
		m_nCustomMinute = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetCustomMode( int n )
{
	if ( ( n >= 0 ) && ( n <= 3 ) )
		m_nCustomMode = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetMondayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nMondayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetTuesdayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nTuesdayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetWednesdayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nWednesdayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetThursdayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nThursdayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetFridayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nFridayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetSaturdayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nSaturdayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetSundayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nSundayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetMonFriMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nMonFriMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetMonSatMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nMonSatMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetEverydayMask( int n )
{
	if ( ( n >= 0 ) && ( n <= 0xFFFFFF ) )
		m_nEverydayMask = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetPath( const char* sz )
{
	m_strPath = sz;
}

/**********************************************************************/

void CAutoBackupOptions::SetArchiveCount( int n )
{
	if ( ( n >= 0 ) && ( n <= 10 ) )
		m_nArchiveCount = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetHoursBeforePrompt( int n )
{
	if ( ( n >= 0 ) && ( n <= 168 ) )
		m_nHoursBeforePrompt = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetMinutesBeforeOverdue( int n )
{
	if ( ( n >= 0 ) && ( n <= 60 ) )
		m_nMinutesBeforeOverdue = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetCompressionLevel(int n)
{
	if ((n >= 0) && (n <= 9))
		m_nCompressionLevel = n;
}

/**********************************************************************/

void CAutoBackupOptions::SetOleTimeNextBackup( const char* sz )
{
	CString str = sz;
	if ( str.GetLength() == 12 )
	{
		COleDateTime time = COleDateTime(
			atoi( str.Left(4) ),
			atoi( str.Mid(4,2) ),
			atoi( str.Mid(6,2) ),
			atoi( str.Mid(8,2) ),
			atoi( str.Mid(10,2) ),
			0 );

		if ( time.m_status == COleDateTime::valid )
			m_OleTimeNextBackup = time;
	}
}

/**********************************************************************/

void CAutoBackupOptions::SetOleTimeNextBackup( const COleDateTime time )
{
	if ( time.m_status == COleDateTime::valid )
		m_OleTimeNextBackup = time;
}

/**********************************************************************/

void CAutoBackupOptions::SetOleTimeLastBackup( const char* sz )
{
	CString str = sz;
	if ( str.GetLength() == 12 )
	{
		COleDateTime time = COleDateTime(
			atoi( str.Left(4) ),
			atoi( str.Mid(4,2) ),
			atoi( str.Mid(6,2) ),
			atoi( str.Mid(8,2) ),
			atoi( str.Mid(10,2) ),
			0 );

		if ( time.m_status == COleDateTime::valid )
			m_OleTimeLastBackup = time;
	}
}

/**********************************************************************/

void CAutoBackupOptions::SetOleTimeLastBackup()
{
	m_OleTimeLastBackup = COleDateTime::GetCurrentTime();
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

bool CAutoBackupOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		bResult = WriteInternal();
		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/
#else
/**********************************************************************/

bool CAutoBackupOptions::Write()
{
	return WriteInternal();
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CAutoBackupOptions::WriteInternal()
{
	bool bResult = FALSE;

	::CreateSyssetSubdirectory("SysFiles");

	CFilenameUpdater FnUp(SysFiles::AutoBackupOptions);

	CSSFile fileOptions;
	if (fileOptions.Open(FnUp.GetFilenameToUse(), "wb") == TRUE)
	{
		fileOptions.WriteLine(GetCSVLine());
		fileOptions.Close();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CAutoBackupOptions::GetOleTimeLastBackup( COleDateTime& time )
{
	time = m_OleTimeLastBackup;
	return ( time > COleDateTime( 1980, 1, 1, 0, 0, 0 ) );
}

/**********************************************************************/