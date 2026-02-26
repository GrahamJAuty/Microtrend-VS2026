/**********************************************************************/
 
/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "SPOSVersions.h"
//#include "globalfunctions.h"
#include "ModifierCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportModifiers.h"
/**********************************************************************/

CCommsExportModifiers::CCommsExportModifiers( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportModifiers::PrepareFile()
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
	
	if ( FALSE == m_bFromMemory )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenModifier( info );
	}

	if ( DataManager.Modifier.GetSize() != 0 )
	{
		if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile();
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportModifiers::PrepareSPOSFile()
{
	CSSFile fileModifiers;
	m_strSourceFilename = "Modifier.sql";
	if ( fileModifiers.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[ModNo]" );
	csvHeader.Add( "[Mult]" );
	csvHeader.Add( "[Name]" );

	CSQLOut SQLOut ( fileModifiers, "PLUModifiers", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.Modifier.GetSize();
	for ( int nModIdx = 0; nModIdx < nSize; nModIdx++ )
	{
		StatusProgress.SetPos ( nModIdx, nSize );

		CModifierCSVRecord Modifier;
		DataManager.Modifier.GetAt ( nModIdx, Modifier );

		SQLOut.Add( nModIdx + 1 );
		SQLOut.Add( Modifier.GetMultiplierForQty(), 6 );
		SQLOut.Add( Modifier.GetName() );
		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileModifiers.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

