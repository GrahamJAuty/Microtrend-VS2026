/**********************************************************************/
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "RepCSV.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ZDeliveryImporterEcrman.h"
/**********************************************************************/

CZDeliveryImporterEcrman::CZDeliveryImporterEcrman()
{
	m_nLocIdx = 0;
	m_nEcrIdx = 0;
	m_nDbIdx = 0;
	m_nNetworkIdx = 0;
}

/**********************************************************************/

void CZDeliveryImporterEcrman::LookForDeliveries( int nLocIdx, int nEcrIdx )
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
	case CONNECTION_TYPE_SPTBOOK_WEB:
	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		return;
	}

	CString strPath = dbNetwork.GetFolderPathSyssetStock( m_nNetworkIdx );
	strPath += "\\";

	CString strFilename;
	strFilename.Format( "stock_delivery.%3.3d",
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

void CZDeliveryImporterEcrman::ProcessPendingFiles()
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

bool CZDeliveryImporterEcrman::ProcessPendingFile( CString& strPath, CString& strFilename )
{				
	//MAKE SURE THAT WE CAN READ THE SOURCE FILE
	CSSFile fileSource;
	if ( fileSource.Open( strPath + strFilename, "rb" ) == FALSE )
		return FALSE;
	
	bool bWritePluDatabase = FALSE;

	CStringArray arrayDeliveryAudit;
				
	CString strBuffer;
	while ( fileSource.ReadString ( strBuffer ) == TRUE )  
	{		
		if ( strBuffer.Left(1) == "/" )
			continue;

		CCSVPendingStockDelivery csvDelivery ( strBuffer );

		CSQLPluNoInfo infoPluNo;
		infoPluNo.m_nEposPluNo = csvDelivery.GetPluNo();
		::ProcessPluNo( infoPluNo );
		if ( FALSE == infoPluNo.m_bValid )
			continue;

		double dMultiplier = 1.0;
		if ( infoPluNo.m_nModifier != 0 )
			dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
		
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
		{
			double dLineQty = csvDelivery.GetQty() * dMultiplier;
		
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );

			if ( PluRecord.GetNonStockExternalFlag() == FALSE )
			{
				PluRecord.SetApparentLevel( PluRecord.GetApparentLevel() + dLineQty );
				DataManager.Plu.SetAt( nPluIdx, PluRecord );
				bWritePluDatabase = TRUE;

				if ( EcrmanOptions.AllowPluDeliveryAudit() == TRUE )
				{
					CString strDate = csvDelivery.GetDate();
					CString strTime = csvDelivery.GetTime();
					::MakeYearFirstDate( strDate );
					strTime = strTime.Left(4);

					__int64 nPluNo = PluRecord.GetPluNoInt();
					CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
					csv.Add ( strDate );
					csv.Add ( strTime );
					csv.Add ( nPluNo );
					csv.Add ( PluRecord.GetReportText() );
					csv.Add ( PluRecord.GetBaseDeptNo() );
					csv.Add ( 0 );
					csv.Add ( dLineQty, SysInfo.GetDPQty() );
					csv.Add ( PluRecord.GetSupplierNo() );
					arrayDeliveryAudit.Add( csv.GetLine() );
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

	if ( arrayDeliveryAudit.GetSize() > 0 )
	{
		CString strFilename = dbDatabase.GetFilePathDataPluStockAudit( m_nDbIdx );

		CSSFile fileAudit;
		if ( fileAudit.Open( strFilename, "ab" ) == TRUE )
		{
			for ( int n = 0; n < arrayDeliveryAudit.GetSize(); n++ )
				fileAudit.WriteLine( arrayDeliveryAudit.GetAt(n) );
		}
	}

	return TRUE;
}


/**********************************************************************/


