/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "PMSReportFileFinder.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPluVipDeptSummary.h"
/**********************************************************************/

CEposReportPluVipDeptSummary::CEposReportPluVipDeptSummary( CEposSelectArray& SelectArray, int nReportType ) : CEposReport( SelectArray )
{
	switch( nReportType )
	{
	case REPORT_TYPE_PLUWASTE_DEPT_SUMMARY:
		m_nReportType = nReportType;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = FALSE;
		m_bPremiumDetail = FALSE;
		m_bVIPReport = FALSE;
		break;

	case REPORT_TYPE_VIPSALES_DEPT_SUMMARY:
		m_nReportType = nReportType;
		m_bAllowValue = EcrmanOptions.GetReportsVIPRetailFlag();
		m_bDiscountDetail = m_bAllowValue && EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = m_bAllowValue && EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = TRUE;
		break;

	case REPORT_TYPE_PLUSALES_DEPT_SUMMARY:
	default:
		m_nReportType = REPORT_TYPE_PLUSALES_DEPT_ITEM;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;
		break;
	}

	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

CEposReportPluVipDeptSummary::~CEposReportPluVipDeptSummary()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportPluVipDeptSummary::InitialiseConsolidationBlocks()
{
	m_bAllowGlobal = ( ( NODE_SYSTEM == m_nConLevel ) && ( EcrmanOptions.GetGlobalDepartmentFlag() == TRUE ) ); 

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
	
	if ( ( NODE_SYSTEM != m_nConLevel ) && ( TRUE == Params.m_bConsolSys ) )
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

		CEposReportDeptSummaryBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolDeptSummary>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

bool CEposReportPluVipDeptSummary::CheckGraph()
{
	if ( ( EcrmanOptions.GetGlobalDepartmentFlag() == FALSE ) && ( NODE_SYSTEM == m_nConLevel ) )
	{
		Prompter.Error( "You cannot create a graph of this report across multiple databases\n\nwhen the department texts are database specific." );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CEposReportPluVipDeptSummary::Consolidate()
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
						bAllowType &= ( 0 == m_ReportFilters.PluFilter.GetCurrentFilterNo() );
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
				if (csv.GetPluSaleWastageFlag() != IsWastageReport())
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

				//FILTER FOR VIP SALE
				if ( TRUE == m_bVIPReport )
				{
					if ( csv.IsVIPSale() == FALSE )
						continue;
				}
	
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

				CEposReportConsolDeptSummary ConsolInfo;
				ConsolInfo.m_nDeptNo = nDeptNo;

				switch( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_dQty = 0.0;
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_dQty = csv.GetSaleQty();
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;

				case CSVPLUDATA_PLU_SALE:
					ConsolInfo.m_dQty = csv.GetSaleQty() * dMultiplier;
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
					break;
				}

				ConsolInfo.m_nTaxDateCode = nTaxDateCode;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = 0;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = GetDatabaseTaxBand( csv, nLineType );

				for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
				{
					int nBlock = m_arrayTerminalBlocks.GetAt(n);
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

bool CEposReportPluVipDeptSummary::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	AddReportColumn ( "Dept No", TA_LEFT, 200, FALSE );
	AddReportColumn ( "Description", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Quantity", TA_RIGHT, 300, TRUE );

	if ( TRUE == m_bAllowValue )
	{
		if ( TRUE == IsWastageReport() )
			AddReportColumn ( "Waste", TA_RIGHT, 300, TRUE );
		else
		{
			AddReportColumn ( "Sales", TA_RIGHT, 300, TRUE );

			if ( TRUE == m_bDiscountDetail )
			{
				AddReportColumn ( "Discount", TA_RIGHT, 250, TRUE );

				if (TRUE == m_bPremiumDetail)
				{
					AddReportColumn("Premium", TA_RIGHT, 250, TRUE);
				}

				AddReportColumn ( "Retail", TA_RIGHT, 250, TRUE );
			}

			AddReportColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 300, TRUE );

			if ( TRUE == m_bNetOfTax )
				AddReportColumn ( "Est. Net", TA_RIGHT, 300, TRUE );
		}

		AddReportColumn ( "Average", TA_RIGHT, 300, TRUE );
		AddReportColumn ( "%Sale", TA_RIGHT, 300, FALSE  );
		AddReportColumn ( "%Total", TA_RIGHT, 300, FALSE );
	}

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportDeptSummaryBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
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
		CEposReportDeptSummaryBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ( ReportBlock.GotData() == FALSE ) && ( NODE_SYSTEM != ReportBlock.m_nEntityType ) )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		if ( ( NODE_SYSTEM != ReportBlock.m_nEntityType ) || ( TRUE == m_bAllowGlobal ) )
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

void CEposReportPluVipDeptSummary::PostConsolidateSalesTax( CEposReportDeptSummaryBlock& Block, CArray<CEposReportConsolDeptSummaryTax,CEposReportConsolDeptSummaryTax>& arraySales )
{
	arraySales.RemoveAll();

	CEposReportConsolDeptSummaryTax infoSales;
	infoSales.m_nDeptNo = -1;
	
	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolDeptSummary Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( Sales.m_nDeptNo != infoSales.m_nDeptNo )
		{
			if ( infoSales.m_nDeptNo != -1 )
				arraySales.Add( infoSales );
			
			infoSales.Reset();
			infoSales.m_nDeptNo = Sales.m_nDeptNo;
		}		

		infoSales.m_dQty += Sales.m_dQty;
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

	if ( infoSales.m_nDeptNo != -1 )
		arraySales.Add( infoSales );
}

/**********************************************************************/

void CEposReportPluVipDeptSummary::CreateSalesSection( CEposReportDeptSummaryBlock& Block )
{
	CArray<CEposReportConsolDeptSummaryTax,CEposReportConsolDeptSummaryTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );
	
	if ( ( NODE_SYSTEM != Block.m_nEntityType ) || ( TRUE == m_bAllowGlobal ) )
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
			CEposReportConsolDeptSummaryTax Sales = arraySales.GetAt(s);
	
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
			
			CString strNum;
			strNum.Format ( "D%3.3d", Sales.m_nDeptNo );

			CCSV csvOut ( '\t' );
			csvOut.Add( strNum );
			csvOut.Add( DataManager.Department.GetReportTextByDeptNo( Sales.m_nDeptNo ) );
			csvOut.Add( Sales.m_dQty, SysInfo.GetDPQty() );

			if ( TRUE == m_bAllowValue )
			{
				if ( TRUE == m_bDiscountDetail )
				{
					csvOut.Add ( dGross, nDPVal );
					csvOut.Add ( dDiscount, nDPVal );

					if (TRUE == m_bPremiumDetail)
					{
						csvOut.Add(dPremium, nDPVal);
					}
				}
				
				csvOut.Add ( dRetail, nDPVal );
				csvOut.Add ( dTax, nDPVal );

				if (TRUE == m_bNetOfTax)
				{
					csvOut.Add(dNET, nDPVal);
				}

				csvOut.Add ( ReportHelpers.CalcAverage ( Sales.m_dQty, dLineAmount ), nDPVal );
				csvOut.Add( dPercentSale, 2 );
				csvOut.Add( dPercentTotal, 2 );
			}
			
			m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

bool CEposReportPluVipDeptSummary::CreateGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return FALSE;

	CCSV csvOut;
	csvOut.Add ( "Dept No" );

	if ( TRUE == IsWastageReport() )
		csvOut.Add ( "Waste Value" );
	else
		csvOut.Add ( "Sales Value" );

	csvOut.Add ( GetReportNameInternal( m_nReportType ) );
	csvOut.Add ( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );

	CEposReportDeptSummaryBlock Block = m_arrayReportBlocks.GetAt( 0 );

	CArray<CEposReportConsolDeptSummaryTax,CEposReportConsolDeptSummaryTax> arraySales;
	PostConsolidateSalesTax( Block, arraySales );
	
	for ( int i = 0 ; i < arraySales.GetSize() ; i++ )
	{
		CEposReportConsolDeptSummaryTax Sales = arraySales.GetAt(i);

		double dGross = Sales.m_dValBeforeDiscount;
		double dDiscount = Sales.m_dDiscountVal;
		double dPremium = Sales.m_dPremiumVal;
		double dRetail = dGross - dDiscount + dPremium;
		double dTax = Sales.m_dTaxAmount;
		double dNET = dRetail - dTax;

		double dLineAmount = ( m_bNetOfTax ) ? dNET : dRetail;

		CString strLabel;
		strLabel.Format( "%3.3d", Sales.m_nDeptNo );

		CString strLongLabel;
		strLongLabel.Format ( "%s", DataManager.Department.GetReportTextByDeptNo( Sales.m_nDeptNo ) );

		CCSV csvOut;
		csvOut.Add ( strLabel );
		csvOut.Add ( dLineAmount, SysInfo.GetDPValue() );
		csvOut.Add ( strLongLabel );
		fileGraph.WriteLine( csvOut.GetLine() );
	}
	
	return TRUE;
}

/**********************************************************************/

void CEposReportPluVipDeptSummary::WriteBlockTotals( CEposReportDeptSummaryBlock& Block )
{
	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType, FALSE == m_bAllowGlobal, FALSE );
	WriteConsolidatedTotals( Block.m_BlockTotal, strType, ( NODE_SYSTEM == Block.m_nEntityType ) );
}

/**********************************************************************/

void CEposReportPluVipDeptSummary::WriteConsolidatedTotals( CEposReportConsolDeptSummaryTax& Totals, const char* szType, bool bSystemTotal )
{
	if ( ( FALSE == bSystemTotal ) || ( TRUE == m_bAllowGlobal ) )
	{
		m_ReportFile.WriteReportMiscLine( m_UnderlineMaker.GetLine() );
	}

	int nDPVal = SysInfo.GetDPValue();

	double dGross = Totals.m_dValBeforeDiscount;
	double dDiscount = Totals.m_dDiscountVal;
	double dPremium = Totals.m_dPremiumVal;
	double dRetail = dGross - dDiscount + dPremium;
	double dTax = Totals.m_dTaxAmount;
	double dNET = dRetail - dTax;
			
	double dLineAmount = ( m_bNetOfTax ) ? dNET : dRetail;

	CString strType = szType;
	if ( strType != "" ) strType += " ";
	strType += " Total";

	CCSV csvOut ( '\t' );
	csvOut.Add ( "" );
	csvOut.Add ( strType );

	csvOut.Add( Totals.m_dQty, SysInfo.GetDPQty() );

	if ( TRUE == m_bAllowValue )
	{
		if ( TRUE == m_bDiscountDetail )
		{
			csvOut.Add ( dGross, nDPVal );
			csvOut.Add ( dDiscount, nDPVal );

			if (TRUE == m_bPremiumDetail)
			{
				csvOut.Add(dPremium, nDPVal);
			}
		}
				
		csvOut.Add ( dRetail, nDPVal );
		csvOut.Add ( dTax, nDPVal );

		if (TRUE == m_bNetOfTax)
		{
			csvOut.Add(dNET, nDPVal);
		}

		csvOut.Add ( ReportHelpers.CalcAverage ( Totals.m_dQty, dLineAmount ), nDPVal );
	}

	m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
}

/**********************************************************************/
