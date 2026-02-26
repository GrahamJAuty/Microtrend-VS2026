/**********************************************************************/
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportTableName.h"
/**********************************************************************/

CCommsExportTableName::CCommsExportTableName( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportTableName::PrepareFile()
{
	if ( m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize() )
		return COMMS_ERROR_BADNETIDX;

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		return COMMS_ERROR_BADLOCIDX;

	m_strSourceFolder = dbNetwork.GetFolderPathData ( m_nNetworkIdx );
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	int nResult = COMMS_ERROR_NODATA;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;
	
	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenTableNames( DB_READONLY, m_nLocIdx, info );
	}

	if ( DataManagerNonDb.TableNames.GetSize() != 0 )
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx ); 
		
		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile( nSPOSVersion );
		else	
			nResult = COMMS_ERROR_CREATEFILE;
	}

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.CloseTableNames( info );
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportTableName::PrepareSPOSFile( int nSPOSVersion )
{
	CSSFile fileTableNames;
	m_strSourceFilename = "Tables.sql";
	if ( fileTableNames.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[TblNo]" );
	csvHeader.Add( "[Name]" );
	csvHeader.Add( "[Account]" );
	csvHeader.Add( "[StatusAutoEnable]" );
	csvHeader.Add( "[StatusAutoPaidToUnused]" );
	csvHeader.Add( "[LimitEnable]" );
	csvHeader.Add( "[Limit]" );
	
	CSQLOut SQLOut ( fileTableNames, "Tables", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManagerNonDb.TableNames.GetSize();
	for ( int nTableIdx = 0; nTableIdx < nSize; nTableIdx++ )
	{
		StatusProgress.SetPos ( nTableIdx, nSize );

		CTableNameCSVRecord Table;
		DataManagerNonDb.TableNames.GetAt ( nTableIdx, Table );

		CMinimumSPOSVersion Version;
		Table.CheckMinimumSPOSVersion( Version );
		
		int nErrorType = -1;
		Version.CheckSPOSVersion( nSPOSVersion, nErrorType );

		if ( nErrorType != -1 )
		{
			TallyDownloadExclusion( NODE_TABLE_NAME, nErrorType );
			continue;
		}

		SQLOut.Add( Table.GetTableNo() );
		SQLOut.Add( Table.GetTableName() );
		SQLOut.Add( Table.GetAccountTableFlag() );
		SQLOut.Add( Table.GetAutoStatusEnableFlag() );
		SQLOut.Add( Table.GetAutoStatusUnusedAfterPaymentFlag() );
		SQLOut.Add( Table.GetEnableLimitFlag() );
		SQLOut.Add( Table.GetLimitPounds() );

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileTableNames.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/
