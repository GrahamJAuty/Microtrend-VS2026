#pragma once
/**********************************************************************/
#include "SelectLocInfo.h"
/**********************************************************************/
#define LOCIDX_SPECIAL_UNKNOWN 0x1000000
#define LOCIDX_SPECIAL_IMPORT 0x2000000
#define LOCIDX_SPECIAL_MANUAL 0x3000000
/**********************************************************************/
#define LOCATION_VERSION 23
/**********************************************************************/
#define MAX_PRICE_SETS 3
/**********************************************************************/
#define MAX_TERMINALS 254
/**********************************************************************/
#define PLUDOWNLOADRANGE_COUNT 5
#define PLUDOWNLOADRANGE_NONE 0
#define PLUDOWNLOADRANGE_INCLUDE 1
#define PLUDOWNLOADRANGE_EXCLUDE 2
/**********************************************************************/
#define IDRAUGHT_STATUS_UNKNOWN 0
#define IDRAUGHT_STATUS_VALID 1
#define IDRAUGHT_STATUS_INVALID_SITE 2
#define IDRAUGHT_STATUS_INVALID_TERMINAL 3
#define IDRAUGHT_STATUS_INVALID_DBLINK 4
/**********************************************************************/
#define EODIMPORT_FOLDERTYPE_NONE 0
#define EODIMPORT_FOLDERTYPE_ROOT 1
#define EODIMPORT_FOLDERTYPE_INTRADING 2
/**********************************************************************/

struct CTerminalLinks
{
public:
	CTerminalLinks();

public:
	int m_nTNo;
	int m_nSpNo;
	int m_nRpNoLegacy;
	int m_nRpNoReport;
	__int64 m_nRpFlags;
};

/**********************************************************************/

class CLocationCSVRecord : public CCSVRecord
{
public:
//csv line handling
	CLocationCSVRecord(); 
	virtual ~CLocationCSVRecord(){};
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Validate( int& nTarget, int nMin, int nMax, int nDefault );

//location level functions
private:
	void SetLocNo ( int nLocNo ) { m_nLocNo = nLocNo; }
	void SetNetworkNo( int nNetworkNo ) { m_nNetworkNo = nNetworkNo; }
	void SetDbNo( int nDbNo ) { m_nDbNo = nDbNo; }
	void SetName( const char* szName );
	void SetIDraughtID( const char* szID );
	void SetRemoteID( const char* szID );
	void SetPluPriceSetCount( int nCount ) { m_nPluPriceSetCount = nCount; }
	void SetDefaultPriceSet ( int nSet ) { m_nDefaultPriceSet = nSet; }
	void SetDbPriceSet1( int nSet ) { m_nDbPriceSet1 = nSet; }
	void SetDbPriceSet2( int nSet ) { m_nDbPriceSet2 = nSet; }
	void SetDbPriceSet3( int nSet ) { m_nDbPriceSet3 = nSet; }
	void SetPriceSetKeyboard1( int nKbd ) { m_nPriceSetKeyboard1 = nKbd; }
	void SetPriceSetKeyboard2( int nKbd ) { m_nPriceSetKeyboard2 = nKbd; }
	void SetPriceSetKeyboard3( int nKbd ) { m_nPriceSetKeyboard3 = nKbd; }
	void SetDbKeyboardSet( int nSet ){ m_nDbKeyboardSet = nSet; }
	void SetTerminalExportPath( const char* szPath ) { m_strTerminalExportPath = szPath; }
	void SetDownloadPath( const char* szPath ) { m_strDownloadPath = szPath; }
	void SetDownloadNowPath( const char* szPath ) { m_strDownloadNowPath = szPath; }
	void SetExportControlFlag( bool b ){ m_bExportControl = b; }
	void SetExportControlPath( const char* szPath ) { m_strExportControlPath = szPath; }
	void SetExportControlDelay( int n ){ m_nExportControlDelay = n; }
	void SetProdCSVImportFolderType( int n ){ m_nProdCSVImportFolderType = n; }
	void SetProdCSVImportExistingMode( int n ){ m_nProdCSVImportExistingMode = n; }
	void SetProdCSVImportTypeMask( int n ){ m_nProdCSVImportTypeMask = n; }
	void SetProdCSVProtectFieldMask( int n ){ m_nProdCSVProtectFieldMask = n; }
	void SetProdCSVImportModifierMode( int n );
	void SetCustomerImportFolderType( int n ){ m_nCustomerImportFolderType = n; }
	void SetPluDownloadRangeType( int n ){ m_nPluDownloadRangeType = n; }
	void SetPluDownloadRangeStart( int x, __int64 n );
	void SetPluDownloadRangeEnd( int x, __int64 n );
	void SetBritanniaDefaultPriceBand( int n ){ m_nBritanniaDefaultPriceBand = n; }
	void SetAgressoLocationGroup( const char* sz );
	void SetLoyaltyFolderSet(int n) { m_nLoyaltyFolderSet = n; }
	
	void SetObsoleteSiteNo( int nSiteNo ) { m_nObsoleteSiteNo = nSiteNo; }
	void SetObsoleteUploadPathFlag( bool bFlag ) { m_bObsoleteUploadPath = bFlag; }
	void SetObsoleteRecordFlag( bool bFlag ) { m_bObsoleteRecord = bFlag; }
	void SetObsoleteUploadPath( const char* szPath ) { m_strObsoleteUploadPath = szPath; }

	void TidyPluDownloadRanges();

//terminal level function
private:
	int GetTerminalCount() { return m_arrayTerminals.GetSize(); }

	bool FindTerminalByNumber( int nTNo, int& nTerminalIdx );
	bool AddTerminal( int nTNo, int nRpNoLegacy, __int64 nRpFlags, int nSpNo, int& nTerminalIdx );
	void RemoveTerminal( int nTNo );
	void FixWebSalesTerminals();
	
	int GetTNo( int nTerminalIdx );
	int GetRpNoLegacy( int nTerminalIdx );
	int GetRpNoReport( int nTerminalIdx );
	__int64 GetRpFlags( int nTerminalIdx );
	int GetSpNo( int nTerminalIdx );

	void SetRpNoLegacy( int nTerminalIdx, int nRpNo );
	void SetRpNoReport( int nTerminalIdx, int nRpNo );
	void SetRpFlags( int nTerminalIdx, __int64 nFlags );
	void SetSpNo( int nTerminalIdx, int nSpNo );

	void SetRpFlag( int nTerminalIdx, int nRpNo );
	void ClearRpFlag( int nTerminalIdx, int nRpNo );
	bool GetRpFlag( int nTerminalIdx, int nRpNo );

	const char* GetSystemName();

	__int64 GetPluDownloadRangeStart( int x );
	__int64 GetPluDownloadRangeEnd( int x );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3ConvertFromCSV( CCSV& csv );
	void V4ConvertFromCSV( CCSV& csv );
	void V5To7ConvertFromCSV( CCSV& csv, int nVer );
	void V8To9ConvertFromCSV( CCSV& csv, int nVer );
	void V10To15ConvertFromCSV( CCSV& csv, int nVer );
	void V16To21ConvertFromCSV( CCSV& csv, int nVer );
	void V22To23ConvertFromCSV( CCSV& csv, int nVer );

private:
	int m_nLocNo;					
	int m_nNetworkNo;				
	int m_nDbNo;
	int m_nSiteNo;
	CString m_strName;	
	CString m_strIDraughtID;
	CString m_strSystemName;
	int m_nPluPriceSetCount;
	int m_nDefaultPriceSet;
	int m_nDbPriceSet1;
	int m_nDbPriceSet2;
	int m_nDbPriceSet3;
	int m_nPriceSetKeyboard1;
	int m_nPriceSetKeyboard2;
	int m_nPriceSetKeyboard3;
	int m_nDbKeyboardSet;
	CString m_strTerminalExportPath;
	CString m_strDownloadPath;
	CString m_strDownloadNowPath;
	bool m_bExportControl;
	CString m_strExportControlPath;
	int m_nExportControlDelay;
	int m_nProdCSVImportFolderType;
	int m_nProdCSVImportExistingMode;
	int m_nProdCSVImportTypeMask;
	int m_nProdCSVProtectFieldMask;
	int m_nProdCSVImportModifierMode;
	int m_nCustomerImportFolderType;
	int m_nPluDownloadRangeType;
	__int64 m_nPluDownloadRangeStart[PLUDOWNLOADRANGE_COUNT];
	__int64 m_nPluDownloadRangeEnd[PLUDOWNLOADRANGE_COUNT];
	int m_nBritanniaDefaultPriceBand;
	CString m_strAgressoLocationGroup;
	CString m_strRemoteID;
	int m_nLoyaltyFolderSet;
	
private:
	CArray<CTerminalLinks,CTerminalLinks> m_arrayTerminals;

private:
	int m_nObsoleteSiteNo;
	bool m_bObsoleteUploadPath;
	bool m_bObsoleteRecord;
	CString m_strObsoleteUploadPath;

	friend class CLocationCSVArray;
};

/**********************************************************************/

class CLocationCSVArray : public CPtrArray
{
//ARRAY MANAGEMENT 
public:
	CLocationCSVArray();
	~CLocationCSVArray();
	void RemoveAll();
	
	int AddLocationByUser ( int nNetworkNo, int nConnectionType );
	int AddLocation( int nNetworkNo, int nLocNo, const char* szPath );
	bool DeleteLocationByIndex ( int nLocIdx, bool bDeleteFiles );
	bool DeleteLocationByNumber ( int nNetworkNo, int nLocNo );

	void ClearLocationFolder( int nLocIdx );
	void ClearDeletionFolder( int nLocIdx );
	bool RenameForDeletion( int nLocIdx );

	void LoadLocationBuffer( int nLocIdx, CLocationCSVArray& buffer );
	void SaveLocationBuffer( int nLocIdx, CLocationCSVArray& buffer );
	
	void PurgeNetwork( int nNetworkIdx );
	void PurgeDatabase( int nDbIdx );

	bool FindLocationByNumber ( int nNetworkNo, int nLocNo, int& nIndex );	

	void GetDatabaseLocIdxSet( int nDbIdx, CArray<int,int>& Set );
	void GetNetworkLocIdxRange ( int nNetworkIdx, int& nStartIdx, int& nEndIdx );

	bool WriteLocation ( int nLocIdx );
	bool CanDoReports();

	void CreatePMSPseudoLocation();
	void RemovePMSPseudoLocation();
	bool IsPMSPseudoLocation( int nLocIdx );
	bool IsWebSalesLocation( int nLocIdx );
	int GetConnectionType( int nLocIdx );

	int GetWebSalesLocationCount();
	int GetSharpPosLocationCount();
	
	void SortByName( CArray<int,int>& arrayLocIdx, int nDbIdx = -1, int nNwkIdx = -1, bool bForceGroupByDb = FALSE );
	void SortLocListByName( CArray<int,int>& arrayLocIdx );

	bool CheckPluDownloadRange( int nLocIdx, __int64 nPluNo );
	void TidyPluDownloadRanges( int nLocIdx );
	
//IDRAUGHT LINK
	void UpdateIDraughtStatus();
	void SetIDraughtStatus( int nStatus );
	int GetIDraughtStatus() { return m_nIDraughtStatus; }

//LOCATION ACCESS
	int GetLocNo( int nLocIdx );
	int GetNetworkNo ( int nLocIdx );
	int GetNetworkLocNo ( int nLocIdx );
	int GetDbNo( int nLocIdx );
	const char* GetName ( int nLocIdx );
	const char* GetTreeName ( int nLocIdx );
	const char* GetSystemName ( int nLocIdx );
	const char* GetExtendedSystemName ( int nLocIdx );
	const char* GetIDraughtID( int nLocIdx );
	const char* GetRemoteID( int nLocIdx );
	int GetDefaultPriceSet ( int nLocIdx );
	int GetPluPriceSetCount ( int nLocIdx );
	int GetDbPriceSet1( int nLocIdx );
	int GetDbPriceSet2( int nLocIdx );
	int GetDbPriceSet3( int nLocIdx );
	int GetPriceSetKeyboard1( int nLocIdx );
	int GetPriceSetKeyboard2( int nLocIdx );
	int GetPriceSetKeyboard3( int nLocIdx );
	int GetPriceSetKeyboardJob( int nLocIdx, int nPriceSet );
	int GetDbKeyboardSet( int nLocIdx );
	const char* GetTerminalExportPath( int nLocIdx );
	const char* GetDownloadPath( int nLocIdx );
	const char* GetDownloadNowPath( int nLocIdx );
	void GetSafeTerminalExportPath ( int nLocIdx, CString& strPath );
	void GetSafeDownloadPath ( int nLocIdx, CString& strPath );
	void GetSafeDownloadNowPath ( int nLocIdx, CString& strPath );
	void GetSafeExportControlPath ( int nLocIdx, CString& strPath );
	bool GetExportControlFlag( int nLocIdx );
	const char* GetExportControlPath( int nLocIdx );
	int GetExportControlDelay( int nLocIdx );
	int GetProdCSVImportFolderType( int nLocIdx );
	int GetProdCSVImportExistingMode( int nLocIdx );
	int GetProdCSVImportTypeMask( int nLocIdx );
	int GetProdCSVProtectFieldMask( int nLocIdx );
	int GetProdCSVImportModifierMode( int nLocIdx );
	int GetCustomerImportFolderType( int nLocIdx );
	int GetPluDownloadRangeType( int nLocIdx );
	__int64 GetPluDownloadRangeStart( int nLocIdx, int n );
	__int64 GetPluDownloadRangeEnd( int nLocIdx, int n );
	int GetBritanniaDefaultPriceBand( int nLocIdx );
	const char* GetAgressoLocationGroup( int nLocIdx );
	int GetLoyaltyFolderSet(int nLocIdx);
	
	void GetPluPresetFilename( int nLocIdx, int nTerminalNo, CString& strFilename );
	const char* GetPluPresetDate( int nLocIdx, int nTerminalNo );
	void SetPluPresetDate( int nLocIdx, int nTerminalNo );
	void ResetPluPresetDate( int nLodIdx, int nTerminalNo );

	void GetPluInfoFilename( int nLocIdx, int nTerminalNo, CString& strFilename );
	const char* GetPluInfoDate( int nLocIdx, int nTerminalNo );
	void SetPluInfoDate( int nLocIdx, int nTerminalNo );
	void ResetPluInfoDate( int nLodIdx, int nTerminalNo );
	
	int GetPriceSetUsageCount( int nLocIdx, int nSet );
	void GetLocSPOSVersion( int nLocIdx, int& nSPOSVersion, int& nConnectionType );

	int GetObsoleteSiteNo( int nLocIdx );
	bool GetObsoleteUploadPathFlag( int nLocIdx );
	bool GetObsoleteRecordFlag( int nLocIdx );
	const char* GetObsoleteUploadPath( int nLocIdx );
	
	void SetLocNo ( int nLocIdx, int nLocNo );
	void SetNetworkNo ( int nLocIdx, int nNetworkNo );
	void SetDbNo ( int nLocIdx, int nDbNo );
	void SetName ( int nLocIdx, const char* szName );
	void SetIDraughtID( int nLocIdx, const char* szID );
	void SetRemoteID( int nLocIdx, const char* szID );
	void SetDefaultPriceSet ( int nLocIdx, int nSet );
	void SetPluPriceSetCount ( int nLocIdx, int nCount );
	void SetDbPriceSet1( int nLocIdx, int nSet );
	void SetDbPriceSet2( int nLocIdx, int nSet );
	void SetDbPriceSet3( int nLocIdx, int nSet );
	void SetPriceSetKeyboard1( int nLocIdx, int nKbd );
	void SetPriceSetKeyboard2( int nLocIdx, int nKbd );
	void SetPriceSetKeyboard3( int nLocIdx, int nKbd );
	void SetDbKeyboardSet( int nLocIdx, int nSet );
	void SetTerminalExportPath( int nLocIdx, const char* szPath );
	void SetDownloadPath( int nLocIdx, const char* szPath );
	void SetDownloadNowPath( int nLocIdx, const char* szPath );
	void SetExportControlFlag( int nLocIdx, bool bFlag );
	void SetExportControlPath( int nLocIdx, const char* szPath );
	void SetExportControlDelay( int nLocIdx, int nDelay );
	void SetProdCSVImportFolderType( int nLocIdx, int nType );
	void SetProdCSVImportExistingMode( int nLocIdx, int nMode );
	void SetProdCSVImportTypeMask( int nLocIdx, int nType );
	void SetProdCSVProtectFieldMask( int nLocIdx, int nType );
	void SetProdCSVImportModifierMode( int nLocIdx, int nMode );
	void SetCustomerImportFolderType( int nLocIdx, int nType );
	void SetPluDownloadRangeType( int nLocIdx, int n );
	void SetPluDownloadRangeStart( int nLocIdx, int x, __int64 n );
	void SetPluDownloadRangeEnd( int nLocIdx, int x, __int64 n );
	void SetInTradingFolderFlag( int nLocIdx, bool bFlag );
	void SetBritanniaDefaultPriceBand( int nLocIdx, int nBand );
	void SetAgressoLocationGroup( int nLocIdx, const char* sz );
	void SetLoyaltyFolderSet(int nLocIdx, int nSet);
	
	void SetObsoleteSiteNo ( int nLocIdx, int nSiteNo );
	void SetObsoleteUploadPathFlag( int nLocIdx, bool bFlag );
	void SetObsoleteUploadPath( int nLocIdx, const char* szPath );
	
	const char* GetCSVLine( int nLocIdx );
	
//TERMINAL ACCESS
	int GetTerminalCount( int nLocIdx );

	bool FindTerminalByNumber( int nLocIdx, int nTNo, int& nTerminalIdx );
	bool AddTerminal( int nLocIdx, int nTNo, int nRpNoLegacy, __int64 nRpFlags, int nSpNo, int& nTerminalIdx );
	void RemoveTerminal( int nLocIdx, int nTNo );
	void FixWebSalesTerminals( int nLocIdx );

		int GetTNo ( int nLocIdx, int nTerminalIdx );
	const char* GetTerminalName ( int nLocIdx, int nTerminalIdx );
	int GetSpNo ( int nLocIdx, int nTerminalIdx );	
	void SetSpNo ( int nLocIdx, int nTerminalIdx, int nSpNo );
	
public:
	bool HasRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo );
	bool CanAddRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo );
	void ClearRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo );
	void SetRpNoLink( int nLocIdx, int nTerminalIdx, int nRpNo );
	void ClearAllRpNoLinks( int nLocIdx, int nTerminalIdx );
	int GetRpNoReport ( int nLocIdx, int nTerminalIdx );
	void ResetRpNoReport();

private:
	int GetRpNoLegacy ( int nLocIdx, int nTerminalIdx );
	void SetRpNoLegacy ( int nLocIdx, int nTerminalIdx, int nRpNo );
	
	__int64 GetRpFlags ( int nLocIdx, int nTerminalIdx );
	void SetRpFlags ( int nLocIdx, int nTerminalIdx, __int64 nFlags );

	void SetRpFlag( int nLocIdx, int nTerminalIdx, int nRpNo );
	void ClearRpFlag( int nLocIdx, int nTerminalIdx, int nRpNo );
	bool GetRpFlag( int nLocIdx, int nTerminalIdx, int nRpNo );

public:
//PATH FUNCTIONS	
	const char* GetFolderPathData ( int nLocIdx );
	const char* GetFolderPathDataDelete ( int nLocIdx );
	const char* GetFolderPathDataKeyboard ( int nLocIdx, int nKbNo );
	
	const char* GetFilePathInfo( int nLocIdx );
	const char* GetFilePathDataLocalPluExclude( int nLocIdx );
	const char* GetFilePathDataLocalPluExcludeTemp( int nLocIdx );
	const char* GetFilePathDataLocalPluInclude( int nLocIdx );
	const char* GetFilePathDataLocalPluIncludeTemp( int nLocIdx );
	const char* GetFilePathDataLocalPluField( int nLocIdx, int nSet, bool bForceLocSpecific = FALSE );
	const char* GetFilePathDataLocalPluFieldTemp( int nLocIdx, int nSet, bool bForceLocSpecific = FALSE );

	const char* GetFilePathSyssetPluEcr( int nLocIdx );
	
	const char* GetFolderPathSysset( int nLocIdx );
	const char* GetFolderPathSyssetDelete ( int nLocIdx );
	const char* GetFolderPathSyssetPendingProductsLocal ( int nLocIdx );
	const char* GetFolderPathSyssetPendingProductsRemote ( int nLocIdx );
	const char* GetFolderPathSyssetPendingCustomerLocal ( int nLocIdx );
	const char* GetFolderPathSyssetPendingCustomerRemote ( int nLocIdx );

private:
	CLocationCSVRecord* GetLocation ( int nLocIdx ) { return (CLocationCSVRecord*) GetAt ( nLocIdx ); }
	void FilterPluFile ( bool bChangesOnly, bool bPluByDate, int nLocIdx, int nTerminalIdx );
	
public:
	void BuildSortIdxTables();
	int GetSortIdxFromLocIdx( int nLocIdx );
	int GetLocIdxFromSortIdx( int nSortIdx );

private:
	CString m_strCSVLine;

	CString m_strKey;
	CString m_strName;
	CString m_strTreeName;
	CString m_strSystemName;
	CString m_strIDraughtID;
	CString m_strRemoteID;
	CString m_strAgressoLocationGroup;
	
	CString m_strTerminalExportPath;
	CString m_strDownloadPath;
	CString m_strDownloadNowPath;
	CString m_strExportControlPath;
	CString m_strObsoleteUploadPath;

	CString m_strPluPresetDate;
	CString m_strPluInfoDate;

	CString m_strTerminalName;
	CString m_strTNo;
	CString m_strMasterTNo;
	
	CString m_strFolderPath;
	CString m_strFolderPathDelete;
	
	CString m_strPath;
	CString m_strInfoPath;
	CString m_strKeyboardPath;

	CString m_strFilePathInfo;

	CString m_strFilePathLocalPluExclude;
	CString m_strFilePathLocalPluExcludeTemp;
	CString m_strFilePathLocalPluInclude;
	CString m_strFilePathLocalPluIncludeTemp;
	CString m_strFilePathLocalPluField;
	CString m_strFilePathLocalPluFieldTemp;	

	CString m_strFilePathPluEcr;

	CArray<int,int> m_arrayLocIdxToSortIdx;
	CArray<int,int> m_arraySortIdxToLocIdx;

	bool m_bGotPMSPseudoLocation;
	int m_nIDraughtStatus;
};

/**********************************************************************/
extern CLocationCSVArray dbLocation;
/**********************************************************************/
