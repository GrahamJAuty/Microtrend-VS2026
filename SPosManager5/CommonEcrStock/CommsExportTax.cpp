/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "CommsExportTax.h"
/**********************************************************************/

CCommsExportTax::CCommsExportTax( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportTax::PrepareFile()
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
		DataManager.ReopenTaxRates( info );
	}

	int nResult = COMMS_ERROR_NODATA;

	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == TRUE )
		nResult = PrepareSPOSFile();
	else
		nResult = COMMS_ERROR_CREATEFILE;
	
	return nResult;
}

/**********************************************************************/

int CCommsExportTax::PrepareSPOSFile()
{
	CSSFile fileTax;
	m_strSourceFilename = "Tax.sql";
	if ( fileTax.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	fileTax.WriteLine( "-->FILEBEGIN" );

	int nMaxTaxBand = MAX_TAX_BANDS;
	if (CSPOSVersions::CheckSPOSVersion(dbNetwork.GetSPOSVersion(m_nNetworkIdx), SPOS_V4_2_1081) == FALSE)
	{
		nMaxTaxBand = 6;
		TallyDownloadExclusion(NODE_TAX, SPOS_V4_2_1081, MAX_TAX_BANDS - nMaxTaxBand);
	}

	for ( int n = 1; n <= nMaxTaxBand; n++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( n, TaxRateInfo );

		CString strLine;
		strLine.Format( "UPDATE CfgSys SET VATV%d=%f",
			n, TaxRateInfo.m_dTaxRate );

		fileTax.WriteLine( strLine );
		fileTax.WriteLine ( "--->GO" );
		TallyDownloadAccept();
	}

	fileTax.WriteLine( "-->FILEEND" );
	fileTax.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

