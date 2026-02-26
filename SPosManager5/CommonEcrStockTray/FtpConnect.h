#pragma once
/**********************************************************************/
#include <afxinet.h>
/**********************************************************************/
#include "ChartwellsOptions.h"
#include "Consolidation.h"
#include "FNBDatabaseOptions.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
//#define FTP_FILTERTYPE_FILE_CASHRSP_CONSOL 0
#define FTP_FILTERTYPE_FILE_SSFILE 1
#define FTP_FILTERTYPE_FOLDER_EOD 2
#define FTP_FILTERTYPE_FOLDER_DATE 3
#define FTP_FILTERTYPE_FILE_REASON 4
#define FTP_FILTERTYPE_FILE_PRODCSV 5
#define FTP_FILTERTYPE_FILE_CUSTOMER 6
//#define FTP_FILTERTYPE_FILE_CASHRSP_BYTRAN 7
/**********************************************************************/
#include "..\CommonEcrStock\JobListCSVArray.h"
/**********************************************************************/

struct CFtpTransferInfo
{
public:
	CFtpTransferInfo();
	void Reset();

public:
	int m_nFailedRename;
	int m_nFailedTransfer;
	int m_nFilesCopied;
	int m_nFailedDelete;
	int m_nPutAside;
};

/**********************************************************************/

class CFtpConnect
{
public:
	CFtpConnect(void);
	~CFtpConnect(void);

	bool ConnectIDraught();
	bool ConnectFNBExport( CFNBDatabaseOptions& Options );
	bool ConnectChartwellsExport(CChartwellsOptions& Options);
	bool ConnectS4LabourExport();
	bool ConnectPosTrayExport();

	void Disconnect();

	bool GetWantErrorDisconnectFlag(){ return m_bWantErrorDisconnect; }
	int GetLastFtpError(){ return m_nLastError; }

public:
	bool PushDirectory( const char* szPath );
	bool PopDirectory( CString& strOldPath );
	bool GetCurrentDirectory( CString& strDirectory );
	bool CreateDirectory( const char* szPath );

public:
	bool OpenFile( CInternetFile** pFile, const char* szFilename, DWORD dwType );
	bool ReadFromFile( CInternetFile* pFile, CString& strBuffer );
	bool WriteToFile( CInternetFile* pFile, const char* sz );
	bool CloseFile( CInternetFile* pFile );
	
public:
	bool GetFile( const char* szSource, const char* szDest, bool bFailIfExists );
	bool PutFile( const char* szSource, const char* szDest );
	bool DeleteFile( const char* szFile );
	bool RenameFile( const char* szOld, const char* szNew );
	bool FileExists( const char* szFile );

public:
	bool GetFilteredFileList( CReportConsolidationArray<CSortedStringItem>& arrayFiles, CStatic& staticInfo, int nFilterType );

public:
	CFtpConnection* GetFtpConnection(){ return m_pFtpConnection; }

private:
	bool PushDirectoryInternal( const char* szPath );
	bool PopDirectoryInternal( CString& strOldPath );

private:
	bool GetFilteredFileListInternal( CReportConsolidationArray<CSortedStringItem>& arrayFiles, CStatic& staticInfo, int nFilterType );

private:
	CInternetSession m_InternetSession;
	CFtpConnection* m_pFtpConnection;

private:
	CString m_strCurrentServer;
	CString m_strCurrentUserName;
	CString m_strCurrentPassword;
	int m_nCurrentPortNo;

private:
	CStringArray m_arrayFolderStack;

private:
	bool m_bWantErrorDisconnect;
	int m_nLastError;
};

/**********************************************************************/
extern CFtpConnect FtpConnect;
/**********************************************************************/

