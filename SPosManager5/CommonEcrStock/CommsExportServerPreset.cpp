/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
/**********************************************************************/
#include "CommsExportServerPreset.h"
/**********************************************************************/

CCommsExportServerPreset::CCommsExportServerPreset( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportServerPreset::PrepareFile()
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

	int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );
	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
		nResult = PrepareSPOSFile( nSPOSVersion );
	else
		nResult = COMMS_ERROR_CREATEFILE;
	
	return nResult;
}

/**********************************************************************/

int CCommsExportServerPreset::PrepareSPOSFile( int nSPOSVersion )
{
	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;

	CSSFile filePresets;
	m_strSourceFilename = "SvrPset.sql";
	if ( filePresets.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[TillNo]" );
	csvHeader.Add( "[Preset]" );
	csvHeader.Add( "[SrvNo]" );
	
	CSQLOut SQLOut ( filePresets, "PresetSrvrs", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	CDataManagerInfo infoDM;
	DataManagerNonDb.OpenServerPresetSystem( DB_READONLY, infoDM );
	DataManagerNonDb.OpenServerPresetDatabase( nDbIdx, DB_READONLY, infoDM );
	DataManagerNonDb.OpenServerPresetLocation( m_nLocIdx, DB_READONLY, infoDM );
	
	CWordArray arrayConLevels;
	arrayConLevels.Add( NODE_LOCATION_TERMINAL );
	arrayConLevels.Add( NODE_LOCATION );
	arrayConLevels.Add( NODE_DATABASE );
	arrayConLevels.Add( NODE_SYSTEM );

	for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( m_nLocIdx ); nTermIdx++ )
	{
		if ( nTermIdx != 0 )
			DataManagerNonDb.CloseServerPresetTerminal( infoDM );

		int nTNo = dbLocation.GetTNo( m_nLocIdx, nTermIdx );
		DataManagerNonDb.OpenServerPresetTerminal( m_nLocIdx, nTNo, DB_READONLY, infoDM );

		for ( int nPresetIdx = 0; nPresetIdx < DataManagerNonDb.ServerPresetSystem.GetPresetCount(); nPresetIdx++ )
		{
			int nServerNo = 0;
			int nPresetNo = 0;
			for ( int nList = 0; nList < arrayConLevels.GetSize(); nList++ )
			{
				CServerPresetMiniInfo infoMini;

				switch ( arrayConLevels.GetAt(nList ) )
				{
				case NODE_LOCATION_TERMINAL:	DataManagerNonDb.ServerPresetTerminal.GetPreset( nPresetIdx, infoMini );	break;
				case NODE_LOCATION:				DataManagerNonDb.ServerPresetLocation.GetPreset( nPresetIdx, infoMini );	break;
				case NODE_DATABASE:				DataManagerNonDb.ServerPresetDatabase.GetPreset( nPresetIdx, infoMini );	break;
				case NODE_SYSTEM:				DataManagerNonDb.ServerPresetSystem.GetPreset( nPresetIdx, infoMini );		break;
				}

				nPresetNo = infoMini.m_nPresetNo;
				nServerNo = infoMini.m_nServerNo;
				
				if ( nServerNo >= 0 )
					break;
			}

			if ( ( nServerNo < 0 ) || ( nServerNo > Server::ServerNo.Max ) )
				nServerNo = 0;

			bool bAllow = TRUE;

			if ( nPresetNo > 10 )
			{
				if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_827 ) == FALSE )
				{
					bAllow = FALSE;
					TallyDownloadExclusion( NODE_SVRPRESET_SYSTEM, SPOS_V4_827, 1 );
				}
			}

			if ( TRUE == bAllow )
			{
				SQLOut.Add( nTNo );
				SQLOut.Add( nPresetNo );
				SQLOut.Add( nServerNo );
				SQLOut.WriteInsert();
				TallyDownloadAccept();
			}
		}
	}

	SQLOut.WriteFileEnd();
	
	DataManagerNonDb.CloseServerPresetSystem( infoDM );
	DataManagerNonDb.CloseServerPresetDatabase( infoDM );
	DataManagerNonDb.CloseServerPresetLocation( infoDM );
	DataManagerNonDb.CloseServerPresetTerminal( infoDM );

	return COMMS_ERROR_NONE;
}

/**********************************************************************/

