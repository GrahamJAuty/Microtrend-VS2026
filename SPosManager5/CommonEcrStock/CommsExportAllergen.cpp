/**********************************************************************/
 
/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
//#include "DataManagerNonDb.h"
#include "SPOSVersions.h"
#include "GroupCSVArrayAllergen.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportAllergen.h"
/**********************************************************************/

CCommsExportAllergen::CCommsExportAllergen( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportAllergen::PrepareFile()
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

	if ( m_bFromMemory == FALSE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenAllergen( DB_READONLY, info );
	}

	int nResult = COMMS_ERROR_NODATA;

	if ( DataManagerNonDb.Allergen.GetSize() != 0 )
	{
		if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
			nResult = PrepareSPOSFile();
		else
			nResult = COMMS_ERROR_CREATEFILE;
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportAllergen::PrepareSPOSFile()
{
	CSSFile fileAllergen;
	m_strSourceFilename = "Allergens.sql";
	if ( fileAllergen.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[AllergyCode]" );
	csvHeader.Add( "[Description]" );
	
	CSQLOut SQLOut ( fileAllergen, "Allergies", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	
	StatusProgress.Lock();
	int nSize = DataManagerNonDb.Allergen.GetSize();
	for ( int nIdx = 0; nIdx < nSize; nIdx++ )
	{
		StatusProgress.SetPos ( nIdx, nSize );
		
		CGroupCSVRecordAllergen Allergen;
		DataManagerNonDb.Allergen.GetAt( nIdx, Allergen );
		
		SQLOut.Add( Allergen.GetGroupNo() );
		SQLOut.Add( Allergen.GetEposText() );

		CString strDelete;
		strDelete.Format( "DELETE FROM Allergies WHERE AllergyCode=%d", Allergen.GetGroupNo() );
		fileAllergen.WriteLine( strDelete );
		fileAllergen.WriteLine( "--->GO" );
		
		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	fileAllergen.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

