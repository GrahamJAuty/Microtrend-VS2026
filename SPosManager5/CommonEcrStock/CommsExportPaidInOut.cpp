/**********************************************************************/
 
/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "SPOSVersions.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportPaidInOut.h"
/**********************************************************************/

CCommsExportPaidInOut::CCommsExportPaidInOut( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportPaidInOut::PrepareFile()
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

	if ( FALSE == m_bFromMemory )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenPaidInText( info );
		DataManager.ReopenPaidOutText( info );
	}
	
	if ( ( DataManager.PaidInText.GetSize() != 0 ) || ( DataManager.PaidOutText.GetSize() != 0 ) )
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion( m_nNetworkIdx );
		if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile( nSPOSVersion );
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportPaidInOut::PrepareSPOSFile( int nSPOSVersion )
{
	CSSFile filePaidInOut;
	m_strSourceFilename = "PaidIO.sql";
	if ( filePaidInOut.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	csvHeader.Add( "[fValue]" );
	csvHeader.Add( "[sValue]" );
	
	CSQLOut SQLOut ( filePaidInOut, "SysValues", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();

	int nMaxText = PAIDIO_TEXT_COUNT;

	if ( CSPOSVersions::CheckSPOSVersion( nSPOSVersion, SPOS_V4_2_369 ) == FALSE )
	{
		nMaxText = 8;
		TallyDownloadExclusion( NODE_PAIDIN_TEXT, SPOS_V4_2_369, ( PAIDIO_TEXT_COUNT - 8 ) * 2 );
	}

	for ( int nNo = 1; nNo <= nMaxText; nNo++ )
	{
		CPaidInOutTextCSVRecord Record;
		DataManager.PaidInText.GetPaidInOutText( nNo, Record );

		SQLOut.AddCondition ( "PAIDIO.IN" );
		SQLOut.AddCondition ( nNo );
		SQLOut.Add( 0 );
		SQLOut.Add( 0.0, 3 );
		SQLOut.Add( Record.GetEposText() );
		SQLOut.WriteModify();
		TallyDownloadAccept();
	}

	for ( int nNo = 1; nNo <= nMaxText; nNo++ )
	{
		CPaidInOutTextCSVRecord Record;
		DataManager.PaidOutText.GetPaidInOutText( nNo, Record );

		SQLOut.AddCondition ( "PAIDIO.OUT" );
		SQLOut.AddCondition ( nNo );
		SQLOut.Add( 0 );
		SQLOut.Add( 0.0, 3 );
		SQLOut.Add( Record.GetEposText() );
		SQLOut.WriteModify();
		TallyDownloadAccept();
	}

	SQLOut.WriteFileEnd();
	filePaidInOut.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

