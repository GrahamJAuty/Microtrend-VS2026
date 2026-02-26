/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "DateRangeHelpers.h"
//#include "EcrmanOptionsIni.h"
#include "EposReportFileListMaker.h"
#include "LocationCSVArray.h"
#include "LocationSelectorEntity.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "ReportPointCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayExportConsolidatorPayment.h"
/**********************************************************************/

CPosTrayExportConsolidatorPayment::CPosTrayExportConsolidatorPayment( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg ) : CPosTrayExportConsolidator( nReportDateType, SelectArray, bQuitNow, pHostDlg )
{
	m_CashRSPVersionChecker.Reset();

	m_PMSModes.SetDateMode( PMS_DATEMODE_ACTUAL );
	m_PMSModes.SetAllowMode( PMS_ALLOWMODE_ALL );
	m_PMSModes.SetEposFlag( FALSE );
	m_PMSModes.SetWantPendingFlag( TRUE );
	m_PMSModes.SetWantActiveFlag( TRUE );
	m_PMSModes.SetWantCompleteFlag( TRUE );
	m_PMSModes.SetWantCancelledFlag( FALSE );
	m_PMSModes.SetWantDepositsFlag( TRUE );

	m_bEODMode = FALSE;
	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);

	AddAcceptedField ( POSTRAY_EXPORTFIELD_DBNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LOCNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_RPNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TRMNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_CAL );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_BUS );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_YEARNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MONTHNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_WEEK );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKDAYNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TIME_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SERVER_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PAYNO );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DBNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LOCNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_RPNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SRVNAME_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MONTHNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKDAYNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PAYNAME );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_QTY_PAY );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_QTY_COVERS );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_TENDER );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_AMOUNT );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_GRATUITY );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_CASHBACK );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_SURPLUS );
}

/**********************************************************************/

void CPosTrayExportConsolidatorPayment::ResetTaskSpecific( CPosTrayTask& EmailTask )
{
	if ( m_arrayOutputFields.GetSize() == 0 )
	{
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_PAYNO );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_PAYNAME );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_QTY_PAY );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_TENDER );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_AMOUNT );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_GRATUITY );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_CASHBACK );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_SURPLUS );
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_PAYNO );
	}

	if ( m_arrayConsolidationFields.GetSize() == 0 )
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_DUMMY );

	m_PMSModes.SetEposFlag ( EmailTask.GetBatchPMSTerminalFlag() );

	CLocationSelectorLookup lookup( EmailTask );
	CLocationSelectorEntity entity( lookup );
	entity.GetEPOSSelectArray( m_SelectArray );
	m_SelectArray.MakeList( TRUE );

	m_PaymentTree.ClearConsolidatedLevels();
	m_PaymentTree.ClearFieldStructure();
}

/**********************************************************************/

bool CPosTrayExportConsolidatorPayment::PrepareFields()
{
	return CPosTrayExportConsolidator::PrepareFields( m_PaymentTree );
}

/**********************************************************************/

void CPosTrayExportConsolidatorPayment::Consolidate() 
{
	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	dbLocation.ResetRpNoReport();
	dbReportpoint.BuildSortIdxTables();

	m_PaymentTree.ClearConsolidatedLevels();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, FALSE, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, FALSE, nTotalFileSize );
	
	int nProgress = 0;
	__int64 nInternalTranNo = 0;

	if ( NULL == m_pHostDialog )
		StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	int nLastDbIdx = -1;
	bool bInclude = FALSE;
	CDateRangeHelpers DateRangeHelpers;
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( FALSE == m_bQuitNow ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		//CHECK PMS STATUS
		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		//CHECK SPORTS BOOKER NETWORK STATUS
		int nConnectionType;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		//CHECK LOCATION SETTINGS
		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			continue;

		int nRpIdx = 0;
		if ( infoFile.m_nTermIdx >= 0 )
		{
			int nRpNo = dbLocation.GetRpNoReport( infoFile.m_nLocIdx, infoFile.m_nTermIdx );
			if ( dbReportpoint.FindReportpointByNumber( nDbNo, nRpNo, nRpIdx ) == TRUE )
				nRpIdx = dbReportpoint.GetSortIdxFromRpIdx( nRpIdx ) + 1;
			else
				nRpIdx = 0;
		}

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
		int nSortLocIdx = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
		
		int nTNo = 0;
		if ( infoFile.m_nTermIdx >= 0 )
			nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx );

		//LOCATION SETTINGS APPLY TO THE ENTIRE FILE
		for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
		{
			switch ( m_arrayConsolidationFields.GetAt( n ) )
			{
			case POSTRAY_EXPORTFIELD_DUMMY:	m_PaymentTree.SetBufferInt( n, 0 );					break;
			case POSTRAY_EXPORTFIELD_DBNO:	m_PaymentTree.SetBufferInt( n, nSortDbIdx );		break;
			case POSTRAY_EXPORTFIELD_LOCNO:	m_PaymentTree.SetBufferInt( n, nSortLocIdx );		break;
			case POSTRAY_EXPORTFIELD_TRMNO:	m_PaymentTree.SetBufferInt( n, nTNo );				break;
			case POSTRAY_EXPORTFIELD_RPNO:	m_PaymentTree.SetBufferInt( n, nRpIdx );			break;	
			}
		}

		//OPEN SOURCE FILE
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		//MAKE SURE THAT THE RIGHT DATABASE IS LOADED
		if ( nDbIdx != nLastDbIdx )
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			nLastDbIdx = nDbIdx;
		}

		CCSVEposTermLine csvIn( "" );
		int nTranCovers = 0;
		int nTranSeqNo = 0;

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstTermLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			m_CashRSPVersionChecker.ResetTransactionVersions();
			
			do
			{
				ShowProgress ( nProgress + fileSales.Tell(), nTotalFileSize );
				csvIn.ParseLine ( strBuffer );

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				if ( m_PMSModes.CheckPMSTransactionType( bIsPMSLocation, nCashRSPLineType ) == FALSE )
					continue;

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
					{
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
							
						bInclude = FALSE;
						
						CString strDate = csvIn.GetTransactionDate();
						CString strTime = csvIn.GetTransactionTime();
						if ( strDate.GetLength() != 8 || strTime.GetLength() != 8 )
							continue;
					
						CString strCheckDate;
						strCheckDate.Format ( "20%s%s%s",
							(const char*) strDate.Right(2),
							(const char*) strDate.Mid(3,2),
							(const char*) strDate.Left(2) );

						CString strCheckTime;
						strCheckTime.Format ( "%s%s%s",
							(const char*) strTime.Left(2),
							(const char*) strTime.Mid(3,2),
							(const char*) strTime.Right(2) );

						//CHECK DATE OF PAYMENTS
						COleDateTime oleCalendarDate = COleDateTime ( 
							atoi ( strCheckDate.Left(4) ), 
							atoi ( strCheckDate.Mid(4,2) ), 
							atoi ( strCheckDate.Right(2) ), 
							0, 0, 0 );

						if ( oleCalendarDate.m_status != COleDateTime::valid )
							continue;

						//CHECK FOR PREVIOUS DATE
						if ( m_BusinessDateInfo.ValidateBusinessDate( infoFile.m_nLocIdx, m_bEODMode, strCheckDate, strCheckTime ) == FALSE )
							continue;
						
						bInclude = TRUE;

						nTranCovers = csvIn.GetTransactionCovers();
						if ( ( 0 == nTranCovers ) && ( DealerFlags.GetAssumeCoversFlag() == TRUE ) )
							nTranCovers = 1;

						nTranSeqNo = csvIn.GetTransactionNumber() % 1000000;

						nInternalTranNo++;
						CreateTimeLabel( strCheckTime, TRUE );

						//TRANSLATE SERVER INTO CONSOLIDATION STRING
						CString strTranServer = "";
						if ( TRUE == m_bWantTranServer )
							DataManagerNonDb.ServerNameTable.GetServerConsolidationString( nDbIdx, infoFile.m_nLocIdx, csvIn.GetTransactionServer(), strTranServer );

						//THESE VALUES APPLY THROUGHOUT THE TRANSACTION
						for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
						{
							switch ( m_arrayConsolidationFields.GetAt( n ) )
							{
							case POSTRAY_EXPORTFIELD_DATE_BUS:		m_PaymentTree.SetBufferDate( n, m_BusinessDateInfo.GetBusinessDate() );	 break;
							case POSTRAY_EXPORTFIELD_DATE_CAL:		m_PaymentTree.SetBufferDate( n, oleCalendarDate ); break;
							case POSTRAY_EXPORTFIELD_YEARNO:		m_PaymentTree.SetBufferInt( n, m_BusinessDateInfo.GetBusinessDate().GetYear() ); break;
							case POSTRAY_EXPORTFIELD_MONTHNO:		m_PaymentTree.SetBufferInt( n, m_BusinessDateInfo.GetBusinessDate().GetMonth() ); break;
							case POSTRAY_EXPORTFIELD_DATE_WEEK:		m_PaymentTree.SetBufferDate( n, m_BusinessDateInfo.GetBusinessDateWeek() );	break;
							case POSTRAY_EXPORTFIELD_WEEKNO:		m_PaymentTree.SetBufferInt( n, DateRangeHelpers.GetWeekNo( m_BusinessDateInfo.GetBusinessDateWeek() ) );	break; 
							case POSTRAY_EXPORTFIELD_WEEKDAYNO:		m_PaymentTree.SetBufferInt( n, m_BusinessDateInfo.GetWeekday() ); break;
							case POSTRAY_EXPORTFIELD_TIME_TRAN:		m_PaymentTree.SetBufferString( n, strCheckTime );	break;
							case POSTRAY_EXPORTFIELD_SERVER_TRAN:	m_PaymentTree.SetBufferString( n, strTranServer ); break;
							case POSTRAY_EXPORTFIELD_TRAN_SEQNO:	m_PaymentTree.SetBufferInt( n, nTranSeqNo ); break;
							}
						}
					}
					break;

				case CASHRSP_LINETYPE_TRAN_VOID:
				case CASHRSP_LINETYPE_WASTAGE:
				case CASHRSP_LINETYPE_NOSALE:
				case CASHRSP_LINETYPE_AGEVERIFY:
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
					bInclude = FALSE;
					break;

				case CASHRSP_LINETYPE_PAYMENT:
				case CASHRSP_LINETYPE_DEPOSIT_PMS:
					if ( TRUE == bInclude )
					{
						int nPaymentNo = csvIn.GetPaymentNumber();

						switch( nConnectionType )
						{
						case CONNECTION_TYPE_SPTBOOK_NONE:
							nPaymentNo =( DataManager.SBPaymentMap.GetSharpPosPaymentNo( nPaymentNo ) );
							break;

						case CONNECTION_TYPE_SMARTENT_NONE:
							nPaymentNo =( DataManager.SEPaymentMap.GetSharpPosPaymentNo( nPaymentNo ) );
							break;

						case CONNECTION_TYPE_STANDARD_NONE:
						default:
							{
								if ( SysInfo.IsPMSSystem() == TRUE )
									if ( ( FALSE == m_PMSModes.GetEposFlag() ) && ( FALSE == bIsPMSLocation ) )
										if ( DataManager.Payment.IsPMSPaymentType( nPaymentNo ) == TRUE )
											bInclude = FALSE;
							}
							break;
						}

						if ( TRUE == bInclude )
						{
							if ( ( EcrmanOptions.GetReportsMultiCashMethod() == REPORTCASH_METHOD1_CONSOLIDATE ) && ( nPaymentNo != 1 ) )
							{
								int nType, nDummy32;
								__int64 nDummy64;
								bool bIsCash;
								csvIn.GetPaymentAccountTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64, bIsCash );

								if ( TRUE == bIsCash )
									nPaymentNo = 1;
							}

							//THESE VALUES CHANGE ON A LINE-BY-LINE BASIS
							for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
							{
								switch ( m_arrayConsolidationFields.GetAt( n ) )
								{
								case POSTRAY_EXPORTFIELD_PAYNO:			m_PaymentTree.SetBufferInt( n, nPaymentNo ); break;
								}
							}

							CEntityTreeExportPaymentNode Node;
							Node.SetPaymentQty(1);
							Node.SetInternalTranNo( nInternalTranNo );
							Node.SetCoversQty( nTranCovers );
							Node.SetTender(csvIn.GetPaymentTender());
							Node.SetAmount(csvIn.GetPaymentAmount());
							Node.SetGratuity(csvIn.GetPaymentGratuity());
							Node.SetCashback(csvIn.GetPaymentCashback());
							Node.SetSurplus(csvIn.GetPaymentSurplus( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() ));
							m_PaymentTree.Consolidate( Node );
						}
					}
					break;
				}

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_VERSION:
					m_CashRSPVersionChecker.ProcessCashRSPVersionLine( csvIn );
					break;

				case CASHRSP_LINETYPE_APPLICATION:
					m_CashRSPVersionChecker.ProcessAppTypeLine( csvIn );
					break;

				default:
					m_CashRSPVersionChecker.ResetPendingTransactionInfo();
					break;
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) && ( FALSE == m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() ) && ( FALSE == m_bQuitNow ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	if ( NULL == m_pHostDialog )
		StatusProgress.Unlock();
}

/**********************************************************************/

__int64 CPosTrayExportConsolidatorPayment::CreateExport( const char* szFilename )
{
	bool bResult;
	return CPosTrayExportConsolidator::CreateExport( &m_PaymentTree, "Creating Payment Export", szFilename, bResult, FALSE );
}

/**********************************************************************/

void CPosTrayExportConsolidatorPayment::GetNodeTotals( int nBranchIdx, int nLeafIdx )
{
	CEntityTreeExportPaymentNode PaymentNode;
	m_PaymentTree.GetNode( nBranchIdx, nLeafIdx, PaymentNode );
	m_NodeTotals.m_nPaymentQty = PaymentNode.GetPaymentQty();
	m_NodeTotals.m_nCoversQty = PaymentNode.GetCoversQty();
	m_NodeTotals.m_dTender = ( ( double) PaymentNode.GetTender() ) / 100.0;
	m_NodeTotals.m_dAmount = ( ( double) PaymentNode.GetAmount() ) / 100.0;
	m_NodeTotals.m_dGratuity = ( ( double) PaymentNode.GetGratuity() ) / 100.0;
	m_NodeTotals.m_dCashback = ( ( double) PaymentNode.GetCashback() ) / 100.0;
	m_NodeTotals.m_dSurplus = ( ( double) PaymentNode.GetSurplus() ) / 100.0;
}

/**********************************************************************/