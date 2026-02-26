/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCoversHourly.h"
/**********************************************************************/

CEposReportCoversHourly::CEposReportCoversHourly( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_COVERS_HOURLY;
	m_nTimeSliceType = SH_TIMESLICE_60MIN;
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailCoversFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailCovers();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxCoversFlag();
}

/**********************************************************************/

CEposReportCoversHourly::CEposReportCoversHourly( CEposSelectArray& SelectArray, const char* szCustomSettings ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_COVERS_HOURLY;
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailCoversFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailCovers();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxCoversFlag();
	LoadTimeSliceSettings( szCustomSettings, m_ReportSettings, m_nTimeSliceType, m_TimeMap );
}

/**********************************************************************/

CEposReportCoversHourly::~CEposReportCoversHourly()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportCoversHourly::InitialiseConsolidationBlocks()
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

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}

	if ( ( TRUE == Params.m_bConsolSys ) && ( NODE_SYSTEM != m_nConLevel ) )
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

		CEposReportCoversHourlyBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolCoversHourly>;
		block.m_nCoversCount = 0;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

bool CEposReportCoversHourly::CheckGraph()
{
	return TRUE;
}

/**********************************************************************/

void CEposReportCoversHourly::Consolidate()
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
			m_arrayTranTimeCode.RemoveAll();
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
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					break;

				case CSVPLUDATA_PLU_CHEAPESTFREE:
				default:
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

				int nLineCovers = csv.GetCovers();
				if ( 0 == nLineCovers ) 
				{
					if ( DealerFlags.GetAssumeCoversFlag() == TRUE )
						nLineCovers = 1;
					else
						continue;
				}

				int nDayShift = 0;
				CString strTimeCode = "";
				GetTimeSliceTimeCode( strThisSaleTime, strSODLabel, m_nTimeSliceType, m_TimeMap, strTimeCode, nDayShift ); 
				
				int nCoversForTimeCode = 0;
				int nCoversForBlockTotal = 0;
				CString strThisTranTime = csv.GetTranTime();
				if ( ( strThisTranTime != strLastTranTime ) || ( strThisSaleTime != strLastSaleTime ) )
				{
					CEposReportConsolTranTimeCode infoTranTimeCode;
					infoTranTimeCode.m_strDateTran = infoFile.m_strDateTran;
					infoTranTimeCode.m_strTimeTran = strThisTranTime;
					infoTranTimeCode.m_strTimeCode = strTimeCode;
					infoTranTimeCode.m_nDayShift = nDayShift;

					//INCREMENT COVERS BY TIME CODE FOR NEW COMBINATIONS OF TRAN DATE/TIME AND TIME CODE
					int nCoversPos;
					if ( m_arrayTranTimeCode.Find( infoTranTimeCode, nCoversPos ) == FALSE )
					{
						nCoversForTimeCode = nLineCovers;
						m_arrayTranTimeCode.InsertAt( nCoversPos, infoTranTimeCode );
						
						//INCREMENT BLOCK TOTAL COVERS FOR NEW TRAN DATE/TIME ONLY
						if ( strThisTranTime != strLastTranTime )
						{
							nCoversForBlockTotal = nCoversForTimeCode;

							for ( int x = 0; x <= 1; x++ )
							{
								int nPos = ( nCoversPos - 1 ) + ( x * 2 );
								if ( ( nPos >= 0 ) && ( nPos < m_arrayTranTimeCode.GetSize() ) )
								{
									CEposReportConsolTranTimeCode infoTemp;
									m_arrayTranTimeCode.GetAt( nPos, infoTemp );
								
									if ( infoTemp.TranCompare( infoTranTimeCode ) == TRUE )
									{
										nCoversForBlockTotal = 0;
										break;
									}
								}
							}
						}
					}

					strLastTranTime = strThisTranTime;
					strLastSaleTime = strThisSaleTime;
				}

				CEposReportConsolCoversHourly ConsolInfo;
				ConsolInfo.m_nDayShift = nDayShift;
				ConsolInfo.m_strTimeCode = strTimeCode;
				ConsolInfo.m_nCoversCount = nCoversForTimeCode;
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
					m_arrayReportBlocks[ nBlock ].m_nCoversCount += nCoversForBlockTotal;
					m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
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

bool CEposReportCoversHourly::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Total Spend", TA_RIGHT, 300 );

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

	m_ReportFile.AddColumn ( "Covers", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "Per Head", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "%Sale", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "%Total", TA_RIGHT, 300 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportCoversHourlyBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
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
		CEposReportCoversHourlyBlock ReportBlock = m_arrayReportBlocks.GetAt( nSortIdx );

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

void CEposReportCoversHourly::PostConsolidateSalesTax( CEposReportCoversHourlyBlock& Block, CArray<CEposReportConsolCoversHourlyTax,CEposReportConsolCoversHourlyTax>& arraySales )
{
	arraySales.RemoveAll();

	CEposReportConsolCoversHourlyTax infoSales;
	infoSales.m_strTimeCode = "";
	infoSales.m_nDayShift = -1;

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolCoversHourly Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( ( Sales.m_nDayShift != infoSales.m_nDayShift ) || ( Sales.m_strTimeCode != infoSales.m_strTimeCode ) )
		{
			if ( infoSales.m_nDayShift != -1 )
				arraySales.Add( infoSales );
			
			infoSales.Reset();
			infoSales.m_nDayShift = Sales.m_nDayShift;
			infoSales.m_strTimeCode = Sales.m_strTimeCode;
		}

		infoSales.m_nCoversCount += Sales.m_nCoversCount;
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

	if ( infoSales.m_nDayShift != -1 )
		arraySales.Add( infoSales );
	
}

/**********************************************************************/

void CEposReportCoversHourly::CreateSalesSection( CEposReportCoversHourlyBlock& Block )
{
	CArray<CEposReportConsolCoversHourlyTax,CEposReportConsolCoversHourlyTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );

	int nDPVal= SysInfo.GetDPValue();
	double dTotalSpend = Block.m_BlockTotal.m_dValBeforeDiscount - Block.m_BlockTotal.m_dDiscountVal + Block.m_BlockTotal.m_dPremiumVal;
	double dSpendSoFar = 0.0;

	if ( TRUE == m_bNetOfTax )
		dTotalSpend -= Block.m_BlockTotal.m_dTaxAmount;

	for ( int s = 0; s < arraySales.GetSize(); s++ )
	{
		CEposReportConsolCoversHourlyTax Sales = arraySales.GetAt(s);

		double dGross = Sales.m_dValBeforeDiscount;
		double dDiscount = Sales.m_dDiscountVal;
		double dPremium = Sales.m_dPremiumVal;
		double dRetail = dGross - dDiscount + dPremium;
		double dTax = Sales.m_dTaxAmount;
		double dNET = dRetail - dTax;

		double dLineSpend = ( m_bNetOfTax ) ? dNET : dRetail;

		CCSV csv ( '\t' );
		csv.Add ( GetTimeSliceString( m_nTimeSliceType, m_TimeMap, Sales.m_strTimeCode ) );

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
		
		csv.Add ( Sales.m_nCoversCount );
		csv.Add ( ReportHelpers.CalcAverage ( Sales.m_nCoversCount, dLineSpend ), nDPVal );
		
		double dPercentSale = 0.0;
		double dPercentSaleSoFar = 0.0;

		dSpendSoFar += dLineSpend;
	
		if ( dTotalSpend != 0.0 )
		{
			dPercentSale = ( dLineSpend / dTotalSpend ) * 100.0;
			dPercentSaleSoFar = ( dSpendSoFar / dTotalSpend ) * 100.0;
		}

		csv.Add ( dPercentSale, 2 );
		csv.Add ( dPercentSaleSoFar, 2 );
		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
}

/**********************************************************************/

bool CEposReportCoversHourly::CreateGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return FALSE;

	CCSV csvOut;
	csvOut.Add ( "Time" );
	csvOut.Add ( "Sales Value" );
	csvOut.Add ( GetReportNameInternal( m_nReportType ) );
	csvOut.Add ( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );

	CString strSODLabel;
	strSODLabel.Format ( "%2.2d", EcrmanOptions.GetReportsDayStartHour() );

	CEposReportCoversHourlyBlock ReportBlock = m_arrayReportBlocks.GetAt( 0 );
	CArray<CEposReportConsolCoversHourlyTax,CEposReportConsolCoversHourlyTax> arraySales;
	PostConsolidateSalesTax( ReportBlock, arraySales );

	int nStartPos = 0;
	int nSize = arraySales.GetSize();
	
	for ( int i = 0; i < nSize; i++ )
	{
		CEposReportConsolCoversHourlyTax ConsolInfo = arraySales.GetAt(i);
		
		if ( strSODLabel <= ConsolInfo.m_strTimeCode )
		{
			nStartPos = i;
			break;
		}
	}

	for ( int i = 0 ; i < nSize ; i++ )
	{
		int nPos = ( nStartPos + i ) % nSize;

		CEposReportConsolCoversHourlyTax ConsolInfo = arraySales.GetAt(nPos);

		CCSV csvOut;
		csvOut.Add ( GetTimeSliceString( m_nTimeSliceType, m_TimeMap, ConsolInfo.m_strTimeCode ) );
		csvOut.Add ( ConsolInfo.m_dValBeforeDiscount - ConsolInfo.m_dDiscountVal + ConsolInfo.m_dPremiumVal, SysInfo.GetDPValue() );
		fileGraph.WriteLine( csvOut.GetLine() );
	}
	return TRUE;
}

/**********************************************************************/

void CEposReportCoversHourly::WriteBlockTotals( CEposReportCoversHourlyBlock& Block )
{
	CString strType = GetTotalTypeName( TRUE, Block.m_nEntityType, FALSE, TRUE );
	
	int nDPVal= SysInfo.GetDPValue();

	double dGross = Block.m_BlockTotal.m_dValBeforeDiscount;
	double dDiscount = Block.m_BlockTotal.m_dDiscountVal;
	double dPremium = Block.m_BlockTotal.m_dPremiumVal;
	double dRetail = dGross - dDiscount + dPremium;
	double dTax = Block.m_BlockTotal.m_dTaxAmount;
	double dNET = dRetail - dTax;

	double dTotalSpend = ( m_bNetOfTax ) ? dNET : dRetail;

	{
		int nTotals = 4;
		if ( TRUE == m_bDiscountDetail ) nTotals += 2;
		if ( TRUE == m_bPremiumDetail) nTotals += 1;
		if ( TRUE == m_bNetOfTax ) nTotals += 1;

		CString strUnderline = "";
		for (int n = 0; n < nTotals; n++)
		{
			strUnderline += "\t<LI>";
		}

		m_ReportFile.WriteReportMiscLine( strUnderline );
	}

	CCSV csv ( '\t' );
	csv.Add ( strType );
		
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

	if (TRUE == m_bNetOfTax)
	{
		csv.Add(dNET, nDPVal);
	}

	csv.Add ( Block.m_nCoversCount );
	csv.Add ( ReportHelpers.CalcAverage ( Block.m_nCoversCount, dTotalSpend ), nDPVal );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
