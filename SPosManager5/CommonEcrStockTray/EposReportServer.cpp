/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportServer.h"
/**********************************************************************/

CEposReportServer::CEposReportServer( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TRANSERVER_SUMMARY;
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

CEposReportServer::~CEposReportServer()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportServer::InitialiseConsolidationBlocks()
{
	m_bAllowSystemSupplierLines = TRUE;
	m_bAllowDatabaseSupplierLines = TRUE;

	switch( m_nServerReportType )
	{
	case SERVER_REPORT_CODE:
	case SERVER_REPORT_NAME:
	case SERVER_REPORT_SYS:
		break;

	case SERVER_REPORT_DB:
		m_bAllowSystemSupplierLines = FALSE;
		break;

	case SERVER_REPORT_LOC:
	default:
		m_bAllowSystemSupplierLines = FALSE;
		m_bAllowDatabaseSupplierLines = FALSE;
		break;
	}

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

	switch( m_nPreferConsolType )
	{
	case EPOSREPORT_CONSOL_DEFAULT:
	case EPOSREPORT_CONSOL_SYSTEM:
		{
			switch( m_nServerReportType )
			{
			case SERVER_REPORT_CODE:
			case SERVER_REPORT_NAME:
			case SERVER_REPORT_SYS:
				Params.m_bConsolTerm = FALSE;
				Params.m_bConsolLoc = FALSE;
				Params.m_bConsolDb = FALSE;
				Params.m_bConsolSys = TRUE;
				break;

			case SERVER_REPORT_DB:
				Params.m_bConsolTerm = FALSE;
				Params.m_bConsolLoc = FALSE;
				Params.m_bConsolDb = TRUE;
				Params.m_bConsolSys = TRUE;
				break;

			case SERVER_REPORT_LOC:
			default:
				Params.m_bConsolTerm = FALSE;
				Params.m_bConsolLoc = TRUE;
				Params.m_bConsolDb = FALSE;
				Params.m_bConsolSys = TRUE;
				break;
			}
		}
		break;
	}

	if ( TRUE == m_bGraphMode )
	{
		Params.m_bConsolTerm = FALSE;
		Params.m_bConsolLoc = FALSE;
		Params.m_bConsolDb = FALSE;
		Params.m_bConsolSys = TRUE;
		m_bAllowDatabaseSupplierLines = TRUE;
		m_bAllowSystemSupplierLines = TRUE;
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

		CEposReportServerBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolServer>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

bool CEposReportServer::CheckGraph()
{
	return TRUE;
}

/**********************************************************************/

void CEposReportServer::Consolidate()
{
	m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_ITEMSERVER_SUMMARY : REPORT_TYPE_TRANSERVER_SUMMARY;

	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	int nLastSelectArrayIdx = -1;

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		if ( ( infoFile.m_nSelectArrayIdx != nLastSelectArrayIdx ) || ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) ) )
		{
			m_arrayTranServer.RemoveAll();
			nLastSelectArrayIdx = infoFile.m_nSelectArrayIdx;
		}

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( bIsPMSLocation, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );
		CString strLastSaleDate = strThisSaleDate;
		CString strLastSaleTime = "";
		CString strLastTranTime = "";

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );
		
		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			COleDateTime oleDatePMS = COleDateTime::GetCurrentTime();
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if ( m_PMSModes.CheckPMSTimeLine( bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude ) == FALSE )
					continue;
			
				csv.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csv.GetLineType();
				{
					bool bAllowType = FALSE;
					
					switch( nLineType )
					{
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						bAllowType = ( DealerFlags.GetHoltsDiscountFlag() == FALSE ); 
						bAllowType &= ( m_ReportFilters.PluFilter.GetCurrentFilterNo() == 0 );
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
						bAllowType = ( DealerFlags.GetHoltsDiscountFlag() == FALSE ); 
						break;

					case CSVPLUDATA_PLU_SALE:
						bAllowType = TRUE;
						break;

					case CSVPLUDATA_PLU_CHEAPESTFREE:
					default:
						bAllowType = FALSE;
						break;
					}

					if ( FALSE == bAllowType )
						continue;
				}
		
				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() == TRUE)
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
						if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
							continue;
	
				//CHECK SALE DATE AND TIME
				if ( CheckPluSaleDateTime( bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime ) == FALSE )
					continue;

				if ( ( TRUE == bIsPMSLocation ) && ( strThisSaleDate != strLastSaleDate ) )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastSaleDate = strThisSaleDate;
				}

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				double dMultiplier = 1.0;

				switch( nLineType )
				{
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					{	
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );

						if ( FALSE == infoPluNo.m_bValid )
							continue;

						if ( m_ReportFilters.CheckPlu( infoPluNo ) == FALSE )
							continue;

						if ( infoPluNo.m_nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );
					}
					break;
				}

				CEposReportConsolServer ConsolInfo;
				ConsolInfo.m_nServerNo = ( m_bItemServer ) ? csv.GetItemServerNo() : csv.GetTranServerNo();

				int nTransactionsForServer = 0;
				int nTransactionsForBlockTotal = 0;
				CString strThisTranTime = csv.GetTranTime();
				if ( ( strThisTranTime != strLastTranTime ) || ( strThisSaleTime != strLastSaleTime ) )
				{
					CEposReportConsolTranServer infoTranServer;
					infoTranServer.m_strDateTran = infoFile.m_strDateTran;
					infoTranServer.m_strTimeTran = strThisTranTime;
					infoTranServer.m_nServerNo = ConsolInfo.m_nServerNo;
					
					//INCREMENT TRANSACTIONS BY SERVER FOR NEW COMBINATIONS OF TRAN DATE/TIME AND SERVER
					int nTranPos;
					if ( m_arrayTranServer.Find( infoTranServer, nTranPos ) == FALSE )
					{
						nTransactionsForServer = 1;
						m_arrayTranServer.InsertAt( nTranPos, infoTranServer );
						
						//INCREMENT BLOCK TOTAL COVERS FOR NEW TRAN DATE/TIME ONLY
						if ( strThisTranTime != strLastTranTime )
						{
							nTransactionsForBlockTotal = 1;

							for ( int x = 0; x <= 1; x++ )
							{
								int nPos = ( nTranPos - 1 ) + ( x * 2 );
								if ( ( nPos >= 0 ) && ( nPos < m_arrayTranServer.GetSize() ) )
								{
									CEposReportConsolTranServer infoTemp;
									m_arrayTranServer.GetAt( nPos, infoTemp );
								
									if ( infoTemp.TranCompare( infoTranServer ) == TRUE )
									{
										nTransactionsForBlockTotal = 0;
										break;
									}
								}
							}
						}
					}

					strLastTranTime = strThisTranTime;
					strLastSaleTime = strThisSaleTime;
				}

				switch( m_nServerReportType )
				{
				case SERVER_REPORT_CODE:
				case SERVER_REPORT_SYS:
					ConsolInfo.m_nServerLocOrDbIdx = 0;
					break;

				case SERVER_REPORT_NAME:
					ConsolInfo.m_nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( infoFile.m_nLocIdx, ConsolInfo.m_nServerNo );
					break;

				case SERVER_REPORT_DB:
					ConsolInfo.m_nServerLocOrDbIdx = infoFile.m_nDbIdx;
					break;
		
				case SERVER_REPORT_LOC:
					ConsolInfo.m_nServerLocOrDbIdx = infoFile.m_nLocIdx;
					break;
				}

				ConsolInfo.m_nTaxDateCode = nTaxDateCode;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = 0;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = GetDatabaseTaxBand( csv, nLineType );

				if ( nLineType == CSVPLUDATA_PLU_SALE )
				{
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
				}
				else
				{
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
				}

				for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
				{
					int nBlock = m_arrayTerminalBlocks.GetAt(n);
					ConsolInfo.m_nTranCount = nTransactionsForServer; 
					m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
					ConsolInfo.m_nTranCount = nTransactionsForBlockTotal; 
					m_arrayReportBlocks[ nBlock ].m_BlockTotal.Add( ConsolInfo );
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportServer::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Server", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Transactions", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "Value", TA_RIGHT, 300 );

	if ( TRUE == m_bDiscountDetail )
	{
		m_ReportFile.AddColumn ( "Discount", TA_RIGHT, 300 );

		if (TRUE == m_bPremiumDetail)
		{
			m_ReportFile.AddColumn("Premium", TA_RIGHT, 300);
		}

		m_ReportFile.AddColumn ( "Retail", TA_RIGHT, 300 );
	}

	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 300 );

	if ( TRUE == m_bNetOfTax )
		m_ReportFile.AddColumn ( "Est. Net", TA_RIGHT, 300 );

	m_ReportFile.AddColumn ( "Average", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "%Sale", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "%Total", TA_RIGHT, 300 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportServerBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportServerBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ( ReportBlock.GotData() == FALSE ) && ( NODE_SYSTEM != ReportBlock.m_nEntityType ) )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		if ( ( NODE_SYSTEM != ReportBlock.m_nEntityType ) || ( TRUE == m_bAllowSystemSupplierLines ) )
			ReportBlock.GetHeaderText( strHeader );
		else
			strHeader = "<..>Grand Total";

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

void CEposReportServer::PostConsolidateSalesTax( CEposReportServerBlock& Block, CArray<CEposReportConsolServerTax,CEposReportConsolServerTax>& arraySales )
{
	arraySales.RemoveAll();

	CEposReportConsolServerTax infoSales;
	infoSales.m_nServerLocOrDbIdx = -1;
	infoSales.m_nServerNo = -1;

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolServer Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( ( Sales.m_nServerLocOrDbIdx != infoSales.m_nServerLocOrDbIdx ) || ( Sales.m_nServerNo != infoSales.m_nServerNo ) )
		{
			if ( infoSales.m_nServerLocOrDbIdx != -1 )
				arraySales.Add( infoSales );
			
			infoSales.Reset();
			infoSales.m_nServerLocOrDbIdx = Sales.m_nServerLocOrDbIdx;
			infoSales.m_nServerNo = Sales.m_nServerNo;
		}

		infoSales.m_nTranCount += Sales.m_nTranCount;
		infoSales.m_dValBeforeDiscount += Sales.m_dValBeforeDiscount;
		infoSales.m_dDiscountVal += Sales.m_dDiscountVal;
		infoSales.m_dPremiumVal += Sales.m_dPremiumVal;

		if ( Sales.m_nTaxBand != 0 )
		{
			double dNet = Sales.m_dValBeforeDiscount - Sales.m_dDiscountVal + Sales.m_dPremiumVal;
			double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( Sales.m_nTaxDateCode, Sales.m_nTaxBand );
			double dTaxAmount = ( dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate ) );
			infoSales.m_dTaxAmount += dTaxAmount;
			Block.m_BlockTotal.m_dTaxAmount += dTaxAmount;
		}
	}

	if ( infoSales.m_nServerLocOrDbIdx != -1 )
		arraySales.Add( infoSales );
	
}

/**********************************************************************/

void CEposReportServer::GetServerName( CEposReportConsolServerTax& Sales, CString& strServerName )
{
	strServerName = "";
	switch ( m_nServerReportType )
	{
	case SERVER_REPORT_CODE:
		strServerName.Format( "S%4.4d, Server %d",
			Sales.m_nServerNo,
			Sales.m_nServerNo );
		break;

	case SERVER_REPORT_NAME:
		strServerName.Format( "S%4.4d, %s",
			Sales.m_nServerNo,
			DataManagerNonDb.ServerNameTable.GetSortedName(Sales.m_nServerLocOrDbIdx) );
		break;

	case SERVER_REPORT_SYS:
		{
			CServerSelectionInfo infoServer;
			DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, 0, 0, 0, Sales.m_nServerNo );

			strServerName.Format ( "S%4.4d, %s",
				Sales.m_nServerNo,
				infoServer.m_strServerName );
		}
		break;

	case SERVER_REPORT_DB:
		{
			int nDbIdx = Sales.m_nServerLocOrDbIdx;
			int nDbNo = dbDatabase.GetDbNo( nDbIdx );

			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, nDbNo, 0, 0, Sales.m_nServerNo );

			strServerName.Format( "S%4.4d, %s",
				Sales.m_nServerNo,
				info.m_strServerName );
		}
		break;

	case SERVER_REPORT_LOC:
		{
			int nLocIdx = Sales.m_nServerLocOrDbIdx;

			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
			int nLocNo = dbLocation.GetLocNo( nLocIdx );

			CServerSelectionInfo info;
			DataManagerNonDb.ServerNameTable.GetServerStatus( info, nDbNo, nNwkNo, nLocNo, Sales.m_nServerNo );

			strServerName.Format( "S%4.4d, %s",
				Sales.m_nServerNo,
				info.m_strServerName );
		}
		break;
	}
}

/**********************************************************************/

void CEposReportServer::CreateSalesSection( CEposReportServerBlock& Block )
{
	CArray<CEposReportConsolServerTax,CEposReportConsolServerTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );
		
	bool bShowSupplierLines = TRUE;

	switch( Block.m_nEntityType )
	{
	case NODE_SYSTEM:	bShowSupplierLines = m_bAllowSystemSupplierLines;	break;
	case NODE_DATABASE:	bShowSupplierLines = m_bAllowDatabaseSupplierLines;	break;
	}

	if ( TRUE == bShowSupplierLines )
	{
		double dTotalAmount = Block.m_BlockTotal.m_dValBeforeDiscount - Block.m_BlockTotal.m_dDiscountVal + Block.m_BlockTotal.m_dPremiumVal;
		
		if (TRUE == m_bNetOfTax)
		{
			dTotalAmount -= Block.m_BlockTotal.m_dTaxAmount;
		}

		double dPercentTotal = 0.0;

		int nDPVal = SysInfo.GetDPValue();

		for ( int s = 0; s < arraySales.GetSize(); s++ )
		{
			CEposReportConsolServerTax Sales = arraySales.GetAt(s);
			
			CString strServer = "";
			GetServerName( Sales, strServer );

			double dGross = Sales.m_dValBeforeDiscount;
			double dDiscount = Sales.m_dDiscountVal;
			double dPremium = Sales.m_dPremiumVal;
			double dRetail = dGross - dDiscount + dPremium;
			double dTax = Sales.m_dTaxAmount;
			double dNET = dRetail - dTax;

			double dLineAmount = ( m_bNetOfTax ) ? dNET : dRetail;

			double dPercentSale = 0.0;
		
			if (dTotalAmount != 0.0)
			{
				dPercentSale = (dLineAmount / dTotalAmount) * 100.0;
			}

			dPercentTotal += dPercentSale;

			CCSV csvOut ( '\t' );
			csvOut.Add( strServer );
			csvOut.Add( Sales.m_nTranCount );

			if ( TRUE == m_bDiscountDetail )
			{
				csvOut.Add ( dGross, nDPVal );
				csvOut.Add ( dDiscount, nDPVal );

				if (TRUE == m_bPremiumDetail)
				{
					csvOut.Add( dPremium, nDPVal);
				}
			}
			
			csvOut.Add ( dRetail, nDPVal );
			csvOut.Add ( dTax, nDPVal );

			if (TRUE == m_bNetOfTax)
			{
				csvOut.Add(dNET, nDPVal);
			}

			csvOut.Add ( ReportHelpers.CalcAverage ( Sales.m_nTranCount, dLineAmount ), nDPVal );
			csvOut.Add( dPercentSale, 2 );
			csvOut.Add( dPercentTotal, 2 );

			m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

bool CEposReportServer::CreateGraph()
{
	CSSFile fileGraph;
	if (fileGraph.Open(Super.ReportFilename(), "wb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvOut;
	csvOut.Add( "Server" );
	csvOut.Add( "Sales Value" );
	csvOut.Add ( GetReportNameInternal( m_nReportType ) );
	csvOut.Add( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );

	CEposReportServerBlock Block = m_arrayReportBlocks.GetAt( 0 );

	CArray<CEposReportConsolServerTax,CEposReportConsolServerTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );
	
	for ( int i = 0 ; i < arraySales.GetSize() ; i++ )
	{
		CEposReportConsolServerTax Sales = arraySales.GetAt(i);
			
		double dGross = Sales.m_dValBeforeDiscount;
		double dDiscount = Sales.m_dDiscountVal;
		double dPremium = Sales.m_dPremiumVal;
		double dRetail = dGross - dDiscount + dPremium;
		double dTax = Sales.m_dTaxAmount;
		double dNET = dRetail - dTax;

		double dLineAmount = ( m_bNetOfTax ) ? dNET : dRetail;

		CString strServerName;
		GetServerName( Sales, strServerName );

		CString strServerNo = "";

		switch( m_nServerReportType )
		{
		case SERVER_REPORT_DB:
			{
				CString strServer;
				strServerNo.Format( "%s, Server %d",
					dbDatabase.GetName( Sales.m_nServerLocOrDbIdx ),
					Sales.m_nServerNo );
			}
			break;

		case SERVER_REPORT_LOC:
			{
				CString strServer;
				strServerNo.Format( "%s, Server %d",
					dbLocation.GetName( Sales.m_nServerLocOrDbIdx ),
					Sales.m_nServerNo );
			}
			break;

		default:
			strServerNo.Format( "Server %d", Sales.m_nServerNo );
			break;
		}

		CCSV csvOut;
		csvOut.Add ( strServerName );
		csvOut.Add ( dLineAmount, SysInfo.GetDPValue() );
		csvOut.Add ( strServerNo );

		fileGraph.WriteLine( csvOut.GetLine() );
	}
	return TRUE;
}

/**********************************************************************/

void CEposReportServer::WriteBlockTotals( CEposReportServerBlock& Block )
{
	bool bShowUnderlines = TRUE;

	switch( Block.m_nEntityType )
	{
	case NODE_SYSTEM:	bShowUnderlines = m_bAllowSystemSupplierLines;		break;
	case NODE_DATABASE:	bShowUnderlines = m_bAllowDatabaseSupplierLines;	break;
	}

	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType, FALSE == m_bAllowSystemSupplierLines );
	WriteConsolidatedTotals( Block.m_BlockTotal, strType, bShowUnderlines );
}

/**********************************************************************/

void CEposReportServer::WriteConsolidatedTotals(CEposReportConsolServerTax& Totals, const char* szType, bool bShowUnderlines)
{
	int nDPVal = SysInfo.GetDPValue();

	if (TRUE == bShowUnderlines)
	{
		CString strLine = "\t<LI>\t<LI>\t<LI>\t<LI>";
		if (TRUE == m_bDiscountDetail) strLine += "\t<LI>\t<LI>";
		if (TRUE == m_bPremiumDetail) strLine += "\t<LI>";
		if (TRUE == m_bNetOfTax) strLine += "\t<LI>";
		m_ReportFile.WriteReportMiscLine(strLine);
	}

	double dGross = Totals.m_dValBeforeDiscount;
	double dDiscount = Totals.m_dDiscountVal;
	double dPremium = Totals.m_dPremiumVal;
	double dRetail = dGross - dDiscount + dPremium;
	double dTax = Totals.m_dTaxAmount;
	double dNET = dRetail - dTax;

	double dLineAmount = (m_bNetOfTax) ? dNET : dRetail;

	CString strType = szType;
	if (strType != "")
		strType += " Total";

	CCSV csvOut('\t');
	csvOut.Add(strType);
	csvOut.Add(Totals.m_nTranCount);

	if (TRUE == m_bDiscountDetail)
	{
		csvOut.Add(dGross, nDPVal);
		csvOut.Add(dDiscount, nDPVal);

		if (TRUE == m_bPremiumDetail)
		{
			csvOut.Add(dPremium, nDPVal);
		}
	}

	csvOut.Add(dRetail, nDPVal);
	csvOut.Add(dTax, nDPVal);

	if (TRUE == m_bNetOfTax)
	{
		csvOut.Add(dNET, nDPVal);
	}

	csvOut.Add(ReportHelpers.CalcAverage(Totals.m_nTranCount, dLineAmount), nDPVal);

	m_ReportFile.WriteReportMiscLine(csvOut.GetLine());
}

/**********************************************************************/
