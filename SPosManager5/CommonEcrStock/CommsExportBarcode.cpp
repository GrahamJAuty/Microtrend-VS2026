/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
#include "SPOSVersions.h"
//#include "DealerFlags.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsExportBarcode.h"
/**********************************************************************/

CCommsExportBarcode::CCommsExportBarcode( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportBarcode::PrepareFile()
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

	if ( FALSE == m_bFromMemory )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles( nDbIdx, TRUE );
		DataManager.ReopenBarcode( info );
	}
	
	int nResult = COMMS_ERROR_NODATA;

	if ( DataManager.Barcode.GetSize() != 0 )
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

int CCommsExportBarcode::PrepareSPOSFile( int nSPOSVersion )
{
	CSSFile fileBarcode;
	m_strSourceFilename = "Barcodes.sql";
	if ( fileBarcode.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[PLU]" );
	csvHeader.Add( "[Barcode]" );
	
	CSQLOut SQLOut ( fileBarcode, "Barcodes", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.Barcode.GetSize();
	for ( int nBarcodeIdx = 0; nBarcodeIdx < nSize; nBarcodeIdx++ )
	{
		StatusProgress.SetPos ( nBarcodeIdx, nSize );

		CBarcodeCSVRecord BarcodeRecord;
		DataManager.Barcode.GetAt ( nBarcodeIdx, BarcodeRecord );

		CString strBarcodeNo = BarcodeRecord.GetBarcodeNoForDownload();
		if ( "" == strBarcodeNo )
			continue;
		
		__int64 nSSPluNo = BarcodeRecord.GetPluNoInt();
		int nMod = ( BarcodeRecord.GetModifier() % 10 );
		
		__int64 nSharpPluNo;			
		if ( SysInfo.IsModifiableSSPluNo( nSSPluNo ) == FALSE )
			nSharpPluNo = nSSPluNo;
		else if ( EcrmanOptions.GetPluBarcodeModifierFlag() == TRUE )
			nSharpPluNo = ( nSSPluNo * 10 ) + nMod;
		else
			nSharpPluNo = ( nSSPluNo * 10 );
			
		SQLOut.Add ( nSharpPluNo );
		SQLOut.Add ( strBarcodeNo );
		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}

	StatusProgress.Unlock();
	
	SQLOut.WriteFileEnd();
	fileBarcode.Close();

	return COMMS_ERROR_NONE;
}

/**********************************************************************/

