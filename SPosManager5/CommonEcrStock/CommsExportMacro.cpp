/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
/**********************************************************************/
#include "CommsExportMacro.h"
/**********************************************************************/

CCommsExportMacro::CCommsExportMacro( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportMacro::PrepareFile()
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
	
	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;

	int nResult = COMMS_ERROR_NODATA;

	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
		nResult = PrepareSPOSFile( nDbIdx );
	else
		nResult = COMMS_ERROR_CREATEFILE;
	
	return nResult;
}

/**********************************************************************/

int CCommsExportMacro::PrepareSPOSFile( int nDbIdx )
{
	CSSFile fileMacro;
	m_strSourceFilename = "Macros.sql";
	if ( fileMacro.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[MacNo]" );
	csvHeader.Add( "[MacText]" );
	csvHeader.Add( "[Description]" );
	
	CSQLOut SQLOut ( fileMacro, "Macros", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();

	CDataManagerInfo infoDM;
	DataManagerNonDb.OpenCCMacroLocation( m_nLocIdx, DB_READONLY, infoDM );
	DataManagerNonDb.CCMacroLocation.LoadParentMacrosDatabase(nDbIdx);
	DataManagerNonDb.CCMacroLocation.LoadParentMacrosSystem();

	for ( int nMacroIdx = 0; nMacroIdx < DataManagerNonDb.CCMacroLocation.GetSize(); nMacroIdx++ )
	{
		CCascadingMacroCSVRecord Macro;
		DataManagerNonDb.CCMacroLocation.GetAt( nMacroIdx, Macro );

		CString strName, strData;
		if ( Macro.GetLocalMacroFlag() == FALSE )
		{
			strName = Macro.GetParentMacroName();
			strData = Macro.GetParentMacroData();
		}
		else
		{
			strName = Macro.GetLocalMacroName();
			strData = Macro.GetLocalMacroData();
		}

		int nMacroNo = Macro.GetMacroNo();

		{
			CString strDelete;
			strDelete.Format( "DELETE FROM Macros WHERE MacNo=%d", nMacroNo );
			fileMacro.WriteLine( strDelete );
			fileMacro.WriteLine( "--->GO" );
		}
		
		SQLOut.Add( nMacroNo );
		SQLOut.Add( strData );
		SQLOut.Add( strName );
		SQLOut.WriteInsert();

		TallyDownloadAccept();
	}

	SQLOut.WriteFileEnd();

	DataManagerNonDb.CloseCCMacroLocation( infoDM );
	
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

