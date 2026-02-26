/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
/**********************************************************************/
#include "CommsExportFunction.h"
/**********************************************************************/

CCommsExportFunction::CCommsExportFunction( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportFunction::PrepareFile()
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

int CCommsExportFunction::PrepareSPOSFile( int nDbIdx )
{
	CSSFile fileFunction;
	m_strSourceFilename = "Function.sql";
	if ( fileFunction.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[FuncNo]" );
	csvHeader.Add( "[Auth]" );
	csvHeader.Add( "[Password]" );
	csvHeader.Add( "[Audit]" );
	csvHeader.Add( "[Log]" );
	
	CSQLOut SQLOut ( fileFunction, "Functions", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();

	CDataManagerInfo infoDM;

	{
		CFilenameUpdater FnUpLoc ( SysFiles::FunctionAuthority, m_nLocIdx, -1, FNUP_FAMILY_LOCATION );
		DataManagerNonDb.OpenFunctionLocation( m_nLocIdx, DB_READONLY, infoDM );
			
		CFilenameUpdater FnUpDb ( SysFiles::FunctionAuthority, nDbIdx, -1, FNUP_FAMILY_DATABASE );
		DataManagerNonDb.OpenFunctionDatabase( nDbIdx, DB_READONLY, infoDM );
			
		CFilenameUpdater FnUpSys ( SysFiles::FunctionAuthority );
		DataManagerNonDb.OpenFunctionSystem( DB_READONLY, infoDM );
	}

	for ( int nFuncIdx = 0; nFuncIdx < DataManagerNonDb.FunctionLocation.GetFunctionCount(); nFuncIdx++ )
	{
		CFunctionAuthMiniInfo infoMini;
		DataManagerNonDb.FunctionLocation.GetFunction( nFuncIdx, infoMini );

		if ( infoMini.m_nAuthLevel < 0 )
			DataManagerNonDb.FunctionDatabase.GetFunction( nFuncIdx, infoMini );

		if ( infoMini.m_nAuthLevel < 0 )
			DataManagerNonDb.FunctionSystem.GetFunction( nFuncIdx, infoMini );

		SQLOut.AddCondition ( infoMini.m_nFuncNo );
		SQLOut.Add( infoMini.m_nAuthLevel );
		SQLOut.Add( infoMini.m_bPassword );
		SQLOut.Add( infoMini.m_bAudit );
		SQLOut.Add( infoMini.m_bLog );
		SQLOut.WriteModify();

		TallyDownloadAccept();
	}

	SQLOut.WriteFileEnd();

	DataManagerNonDb.CloseFunctionLocation( infoDM );
	DataManagerNonDb.CloseFunctionDatabase( infoDM );
	DataManagerNonDb.CloseFunctionSystem( infoDM );

	return COMMS_ERROR_NONE;
}

/**********************************************************************/

