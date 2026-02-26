/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PosTrayTokenFiles.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "ReportpointCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SystemMap.h"
/**********************************************************************/

CSystemMap::CSystemMap()
{
	m_oleTimePosUtilityToken = COleDateTime( 1980, 1, 1, 0, 0, 0 );
}

/**********************************************************************/

CSystemMap::~CSystemMap()
{
}

/**********************************************************************/

void CSystemMap::ValidateFolder( CString& strFolder, const char* sz, bool bDeletion, int& nFolderNo )
{
	nFolderNo = 0;

	CString strTag = sz;
	
	strTag.MakeUpper();
	strFolder.MakeUpper();
	
	int nTagLen = strTag.GetLength();
	int nLength = nTagLen + 3;
	
	if ( bDeletion == TRUE )
		nLength += 3;

	if ( strFolder.GetLength() != nLength )
		return;

	if ( strFolder.Left( nTagLen ) != strTag )
		return;

	if ( bDeletion == TRUE )
	{
		if ( strFolder.Right(3) != "XXX" )
			return;
	}

	CString strNumber = strFolder.Mid( nTagLen, 3 );

	for ( int nPos = 0; nPos < strNumber.GetLength(); nPos++ )
		if ( strNumber.GetAt ( nPos ) < '0' || strNumber.GetAt ( nPos ) > '9' )
			return;

	nFolderNo = atoi ( strNumber );
}

/**********************************************************************/

void CSystemMap::CreateSystem()
{
	CPosTrayTokenFiles::GetSystemUpdateFileTime( m_oleTimePosUtilityToken );

	dbNetwork.RemoveAll();
	dbDatabase.RemoveAll();
	dbLocation.RemoveAll();
	dbReportpoint.RemoveAll();
	dbLocationSet.RemoveAll();
	dbKeyboardSet.RemoveAll();

#ifdef STOCKMAN_SYSTEM
	dbStockpoint.RemoveAll();
	dbSite.RemoveAll();
#endif

	CString strProgramPath = "*.*";
	GetDataProgramPath( strProgramPath );

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strProgramPath ) != 0 );

	m_arrayFoldersToDelete.RemoveAll();

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK DIRECTORIES
		if ( FileFinder.IsDirectory() == FALSE )
			continue;

		CString strFolder = FileFinder.GetFileName();
		
		int nFolderNo;
		ValidateFolder( strFolder, "NETWK", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateNetwork( nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "NETWK", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}


		ValidateFolder( strFolder, "DB", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateDatabase( nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "DB", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}

		ValidateFolder( strFolder, "LS", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateLocationSet( 0, nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "LS", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}
	}

	for ( int n = 0; n < m_arrayFoldersToDelete.GetSize(); n++ )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFolder ( m_arrayFoldersToDelete.GetAt(n) );
	}
}

/**********************************************************************/

void CSystemMap::CreateNetwork( int nNetWkNo )
{
	CString strPath;
	strPath.Format ( "NETWK%3.3d\\", nNetWkNo );
	GetDataProgramPath( strPath );
	dbNetwork.AddNetwork ( nNetWkNo, strPath );

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK DIRECTORIES
		if ( FileFinder.IsDirectory() == FALSE )
			continue;

		CString strFolder = FileFinder.GetFileName();
		
		int nFolderNo;
		ValidateFolder( strFolder, "LOC", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateLocation( nNetWkNo, nFolderNo );
			continue;
		}
		
		ValidateFolder( strFolder, "LOC", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}
	}
}
		
/**********************************************************************/

void CSystemMap::CreateDatabase( int nDbNo )
{
	CString strPath;
	strPath.Format ( "DB%3.3d\\", nDbNo );
	GetDataProgramPath( strPath );
	dbDatabase.AddDatabase ( nDbNo, strPath );

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK DIRECTORIES
		if ( FileFinder.IsDirectory() == FALSE )
			continue;

		CString strFolder = FileFinder.GetFileName();
		
		int nFolderNo;
		ValidateFolder( strFolder, "RP", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateReportpoint( nDbNo, nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "RP", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}

		ValidateFolder( strFolder, "KBS", FALSE, nFolderNo );
		if ( ( nFolderNo > 0 ) && ( nFolderNo <= MAX_KEYBOARD_SETS_PER_DATABASE ) )
		{
			CreateKeyboardSet( nDbNo, nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "KBS", TRUE, nFolderNo );
		if ( ( nFolderNo > 0 ) && ( nFolderNo <= MAX_KEYBOARD_SETS_PER_DATABASE ) )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}
		
		ValidateFolder( strFolder, "LS", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateLocationSet( nDbNo, nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "LS", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}
		
#ifdef STOCKMAN_SYSTEM		

		ValidateFolder( strFolder, "SP", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateStockpoint( nDbNo, nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "SP", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}

		ValidateFolder( strFolder, "SITE", FALSE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			CreateSite( nDbNo, nFolderNo );
			continue;
		}

		ValidateFolder( strFolder, "SITE", TRUE, nFolderNo );
		if ( nFolderNo != 0 )
		{
			m_arrayFoldersToDelete.Add( FileFinder.GetFilePath() );
			continue;
		}

#endif

	}
}
		
/**********************************************************************/

void CSystemMap::CreateLocation( int nNetWkNo, int nLocNo )
{
	CString strPath;
	strPath.Format ( "NetWk%3.3d\\LOC%3.3d\\", nNetWkNo, nLocNo );
	GetDataProgramPath( strPath );
	dbLocation.AddLocation ( nNetWkNo, nLocNo, strPath );
}

/**********************************************************************/

void CSystemMap::CreateReportpoint( int nDbNo, int nRpNo )
{
	CString strPath;
	strPath.Format ( "DB%3.3d\\RP%3.3d\\", nDbNo, nRpNo );
	GetDataProgramPath( strPath );
	dbReportpoint.AddReportpoint ( nDbNo, nRpNo, strPath );
}

/**********************************************************************/

void CSystemMap::CreateKeyboardSet( int nDbNo, int nKbsNo )
{
	CString strPath;
	strPath.Format ( "DB%3.3d\\KBS%3.3d\\", nDbNo, nKbsNo );
	GetDataProgramPath( strPath );
	dbKeyboardSet.AddKeyboardSet ( nDbNo, nKbsNo, strPath );
}

/**********************************************************************/

void CSystemMap::CreateLocationSet( int nDbNo, int nLcNo )
{
	CString strPath;
	
	if ( nDbNo == 0 )
		strPath.Format ( "LS%3.3d\\", nLcNo );
	else
		strPath.Format ( "DB%3.3d\\LS%3.3d\\", nDbNo, nLcNo );
	
	GetDataProgramPath( strPath );
	dbLocationSet.AddLocationSet ( nDbNo, nLcNo, strPath );
}

/**********************************************************************/

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CSystemMap::CreateStockpoint( int nDbNo, int nSpNo )
{
	CString strPath;
	strPath.Format ( "DB%3.3d\\SP%3.3d\\", nDbNo, nSpNo );
	GetDataProgramPath( strPath );
	dbStockpoint.AddStockpoint ( nDbNo, nSpNo, strPath );
}

/**********************************************************************/

void CSystemMap::CreateSite( int nDbNo, int nSiteNo )
{
	CString strPath;
	strPath.Format ( "DB%3.3d\\SITE%3.3d\\", nDbNo, nSiteNo );
	GetDataProgramPath( strPath );
	dbSite.AddSite ( nDbNo, nSiteNo, strPath );
}

/**********************************************************************/

void CSystemMap::CreateSiteMap()
{
	m_arrayOrphanSpByDb.RemoveAll();
	m_arraySpBySite.RemoveAll();

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		m_arrayOrphanSpByDb.Add ( "" );

	for ( int nSiteIdx = 0; nSiteIdx < dbSite.GetSize(); nSiteIdx++ )
		m_arraySpBySite.Add ( "" );

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		int nDbNo = dbDatabase.GetDbNo ( nDbIdx );

		int nStartIdx, nEndIdx;
		dbStockpoint.GetSpIdxRange ( nDbIdx, nStartIdx, nEndIdx );

		for ( int nSpIdx = nStartIdx; nSpIdx <= nEndIdx; nSpIdx++ )
		{
			CString strSpIdx;
			strSpIdx.Format ( "%3.3d", nSpIdx );
				
			int nSiteNo = dbStockpoint.GetSiteNo ( nSpIdx );

			int nSiteIdx;
			if ( dbSite.FindSiteByNumber ( nDbNo, nSiteNo, nSiteIdx ) )
			{
				CString strTemp = m_arraySpBySite.GetAt ( nSiteIdx );
				m_arraySpBySite.SetAt ( nSiteIdx, strTemp + strSpIdx );
			}
			else
			{
				CString strTemp = m_arrayOrphanSpByDb.GetAt ( nDbIdx );
				m_arrayOrphanSpByDb.SetAt ( nDbIdx, strTemp + strSpIdx );
			}
		}
	}
}

/**********************************************************************/

int CSystemMap::GetSiteSpCount ( int nSiteIdx )
{
	if ( nSiteIdx < 0 || nSiteIdx >= m_arraySpBySite.GetSize() )
		return 0;

	return ( ( m_arraySpBySite.GetAt ( nSiteIdx ).GetLength() ) / 3 );
}

/**********************************************************************/

int CSystemMap::GetSiteSpIdx ( int nSiteIdx, int nPos )
{
	if ( nSiteIdx < 0 || nSiteIdx >= m_arraySpBySite.GetSize() )
		return -1;

	nPos *= 3;
	CString strTemp = m_arraySpBySite.GetAt ( nSiteIdx );

	if ( nPos > strTemp.GetLength() - 3 )
		return - 1;

	return atoi ( strTemp.Mid ( nPos, 3 ) );
}

/**********************************************************************/

int CSystemMap::GetOrphanSpCount ( int nDbIdx )
{
	if ( nDbIdx < 0 || nDbIdx >= m_arrayOrphanSpByDb.GetSize() )
		return 0;

	return ( ( m_arrayOrphanSpByDb.GetAt ( nDbIdx ).GetLength() ) / 3 );
}

/**********************************************************************/

int CSystemMap::GetOrphanSpIdx ( int nDbIdx, int nPos )
{
	if ( nDbIdx < 0 || nDbIdx >= m_arrayOrphanSpByDb.GetSize() )
		return -1;

	nPos *= 3;
	CString strTemp = m_arrayOrphanSpByDb.GetAt ( nDbIdx );

	if ( nPos > strTemp.GetLength() - 3 )
		return - 1;

	return atoi ( strTemp.Mid ( nPos, 3 ) );
}

/**********************************************************************/
#endif
/**********************************************************************/

void CSystemMap::WriteSystem( bool bTidyTasks )
{
	for ( int nNetWkIdx = 0; nNetWkIdx < dbNetwork.GetSize(); nNetWkIdx++ )
	{
		::CreateSubdirectory( dbNetwork.GetFolderPathData( nNetWkIdx ) );
		::CreateSubdirectory( dbNetwork.GetFolderPathSysset( nNetWkIdx ) );

		CSSFile fileInfo;
		if ( fileInfo.Open ( dbNetwork.GetFilePathInfo( nNetWkIdx ), "wb" ) )
			fileInfo.WriteLine ( dbNetwork.GetCSVLine ( nNetWkIdx ) );
	}

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		::CreateSubdirectory( dbDatabase.GetFolderPathData( nDbIdx ) );
		::CreateSubdirectory( dbDatabase.GetFolderPathSysset( nDbIdx ) );

		CSSFile fileInfo;
		if ( fileInfo.Open ( dbDatabase.GetFilePathInfo( nDbIdx ), "wb" ) )
			fileInfo.WriteLine ( dbDatabase.GetCSVLine ( nDbIdx ) );
	}

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		::CreateSubdirectory( dbLocation.GetFolderPathData( nLocIdx ) );
		::CreateSubdirectory( dbLocation.GetFolderPathSysset( nLocIdx ) );
		
		CSSFile fileInfo;
		if ( fileInfo.Open ( dbLocation.GetFilePathInfo( nLocIdx ), "wb" ) )
			fileInfo.WriteLine ( dbLocation.GetCSVLine ( nLocIdx ) );
	}

	for ( int nRpIdx = 0; nRpIdx < dbReportpoint.GetSize(); nRpIdx++ )
	{
		::CreateSubdirectory( dbReportpoint.GetFolderPath( nRpIdx ) );
			
		CSSFile fileInfo;
		if ( fileInfo.Open ( dbReportpoint.GetFilePathInfo( nRpIdx ), "wb" ) )
			fileInfo.WriteLine ( dbReportpoint.GetCSVLine ( nRpIdx ) );
	}

	for ( int nKbsIdx = 0; nKbsIdx < dbKeyboardSet.GetSize(); nKbsIdx++ )
	{
		::CreateSubdirectory( dbKeyboardSet.GetFolderPath( nKbsIdx ) );
			
		CSSFile fileInfo;
		if ( fileInfo.Open ( dbKeyboardSet.GetFilePathInfo( nKbsIdx ), "wb" ) )
			fileInfo.WriteLine ( dbKeyboardSet.GetCSVLine ( nKbsIdx ) );
	}

	for ( int nLcIdx = 0; nLcIdx < dbLocationSet.GetSize(); nLcIdx++ )
	{
		::CreateSubdirectory( dbLocationSet.GetFolderPath( nLcIdx ) );
			
		CSSFile fileInfo;
		if ( fileInfo.Open ( dbLocationSet.GetFilePathInfo( nLcIdx ), "wb" ) )
			fileInfo.WriteLine ( dbLocationSet.GetCSVLine ( nLcIdx ) );
	}

#ifdef STOCKMAN_SYSTEM
	for ( int nSpIdx = 0; nSpIdx < dbStockpoint.GetSize(); nSpIdx++ )
	{
		::CreateSubdirectory( dbStockpoint.GetFolderPath( nSpIdx ) );
		
		CSSFile fileInfo;
		if ( fileInfo.Open ( dbStockpoint.GetFilePathInfo( nSpIdx ), "wb" ) )
			fileInfo.WriteLine ( dbStockpoint.GetCSVLine ( nSpIdx ) );
	}

	for ( int nSiteIdx = 0; nSiteIdx < dbSite.GetSize(); nSiteIdx++ )
	{
		::CreateSubdirectory( dbSite.GetFolderPath( nSiteIdx ) );
				
		CSSFile fileInfo;
		if ( fileInfo.Open ( dbSite.GetFilePathInfo( nSiteIdx ), "wb" ) )
			fileInfo.WriteLine ( dbSite.GetCSVLine ( nSiteIdx ) );
	}
#endif
}

/**********************************************************************/

void CSystemMap::FixWebSalesNetworks()
{
	for ( int nNetWkIdx = 0; nNetWkIdx < dbNetwork.GetSize(); nNetWkIdx++ )
	{
		switch( dbNetwork.GetConnectionType( nNetWkIdx ) )
		{
		case CONNECTION_TYPE_SPTBOOK_WEB:
		case CONNECTION_TYPE_SPTBOOK_NONE:
		case CONNECTION_TYPE_SMARTENT_WEB:
		case CONNECTION_TYPE_SMARTENT_NONE:
			{
				int nStartIdx, nEndIdx;
				dbLocation.GetNetworkLocIdxRange( nNetWkIdx, nStartIdx, nEndIdx );

				while( nEndIdx > nStartIdx )
				{
					if ( dbLocation.GetNetworkNo( nEndIdx ) == dbNetwork.GetNetworkNo( nNetWkIdx ) )
						dbLocation.DeleteLocationByIndex( nEndIdx, FALSE );

					nEndIdx--;
				}

				if ( nEndIdx == nStartIdx )
					if ( dbLocation.GetNetworkNo( nEndIdx ) == dbNetwork.GetNetworkNo( nNetWkIdx ) )
						dbLocation.FixWebSalesTerminals( nEndIdx );
			}
			break;
		}
	}
}

/**********************************************************************/
