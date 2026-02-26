/**********************************************************************/
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\DateRangeHelpers.h"
#include "..\SPosLoyalty4Shared\FileRemover.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
/**********************************************************************/
#include "BackupErrors.h"
#include "BackupPromptDlg.h"
#include "LockManager.h"
#include "PCOptions.h"
#include "SQLScriptExport.h"
#include "SystemBackup.h"
/**********************************************************************/
#include "AutoBackupProcessor.h"
/**********************************************************************/

CAutoBackupResult::CAutoBackupResult()
{
	m_oleDateTime = COleDateTime::GetCurrentTime();
	m_nReason = 0;
	m_nResult = 0;
	m_strError = "";
}

/**********************************************************************/

CAutoBackupProcessor::CAutoBackupProcessor()
{
	m_bWantQuit = FALSE;
	m_bLoggedResult = FALSE;
	m_bReadyToGo = FALSE;
	m_OleNextBackupTime = COleDateTime( 9000, 1, 1, 0, 0, 0 );
	m_OleNextAttemptTime = COleDateTime( 9000, 1, 1, 0, 0, 0 );
	m_OleForcePromptTime = COleDateTime( 9000, 1, 1, 0, 0, 0 );
	m_nNextPromptReason = BACKUP_REASON_SCHEDULED_HOURLY;
}

/**********************************************************************/

void CAutoBackupProcessor::TestPendingChanges( COleDateTime& oleTimeNextBackup, COleDateTime& oleTimeNextAttempt, COleDateTime& oleTimeForcePrompt, int& nReason )
{
	COleDateTime oleSavedNextBackupTime = m_OleNextBackupTime;
	COleDateTime oleSavedNextAttemptTime = m_OleNextAttemptTime;
	COleDateTime oleSavedForcePromptTime = m_OleForcePromptTime;
	int nSavedReason = m_nNextPromptReason;

	ScheduleNextBackup( FALSE );
	oleTimeNextBackup = m_OleNextBackupTime;
	oleTimeNextAttempt = m_OleNextAttemptTime;
	oleTimeForcePrompt = m_OleForcePromptTime;
	nReason = GetNextAttemptReason();

	m_OleNextBackupTime = oleSavedNextBackupTime;
	m_OleNextAttemptTime = oleSavedNextAttemptTime;
	m_OleForcePromptTime = oleSavedForcePromptTime;
	m_nNextPromptReason = nSavedReason;
}

/**********************************************************************/

int CAutoBackupProcessor::GetBackupReasonFromAutoBackupType()
{
	int nReason = BACKUP_REASON_NONE;

	switch( AutoBackupOptions.GetAutoBackupType() )
	{
	case AUTOBACKUP_TYPE_HOURLY:
		nReason = BACKUP_REASON_SCHEDULED_HOURLY;
		break;

	case AUTOBACKUP_TYPE_DAILY:
		nReason = BACKUP_REASON_SCHEDULED_DAILY;
		break;

	case AUTOBACKUP_TYPE_WEEKLY:
		nReason = BACKUP_REASON_SCHEDULED_WEEKLY;
		break;

	case AUTOBACKUP_TYPE_CUSTOM:
		nReason = BACKUP_REASON_SCHEDULED_CUSTOM;
		break;

	case AUTOBACKUP_TYPE_NONE:
	default:
		nReason = BACKUP_REASON_NONE;
		break;
	}

	return nReason;
}

/**********************************************************************/

int CAutoBackupProcessor::GetNextAttemptReason()
{
	int nReason = BACKUP_REASON_NONE;

	if (m_OleNextAttemptTime != m_OleForcePromptTime)
	{
		nReason = GetBackupReasonFromAutoBackupType();
	}
	else
	{
		nReason = m_nNextPromptReason;
	}

	return nReason;
}

/**********************************************************************/

void CAutoBackupProcessor::ResetNextAttemptTime()
{
	m_OleNextBackupTime = COleDateTime( 9000, 1, 1, 0, 0, 0 );
	m_OleNextAttemptTime = COleDateTime( 9000, 1, 1, 0, 0, 0 );
	m_OleForcePromptTime = COleDateTime( 9000, 1, 1, 0, 0, 0 );
	m_nNextPromptReason = BACKUP_REASON_NONE;

	if ( AutoBackupOptions.GetAutoBackupType() != AUTOBACKUP_TYPE_NONE )
	{	
		m_nNextPromptReason = GetBackupReasonFromAutoBackupType();
		
		COleDateTime time1; 
		AutoBackupOptions.GetOleTimeNextBackup( time1 );
		COleDateTime time2 = CalculateNextBackupTime();

		if (time2 < time1)
		{
			time1 = time2;
		}

		m_OleNextBackupTime = time1;
		m_OleNextAttemptTime = time1;
	
		if ( AutoBackupOptions.GetMinutesBeforeOverdue() != 0 )
		{
			m_OleForcePromptTime = m_OleNextBackupTime + COleDateTimeSpan( 0, 0, AutoBackupOptions.GetMinutesBeforeOverdue(), 0 );
			
			if ( m_OleForcePromptTime <= COleDateTime::GetCurrentTime() )
			{
				m_OleNextBackupTime = m_OleForcePromptTime;
				m_OleNextAttemptTime = m_OleForcePromptTime;
				m_nNextPromptReason = BACKUP_REASON_OVERDUE;
			}
		}
	}
		
	if ( AutoBackupOptions.GetHoursBeforePrompt() != 0 )
	{
		COleDateTime timePrompt;
		if ( AutoBackupOptions.GetOleTimeLastBackup( timePrompt ) == TRUE )
		{
			timePrompt += COleDateTimeSpan( 0, AutoBackupOptions.GetHoursBeforePrompt(), 0, 0 );

			if ( timePrompt < m_OleNextAttemptTime )
			{
				m_OleNextBackupTime = timePrompt;
				m_OleNextAttemptTime = timePrompt;
				m_OleForcePromptTime = timePrompt;
				m_nNextPromptReason = BACKUP_REASON_TIMESINCELAST;
			}
		}
	}
	
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	TraceStatus( timeNow, "RESET NEXT BACKUP ATTEMPT TIME" );
}

/**********************************************************************/

bool CAutoBackupProcessor::CheckNextAttemptTime()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	TraceStatus( timeNow, "CHECK NEXT BACKUP ATTEMPT TIME" );
	return timeNow >= m_OleNextAttemptTime;
}

/**********************************************************************/

bool CAutoBackupProcessor::CheckForcePromptTime()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	TraceStatus( timeNow, "CHECK NEXT FORCE PROMPT TIME" );
	return timeNow >= m_OleForcePromptTime;
}

/**********************************************************************/

void CAutoBackupProcessor::NudgeNextAttemptTime(int nMinutes)
{
	bool bPrompt = (m_OleNextAttemptTime == m_OleForcePromptTime);

	m_OleNextAttemptTime = COleDateTime::GetCurrentTime();
	m_OleNextAttemptTime += COleDateTimeSpan(0, 0, nMinutes, 0);

	if (FALSE == bPrompt)
	{
		m_OleForcePromptTime = COleDateTime(9000, 1, 1, 0, 0, 0);
	}
	else
	{
		m_OleForcePromptTime = m_OleNextAttemptTime;
	}
}

/**********************************************************************/

void CAutoBackupProcessor::ScheduleNextPrompt( int nWait, bool bForcePrompt )
{
	if ( ( nWait >= 1 ) && ( nWait <= 3 ) )
	{
		COleDateTime timePrompt = COleDateTime::GetCurrentTime();

		switch( nWait )
		{
		case 1:	timePrompt += COleDateTimeSpan( 0, 0, 10, 0 );	break;
		case 2:	timePrompt += COleDateTimeSpan( 0, 0, 20, 0 );	break;
		case 3:	timePrompt += COleDateTimeSpan( 0, 0, 30, 0 );	break;
		}

		m_OleNextAttemptTime = timePrompt;
		
		if (FALSE == bForcePrompt)
		{
			m_OleForcePromptTime = COleDateTime(9000, 1, 1, 0, 0, 0);
		}
		else
		{
			m_OleForcePromptTime = m_OleNextAttemptTime;
		}

		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		TraceStatus( timeNow, "SET USER REQUEST REMINDER TIME" );
	}
}

/**********************************************************************/

void CAutoBackupProcessor::ScheduleNextBackup( bool bCompletedLastBackup )
{
	if ( TRUE == bCompletedLastBackup )
	{
		AutoBackupOptions.SetOleTimeLastBackup();
		AutoBackupOptions.SetOleTimeNextBackup( CalculateNextBackupTime() );
		ResetNextAttemptTime();
	}
	else
	{
		AutoBackupOptions.SetOleTimeNextBackup( CalculateNextBackupTime() );
		ResetNextAttemptTime();
	}
}

/**********************************************************************/

COleDateTime CAutoBackupProcessor::CalculateNextBackupTime()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	COleDateTime timeNextBackup = timeNow;

	CDateRangeHelpers DateRangeHelpers;

	switch( AutoBackupOptions.GetAutoBackupType() )
	{
	case AUTOBACKUP_TYPE_HOURLY:
		{
			timeNextBackup = COleDateTime(
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				AutoBackupOptions.GetHourlyMinute(),
				0 );

			while( timeNextBackup <= timeNow )
				timeNextBackup += COleDateTimeSpan( 0, 1, 0, 0 );	
		}
		break;

	case AUTOBACKUP_TYPE_DAILY:
		
		timeNextBackup = COleDateTime(
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			AutoBackupOptions.GetDailyHour(),
			AutoBackupOptions.GetDailyMinute(),
			0);

		while (timeNextBackup <= timeNow)
		{
			DateRangeHelpers.AddDaysToTime(timeNextBackup, 1);
		}
		break;

	case AUTOBACKUP_TYPE_WEEKLY:
		{
			timeNextBackup = COleDateTime(
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				AutoBackupOptions.GetWeeklyHour(),
				AutoBackupOptions.GetWeeklyMinute(),
				0 );

			int nCurrentWeekday = timeNow.GetDayOfWeek();
			int nBackupWeekday = AutoBackupOptions.GetWeeklyDay() + 1;

			if (nBackupWeekday < nCurrentWeekday)
			{
				nBackupWeekday += 7;
			}

			DateRangeHelpers.AddDaysToTime( timeNextBackup, nBackupWeekday - nCurrentWeekday );

			while (timeNextBackup <= timeNow)
			{
				DateRangeHelpers.AddDaysToTime(timeNextBackup, 7);
			}
		}
		break;

	case AUTOBACKUP_TYPE_CUSTOM:
		{
			timeNextBackup = COleDateTime(
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				AutoBackupOptions.GetCustomMinute(),
				0 );

			if (timeNextBackup <= timeNow)
			{
				timeNextBackup += COleDateTimeSpan(0, 1, 0, 0);
			}

			COleDateTime timeBreak = timeNextBackup;
			DateRangeHelpers.AddDaysToTime( timeBreak, 7 );

			int nWeekdayMask[7] = {0};
			switch( AutoBackupOptions.GetCustomMode() )
			{
			case BACKUP_MODE_MON_FRI:
				nWeekdayMask[0] = AutoBackupOptions.GetSundayMask();
				nWeekdayMask[1] = AutoBackupOptions.GetMonFriMask();
				nWeekdayMask[2] = AutoBackupOptions.GetMonFriMask();
				nWeekdayMask[3] = AutoBackupOptions.GetMonFriMask();
				nWeekdayMask[4] = AutoBackupOptions.GetMonFriMask();
				nWeekdayMask[5] = AutoBackupOptions.GetMonFriMask();
				nWeekdayMask[6] = AutoBackupOptions.GetSaturdayMask();
				break;

			case BACKUP_MODE_MON_SAT:
				nWeekdayMask[0] = AutoBackupOptions.GetSundayMask();
				nWeekdayMask[1] = AutoBackupOptions.GetMonSatMask();
				nWeekdayMask[2] = AutoBackupOptions.GetMonSatMask();
				nWeekdayMask[3] = AutoBackupOptions.GetMonSatMask();
				nWeekdayMask[4] = AutoBackupOptions.GetMonSatMask();
				nWeekdayMask[5] = AutoBackupOptions.GetMonSatMask();
				nWeekdayMask[6] = AutoBackupOptions.GetMonSatMask();
				break;

			case BACKUP_MODE_EVERYDAY:
				nWeekdayMask[0] = AutoBackupOptions.GetEverydayMask();
				nWeekdayMask[1] = AutoBackupOptions.GetEverydayMask();
				nWeekdayMask[2] = AutoBackupOptions.GetEverydayMask();
				nWeekdayMask[3] = AutoBackupOptions.GetEverydayMask();
				nWeekdayMask[4] = AutoBackupOptions.GetEverydayMask();
				nWeekdayMask[5] = AutoBackupOptions.GetEverydayMask();
				nWeekdayMask[6] = AutoBackupOptions.GetEverydayMask();
				break;

			case BACKUP_MODE_INDIVIDUAL:
				nWeekdayMask[0] = AutoBackupOptions.GetSundayMask();
				nWeekdayMask[1] = AutoBackupOptions.GetMondayMask();
				nWeekdayMask[2] = AutoBackupOptions.GetTuesdayMask();
				nWeekdayMask[3] = AutoBackupOptions.GetWednesdayMask();
				nWeekdayMask[4] = AutoBackupOptions.GetThursdayMask();
				nWeekdayMask[5] = AutoBackupOptions.GetFridayMask();
				nWeekdayMask[6] = AutoBackupOptions.GetSaturdayMask();
				break;
			}

			bool bFoundNextTime = FALSE;
			while( timeNextBackup < timeBreak )
			{
				int nHour = timeNextBackup.GetHour();
				int nWeekday = timeNextBackup.GetDayOfWeek();

				int nDayMask = nWeekdayMask[ ( nWeekday - 1 ) % 7 ];
				int nHourMask = 1 << nHour;

				if ( ( nDayMask & nHourMask ) == nHourMask )
				{
					bFoundNextTime = TRUE;
					break;
				}

				timeNextBackup += COleDateTimeSpan( 0, 1, 0, 0 );
			}

			if (bFoundNextTime == FALSE)
			{
				timeNextBackup = COleDateTime(9000, 1, 1, 0, 0, 0);
			}
		}
		break;
	}

	return timeNextBackup;
}

/**********************************************************************/

void CAutoBackupProcessor::ExpireOldBackups()
{
	int nArchiveCount = AutoBackupOptions.GetArchiveCount();
	if ((nArchiveCount < 2) || (nArchiveCount > 10))
	{
		return;
	}

	CString strPath = AutoBackupOptions.GetPath();
	if ( strPath.GetLength() > 0 )
	{
		if (strPath.Right(1) != "\\")
		{
			strPath += "\\";
		}
	}

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	CReportConsolidationArray<CSortedStringItem> arrayFolders;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() == TRUE)
		{
			continue;
		}

		if (finder.IsDirectory() == FALSE)
		{
			continue;
		}

		CString strFolderName = finder.GetFileName();
		if (strFolderName.GetLength() != 15)
		{
			continue;
		}

		CString strDate = strFolderName.Left(8);
		CString strTime = strFolderName.Right(6);

		if (::TestNumeric(strDate) == FALSE)
		{
			continue;
		}

		if (::TestNumeric(strTime) == FALSE)
		{
			continue;
		}

		COleDateTime oleDate = COleDateTime(
			atoi( strDate.Left(4) ),
			atoi( strDate.Mid(4,2) ),
			atoi( strDate.Right(2) ),
			atoi( strTime.Left(2) ),
			atoi( strTime.Mid(2,2) ),
			atoi( strTime.Right(2) ) );
			
		if (oleDate.GetStatus() != COleDateTime::valid)
		{
			continue;
		}

		int nVersion;
		int nFileSize;
		CString strExtra = "";
		if (CheckZipFolder(strPath + strFolderName, nVersion, strExtra, nFileSize) == FALSE)
		{
			continue;
		}

		CSortedStringItem item;
		item.m_strItem = strFolderName;
		arrayFolders.Consolidate( item );

		if ( arrayFolders.GetSize() > nArchiveCount )
		{
			CSortedStringItem item;
			arrayFolders.GetAt( 0, item );
			CString strFolder = strPath + item.m_strItem;

			CFileRemover FileRemover;
			FileRemover.RemoveFolder( strFolder );
			arrayFolders.RemoveAt(0);
		}
	}
}

/**********************************************************************/

bool CAutoBackupProcessor::CheckZipFolder( const char* szFolder, int& nVersion, CString& strExtra, int& nFileSize )
{
	nVersion = 0;
	nFileSize = 0;
	strExtra = "";

	CFileFind finder;

	CString strFolder = szFolder;
	strFolder += "\\";
	BOOL bWorking = finder.FindFile ( strFolder + "*.*" );

	bool bResult = FALSE;
	int nFileCount = 0;

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() == TRUE)
		{
			continue;
		}

		if (finder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = finder.GetFileName();
		strFilename.MakeUpper();

		if (strFilename.Right(4) != ".ZIP")
		{
			continue;
		}

		nFileCount++;
		if (nFileCount >= 2)
		{
			bResult = FALSE;
			break;
		}

		if (strFilename.GetLength() != 12)
		{
			continue;
		}

		if (strFilename.Left(2) != "MB")
		{
			continue;
		}

		CString strVersion = strFilename.Mid(4,3); 
		if (::TestNumeric(strVersion) == FALSE)
		{
			continue;
		}

		nVersion = atoi( strFilename.Mid(4,3) );

		CSSFile fileTest;
		if ( fileTest.Open( strFolder + strFilename, "rb" ) == TRUE )
		{
			nFileSize = fileTest.GetLength() / 1024;
			fileTest.Close();
		}
		
		CString str = "";
		str.Format( "%2.2d", SYSSET_LOYALTY4 );

		if (strFilename.Mid(2, 2) != str)
		{
			continue;
		}

		strExtra = strFilename.Mid(7,1);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CAutoBackupProcessor::BuildAutoBackupList( CStringArray& arrayAutoBackup )
{
	arrayAutoBackup.RemoveAll();

	CString strPath = AutoBackupOptions.GetPath();
	if ( strPath.GetLength() > 0 )
	{
		if (strPath.Right(1) != "\\")
		{
			strPath += "\\";
		}
	}

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPath + "*.*" );

	CReportConsolidationArray<CSortedStringByString> arrayFolders;
	
	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() == TRUE)
		{
			continue;
		}

		if (finder.IsDirectory() == FALSE)
		{
			continue;
		}

		CString strFolderName = finder.GetFileName();
		if (strFolderName.GetLength() != 15)
		{
			continue;
		}

		CString strDate = strFolderName.Left(8);
		CString strTime = strFolderName.Right(6);

		if (::TestNumeric(strDate) == FALSE)
		{
			continue;
		}

		if (::TestNumeric(strTime) == FALSE)
		{
			continue;
		}

		COleDateTime oleDate = COleDateTime(
			atoi( strDate.Left(4) ),
			atoi( strDate.Mid(4,2) ),
			atoi( strDate.Right(2) ),
			atoi( strTime.Left(2) ),
			atoi( strTime.Mid(2,2) ),
			atoi( strTime.Right(2) ) );
			
		if (oleDate.GetStatus() != COleDateTime::valid)
		{
			continue;
		}

		int nVersion;
		int nFileSize;
		CString strExtra = "";
		if (CheckZipFolder(strPath + strFolderName, nVersion, strExtra, nFileSize) == FALSE)
		{
			continue;
		}

		strDate.Format( "%s/%s/%s",
			(const char*) strFolderName.Mid(6,2),
			(const char*) strFolderName.Mid(4,2),
			(const char*) strFolderName.Left(4) );

		strTime.Format( "%s:%s:%s",
			(const char*) strFolderName.Mid(9,2),
			(const char*) strFolderName.Mid(11,2),
			(const char*) strFolderName.Mid(13,2));
			
		CCSV csvFolder;
		csvFolder.Add( strDate );
		csvFolder.Add( strTime );
		csvFolder.Add( nVersion );
		csvFolder.Add( nFileSize );
		csvFolder.Add( strExtra );

		CSortedStringByString item;
		item.m_strKey = strFolderName;
		item.m_strData = csvFolder.GetLine();
		arrayFolders.Consolidate( item );

		if (arrayFolders.GetSize() > 20)
		{
			arrayFolders.RemoveAt(0);
		}
	}

	for ( int n = arrayFolders.GetSize() - 1; n >= 0; n-- )
	{
		CSortedStringByString item;
		arrayFolders.GetAt( n, item );

		CString str = item.m_strKey;
		str += ",";
		str += item.m_strData;
		arrayAutoBackup.Add( str );
	}
}

/**********************************************************************/

void CAutoBackupProcessor::ProcessBackupResult( int nResult, bool bFromMenu )
{
	if ( FALSE == m_bWantQuit )
	{
		switch ( nResult )
		{
		case BACKUP_RESULT_OK:
			ScheduleNextBackup( TRUE );
			AutoBackupOptions.Write();
			ExpireOldBackups();
			break;
		
		case BACKUP_RESULT_ERROR_CWD_FIND:
		case BACKUP_RESULT_ERROR_CWD_CHANGE:
		case BACKUP_RESULT_ERROR_LOCK_USERS:
		case BACKUP_RESULT_ERROR_LOCK_PROGS:
			if (FALSE == bFromMenu)
			{
				NudgeNextAttemptTime(2);
			}
			break;

		case BACKUP_RESULT_ERROR_PROCESS:
		case BACKUP_RESULT_ERROR_UNKNOWN:
		case BACKUP_RESULT_ERROR_BACKUPSQL:
		default:
			if (FALSE == bFromMenu)
			{
				NudgeNextAttemptTime(10);
			}
			break;
		}
	}
}

/**********************************************************************/

void CAutoBackupProcessor::CheckAutoBackup( CWnd* pParent )
{
	if (PCOptionsHost.GetBackupAccessType() == 0)
	{
		m_bLoggedResult = FALSE;

		if (CheckNextAttemptTime() == TRUE)
		{
			bool bForcePrompt = CheckForcePromptTime();
			bool bAlwaysPrompt = AutoBackupOptions.GetAlwaysPromptFlag();

			if ((TRUE == bAlwaysPrompt) || (TRUE == bForcePrompt))
			{
				PromptAutoBackup(pParent, bForcePrompt);
			}
			else
			{
				ProcessBackupResult(ProcessAutoBackup(pParent), FALSE);
			}
		}
	}
}

/**********************************************************************/

void CAutoBackupProcessor::PromptAutoBackup( CWnd* pParent, bool bForcePrompt )
{
	if (PCOptionsHost.GetBackupAccessType() == 0)
	{
		bool bLockCheck = TRUE;

		if (FALSE == bForcePrompt)
		{
			if (System.LockAllUsers() == TRUE)
			{
				System.UnlockAllUsers();
			}
			else
			{
				bLockCheck = FALSE;
			}
		}

		if (TRUE == bLockCheck)
		{
			CBackupPromptDlg dlg(bForcePrompt, pParent);

			if (dlg.DoModal() == IDOK)
			{
				if (dlg.m_nSelectedAction != 0)
				{
					ScheduleNextPrompt(dlg.m_nSelectedAction, bForcePrompt);
				}
				else
				{
					if (AutoBackupOptions.GetAutoBackupType() == AUTOBACKUP_TYPE_NONE)
					{
						ProcessBackupResult(ManualBackupInternal(pParent), FALSE);
					}
					else
					{
						ProcessBackupResult(ProcessAutoBackup(pParent), FALSE);
					}
				}
			}
		}
	}
}

/**********************************************************************/

int CAutoBackupProcessor::ProcessAutoBackup( CWnd* pParent )
{
	m_bWantQuit = FALSE;
	
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "";
	infoLock.m_strStopCommsPrompt = "Auto Backup";
	infoLock.m_bStopLoyalty = TRUE;
	infoLock.m_pParentWnd = pParent;

	int nResult = 0;
	int nLockResult = 0;
	bool bAttempted = FALSE;
	CString strOnScreenError = "";
	
	{
		CLockManager LockManager( infoLock );

		if ( LockManager.GetLock() == TRUE )
		{
			nResult = ProcessAutoBackupInternal( strOnScreenError );
			bAttempted = TRUE;
		}
		else if (LockManager.GetLockResult() == LOCK_RESULT_FAIL_USERS)
		{
			nResult = BACKUP_RESULT_ERROR_LOCK_USERS;
		}
		else
		{
			nResult = BACKUP_RESULT_ERROR_LOCK_PROGS;
		}
	}

	LogResult( GetNextAttemptReason(), nResult, strOnScreenError );
	return nResult;
}

/**********************************************************************/

int CAutoBackupProcessor::ProcessAutoBackupInternal( CString& strOnScreenError )
{
	{
		CSQLDb SQLDb;
		if (SQLDb.BackupDatabase() != SQLCRUD_ERR_NONE)
		{
			return BACKUP_RESULT_ERROR_BACKUPSQL;
		}
	}

	strOnScreenError = "";
	m_bWantQuit = FALSE;

	CString strCwd = "";
	CString strDataPath = Sysset.GetDataPath();
	
	if (strDataPath != "")
	{
		char cBuffer[_MAX_PATH];
		if (_getcwd(cBuffer, _MAX_PATH) == NULL)
		{
			return BACKUP_RESULT_ERROR_CWD_FIND;
		}

		strCwd = cBuffer;

		if (_chdir(strDataPath) != 0)
		{
			return BACKUP_RESULT_ERROR_CWD_CHANGE;
		}
	}
	
	bool bComplete = FALSE;

	CString strBackupPath = AutoBackupOptions.GetPath();
	if (strBackupPath.GetLength() > 0)
	{
		if (strBackupPath.Right(1) != "\\")
		{
			strBackupPath += "\\";
		}
	}

	COleDateTime oleTimeNow = COleDateTime::GetCurrentTime();
	CString strTime;
	strTime.Format( "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		oleTimeNow.GetYear(),
		oleTimeNow.GetMonth(),
		oleTimeNow.GetDay(),
		oleTimeNow.GetHour(),
		oleTimeNow.GetMinute(),
		oleTimeNow.GetSecond());
	strBackupPath += strTime;
	::CreateSubdirectory( strBackupPath );

	{
		CSQLScriptExport Export;
		Export.DoExport();
	}

	{
		//StatusProgress.Lock();
		Prompter.SetPreventParentEnableFlag( TRUE );
		CSystemBackup MyBackup ( strBackupPath );
		Prompter.SetPreventParentEnableFlag( FALSE );
		//StatusProgress.Unlock();
		strOnScreenError = MyBackup.GetOnScreenError();
		bComplete = MyBackup.IsComplete();
	}

	if ( strDataPath != "" )
	{
		if (_chdir(strCwd) != 0)
		{
			m_bWantQuit = TRUE;
		}
	}

	if (strOnScreenError != "")
	{
		return BACKUP_RESULT_ERROR_PROCESS;
	}

	if (bComplete == FALSE)
	{
		return BACKUP_RESULT_ERROR_UNKNOWN;
	}

	return BACKUP_RESULT_OK;
}

/**********************************************************************/

void CAutoBackupProcessor::ManualBackup( CWnd* pParent ) 
{
	ProcessBackupResult( ManualBackupInternal( pParent ), TRUE );
}

/**********************************************************************/

int CAutoBackupProcessor::ManualBackupInternal( CWnd* pParent )
{
	CString strOnScreenError = "";
	int nResult = BACKUP_RESULT_OK;
	m_bWantQuit = FALSE;	
	
	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = TRUE;
	infoLock.m_strLockAllUsersPrompt = "";
	infoLock.m_strStopCommsPrompt = "Backup System";
	infoLock.m_bStopLoyalty = TRUE;
	infoLock.m_pParentWnd = pParent;

	bool bAttempted = FALSE;
	{
		CLockManager LockManager(infoLock);

		if (LockManager.GetLock() == TRUE)
		{
			nResult = ManualBackupInternal(strOnScreenError, infoLock);
			bAttempted = TRUE;
		}
		else if (LOCK_RESULT_FAIL_USERS == LockManager.GetLockResult())
		{
			nResult = BACKUP_RESULT_ERROR_LOCK_USERS;
		}
		else
		{
			nResult = BACKUP_RESULT_ERROR_LOCK_PROGS;
		}
	}

	LogResult( 99, nResult, strOnScreenError );
	return nResult;
}

/**********************************************************************/

int CAutoBackupProcessor::ManualBackupInternal( CString& strOnScreenError, CLockManagerInfo& infoLock )
{
	{
		CSQLDb SQLDb;
		if (SQLDb.BackupDatabase() != SQLCRUD_ERR_NONE)
		{
			return BACKUP_RESULT_ERROR_BACKUPSQL;
		}
	}

	strOnScreenError = "";

	m_bWantQuit = FALSE;

	CString strCwd = "";
	CString strDataPath = Sysset.GetDataPath();
	
	if (strDataPath != "")
	{
		char cBuffer[_MAX_PATH];
		if (_getcwd(cBuffer, _MAX_PATH) == NULL)
		{
			return BACKUP_RESULT_ERROR_CWD_FIND;
		}

		strCwd = cBuffer;

		if (_chdir(strDataPath) != 0)
		{
			return BACKUP_RESULT_ERROR_CWD_CHANGE;
		}
	}
	
	bool bComplete = FALSE;

	{
		CSQLScriptExport Export;
		Export.DoExport();
	}

	{
		//StatusProgress.Lock();
		Prompter.SetPreventParentEnableFlag( TRUE );
		CSystemBackup MyBackup ( TRUE, FALSE, FALSE, infoLock );
		Prompter.SetPreventParentEnableFlag( FALSE );
		//StatusProgress.Unlock();
		strOnScreenError = MyBackup.GetOnScreenError();
		bComplete = MyBackup.IsComplete();
	}

	if ( strDataPath != "" )
	{
		if (_chdir(strCwd) != 0)
		{
			m_bWantQuit = TRUE;
		}
	}

	if (strOnScreenError != "")
	{
		return BACKUP_RESULT_ERROR_PROCESS;
	}

	if (bComplete == FALSE)
	{
		return BACKUP_RESULT_ERROR_UNKNOWN;
	}

	return BACKUP_RESULT_OK;
}

/**********************************************************************/

void CAutoBackupProcessor::LogResult( int nReason, int nResult, const char* szError )
{
	CAutoBackupResult result;
	result.m_oleDateTime = COleDateTime::GetCurrentTime();
	result.m_nReason = nReason;
	result.m_nResult = nResult;

	if (BACKUP_RESULT_ERROR_PROCESS == nResult)
	{
		result.m_strError = szError;
	}
	else
	{
		result.m_strError = "";
	}

	m_arrayResults.InsertAt( 0, result );

	if (m_arrayResults.GetSize() > 10)
	{
		m_arrayResults.RemoveAt(m_arrayResults.GetSize() - 1);
	}

	m_bLoggedResult = TRUE;
}

/**********************************************************************/

int CAutoBackupProcessor::GetLastResultLineCount()
{
	int nSize = m_arrayResults.GetSize();
	
	if (0 == nSize)
	{
		nSize = 1;
	}

	return nSize;
}

/**********************************************************************/

const char* CAutoBackupProcessor::GetLastResultLine( int nIndex )
{
	m_strLastResultLine = "";
	
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayResults.GetSize() ) )
	{
		CAutoBackupResult result = m_arrayResults.GetAt(nIndex);

		CString strDate;
		strDate.Format( "%2.2d/%2.2d/%4.4d",
			result.m_oleDateTime.GetDay(),
			result.m_oleDateTime.GetMonth(),
			result.m_oleDateTime.GetYear() );

		CString strTime;
		strTime.Format( "%2.2d:%2.2d:%2.2d",
			result.m_oleDateTime.GetHour(),
			result.m_oleDateTime.GetMinute(),
			result.m_oleDateTime.GetSecond() );

		CString strReason = GetBackupReasonText( result.m_nReason );

		CString strResult = "";
		switch( result.m_nResult )
		{
		case BACKUP_RESULT_OK:				
			strResult = "Backup completed successfully";	
			break;

		case BACKUP_RESULT_ERROR_CWD_FIND:	
			strResult = "Unable to find current working directory";
			break;

		case BACKUP_RESULT_ERROR_CWD_CHANGE:
			strResult = "Unable to change the working directory";
			break;

		case BACKUP_RESULT_ERROR_BACKUPSQL:
			strResult = "Unable to backup SQL database";
			break;

		case BACKUP_RESULT_ERROR_PROCESS:
			strResult = result.m_strError;
			break;

		case BACKUP_RESULT_ERROR_LOCK_PROGS:
			strResult = "Unable to close other programs";
			break;

		case BACKUP_RESULT_ERROR_LOCK_USERS:
			strResult = "Unable to backup while other users are logged on";
			break;

		case BACKUP_RESULT_ERROR_UNKNOWN:
		default:
			strResult = "Unknown error during backup";
			break;
		}

		CCSV csv;
		csv.Add( strDate );
		csv.Add( strTime );
		csv.Add( strReason );
		csv.Add( strResult );
		m_strLastResultLine = csv.GetLine();
	}
	else if (0 == nIndex)
	{
		m_strLastResultLine = "-----,-----,-----,No backup attempted";
	}

	return m_strLastResultLine;
}

/**********************************************************************/

const char* CAutoBackupProcessor::GetNextBackupLine()
{
	m_strNextBackupLine = "";

	if (AutoBackupOptions.GetAutoBackupType() == AUTOBACKUP_TYPE_NONE)
	{
		m_strNextBackupLine = "-----,-----,No backup scheduled";
	}
	else
	{
		CString strDate;
		strDate.Format( "%2.2d/%2.2d/%4.4d",
			m_OleNextAttemptTime.GetDay(),
			m_OleNextAttemptTime.GetMonth(),
			m_OleNextAttemptTime.GetYear() );

		CString strTime;
		strTime.Format( "%2.2d:%2.2d:%2.2d",
			m_OleNextAttemptTime.GetHour(),
			m_OleNextAttemptTime.GetMinute(),
			m_OleNextAttemptTime.GetSecond() );

		CString strReason = GetBackupReasonText( GetNextAttemptReason() );

		CCSV csv;
		csv.Add( strDate );
		csv.Add( strTime );
		csv.Add( strReason );
		m_strNextBackupLine = csv.GetLine();
	}

	return m_strNextBackupLine;
}

/**********************************************************************/

const char* CAutoBackupProcessor::GetBackupReasonText( int nReason )
{
	switch( nReason )
	{
	case BACKUP_REASON_SCHEDULED_HOURLY:	m_strReason = "Scheduled Hourly Backup";	break;
	case BACKUP_REASON_SCHEDULED_DAILY:		m_strReason = "Scheduled Daily Backup";		break;
	case BACKUP_REASON_SCHEDULED_WEEKLY:	m_strReason = "Scheduled Weekly Backup";	break;
	case BACKUP_REASON_SCHEDULED_CUSTOM:	m_strReason = "Scheduled Custom Backup";	break;
	case BACKUP_REASON_TIMESINCELAST:		m_strReason = "Time Since last backup";		break;
	case BACKUP_REASON_OVERDUE:				m_strReason = "Next backup is overdue";		break;
	case BACKUP_REASON_FROMMENU:			m_strReason = "Direct menu selection";		break;
	default:								m_strReason = "Unknown";					break;
	}
	return m_strReason;
}

/**********************************************************************/

void CAutoBackupProcessor::TraceTime( const char* szText, COleDateTime& dateTime )
{
#ifdef _DEBUG

	CString str;
	str.Format ( "%s %2.2d:%2.2d:%2.2d  %2.2d:%2.2d:%4.4d \n",
		szText,
		dateTime.GetHour(),
		dateTime.GetMinute(),
		dateTime.GetSecond(),
		dateTime.GetDay(),
		dateTime.GetMonth(),
		dateTime.GetYear() );
	TRACE( str );

#endif
}

/**********************************************************************/

void CAutoBackupProcessor::TraceStatus( COleDateTime timeNow, const char* szHeader )
{
#ifdef _DEBUG

	COleDateTime timeLast;
	AutoBackupOptions.GetOleTimeLastBackup( timeLast );

	CString strHeader;
	strHeader.Format ( "\n%s\n", (const char*) szHeader );
	TRACE( strHeader );
	TraceTime( "Time Now     : ", timeNow );
	
	if (AutoBackupOptions.GetAutoBackupType() != AUTOBACKUP_TYPE_NONE)
	{
		TraceTime("Backup Due   : ", m_OleNextBackupTime);
	}

	TraceTime( "Last Backup  : ", timeLast );
	TraceTime( "Next Attempt : ", m_OleNextAttemptTime );
	TraceTime( "Force Prompt : ", m_OleForcePromptTime );
	CString str;
	str.Format( "Reason : %d\n\n", GetNextAttemptReason() );
	TRACE( str );

#endif
}

/**********************************************************************/
