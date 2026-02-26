#pragma once
/**********************************************************************/
#include "..\CommonEcrStock\JobListCSVArray.h"
#include "PosTrayTask.h"
#include "PosTrayTaskInfo.h"
/**********************************************************************/

struct CMessageLoggerInfo
{
public:
	CMessageLoggerInfo();

public:
	CString m_strMessage;
	CString m_strStatus;
	int m_nHideLevel;
	int m_nNetworkIdx;
	bool m_bAddNetworkName;
	bool m_bAddDate;
	bool m_bAddTimeOfDay;
	bool m_bLeadingLineNetwork;
	bool m_bFollowingLineNetwork;
	bool m_bLeadingLineSystem;
	bool m_bFollowingLineSystem;
	bool m_bLogSystemStartStop;
};

/**********************************************************************/

struct CEmailLoggerInfo
{
public:
	CEmailLoggerInfo();

public:
	CString m_strFilename;
	CString m_strEmail;
	CString m_strSubject;
	CString m_strResult;
	CString m_strDate;
	CString m_strTime;
	int m_nHideLevel;
};

/**********************************************************************/

struct CAutoDeleteCustLogInfo
{
public:
	CAutoDeleteCustLogInfo();

public:
	CString m_strDate;
	CString m_strTime;
	int m_nLocIdx;
	bool m_bOneOff;
	CString m_strInfo;
	int m_nHideLevel;
};

/**********************************************************************/

struct CExportLoggerInfo
{
public:
	CExportLoggerInfo();

public:
	CString m_strType;
	CString m_strTargetFolder;
	CString m_strTargetFile;
	CString m_strResult;
	CString m_strDate;
	CString m_strTime;
	int m_nHideLevel;
};

/**********************************************************************/

class CMessageLogger  
{
public:
	CMessageLogger();

	void Initialise();
	void LogSchedulerMessage ( CMessageLoggerInfo& info, bool bNetwork, bool bSystem, bool bTemp );
	void LogIDraughtSystem ( CMessageLoggerInfo& info );
	void LogFNBSystem ( CMessageLoggerInfo& info );
	void LogChartwellsSystem(CMessageLoggerInfo& info);
	void LogFTPSystem ( CMessageLoggerInfo& info );
	void LogS4LabourSystem ( CMessageLoggerInfo& info );
		
	void LogSystem ( int nContext, int nType, bool bDate );
	void LogSystem ( int nContent, int nType, bool bDate, DWORD dwExtra );
	void LogSystem ( int nContent, int nType, bool bDate, const char* szExtra );
	void LogSystem ( int nContext, int nType, bool bDate, CDWordArray& dwExtra );
	void LogSystem ( int nContext, int nType, bool bDate, CStringArray& strExtra );	
	void LogSystem ( int nContext, int nType, bool bDate, CDWordArray& dwExtra, CStringArray& strExtra );
	void LogEmailSystem( CEmailLoggerInfo& info );
	void LogExportSystem( CExportLoggerInfo& info );
	void LogAutoDeleteCustomer( CAutoDeleteCustLogInfo& info );

	void LogFileWrite( const char* szFilename, int nResult, int nLastError );
	void LogWebAPICall( const char* szCall );
	
	const char* GetLastNetworkMessage(){ return m_strLastNetworkMessage; }
	const char* GetLastSystemMessage(){ return m_strLastSystemMessage; }
	const char* GetLastEmailMessage(){ return m_strLastEmailMessage; }
	const char* GetLastExportMessage(){ return m_strLastExportMessage; }
	
public:
	static void CheckMaxLogNwkFileSize( CString& strNew, CString& strOld );
	static void CheckMaxLogSysFileSize( CString& strNew, CString& strOld );

	void GetSchedulerLogLine( CMessageLoggerInfo& info, CString& strLine);
	const char* GetEmailLogLine( CEmailLoggerInfo& info );
	const char* GetExportLogLine( CExportLoggerInfo& info );
	const char* GetAutoDeleteCustomerLogLine( CAutoDeleteCustLogInfo& info );

public:
	void LogSchedulerSystem( CMessageLoggerInfo& info );
	void LogSchedulerNetwork( CMessageLoggerInfo& info );

#ifndef POSTRAY_UTILITY

public:
	void LogFTPConnect( int nNetworkIdx, int nSSErrorNo, int nWindowsErrorNo, bool bTemp );
	void LogFTPCommsResult( CJobListCSVRecord& JobInfo, int nSSErrorNo, int nWindowsErrorNo, const char* szExtraInfo, bool bTemp );
	void LogFTPFileTransfer( const char* szSource, const char* szDest, bool bTemp );
	void LogFTPDisconnect( int nNetworkIdx, bool bTemp );

private:
	const char* GetFTPSchedulerErrorText( int nSSErrorNo, int nWindowsErrorNo );
	const char* GetFTPSimpleErrorText( int nError );

#else

public:
	void LogPosTraySystem( CPosTrayEmailScheduleInfo& info, CPosTrayTask& Task, const char* szAction, const char* szResult );
	void LogPosTrayInitialiseSystem( bool bResult );
	void LogPosTrayClose( int nResult );
	
private:
	const char* GetPosTrayLogDateTime( COleDateTime& datetime );
	void CheckPosTrayLogFile();
	
#endif

private:
	int m_nLineIndex;
	CString m_strLastDate;
	CString m_strChecksumSeed;
	CString m_strLastNetworkMessage;
	CString m_strLastSystemMessage;
	CString m_strLastEmailMessage;
	CString m_strLastExportMessage;
	CString m_strLastAutoDeleteCustLogMessage;
	CString m_strSchedulerLogLine;
	CString m_strEmailLogLine;
	CString m_strAutoDeleteCustomerLogLine;
	CString m_strExportLogLine;
	
private:
	CString m_strFTPSimpleErrorText;
	CString m_strFTPSchedulerErrorText;
	CString m_strPosTrayLogDateTime;
	
private:
	CSSLogFile m_LogFileEmail;
	CSSLogFile m_LogFileExport;
	CSSLogFile m_LogFileScheduler;
	CSSLogFile m_LogFileGrayLog;
	CSSLogFile m_LogFileWrite;
	CSSLogFile m_LogFileWebAPICall;
	CSSLogFile m_LogFileAutoDeleteCustLog;
};

/**********************************************************************/
extern CMessageLogger MessageLogger;
/**********************************************************************/
