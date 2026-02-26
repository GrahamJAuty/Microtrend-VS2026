/**********************************************************************/
#include "EposReportConsolParams.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PMSReportFileFinder.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCoversPayment.h"
/**********************************************************************/

CEposReportCoversPayment::CEposReportCoversPayment( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_COVERS_PAYMENT;
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
}

/**********************************************************************/

CEposReportCoversPayment::~CEposReportCoversPayment()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportCoversPayment::InitialiseConsolidationBlocks()
{
	m_bAllowGlobal = ( NODE_SYSTEM == m_nConLevel );
	m_bAllowGlobal &= ( EcrmanOptions.GetGlobalPaymentFlag() == TRUE ); 

	CString strParams = "";
	switch( m_nPreferConsolType )
	{
	case EPOSREPORT_CONSOL_SAVED:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSaved();
		break;

	case EPOSREPORT_CONSOL_ADHOC:
		strParams = m_EposReportSelectInfo.GetConsolPrefsAdhoc();
		break;
		
	case EPOSREPORT_CONSOL_DEFAULT:
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		break;
		
	case EPOSREPORT_CONSOL_SYSTEM:
	default:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSummary();
		break;
	}

	CEposReportConsolParamsStandard Params;
	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}

	if ( ( FALSE == m_bAllowGlobal ) && ( TRUE == Params.m_bConsolSys ) )
	{
		Params.m_bConsolSys = FALSE;
		Params.m_bConsolDb = TRUE;
	}

	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( Params.m_bConsolSys );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportCoversPaymentBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolCoversPayment>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportCoversPayment::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	bool bInclude = FALSE;
	COleDateTime oleTranDate;
	CString strTranDate = "";
	CString strTranTime = "";
	
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( FALSE == m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		bool bTransactionServerMatch = FALSE;

		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			nDbIdx = 0;

		CDataManagerInfo infoDM;
		DataManager.OpenDatabaseReadOnly( nDbIdx, infoDM, FALSE );

		m_PMSModes.GetPMSTransactionDate( bIsPMSLocation, infoFile, strTranDate, strTranTime );

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstTermLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			m_CashRSPVersionChecker.ResetTransactionVersions();
			int nCurrentTransactionCovers = 0;

			CReportConsolidationArray<CSortedIntItem> arrayPaymentTypes;
		
			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				CCSVEposTermLine csvIn ( strBuffer );

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				if ( m_PMSModes.CheckPMSTransactionType( bIsPMSLocation, nCashRSPLineType ) == FALSE )
					continue;

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
					{
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

						CString strDate = csvIn.GetTransactionDate();
						CString strTime = csvIn.GetTransactionTime();

						bInclude = FALSE;

						nCurrentTransactionCovers = csvIn.GetTransactionCovers();
						if ( 0 == nCurrentTransactionCovers ) 
						{
							if ( DealerFlags.GetAssumeCoversFlag() == TRUE )
								nCurrentTransactionCovers = 1;
							else
								continue;
						}

						arrayPaymentTypes.RemoveAll();

						bool bAcceptDateTime = FALSE;

						if ( ( TRUE == bIsPMSLocation ) && ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT ) )
							bAcceptDateTime = TRUE;
						else
						{
							CString strDate = csvIn.GetTransactionDate();
							CString strTime = csvIn.GetTransactionTime();	
				
							//FILTER FOR DATE AND TIME
							if ( strDate.GetLength() != 8 || strTime.GetLength() != 8 )
								continue;
						
							strTranDate.Format ( "20%s%s%s",
								(const char*) strDate.Right(2),
								(const char*) strDate.Mid(3,2),
								(const char*) strDate.Left(2) );

							strTranTime.Format ( "%s%s%s",
								(const char*) strTime.Left(2),
								(const char*) strTime.Mid(3,2),
								(const char*) strTime.Right(2) );

							bAcceptDateTime = SimpleTimeCheck ( infoFile.m_nLocIdx, strTranDate, strTranTime );
						}

						if ( TRUE == bAcceptDateTime )
						{
							bInclude = TRUE;

							oleTranDate = COleDateTime (
									atoi(strTranDate.Left(4)),
									atoi(strTranDate.Mid(4,2)),
									atoi(strTranDate.Right(2)),
									0, 0, 0 );

							CEposReportConsolCoversPayment infoTotal;
							infoTotal.m_nCovers = nCurrentTransactionCovers;
							infoTotal.m_nPaymentNo = 0;
							infoTotal.m_nAmount = 0;

							for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
							{
								int nBlock = m_arrayTerminalBlocks.GetAt(n);
								m_arrayReportBlocks[ nBlock ].m_BlockTotal.Add( infoTotal );
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
					nCurrentTransactionCovers = 0;
					arrayPaymentTypes.RemoveAll();
					break;

				case CASHRSP_LINETYPE_PAYMENT:
				case CASHRSP_LINETYPE_DEPOSIT_PMS:
					if ( TRUE == bInclude )
					{
						int nPaymentNo = csvIn.GetPaymentNumber();
						
						switch( nConnectionType )
						{
						case CONNECTION_TYPE_SPTBOOK_NONE:
							nPaymentNo = DataManager.SBPaymentMap.GetSharpPosPaymentNo( nPaymentNo );
							break;

						case CONNECTION_TYPE_SMARTENT_NONE:
							nPaymentNo = DataManager.SEPaymentMap.GetSharpPosPaymentNo( nPaymentNo );
							break;

						case CONNECTION_TYPE_STANDARD_NONE:
						default:
							{
								if ( SysInfo.IsPMSSystem() == TRUE )
									if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
										if ( DataManager.Payment.IsPMSPaymentType( csvIn.GetPaymentNumber() ) == TRUE )
											bInclude = FALSE;
							}
							break;
						}
						
						if ( TRUE == bInclude )
						{
							CEposReportConsolCoversPayment infoLine;
							infoLine.m_nPaymentNo = nPaymentNo;
							infoLine.m_nAmount = csvIn.GetPaymentAmount();
							
							int nPos;
							CSortedIntItem item;
							item.m_nItem = nPaymentNo;
							if ( arrayPaymentTypes.Find( item, nPos ) == FALSE )
							{
								arrayPaymentTypes.InsertAt( nPos, item );
								infoLine.m_nCovers = nCurrentTransactionCovers;
							}
							else
							{
								infoLine.m_nCovers = 0;
							}

							CEposReportConsolCoversPayment infoTotal;
							infoTotal.m_nCovers = 0;
							infoTotal.m_nPaymentNo = 0;
							infoTotal.m_nAmount = infoLine.m_nAmount;
							
							for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
							{
								int nBlock = m_arrayTerminalBlocks.GetAt(n);
								m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( infoLine );
								m_arrayReportBlocks[ nBlock ].m_BlockTotal.Add( infoTotal );
							}

							m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleTranDate );
							m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleTranDate );
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
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportCoversPayment::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 120 );
	m_ReportFile.AddColumn ( "Name", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "Covers", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "Per Head", TA_RIGHT, 300 );
	
	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportCoversPaymentBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nSortIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportCoversPaymentBlock ReportBlock = m_arrayReportBlocks.GetAt( nSortIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		CreateSalesSection( ReportBlock );
		WriteBlockTotals( ReportBlock );
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCoversPayment::CreateSalesSection( CEposReportCoversPaymentBlock& Block )
{
	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolCoversPayment Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		double dReportAmount = ( (double) Sales.m_nAmount ) / 100.0;

		CCSV csv ( '\t' );
		csv.Add ( Sales.m_nPaymentNo );
		csv.Add ( DataManager.Payment.GetDisplayName( Sales.m_nPaymentNo ) );
		csv.Add ( dReportAmount, SysInfo.GetDPValue() );
		csv.Add ( Sales.m_nCovers );
		
		if (Sales.m_nCovers != 0)
		{
			csv.Add(dReportAmount / Sales.m_nCovers, SysInfo.GetDPValue());
		}

		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
}

/**********************************************************************/

void CEposReportCoversPayment::WriteBlockTotals( CEposReportCoversPaymentBlock& Block )
{
	//TOTAL COVERS FOR BLOCK MAY BE LESS THAN COVERS FOR ALL PAYMENT TYPES
	//ADDED TOGETHER BECAUSE TWO OR MORE PAYMENT TYPES MAY BE USED IN
	//THE SAME TRANSACTION

	CString strType = GetTotalTypeName( TRUE, Block.m_nEntityType, FALSE, TRUE );
	
	double dReportAmount = ( (double) Block.m_BlockTotal.m_nAmount ) / 100.0;

	CCSV csv ( '\t' );
	csv.Add ( strType );
	csv.Add ( "" );
	csv.Add ( dReportAmount, SysInfo.GetDPValue() );
	csv.Add ( Block.m_BlockTotal.m_nCovers );
	
	if (Block.m_BlockTotal.m_nCovers != 0)
	{
		csv.Add(dReportAmount / Block.m_BlockTotal.m_nCovers, SysInfo.GetDPValue());
	}

	m_ReportFile.WriteReportMiscLine ( "\t\t<LI>\t<LI>\t<LI>" );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
