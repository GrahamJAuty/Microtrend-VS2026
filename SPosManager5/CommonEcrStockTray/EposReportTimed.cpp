/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "UnderlineMaker.h"
/**********************************************************************/
#include "EposReportTimed.h"
/**********************************************************************/

CEposReportTimed::CEposReportTimed( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
	m_strDateOrTimeColumn = "";
}

/**********************************************************************/

CEposReportTimed::~CEposReportTimed()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportTimed::InitialiseConsolidationBlocks()
{
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

	if ( ( NODE_SYSTEM != m_nConLevel ) && ( TRUE == Params.m_bConsolSys ) )
	{
		Params.m_bConsolSys = FALSE;
		Params.m_bConsolDb = TRUE;
	}

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
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

		CEposReportTimedBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolTimed>;
		block.m_nTranCount = 0;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

bool CEposReportTimed::CheckGraph()
{
	return TRUE;
}

/**********************************************************************/

void CEposReportTimed::Consolidate()
{
	InitialiseConsolidationBlocks();

	CString strSODLabel;
	strSODLabel.Format ( "%2.2d%2.2d", 
		EcrmanOptions.GetReportsDayStartHour(),
		EcrmanOptions.GetReportsDayStartMinute() );

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
			m_arrayTranDateOrTime.RemoveAll();
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
		CString strLastTaxDate = strThisSaleDate;
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
			
				csv.ParseLine ( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csv.GetLineType();
				{
					bool bAllowType = FALSE;
					
					switch( nLineType )
					{
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
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

				if ( ( TRUE == bIsPMSLocation ) && ( strThisSaleDate != strLastTaxDate ) )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastTaxDate = strThisSaleDate;
				}

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				CString strDateOrTimeLabel;
				if ( GetDateOrTimeLabel( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime, strDateOrTimeLabel ) == FALSE )
					continue;

				int nTransactionsForTimeCode = 0;
				int nTransactionsForBlockTotal = 0;
				CString strThisTranTime = csv.GetTranTime();
				if ( ( strThisTranTime != strLastTranTime ) || ( strThisSaleTime != strLastSaleTime ) )
				{
					CEposReportConsolTranDateOrTime infoTranTimeCode;
					infoTranTimeCode.m_strDateTran = infoFile.m_strDateTran;
					infoTranTimeCode.m_strTimeTran = strThisTranTime;
					infoTranTimeCode.m_strDateOrTime = strDateOrTimeLabel;

					//INCREMENT TRAN COUNT BY TIME CODE FOR NEW COMBINATIONS OF TRAN DATE/TIME AND TIME CODE
					int nCoversPos;
					if ( m_arrayTranDateOrTime.Find( infoTranTimeCode, nCoversPos ) == FALSE )
					{
						nTransactionsForTimeCode = 1;
						m_arrayTranDateOrTime.InsertAt( nCoversPos, infoTranTimeCode );
						
						//INCREMENT BLOCK TOTAL TRANSACTIONS FOR NEW TRAN DATE/TIME ONLY
						if ( strThisTranTime != strLastTranTime )
						{
							nTransactionsForBlockTotal = 1;

							for ( int x = 0; x <= 1; x++ )
							{
								int nPos = ( nCoversPos - 1 ) + ( x * 2 );
								if ( ( nPos >= 0 ) && ( nPos < m_arrayTranDateOrTime.GetSize() ) )
								{
									CEposReportConsolTranDateOrTime infoTemp;
									m_arrayTranDateOrTime.GetAt( nPos, infoTemp );
								
									if ( infoTemp.TranCompare( infoTranTimeCode ) == TRUE )
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

				CEposReportConsolTimed ConsolInfo;
				ConsolInfo.m_strDateOrTime = strDateOrTimeLabel;
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;
				ConsolInfo.m_nTranCount = nTransactionsForTimeCode;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = 0;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = GetDatabaseTaxBand( csv, nLineType );

				switch( nLineType )
				{
				case CSVPLUDATA_PLU_SALE:
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;
				}
				
				for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
				{
					int nBlock = m_arrayTerminalBlocks.GetAt(n);
					m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
					m_arrayReportBlocks[ nBlock ].m_BlockTotal.Add( ConsolInfo );
					m_arrayReportBlocks[ nBlock ].m_nTranCount += nTransactionsForBlockTotal;
				}

				strLastSaleTime = strThisSaleTime;
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportTimed::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( m_strDateOrTimeColumn, TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Transactions", TA_RIGHT, 300 );

	m_ReportFile.AddColumn ( "Sales", TA_RIGHT, 300 );

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
		CEposReportTimedBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
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
		CEposReportTimedBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

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

void CEposReportTimed::PostConsolidateSalesTax( CEposReportTimedBlock& Block, CArray<CEposReportConsolTimedTax,CEposReportConsolTimedTax>& arraySales )
{
	arraySales.RemoveAll();

	CEposReportConsolTimedTax infoSales;
	infoSales.m_strDateOrTime = "";

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolTimed Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( Sales.m_strDateOrTime != infoSales.m_strDateOrTime  )
		{
			if ( infoSales.m_strDateOrTime != "" )
				arraySales.Add( infoSales );
			
			infoSales.Reset();
			infoSales.m_strDateOrTime = Sales.m_strDateOrTime;
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

	if ( infoSales.m_strDateOrTime != "" )
		arraySales.Add( infoSales );
}

/**********************************************************************/

void CEposReportTimed::CreateSalesSection( CEposReportTimedBlock& Block )
{
	CArray<CEposReportConsolTimedTax,CEposReportConsolTimedTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );

	double dTotalVal = Block.m_BlockTotal.m_dValBeforeDiscount - Block.m_BlockTotal.m_dDiscountVal + Block.m_BlockTotal.m_dPremiumVal;
	
	if ( TRUE == m_bNetOfTax )
		dTotalVal -= Block.m_BlockTotal.m_dTaxAmount;
	
	double dTotalSoFar = 0.0;

	int nDPVal = SysInfo.GetDPValue();

	for ( int s = 0; s < arraySales.GetSize(); s++ )
	{
		CEposReportConsolTimedTax Sales = arraySales.GetAt( s );

		double dGross = Sales.m_dValBeforeDiscount;
		double dDiscount = Sales.m_dDiscountVal;
		double dPremium = Sales.m_dPremiumVal;
		double dRetail = dGross - dDiscount + dPremium;
		double dTax = Sales.m_dTaxAmount;
		double dNET = dRetail - dTax;

		double dLineTotal = ( m_bNetOfTax ) ? dNET : dRetail;

		CCSV csv ( '\t' );
		csv.Add ( GetDateOrTimeReport( Sales.m_strDateOrTime ) );
		csv.Add ( Sales.m_nTranCount );

		if ( TRUE == m_bDiscountDetail )
		{
			csv.Add ( dGross, nDPVal );
			csv.Add ( dDiscount, nDPVal );

			if (TRUE == m_bPremiumDetail)
			{
				csv.Add(dPremium, nDPVal);
			}
		}
	
		csv.Add ( dRetail, nDPVal );
		csv.Add ( dTax, nDPVal );

		if ( TRUE == m_bNetOfTax )
			csv.Add ( dNET, nDPVal );

		double dPercentSale = 0.0;
		double dPercentSaleSoFar = 0.0;

		dTotalSoFar += dLineTotal;
	
		if ( dTotalVal != 0.0 )
		{
			dPercentSale = ( dLineTotal / dTotalVal ) * 100.0;
			dPercentSaleSoFar = ( dTotalSoFar / dTotalVal ) * 100.0;
		}

		csv.Add ( ReportHelpers.CalcAverage ( Sales.m_nTranCount, dLineTotal ), SysInfo.GetDPValue() );
		csv.Add ( dPercentSale, 2 );
		csv.Add ( dPercentSaleSoFar, 2 );
		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
}

/**********************************************************************/

bool CEposReportTimed::CreateGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return FALSE;

	CCSV csvOut;
	csvOut.Add ( m_strDateOrTimeColumn );
	csvOut.Add ( "Sales Value" );
	csvOut.Add ( GetReportNameInternal( m_nReportType ) );
	csvOut.Add ( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );

	CEposReportTimedBlock ReportBlock = m_arrayReportBlocks.GetAt( 0 );

	CArray<CEposReportConsolTimedTax,CEposReportConsolTimedTax> arraySales;
	PostConsolidateSalesTax( ReportBlock, arraySales );

	for ( int i = 0 ; i < arraySales.GetSize() ; i++ )
	{
		CEposReportConsolTimedTax Sales = arraySales.GetAt(i);
		
		double dGross = Sales.m_dValBeforeDiscount;
		double dDiscount = Sales.m_dDiscountVal;
		double dPremium = Sales.m_dPremiumVal;
		double dRetail = dGross - dDiscount + dPremium;
		double dTax = Sales.m_dTaxAmount;
		double dNET = dRetail - dTax;

		double dLineAmount = ( m_bNetOfTax ) ? dNET : dRetail;
				
		CCSV csvOut;
		csvOut.Add ( GetDateOrTimeReport( Sales.m_strDateOrTime ) );
		csvOut.Add ( dLineAmount, SysInfo.GetDPValue() );
		fileGraph.WriteLine( csvOut.GetLine() );
	}
	
	return TRUE;
}

/**********************************************************************/

void CEposReportTimed::WriteBlockTotals( CEposReportTimedBlock& Block )
{
	CString strType = GetTotalTypeName( TRUE, Block.m_nEntityType, FALSE, TRUE );
	
	CUnderlineMaker UnderlineMaker;
	UnderlineMaker.AddBlanks(1);
	UnderlineMaker.AddTotals(4);
	if ( TRUE == m_bDiscountDetail ) UnderlineMaker.AddTotals( m_bPremiumDetail ? 3 : 2 );
	if ( TRUE == m_bNetOfTax ) UnderlineMaker.AddTotals(1);
	m_ReportFile.WriteReportMiscLine( UnderlineMaker.GetLine() );

	double dGross = Block.m_BlockTotal.m_dValBeforeDiscount;
	double dDiscount = Block.m_BlockTotal.m_dDiscountVal;
	double dPremium = Block.m_BlockTotal.m_dPremiumVal;
	double dRetail = dGross - dDiscount + dPremium;
	double dTax = Block.m_BlockTotal.m_dTaxAmount;
	double dNET = dRetail - dTax;

	double dLineTotal = ( m_bNetOfTax ) ? dNET : dRetail;

	CCSV csv ( '\t' );
	csv.Add( strType );
	csv.Add( Block.m_nTranCount );
		
	if ( TRUE == m_bDiscountDetail )
	{	
		csv.Add ( dGross, SysInfo.GetDPValue() );
		csv.Add ( dDiscount, SysInfo.GetDPValue() );

		if (TRUE == m_bPremiumDetail)
		{
			csv.Add(dPremium, SysInfo.GetDPValue());
		}
	}
	
	csv.Add ( dRetail, SysInfo.GetDPValue() );
	csv.Add ( dTax, SysInfo.GetDPValue() );

	if (TRUE == m_bNetOfTax)
	{
		csv.Add(dNET, SysInfo.GetDPValue());
	}

	csv.Add ( ReportHelpers.CalcAverage ( Block.m_nTranCount, dLineTotal ), SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
