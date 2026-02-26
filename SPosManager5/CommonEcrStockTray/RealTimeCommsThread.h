#pragma once
/**********************************************************************/
#include "5000SiteArray.h"
/**********************************************************************/
#define REALTIME_FILEBUFFER_SALE 0
#define REALTIME_FILEBUFFER_DEPOSIT 1
#define REALTIME_FILEBUFFER_EOD 2
/**********************************************************************/

struct CRealTimeFileBufferInfo
{
public:
	CRealTimeFileBufferInfo();
	
public:
	const char* GetOriginalFilename();
	const char* GetRenamedFilename();
	const char* GetDestinationFilename();

public:
	CString		m_strFileTitle;
	CString		m_strFileExt;
	int			m_nRenameType;
	int			m_nFileType;

private:
	CString m_strOriginalFilename;
	CString m_strRenamedFilename;
	CString m_strDestinationFilename;
};

/**********************************************************************/

struct CRealTimeCommsThreadInfo
{
public:
	CRealTimeCommsThreadInfo();

public:
	int m_nSpeed;

public:
	bool m_bCASHRSP;
	bool m_bLogAllCASHRSPFiles;
	bool m_bForceQuit;
	C5000SiteInfo m_siteInfoSales;
	bool m_bEnableLocal;
	int m_nTransactionCount;
	int m_nZeroFileCount;

public:
	bool m_bProducts;
	bool m_bCustomer;
	C5000SiteInfo m_siteInfoProducts;
	C5000SiteInfo m_siteInfoCustomer;

public:
	bool m_bPMS;
	bool m_bAllPMS;
	CString m_strPMSDownloadFolder;
	CString m_strPMSRoomFolder;
	CString m_strPMSLockFile;
	CString m_strPMSPostPath;
	CString m_strPMSFolderPending;
	CString m_strPMSFolderSales;

public:
	CString m_strSchedLogAllFilename;
	CString m_strSchedLogOldFilename;
	CString m_strNetworkLogAllFilename;
	CString m_strNetworkLogOldFilename;
	long m_lMaxSchedLogFileSize;
	long m_lMaxNetworkLogFileSize;

public:
	CString m_strPosUtilityStopFile;
	bool m_bPosUtilityStopFlag;
	int m_nMaxUsers;
};

/**********************************************************************/

class CRealTimeCommsThread
{
public:
	static UINT DoComms( LPVOID pParam );

private:
	static void DoCommsInternal( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit );

private:
	static void BuildCASHRSPBuffer ( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int& nForceQuit );
	static void ProcessCASHRSPBuffer( CRealTimeCommsThreadInfo* pInfo, CArray<CRealTimeFileBufferInfo,CRealTimeFileBufferInfo>& arrayBuffer, int& nForceQuit );
	static void BuildProductBuffer ( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit );
	static void ProcessProductBuffer( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit );
	static void BuildCustomerBuffer ( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit );
	static void ProcessCustomerBuffer( CRealTimeCommsThreadInfo* pInfo, CStringArray& arrayBuffer, int& nForceQuit );
	static void ProcessHandheldExports( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit );
	static bool ProcessProductFileInternal( CRealTimeCommsThreadInfo* pInfo, CString& strOriginalFolder, CString& strOriginalFilename, CString& strLocalTmpFolder, CString& strLocalTmpFilename );
	static bool ProcessCustomerFileInternal( CRealTimeCommsThreadInfo* pInfo, CString& strOriginalFolder, CString& strOriginalFilename, CString& strLocalTmpFolder, CString& strLocalTmpFilename );
	static void DownloadPMSRooms( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit );

private:
	static void ImportPMSSales( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit );
	static void ImportPMSSalesInternal( CRealTimeCommsThreadInfo* pInfo, int& nForceQuit );
	static void BuildPMSBuffer( CRealTimeCommsThreadInfo* pInfo, CString& strPostPath, CStringArray& arrayFilenames, int& nForceQuit );
	static void ProcessPMSBuffer( CRealTimeCommsThreadInfo* pInfo, CString& strPostPath, CStringArray& arrayFilenames, int& nForceQuit );
	static bool ProcessPMSFile ( CString& strOriginalFilename, CString& strRenamedFilename, CString& strLocalTempFile, CString& strLocalSalesFile );

public:
	static void LogRealTimeCommsResult( CRealTimeCommsThreadInfo* pInfo, const char* szHeader, const char* szLine1, const char* szFile1, const char* szLine2, const char* szFile2 );
	static void LogRealTimeCommsMessage( CRealTimeCommsThreadInfo* pInfo, const char* szText, bool bLineBefore = FALSE, bool bHide = FALSE );
	static void GetSchedulerLogLine( const char* szText, bool bHide, CString& strLine );
	static void CheckMaxLogFileSize( CString& strNew, CString& strOld, long lSize );
	static bool CheckPosTrayStopFile( CRealTimeCommsThreadInfo* pInfo );
	static int GetFilesToProcess( int nSpeed );
};

/**********************************************************************/
