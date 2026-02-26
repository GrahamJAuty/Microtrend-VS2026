/**********************************************************************/
 
/**********************************************************************/
#include "BillAndReceiptTexts.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "LocationSelectorEntity.h"
#include "NetworkCSVArray.h"
#include "SQLOut.h"
/**********************************************************************/
#include "CommsExportBillText.h"
/**********************************************************************/

CCommsExportBillText::CCommsExportBillText( CJobListCSVRecord& JobInfo, bool bFromMemory ) : CCommsExportBase( JobInfo, bFromMemory )
{
}

/**********************************************************************/

int CCommsExportBillText::PrepareFile()
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
		nResult = PrepareSPOSFile();
	else
		nResult = COMMS_ERROR_CREATEFILE;
	
	return nResult;
}

/**********************************************************************/

int CCommsExportBillText::PrepareSPOSFile()
{
	CSSFile fileBillText;
	m_strSourceFilename = "RcptText.sql";
	if ( fileBillText.Open ( m_strSourceFolder + m_strSourceFilename, "wb" ) == FALSE )
		return COMMS_ERROR_CREATEFILE;

	CCSV csvHeader;
	csvHeader.Add( "[TillNo]" );

	csvHeader.Add( "[BMESS1]" );
	csvHeader.Add( "[BMESS2]" );
	csvHeader.Add( "[BMESS3]" );
	csvHeader.Add( "[BMESS4]" );
	csvHeader.Add( "[BMESS5]" );

	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4_1071 ) == TRUE )
		csvHeader.Add( "[BMESS6]" );

	csvHeader.Add( "[RMESS1]" );
	csvHeader.Add( "[RMESS2]" );
	csvHeader.Add( "[RMESS3]" );
	csvHeader.Add( "[RMESS4]" );
	csvHeader.Add( "[RMESS5]" );

	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4_1071 ) == TRUE )
		csvHeader.Add( "[RMESS6]" );
	
	CSQLOut SQLOut ( fileBillText, "CfgTills", csvHeader.GetLine() );
	SQLOut.WriteFileBegin();

	CBillAndReceiptTexts Texts;

	for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( m_nLocIdx ); nTermIdx++ )
	{
		CLocationSelectorEntity entity;
		entity.SetLocationNode( m_nLocIdx );
		entity.SetTerminalNode( nTermIdx );

		if ( Texts.SetLocSelEntity( entity, TRUE ) == TRUE )
			Texts.Read();

		SQLOut.AddCondition ( dbLocation.GetTNo( m_nLocIdx, nTermIdx ) );
		
		SQLOut.Add( Texts.GetBillText1() );
		SQLOut.Add( Texts.GetBillText2() );
		SQLOut.Add( Texts.GetBillText3() );
		SQLOut.Add( Texts.GetBillText4() );
		SQLOut.Add( Texts.GetBillText5() );

		if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4_1071 ) == TRUE )
			SQLOut.Add( Texts.GetBillText6() );

		SQLOut.Add( Texts.GetReceiptText1() );
		SQLOut.Add( Texts.GetReceiptText2() );
		SQLOut.Add( Texts.GetReceiptText3() );
		SQLOut.Add( Texts.GetReceiptText4() );
		SQLOut.Add( Texts.GetReceiptText5() );

		if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4_1071 ) == TRUE )
			SQLOut.Add( Texts.GetReceiptText6() );

		SQLOut.WriteModify();
		TallyDownloadAccept();
	}

	SQLOut.WriteFileEnd();
	fileBillText.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/

