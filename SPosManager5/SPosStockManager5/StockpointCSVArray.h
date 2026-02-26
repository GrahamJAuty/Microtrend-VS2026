#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "AddressInfo.h"
/**********************************************************************/
#define STOCKPOINT_VERSION 3
/**********************************************************************/
#define SPIDX_SPECIAL_UNKNOWN 0x1000000
/**********************************************************************/

struct CStockSyncInfo
{
	int m_nDbNo;
	int m_nSpNo;
};

/**********************************************************************/

class CStockpointCSVRecord : public CCSVRecord
{
public:
	CStockpointCSVRecord(); 
	virtual ~CStockpointCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
private:
	void SetSpNo ( int nSpNo ) { m_nSpNo = nSpNo; }
	void SetName ( const char* szName );
	void SetDbNo ( int nDbNo ) { m_nDbNo = nDbNo; }
	void SetSiteNo ( int nSiteNo ) { m_nSiteNo = nSiteNo; }
	void SetPeriodStart ( const char* szDate );
	void SetYearStart ( const char* szDate ); 
	void SetPeriodStart();
	void SetYearStart();

	void SetWarehouseFlag( bool b ){ m_bWarehouse = b; }
	void SetTransferInFlag( bool b ){ m_bTransferIn = b; }
	void SetTransferOutFlag( bool b ){ m_bTransferOut = b; }

	void SetImportSiteNo( int n );

	const char* GetSystemName();

private:
	void ClearRecord();
	void V1To3ConvertFromCSV( CCSV& csv, int nVer );
	
private:
	int m_nSpNo;
	CString m_strName;
	CString m_strSystemName;
	int m_nDbNo;
	int m_nSiteNo;
	CString m_strPeriodStart;
	CString m_strYearStart; 
	
	bool m_bWarehouse;
	bool m_bTransferOut;
	bool m_bTransferIn;

	int m_nImportSiteNo;
	
	CString m_strPath;

	friend class CStockpointCSVArray;
};

/**********************************************************************/

class CStockpointCSVArray : public CPtrArray
{
public:
	CStockpointCSVArray();
	~CStockpointCSVArray();

	bool RenameForDeletion( int nSpIdx );
	void ClearStockpointFolder( int nSpIdx );
	void ClearDeletionFolder( int nSpIdx );

	int AddStockpoint ( int nDbNo );
	int AddStockpoint( int nDbNo, int nSpNo, const char* szPath );
	bool WriteStockpoint( int nSpIdx );

	void GetSpIdxRange ( int nDbIdx, int& nStartIdx, int& nEndIdx );
	bool FindStockpointByNumber ( int nDbNo, int nSpNo, int& nSpIdx );
	
	void DeleteStockpointByNumber ( int nDbNo, int nSpNo );
	void DeleteStockpointByIndex ( int nSpIdx, bool bDeleteFiles );
	void PurgeDatabase( int nDbIdx );
	
	void UpdateStockpoints();
	void RemoveAll();

	int GetSpNo ( int nSpIdx );
	int GetDbNo( int nSpIdx ); 
	int GetSiteNo(int nSpIdx ); 
	const char* GetPeriodStart( int nSpIdx ); 
	const char* GetYearStart( int nSpIdx ); 

	bool GetWarehouseFlag( int nSpIdx );
	bool GetTransferOutFlag( int nSpIdx );
	bool GetTransferInFlag( int nSpIdx );

	int GetImportSiteNo( int nSpIdx );

	const char* GetCSVLine( int nSpIdx );
	const char* GetName ( int nSpIdx );
	const char* GetSystemName ( int nSpIdx );
	const char* GetExtendedSystemName ( int nSpIdx );

	void SetSpNo ( int nSpIdx, int nSpNo );
	void SetDbNo ( int nSpIdx, int nDbNo );
	void SetSiteNo ( int nSpIdx, int nSiteNo );
	void SetPeriodStart( int nSpIdx );
	void SetYearStart( int nSpIdx );
	void SetPeriodStart ( int nSpIdx, const char* szDate );
	void SetYearStart ( int nSpIdx, const char* szDate );
	
	void SetWarehouseFlag( int nSpIdx, bool bFlag );
	void SetTransferOutFlag( int nSpIdx, bool bFlag );
	void SetTransferInFlag( int nSpIdx, bool Flag );

	void SetImportSiteNo( int nSpIdx, int n );

	void SetName ( int nSpIdx, const char* szName );

	const char* GetFolderPath( int nSpIdx );
	const char* GetFolderPathDelete( int nSpIdx );
	const char* GetFolderPathHistory( int nSpIdx );
	const char* GetFolderPathAudit( int nSpIdx );
	const char* GetFolderPathSalesXXX( int nSpIdx );
	const char* GetFolderPathSession( int nSpIdx, int nType );

	const char* GetFolderPathDataOrdersRoot( int nDbIdx );
	const char* GetFolderPathDataOrdersSupplier( int nDbIdx, int nSuppNo );
	const char* GetFolderPathDataOrdersType( int nDbIdx, int nSuppNo, int nType );
	
	const char* GetFilePathInfo( int nSpIdx );
	const char* GetFilePathAudit( int nSpIdx, COleDateTime& date, int nType = 0, bool bPMS = FALSE );
	const char* GetFilePathDataOrder( int nDbIdx, int nSuppNo, int nType, int nOrderNum, CString& strDate, CString& strTime );
	
	const char* GetFileNameAudit( int nSpIdx, COleDateTime& date, int nType = 0, bool bPMS = FALSE );
	
	void GetHistoryFileList( int nSpIdx, const char* szMonthFrom, const char* szMonthTo, CStringArray& FileArray, CStringArray& MonthArray, __int64& nTotalLength, bool bAppend, int nVersion = 2 );

	void RemoveTemporaryAudits( int nSpIdx );

	void SortByName( CWordArray& arraySpIdx, int nDbIdx= -1 );
	void SortSpListByName( CArray<int,int>& arraySpIdx );
	void SortSpListForStocktake( CArray<int,int>& arraySpIdx );

	void AddSyncRequestStockLevels( int nDbNo, int nSpNo );
	bool GetSyncRequestStockLevels( int& nDbNo, int& nSpNo );
	void RemoveSyncRequestStockLevels();
	void RemoveSyncRequestStockLevels( int nSpIdx );
	
	void GetAddressInfo( int nSpIdx, CAddressInfo& info );
	void SetAddressInfo( int nSpIdx, CAddressInfo& info );

	void LoadStockpointBuffer( int nSpIdx, CStockpointCSVArray& buffer );
	void SaveStockpointBuffer( int nSpIdx, CStockpointCSVArray& buffer );
	
public:
	void BuildSortIdxTables();
	int GetSortIdxFromSpIdx( int nSpIdx );
	int GetSpIdxFromSortIdx( int nSortIdx );

private:
	CStockpointCSVRecord* GetStockpoint ( int nSpIdx ) { return (CStockpointCSVRecord*) GetAt ( nSpIdx ); }
	bool FindSyncRequestStockLevels( int nDbNo, int nSpNo, int& nIdx );

private:
	CString m_strName;
	CString m_strSystemName;
	CString m_strCSVLine;
	CString m_strPeriodStart;
	CString m_strYearStart;
	
	CString m_strFolderPath;
	CString m_strFolderPathOrder;
	CString m_strFolderPathDelete;
	CString m_strFolderPathHistory;
	CString m_strFolderPathAudit;
	CString m_strFolderPathSalesXXX;
	CString m_strFolderPathSession;
	
	CString m_strFilePath;
	CString m_strFilePathInfo;
	CString m_strFilePathAudit;

	CString m_strFileNameAudit;

	CWordArray m_arraySortIdxToSpIdx;
	CWordArray m_arraySpIdxToSortIdx;

	CArray<CStockSyncInfo,CStockSyncInfo> m_arraySyncStockLevels;
};

/**********************************************************************/
extern CStockpointCSVArray dbStockpoint;
/**********************************************************************/
#endif				//__STOCKPOINTCSVARRAY_H__	
/**********************************************************************/
