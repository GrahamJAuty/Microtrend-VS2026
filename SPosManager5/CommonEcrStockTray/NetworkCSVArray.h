#pragma once
/**********************************************************************/
#define NETWORK_VERSION 8
/**********************************************************************/
#define CONNECTION_TYPE_STANDARD_NONE 0
#define CONNECTION_TYPE_STANDARD_REALTIME 1
#define CONNECTION_TYPE_STANDARD_BATCH 2
//#define CONNECTION_TYPE_STANDARD_FTP1 3
//#define CONNECTION_TYPE_STANDARD_FTP2 4
/**********************************************************************/
#define CONNECTION_TYPE_SPTBOOK_NONE 100
#define CONNECTION_TYPE_SPTBOOK_WEB 101
/**********************************************************************/
#define CONNECTION_TYPE_SMARTENT_NONE 200
#define CONNECTION_TYPE_SMARTENT_WEB 201
/**********************************************************************/
#define MIN_SB_TIMEOUT 15
#define MAX_SB_TIMEOUT 300
/**********************************************************************/

struct CSelectNetworkInfo
{
	int m_nNwkIdx;
	bool m_bSelected;
};

/**********************************************************************/

class CNetworkCSVRecord : public CCSVRecord
{
public:
	CNetworkCSVRecord(); 
	virtual ~CNetworkCSVRecord(){};
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void ValidateConnectionType();
	void ValidateSPOSVersion();

public:
	void GetSPOSVersionTreeName( CString& strName, bool bBrackets );
	static void GetTreeNameExternal( int nConnectionType, int nSPOSVersion, CString& strName, bool bBrackets );
	static void GetVersionNumberExternal( int nConnectionType, int nSPOSVersion, CString& strVersion );

private:
	void SetNetworkNo ( int nNetworkNo ) { m_nNetworkNo = nNetworkNo; }
	void SetSPOSVersion( int nSPOSVersion ) { m_nSPOSVersion = nSPOSVersion; }
	void SetConnectionType ( int nType );
	void SetName ( const char* szName );
	void SetFileNetworkPath( const char* szPath );
	
	void SetSportsBookerSite( const char* szSite );
	void SetSportsBookerUserName( const char* szUserName );
	void SetSportsBookerPassword( const char* szPassword );
	void SetSportsBookerTimeOut( int nTimeOut );
	
	const char* GetSystemName();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3ConvertFromCSV( CCSV& csv );
	void V4ConvertFromCSV( CCSV& csv );
	void V5ConvertFromCSV( CCSV& csv );
	void V6to7ConvertFromCSV( CCSV& csv, int nVer );
	void V8ConvertFromCSV( CCSV& csv );
		
private:
	int m_nNetworkNo;					/* for subdirectory (1 to max networks) */
	int m_nSPOSVersion;
	int m_nConnectionType;
	CString m_strName;
	CString m_strSystemName;
	
	CString m_strFileNetworkPath;
	
	CString m_strSportsBookerSite;
	CString m_strSportsBookerUserName;
	CString m_strSportsBookerPassword;
	int m_nSportsBookerTimeOut;

private:
	//FIELDS SAVED IN MEMORY ONLY
	bool m_bGotIDraughtDate;
	COleDateTime m_oleIDraughtDate;
	CString m_strIDraughtLocationList;
	
	friend class CNetworkCSVArray;
};

/**********************************************************************/

class CNetworkCSVArray : public CPtrArray
{
public:
	CNetworkCSVArray();
	~CNetworkCSVArray();
	void RemoveAll();

	bool FindNetworkByNumber ( int nNwkNo, int& nNwkIdx );
	int AddNetwork( int nConnectionType );
	int AddNetwork( int nNwkNo, const char* szPath );
	
	void DeleteNetworkByNumber ( int nNwkNo );
	void DeleteNetworkByIndex ( int nNwkIdx, bool bDeleteFiles );
	bool RenameForDeletion( int nNwkIdx );
	void ClearNetworkFolder( int nNwkIdx );
	void ClearDeletionFolder( int nNwkIdx );
	
	void GetDatabaseCommsLocArray( int nDbIdx, CArray<int,int>& array, bool& bBlocked );
	bool CanDoWebAPIComms( int nNetworkIdx, int& nDbIdx );

	void GetDownloadPermissions( bool& bCanDoDownloads );

	void SortByName( CWordArray& arrayNwkIdx );
	void SortNetworkListByName( CArray<CSelectNetworkInfo,CSelectNetworkInfo>& arraySelected );

	int GetNetworkNo( int nNetworkIdx );
	int GetSPOSVersion( int nNetworkIdx );
	const char* GetName( int nNetworkIdx );
	const char* GetTreeName( int nNetworkIdx );
	void GetSPOSVersionTreeName( int nNetworkIdx, CString& strName, bool bBrackets );
	const char* GetSystemName( int nNetworkIdx );
	const char* GetExtendedSystemName( int nNetworkIdx );
	int GetConnectionType ( int nNetworkIdx );

	const char* GetFileNetworkPath( int nNetworkIdx );
	void GetSafeFileNetworkPath( int nNetworkIdx, CString& strPath );
	const char* GetFileNetworkPathEODFiles( int nNetworkIdx, const char* szFolder, COleDateTime& date );
	
	const char* GetSportsBookerSite( int nNetworkIdx );
	const char* GetSportsBookerUserName( int nNetworkIdx );
	const char* GetSportsBookerPassword( int nNetworkIdx );
	int GetSportsBookerTimeOut( int nNetworkIdx );
	
	void SetNetworkNo( int nNetworkIdx, int nNo );
	void SetSPOSVersion( int nNetworkIdx, int nSPOSVersion );
	void SetName ( int nNetworkIdx, const char* szName );
	void SetConnectionType ( int nNetworkIdx, int nType );
	
	void SetFileNetworkPath( int nNetworkIdx, const char* szPath );
	
	void SetSportsBookerSite( int nNetworkIdx, const char* szSite );
	void SetSportsBookerUserName ( int nNetworkIdx, const char* szName );
	void SetSportsBookerPassword ( int nNetworkIdx, const char* szPassword );
	void SetSportsBookerTimeOut ( int nNetworkIdx, int nTimeOut );
	
	const char* GetFolderPathData( int nNetworkIdx );
	const char* GetFolderPathDataDelete( int nNetworkIdx );
	const char* GetFolderPathDataHistory( int nNetworkIdx );
	const char* GetFolderPathDataEODFiles( int nNetworkIdx, const char* szFolder );
	
	const char* GetFolderPathSysset( int nNetworkIdx );
	const char* GetFolderPathSyssetDelete( int nNetworkIdx );
	const char* GetFolderPathSyssetPending( int nNetworkIdx, int n );
	const char* GetFolderPathSyssetCashRSP( int nNetworkIdx );
	const char* GetFolderPathSyssetCashRSPFailed( int nNetworkIdx );
	const char* GetFolderPathSyssetStock( int nNetworkIdx );
	const char* GetFolderPathSyssetBatchExport( int nNetworkIdx );
	
	const char* GetFilePathInfo( int nNetworkIdx );

	const char* GetFilePathSyssetSchedLogAll ( int nNwkIdx );
	const char* GetFilePathSyssetSchedLogOld ( int nNwkIdx );

	void WriteReasonTokenFile( int nNetworkIdx );
	void DeleteReasonTokenFile( int nNetworkIdx );
	bool CheckReasonTokenFile( int nNetworkIdx );

	const char* GetCSVLine( int nNetworkIdx );
	
	bool WriteNetwork( int nNetworkIdx );

	bool ValidateNetworks( bool bDisableComms, bool bStandard, bool bSportsBooker, bool bSmartEnt );

	void WriteBadTerminals( CReportFile& ReportFile, CStringArray& arrayTNo, CString& strCurrentClump, int nNetworkIdx );

private:
	CNetworkCSVRecord* GetNetwork( int nNwkIdx ) { return (CNetworkCSVRecord*) GetAt ( nNwkIdx ); }
	
private:
	const char* GetFilePathDataReasonToken( int nNetworkIdx );
	
private:
	CString m_strName;
	CString m_strTreeName;
	CString m_strSystemName;
	CString m_strNetworkPath;
	CString m_strSportsBookerSite;
	CString m_strSportsBookerUserName;
	CString m_strSportsBookerPassword;
	
private:
	CString m_strCSVLine;
	CString m_strFolderPath;
	CString m_strFolderPathDelete;
	CString m_strFolderPathPending;
	CString m_strFilePath;
	CString m_strFilePathInfo;
	CString m_strFilePathSchedLogAll;
	CString m_strFilePathSchedLogOld;

private:
	CString m_strIDraughtLocationList;
};

/**********************************************************************/
extern CNetworkCSVArray dbNetwork;
/**********************************************************************/
