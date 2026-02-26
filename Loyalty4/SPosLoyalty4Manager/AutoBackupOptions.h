#pragma once
/**********************************************************************/
#define AUTOBACKUP_TYPE_NONE 0
#define AUTOBACKUP_TYPE_HOURLY 1
#define AUTOBACKUP_TYPE_DAILY 2
#define AUTOBACKUP_TYPE_WEEKLY 3
#define AUTOBACKUP_TYPE_CUSTOM 4
/**********************************************************************/
#define BACKUP_MODE_MON_FRI 0			
#define BACKUP_MODE_MON_SAT 1			
#define BACKUP_MODE_EVERYDAY 2		
#define BACKUP_MODE_INDIVIDUAL 3
/**********************************************************************/
#define DAYFLAGS_SUN 1
#define DAYFLAGS_MON 2
#define DAYFLAGS_TUE 4
#define DAYFLAGS_WED 8
#define DAYFLAGS_THU 16
#define DAYFLAGS_FRI 32
#define DAYFLAGS_SAT 64
#define DAYFLAGS_MON_FRI 62
#define DAYFLAGS_MON_SAT 126
#define DAYFLAGS_EVERYDAY 127
/**********************************************************************/

class CAutoBackupOptions 
{
public:
	CAutoBackupOptions();
	
	bool Read();
	bool Write();

	const char* GetCSVLine();

	void RememberSettings() { m_strMemory = GetCSVLine(); }
	void RestoreSettings() { ReloadFromString( m_strMemory ); }
	bool HasScheduleChanged();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	int GetAutoBackupType(){ return m_nAutoBackupType; }
	int GetHourlyMinute(){ return m_nHourlyMinute; }
	int GetDailyHour(){ return m_nDailyHour; }
	int GetDailyMinute(){ return m_nDailyMinute; }
	int GetWeeklyHour(){ return m_nWeeklyHour; }
	int GetWeeklyMinute(){ return m_nWeeklyMinute; }
	int GetWeeklyDay(){ return m_nWeeklyDay; }
	int GetCustomMinute(){ return m_nCustomMinute; }
	int GetCustomMode(){ return m_nCustomMode; }
	int GetMondayMask(){ return m_nMondayMask; }
	int GetTuesdayMask(){ return m_nTuesdayMask; }
	int GetWednesdayMask(){ return m_nWednesdayMask; }
	int GetThursdayMask(){ return m_nThursdayMask; }
	int GetFridayMask(){ return m_nFridayMask; }
	int GetSaturdayMask(){ return m_nSaturdayMask; }
	int GetSundayMask(){ return m_nSundayMask; }
	int GetMonFriMask(){ return m_nMonFriMask; }
	int GetMonSatMask(){ return m_nMonSatMask; }
	int GetEverydayMask(){ return m_nEverydayMask; }
	const char* GetPath(){ return m_strPath; }
	int GetArchiveCount(){ return m_nArchiveCount; }
	int GetHoursBeforePrompt(){ return m_nHoursBeforePrompt; }
	int GetMinutesBeforeOverdue(){ return m_nMinutesBeforeOverdue; }
	bool GetOleTimeLastBackup( COleDateTime& time);
	void GetOleTimeNextBackup( COleDateTime& time){ time = m_OleTimeNextBackup; }
	bool GetAlwaysPromptFlag(){ return m_bAlwaysPrompt; }
	bool GetExcludeZeroSizeFilesFlag(){ return m_bExcludeZeroSizeFiles; }
	int GetCompressionLevel() { return m_nCompressionLevel; }

	void SetAutoBackupType( int n );
	void SetHourlyMinute( int n );
	void SetDailyHour( int n );
	void SetDailyMinute( int n );
	void SetWeeklyHour( int n );
	void SetWeeklyMinute( int n );
	void SetWeeklyDay( int n );
	void SetCustomMinute( int n );
	void SetCustomMode( int n );
	void SetMondayMask( int n );
	void SetTuesdayMask( int n );
	void SetWednesdayMask( int n );
	void SetThursdayMask( int n );
	void SetFridayMask( int n );
	void SetSaturdayMask( int n );
	void SetSundayMask( int n );
	void SetMonFriMask( int n );
	void SetMonSatMask( int n );
	void SetEverydayMask( int n );
	void SetPath( const char* szPath );
	void SetArchiveCount( int n );
	void SetHoursBeforePrompt( int n );
	void SetMinutesBeforeOverdue( int n );
	void SetOleTimeNextBackup( const char* sz );
	void SetOleTimeNextBackup( const COleDateTime& time );
	void SetOleTimeLastBackup( const char* sz );
	void SetOleTimeLastBackup();
	void SetAlwaysPromptFlag( bool b ){ m_bAlwaysPrompt = b;  }
	void SetExcludeZeroSizeFilesFlag( bool b ){ m_bExcludeZeroSizeFiles = b; }
	void SetCompressionLevel(int n);

	void SetDefaults();
	void Reset();

private:
	void ReloadFromString( CString& strBuffer );

private:
	int m_nAutoBackupType;
	int m_nHourlyMinute;
	int m_nDailyHour;
	int m_nDailyMinute;
	int m_nWeeklyHour;
	int m_nWeeklyMinute;
	int m_nWeeklyDay;
	int m_nCustomMinute;
	int m_nCustomMode;
	int m_nMondayMask;
	int m_nTuesdayMask;
	int m_nWednesdayMask;
	int m_nThursdayMask;
	int m_nFridayMask;
	int m_nSaturdayMask;
	int m_nSundayMask;
	int m_nMonFriMask;
	int m_nMonSatMask;
	int m_nEverydayMask;
	CString m_strPath;
	int m_nArchiveCount;
	int m_nHoursBeforePrompt;
	int m_nMinutesBeforeOverdue;
	COleDateTime m_OleTimeNextBackup;
	COleDateTime m_OleTimeLastBackup;
	bool m_bAlwaysPrompt;
	bool m_bExcludeDailySales;
	bool m_bExcludeZeroSizeFiles;
	int m_nCompressionLevel;

	CString m_strCSVLine;
	CString m_strMemory;

public:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CAutoBackupOptions AutoBackupOptions; /* global */
/**********************************************************************/

