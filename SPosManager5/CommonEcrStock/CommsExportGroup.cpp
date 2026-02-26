/**********************************************************************/
 
/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "SPOSVersions.h"
#include "GroupCSVArrayEpos.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportGroup.h"
/**********************************************************************/

CCommsExportGroup::CCommsExportGroup( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportGroup::PrepareFile()
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

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenEposGroup( info );
	}

	int nResult = COMMS_ERROR_NODATA;

	if ( DataManager.EposGroup.GetSize() != 0 )
	{
		if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile();
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportGroup::PrepareSPOSFile()
{
	CSSFile fileGroup;
	m_strSourceFilename = "Groups.sql";
	if ( fileGroup.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[GrpNo]" );
	csvHeader.Add( "[Name]" );
	
	CSQLOut SQLOut ( fileGroup, "Groups", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.EposGroup.GetSize();
	for ( int nGroupIdx = 0; nGroupIdx < nSize; nGroupIdx++ )
	{
		StatusProgress.SetPos ( nGroupIdx, nSize );
		
		CGroupCSVRecordEpos GroupRecord;
		DataManager.EposGroup.GetAt( nGroupIdx, GroupRecord );
		
		SQLOut.Add( GroupRecord.GetGroupNo() );
		SQLOut.Add( GroupRecord.GetEposText() );
		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileGroup.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

