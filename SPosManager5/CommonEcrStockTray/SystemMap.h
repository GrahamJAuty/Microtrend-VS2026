#pragma once
/**********************************************************************/
#define SYSTEMMAP_FULL 0
#define SYSTEMMAP_COMMS 1
#define SYSTEMMAP_EDITABLE 2
/**********************************************************************/

class CSystemMap 
{
public:
	CSystemMap();
	~CSystemMap();

	void CreateSystem();
	void WriteSystem( bool bTidyTasks );

public:
	void FixWebSalesNetworks();

	COleDateTime GetOleTimePosUtilityToken() { return m_oleTimePosUtilityToken; }
	
#ifdef STOCKMAN_SYSTEM
	void CreateSiteMap();
	int GetSiteSpCount( int nSiteIdx );
	int GetSiteSpIdx( int nSiteIdx, int nPos );
	int GetOrphanSpCount( int nDbIdx );
	int GetOrphanSpIdx( int nDbIdx, int nPos );
#endif

private:
	void CreateNetwork( int nNetWkNo );
	void CreateDatabase( int nDbNo );
	void CreateLocation( int nNetWkNo, int nLocNo );
	void CreateReportpoint( int nDbNo, int nRpNo );
	void CreateLocationSet( int nDbNo, int nLcNo );
	void CreateKeyboardSet( int nDbNo, int nKbsNo );
	
#ifdef STOCKMAN_SYSTEM
	void CreateStockpoint( int nDbNo, int nSpNo );
	void CreateSite ( int nDbNo, int nSiteNo );
#endif

	void ValidateFolder( CString& strFolder, const char* sz, bool bDeletion, int& nFolderNo );

private:
	COleDateTime m_oleTimePosUtilityToken;
	CStringArray m_arraySpBySite;
	CStringArray m_arrayOrphanSpByDb;
	CStringArray m_arrayFoldersToDelete;
};

/**********************************************************************/
extern CSystemMap SystemMapFull;
/**********************************************************************/
