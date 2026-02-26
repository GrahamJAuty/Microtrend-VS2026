/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ZSalesImporterEcrman.h"
/**********************************************************************/

CZSalesImporterEcrman::CZSalesImporterEcrman()
{
	m_nLocIdx = 0;
	m_nEcrIdx = 0;
	m_nDbIdx = 0;
	m_nNetworkIdx = 0;
}

/**********************************************************************/

void CZSalesImporterEcrman::LookForSales( int nLocIdx, int nEcrIdx )
{
	m_nLocIdx = nLocIdx;
	m_nEcrIdx = nEcrIdx;

	int nNetworkNo = dbLocation.GetNetworkNo( m_nLocIdx );
	if ( dbNetwork.FindNetworkByNumber( nNetworkNo, m_nNetworkIdx ) == FALSE )
		return;

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SMARTENT_NONE:
		return;
	}

	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	CString strFilename;
	strFilename.Format( "stock.%3.3d",
		dbLocation.GetTNo( m_nLocIdx, m_nEcrIdx ) );

	if ( ::FileExists( strPath + strFilename ) == FALSE )
		return;

	int nDbNo = dbLocation.GetDbNo( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, m_nDbIdx ) == FALSE )
		return;

	CSSFile fileLock;
	CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, m_nNetworkIdx );
	if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
		{
			fileLock.Close();
			return;
		}

		m_arrayFilesToImport.RemoveAll();
		m_arrayFilesToImport.Add( strFilename );
		ProcessPendingFiles();	
		
		DataManager.CloseDatabase();
		fileLock.Close();
	}
}

/**********************************************************************/

void CZSalesImporterEcrman::LookForPMSSales()
{
	m_nLocIdx = 0;
	m_nEcrIdx = 0;
	m_nNetworkIdx = -1;

	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	CString strFilename = "stock.001";
	if ( ::FileExists( strPath + strFilename ) == FALSE )
		return;

	if ( dbDatabase.GetSize() == 0 )
		return;

	m_nDbIdx = 0;
	CSSFile fileLock;
	CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, m_nNetworkIdx );
	if ( fileLock.Open( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, info, FALSE ) == FALSE )
		{
			fileLock.Close();
			return;
		}

		m_arrayFilesToImport.RemoveAll();
		m_arrayFilesToImport.Add( strFilename );
		ProcessPendingFiles();	
		
		DataManager.CloseDatabase();
		fileLock.Close();
	}
}

/**********************************************************************/

void CZSalesImporterEcrman::ProcessPendingFiles()
{
	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	for ( int n = 0; n < m_arrayFilesToImport.GetSize(); n++ )
	{
		CString strFilename = m_arrayFilesToImport.GetAt(n);
		if ( ProcessPendingFile( strPath, strFilename ) == TRUE )
			CFileRemover FileRemover( strPath + strFilename );
	}
}

/**********************************************************************/

bool CZSalesImporterEcrman::ProcessPendingFile( CString& strPath, CString& strFilename )
{				
	//MAKE SURE THAT WE CAN READ THE SOURCE FILE
	CSSFile fileSource;
	if ( fileSource.Open( strPath + strFilename, "rb" ) == FALSE )
		return FALSE;
	
	bool bWritePluDatabase = FALSE;
				
	CString strBuffer;
	while ( fileSource.ReadString ( strBuffer ) == TRUE )  
	{		
		if ( strBuffer.Left(1) == "/" )
			continue;

		CCSVPendingStock csvSale ( strBuffer );

		CSQLPluNoInfo infoPluNo;
		infoPluNo.m_nEposPluNo = csvSale.GetPluNo();
		::ProcessPluNo( infoPluNo );
		if ( FALSE == infoPluNo.m_bValid )
			continue;

		double dMultiplier = 1.0;
		if ( infoPluNo.m_nModifier != 0 )
			dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
		
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
		{
			double dBaseQtySold = csvSale.GetQty() * dMultiplier;
		
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			CReportConsolidationArray<CEcrmanRecipeItem> arrayRecipeItems;
			PluRecord.GetEcrmanRecipeTableArray( arrayRecipeItems );

			if ( arrayRecipeItems.GetSize() == 0 )
			{
				if ( PluRecord.GetNonStockInternalFlag() == FALSE )
				{
					PluRecord.SetApparentLevel( PluRecord.GetApparentLevel() - dBaseQtySold );
					DataManager.Plu.SetAt( nPluIdx, PluRecord );
					bWritePluDatabase = TRUE;
				}
			}
			else
			{
				for ( int n = 0; n < arrayRecipeItems.GetSize(); n++ )
				{
					CEcrmanRecipeItem infoItem;
					arrayRecipeItems.GetAt(n, infoItem );

					if ( infoItem.m_dStockQty <= 0.0 )
						continue;

					int nRecipePluIdx;
					if ( DataManager.Plu.FindPluByNumber( infoItem.m_nPluNo, nRecipePluIdx ) == TRUE )
					{
						CPluCSVRecord RecipePluRecord;
						DataManager.Plu.GetAt( nRecipePluIdx, RecipePluRecord );
							
						if ( RecipePluRecord.GetNonStockExternalFlag() == FALSE )
						{
							RecipePluRecord.SetApparentLevel( RecipePluRecord.GetApparentLevel() - ( dBaseQtySold * infoItem.m_dStockQty ) );
							DataManager.Plu.SetAt( nRecipePluIdx, RecipePluRecord );
							bWritePluDatabase = TRUE;
						}
					}
				}
			}
		}
	}

	fileSource.Close();
	
	CFileRemover FileRemover( strPath + strFilename );

	if ( bWritePluDatabase == TRUE )
	{
		CDataManagerInfo info;
		DataManager.SetNoLogWriteFlag( TRUE );
		DataManager.WritePlu( info );
		DataManager.SetNoLogWriteFlag( FALSE );
	}

	return TRUE;
}


/**********************************************************************/


