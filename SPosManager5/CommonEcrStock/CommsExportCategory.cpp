/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "AnalysisCategoryCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportCategory.h"
/**********************************************************************/

CCommsExportCategory::CCommsExportCategory( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportCategory::PrepareFile()
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
		DataManager.ReopenAnalysisCategory( info );
	}
	
	if ( DataManager.AnalysisCategory.GetSize() != 0 )
	{
		if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile();
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportCategory::PrepareSPOSFile()
{
	CSSFile fileCategory;
	m_strSourceFilename = "Analysis.sql";
	if ( fileCategory.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[FieldName]" );
	csvHeader.Add( "[FieldIndex]" );
	csvHeader.Add( "[iValue]" );
	csvHeader.Add( "[fValue]" );
	csvHeader.Add( "[sValue]" );
	
	CSQLOut SQLOut ( fileCategory, "SysValues", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();

	int nMaxCatNo = MAX_ACAT_NO;
	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4_2133 ) == FALSE )
	{
		nMaxCatNo = 9;
		TallyDownloadExclusion( NODE_ACAT, SPOS_V4_2133, MAX_ACAT_NO - nMaxCatNo );
	}

	for ( int nAnalysisCatNo = 1; nAnalysisCatNo <= nMaxCatNo; nAnalysisCatNo++ )
	{
		CAnalysisCategoryCSVRecord Record;
		DataManager.AnalysisCategory.GetDisplayCategory( nAnalysisCatNo, Record );

		SQLOut.AddCondition ( "Category" );
		SQLOut.AddCondition ( nAnalysisCatNo );
		SQLOut.Add( Record.GetType() );
		SQLOut.Add( 0.0, 3 );
		SQLOut.Add( Record.GetEposText() );
		SQLOut.WriteModify();
		TallyDownloadAccept();
	}

	SQLOut.WriteFileEnd();
	fileCategory.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

