/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSReportFileFinder.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportSptBookCascade.h"
/**********************************************************************/

CEposReportSptBookCascade::CEposReportSptBookCascade( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_SPTBOOK_CASCADE;
}

/**********************************************************************/

CEposReportSptBookCascade::~CEposReportSptBookCascade()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportSptBookCascade::InitialiseConsolidationBlocks()
{
	bool bAllowGlobal = ( ( NODE_SYSTEM == m_nConLevel ) 
		&& ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE ) 
		&& ( EcrmanOptions.GetGlobalAnalysisCategoryFlag() == TRUE ) 
		&& ( EcrmanOptions.GetGlobalTaxFlag() == TRUE ) ); 

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

	if ( ( NODE_SYSTEM != m_nConLevel ) || ( FALSE == bAllowGlobal ) )
	{
		if ( TRUE == Params.m_bConsolSys )
		{
			Params.m_bConsolSys = FALSE;
			Params.m_bConsolDb = TRUE;
		}
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

		CEposReportSptBookCascadeBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolSptBookCascade>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportSptBookCascade::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
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

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( bIsPMSLocation, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );
		CString strLastSaleDate = strThisSaleDate;

		int nFileYear = atoi( infoFile.m_strDateTran.Left(4) );
		int nFileMonth = atoi( infoFile.m_strDateTran.Mid(4,2) );

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

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

				//CHECK FOR RELEVANT LINE TYPESS
				int nLineType = csv.GetLineType();
				
				{
					bool bAllowType = FALSE;
						
					switch( nLineType )
					{
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						bAllowType &= ( m_ReportFilters.PluFilter.GetCurrentFilterNo() == 0 );
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
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
				if ( CONNECTION_TYPE_STANDARD_NONE == nConnectionType )
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
					nFileYear = atoi( infoFile.m_strDateTran.Left(4) );
					nFileMonth = atoi( infoFile.m_strDateTran.Mid(4,2) );
				}

				//FILTER FOR DEPARTMENT
				//int nDeptNo = csv.GetDeptNo();
				//if ( m_DepartmentSet.GetDepartmentStatus ( nDeptNo ) == FALSE )
				//	continue;

				__int64 nPluNo = 0;
				int nModifier = 0;

				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					nPluNo = GetSeparatedPromoPluNo( csv.GetPromoNoSeparate() );
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					nPluNo = GetSeparatedMixMatchPluNo( csv.GetMixMatchNo() );
					break;

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
			
						nPluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;
					}
					break;
				}

				CEposReportConsolSptBookCascade ConsolInfo;
				ConsolInfo.m_nYear = nFileYear;
				ConsolInfo.m_nMonth = nFileMonth;
				
				ConsolInfo.m_nTaxBand = 0;
				if ( ( CSVPLUDATA_PROMO_NONTAXABLE != nLineType ) && ( CSVPLUDATA_MIXMATCH_NONTAXABLE != nLineType ) )
					if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
						ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				
				int nPluIdx;
				ConsolInfo.m_nCategoryNo = -1;
				ConsolInfo.m_nDeptNo = -1;
				if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
				{
					CPluCSVRecord PluRecord;
					DataManager.Plu.GetAt( nPluIdx, PluRecord );
					ConsolInfo.m_nCategoryNo = PluRecord.GetModifierAnalysisCategoryForReport( nModifier );

					if ( ( CSVPLUDATA_PROMO_NONTAXABLE != nLineType ) && ( CSVPLUDATA_MIXMATCH_NONTAXABLE != nLineType ) )
					{
						if (DealerFlags.GetUseDatabaseVATBandFlag() == TRUE)
						{
							ConsolInfo.m_nTaxBand = PluRecord.GetModifierTaxBandIntForReport( nModifier );
						}
					}

					ConsolInfo.m_nDeptNo = PluRecord.GetModifierDeptNoForReport(nModifier);
				}

				if ( m_ReportFilters.DeptFilter.GetAllModeFlag() == FALSE )
					if ( m_ReportFilters.CheckDepartment( ConsolInfo.m_nDeptNo ) == FALSE )
						continue;
				
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;
				ConsolInfo.m_dValue = csv.GetValue();

				switch ( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_dValue *= -1;
					break;
				}

				for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
				{
					int nBlock = m_arrayTerminalBlocks.GetAt(n);
					m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
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

bool CEposReportSptBookCascade::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	CString strTax = EcrmanOptions.GetSalesTaxName();
	strTax += " (£)";

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "IncomeYear", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "IncomeMonth", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "TaxCodeID", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "CostCentreDesc", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "DepartmentDesc", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Gross (£)", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( "Net (£)", TA_RIGHT, 300 );
	m_ReportFile.AddColumn ( strTax, TA_RIGHT, 300 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportSptBookCascadeBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
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
		CEposReportSptBookCascadeBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

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
		//WriteBlockTotals( ReportBlock );	
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportSptBookCascade::CreateSalesSection( CEposReportSptBookCascadeBlock& Block )
{
	int nLastTotalLevel = 5;
	CEposReportConsolSptBookCascade ConsolLastLine, ConsolDeptTotal, ConsolGroupTotal, ConsolTaxTotal, ConsolMonthTotal, ConsolYearTotal;
	
	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolSptBookCascade ConsolCurrentLine;
		Block.m_pSales -> GetAt( s, ConsolCurrentLine );
		UpdateCreateReportProgress();

		int nDifferenceLevel = ( 0 == s ) ? 0 : ConsolCurrentLine.GetDifferenceLevel( ConsolLastLine );

		if ( nDifferenceLevel != 0 )
		{
			ShowDeptTotal( nLastTotalLevel, ConsolLastLine, ConsolDeptTotal );

			ConsolGroupTotal.Add( ConsolDeptTotal );
			ConsolTaxTotal.Add( ConsolDeptTotal );
			ConsolMonthTotal.Add( ConsolDeptTotal );
			ConsolYearTotal.Add( ConsolDeptTotal );
			ConsolDeptTotal.Reset();

			if ( nDifferenceLevel >= 2 ) ShowTotal( 2, ConsolLastLine, ConsolGroupTotal );
			if ( nDifferenceLevel >= 3 ) ShowTotal( 3, ConsolLastLine, ConsolTaxTotal );
			if ( nDifferenceLevel >= 4 ) ShowTotal( 4, ConsolLastLine, ConsolMonthTotal );
			if ( nDifferenceLevel >= 5 ) ShowTotal( 5, ConsolLastLine, ConsolYearTotal );
			nLastTotalLevel = nDifferenceLevel;
		}
	
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( ConsolCurrentLine.m_nTaxDateCode, ConsolCurrentLine.m_nTaxBand );
		ConsolCurrentLine.m_dTax = ConsolCurrentLine.m_dValue - ReportHelpers.CalcNonTax ( ConsolCurrentLine.m_dValue, dTaxRate );
		ConsolDeptTotal.Add( ConsolCurrentLine );
		
		ConsolLastLine = ConsolCurrentLine;
	}

	if ( Block.m_pSales -> GetSize() != 0 )
	{
		ShowDeptTotal( nLastTotalLevel, ConsolLastLine, ConsolDeptTotal );
		ConsolGroupTotal.Add( ConsolDeptTotal );
		ConsolTaxTotal.Add( ConsolDeptTotal );
		ConsolMonthTotal.Add( ConsolDeptTotal );
		ConsolYearTotal.Add( ConsolDeptTotal );
		ShowTotal( 2, ConsolLastLine, ConsolGroupTotal );
		ShowTotal( 3, ConsolLastLine, ConsolTaxTotal );
		ShowTotal( 4, ConsolLastLine, ConsolMonthTotal );
		ShowTotal( 5, ConsolLastLine, ConsolYearTotal );
	}
}

/**********************************************************************/

void CEposReportSptBookCascade::ShowDeptTotal( int nLastTotalLevel, CEposReportConsolSptBookCascade& ConsolFields, CEposReportConsolSptBookCascade& ConsolValue )
{
	CCSV csv ( '\t' );
	csv.Add( ( nLastTotalLevel >= 5 ) ? ConsolFields.GetYearText(FALSE) : "" );
	csv.Add( ( nLastTotalLevel >= 4 ) ? ConsolFields.GetMonthText(FALSE) : "" );
	csv.Add( ( nLastTotalLevel >= 3 ) ? ConsolFields.GetTaxBandText(FALSE) : "" );
	csv.Add( ( nLastTotalLevel >= 2 ) ? ConsolFields.GetCategoryText(FALSE) : "" );
	csv.Add( ConsolFields.GetDepartmentText() );

	CString strVal;
	strVal.Format( "£%.2f", ConsolValue.m_dValue );
	csv.Add( strVal );
	strVal.Format( "£%.2f", ConsolValue.m_dValue - ConsolValue.m_dTax );
	csv.Add( strVal );
	strVal.Format( "£%.2f", ConsolValue.m_dTax );
	csv.Add( strVal );

	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportSptBookCascade::ShowTotal( int nLevel, CEposReportConsolSptBookCascade& ConsolFields, CEposReportConsolSptBookCascade& ConsolValue )
{
	CCSV csv ( '\t' );
	csv.Add( ( 5 == nLevel ) ? ConsolFields.GetYearText(TRUE) : "" );
	csv.Add( ( 4 == nLevel ) ? ConsolFields.GetMonthText(TRUE) : "" );
	csv.Add( ( 3 == nLevel ) ? ConsolFields.GetTaxBandText(TRUE) : "" );
	csv.Add( ( 2 == nLevel ) ? ConsolFields.GetCategoryText(TRUE) : "" );
	csv.Add( "" );
	
	CString strVal;
	strVal.Format( "£%.2f", ConsolValue.m_dValue );
	csv.Add( strVal );
	strVal.Format( "£%.2f", ConsolValue.m_dValue - ConsolValue.m_dTax );
	csv.Add( strVal );
	strVal.Format( "£%.2f", ConsolValue.m_dTax );
	csv.Add( strVal );

	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	ConsolValue.Reset();
}

/**********************************************************************/
