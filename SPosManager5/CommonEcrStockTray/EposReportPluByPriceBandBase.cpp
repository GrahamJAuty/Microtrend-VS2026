/**********************************************************************/
#include "CCSVReportLine.h"
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PluSalesLine.h"
#include "PMSReportFileFinder.h"
#include "ReportCostFinder.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "SportsBookerPriceBandCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "UnderlineMaker.h"
/**********************************************************************/
#include "EposReportPluByPriceBandBase.h"
/**********************************************************************/

CEposReportPluByPriceBandBase::CEposReportPluByPriceBandBase(CEposSelectArray& SelectArray, CString strCustomSettings) : CEposReport(SelectArray)
{
	m_bDiscountDetail = FALSE;
	m_bPremiumDetail = FALSE;
	m_bNetOfTax = FALSE;
	m_bVIPReport = FALSE;
	m_nBlankColumnsBeforeDate = 1;
	m_bAllowValue = TRUE;
	m_nServerBlockIdxStart = 0;
	m_nTotalCovers = 0;
	m_nPreferConsolType = EPOSREPORT_CONSOL_SYSTEM;
	m_ReportSettings.SetSettingsCSVLine(strCustomSettings);
}

/**********************************************************************/

CEposReportPluByPriceBandBase::~CEposReportPluByPriceBandBase()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
		delete ( m_arrayReportBlocks[n].m_pItems );
	}
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::InitialiseConsolidationBlocksStageOne()
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

	CEposReportConsolParamsPluSales Params;
	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}
		
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( FALSE );
	m_BlockMap.BuildMap( m_SelectArray );
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::InitialiseConsolidationBlocksStageTwo()
{
	m_ReportFilters.ResetDbIdx();
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportPluByPriceBandBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolPluByPriceBandBase>;
		block.m_pItems = new CReportConsolidationArray<CEposReportConsolPluByPriceBandBase>;
		block.m_nCovers = 0;

		if (m_ReportSettings.GetReportSubType() == EPOSPRICEBANDREPORT_NOZEROSKIP)
		{
			m_ReportFilters.SetDatabase(entity.m_nDbIdx);

			CEposReportConsolPluByPriceBandBase DummySale;
			DummySale.m_bDummyItem = TRUE;
			DummySale.m_nTaxBand = 0;
			DummySale.m_nTaxDateCode = 0;
			DummySale.m_dTotalQty = 0.0;
			DummySale.m_dValBeforeDiscount = 0.0;
			DummySale.m_dDiscountVal = 0.0;
			DummySale.m_dPremiumVal = 0.0;

			for (int n = 0; n < 10; n++)
			{
				DummySale.m_dPriceBandQty[n] = 0.0;
				DummySale.m_dPriceBandVal[n] = 0.0;
			}

			for ( int nPluIdx = 0; nPluIdx < DataManager.Plu.GetSize(); nPluIdx++ )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );
				
				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nBasePluNo = PluRecord.GetPluNoInt();
						
				if ( m_ReportFilters.CheckPlu( infoPluNo ) == TRUE )
				{
					if ( m_ReportFilters.CheckDepartment( PluRecord.GetBaseDeptNo() ) == TRUE )
					{
						DummySale.m_nPluNo = PluRecord.GetPluNoInt();
						DummySale.m_nDeptNo = PluRecord.GetBaseDeptNo();
						block.m_pSales -> DirectAdd( DummySale );
					}
				}
			}
		}

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::Consolidate()
{
	/*
	switch( m_nReportType )
	{
	case REPORT_TYPE_PLUSALES_TRANSERVER:
	case REPORT_TYPE_PLUSALES_ITEMSERVER:
		m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_PLUSALES_ITEMSERVER : REPORT_TYPE_PLUSALES_TRANSERVER;
		break;

	case REPORT_TYPE_VIPSALES_TRANSERVER:
	case REPORT_TYPE_VIPSALES_ITEMSERVER:
		m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_VIPSALES_ITEMSERVER : REPORT_TYPE_VIPSALES_TRANSERVER;
		break;

	case REPORT_TYPE_COVERS_PLU_TRANSERVER:
	case REPORT_TYPE_COVERS_PLU_ITEMSERVER:
		m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_COVERS_PLU_ITEMSERVER : REPORT_TYPE_COVERS_PLU_TRANSERVER;
		break;

	case REPORT_TYPE_PLUWASTE_TRANSERVER:
	case REPORT_TYPE_PLUWASTE_ITEMSERVER:
		m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_PLUWASTE_ITEMSERVER : REPORT_TYPE_PLUWASTE_TRANSERVER;
		break;
	}
	*/

	m_nTotalCovers = 0;
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
		CString strLastTranTime = "";
		int nLastServerNo = -1;
		
		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			COleDateTime oleDatePMS = COleDateTime::GetCurrentTime();
			bool bPMSInclude = FALSE;

			CPluSalesLine csv ( infoFile.IsLegacySalesFolder() );

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if (m_PMSModes.CheckPMSTimeLine(bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude) == FALSE)
				{
					continue;
				}

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
				if (csv.GetPluSaleWastageFlag() != IsWastageReport())
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if (CONNECTION_TYPE_STANDARD_NONE == nConnectionType)
				{
					if (SysInfo.IsPMSSystem() == TRUE)
					{
						if ((m_PMSModes.GetEposFlag() == FALSE) && (FALSE == bIsPMSLocation))
						{
							if (DataManager.Payment.IsPMSPaymentType(csv.GetPluPaymentNo()))
							{
								continue;
							}
						}
					}
				}

				if ( TRUE == m_bVIPReport )
				{
					if (csv.IsVIPSale() == FALSE)
					{
						continue;
					}
				}
		
				//SET CONSOLIDATION SERVER
				int nServerNo = ( m_bItemServer ) ? csv.GetItemServerNo() : csv.GetTranServerNo();
				SetConsolidationServer( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nServerNo );

				//CHECK SALE DATE AND TIME
				if (CheckPluSaleDateTime(bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime) == FALSE)
				{
					continue;
				}

				if ( ( TRUE == bIsPMSLocation ) && ( strThisSaleDate != strLastSaleDate ) )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastSaleDate = strThisSaleDate;
				}

				/*
				int nLineCovers = 0;
				switch( m_nReportType )
				{
				case REPORT_TYPE_COVERS_PLU_TRANSERVER:
				case REPORT_TYPE_COVERS_PLU_ITEMSERVER:
					{
						nLineCovers = csv.GetCovers();
						
						if ( 0 == nLineCovers ) 
						{
							if (DealerFlags.GetAssumeCoversFlag() == TRUE)
							{
								nLineCovers = 1;
							}
						}
				
						if (0 == nLineCovers)
						{
							continue;
						}
					}
					break;
				}
				*/

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if (m_ReportFilters.CheckDepartment(nDeptNo) == FALSE)
				{
					continue;
				}

				double dMultiplier = 1.0;
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
						
						if (FALSE == infoPluNo.m_bValid)
						{
							continue;
						}

						if (m_ReportFilters.CheckPlu(infoPluNo) == FALSE)
						{
							continue;
						}

						dMultiplier = 1.0;
						if (infoPluNo.m_nModifier != 0)
						{
							dMultiplier = DataManager.Modifier.GetMultiplierForQty(infoPluNo.m_nModifier);
						}

						nPluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;
					}
					break;
				}

				//PERFORM COVERS CHECK HERE, AS ITEM WILL NOT BE EXCLUDED BY FILTER
				/*
				int nBlockCovers = 0;
				switch( m_nReportType )
				{
				case REPORT_TYPE_COVERS_PLU_TRANSERVER:
				case REPORT_TYPE_COVERS_PLU_ITEMSERVER:
					{
						CString strThisTranTime = csv.GetTranTime();
						if ( ( strThisTranTime != strLastTranTime ) || ( nServerNo != nLastServerNo ) )
						{
							CEposReportConsolTranServer infoTran;
							infoTran.m_strDateTran = infoFile.m_strDateTran;
							infoTran.m_strTimeTran = strThisTranTime;
							infoTran.m_nServerNo = nServerNo;

							int nPos;
							if ( m_arrayTranServer.Find( infoTran, nPos ) == FALSE )
							{
								nBlockCovers = nLineCovers;
								m_arrayTranServer.InsertAt( nPos, infoTran );

								if ( strThisTranTime != strLastTranTime )
									m_nTotalCovers += nLineCovers;
							}

							strLastTranTime = strThisTranTime;
							nLastServerNo = nServerNo;
						}
					}
					break;
				}
				*/

				int nPriceBand = csv.GetPriceBand();
				if ( CONNECTION_TYPE_SPTBOOK_NONE != nConnectionType )
				{
					if ((nPriceBand < 0) || (nPriceBand >= MAX_PLU_PRICES))
					{
						nPriceBand = 0;
					}
				}
				else
				{
					nPriceBand = DataManager.SBPriceText.GetReportMapPriceBand( nPriceBand - 1 );
				}

				if ((nPriceBand < 0) || (nPriceBand >= MAX_PLU_PRICES))
				{
					continue;
				}

				CEposReportConsolPluByPriceBandBase ConsolInfo;
				ConsolInfo.m_bDummyItem = FALSE;
				ConsolInfo.m_nPluNo = nPluNo;
				ConsolInfo.m_nDeptNo = nDeptNo;
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;
				
				if (DealerFlags.GetReportCostPriceFlag() == TRUE)
				{
					ConsolInfo.m_nLocIdx = infoFile.m_nLocIdx;
				}
				else
				{
					ConsolInfo.m_nLocIdx = 0;
				}

				if ((CSVPLUDATA_PROMO_NONTAXABLE == nLineType) || (CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType))
				{
					ConsolInfo.m_nTaxBand = -1;
				}
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
				{
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				}
				else
				{
					ConsolInfo.m_nTaxBand = nModifier;	//WILL CONVERT TO TAX BAND LATER	
				}

				switch ( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_dTotalQty = 0.0;
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;

				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_dTotalQty = csv.GetSaleQty();
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;

				case CSVPLUDATA_PLU_SALE:
					ConsolInfo.m_dTotalQty = csv.GetSaleQty() * dMultiplier;
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
					break;
				}

				ConsolInfo.m_dPriceBandQty[nPriceBand] = ConsolInfo.m_dTotalQty;
				ConsolInfo.m_dPriceBandVal[nPriceBand] = ConsolInfo.m_dValBeforeDiscount + ConsolInfo.m_dPremiumVal - ConsolInfo.m_dDiscountVal;

				if ( (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE) || ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType )  )
				{
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_pSales -> Consolidate( ConsolInfo );
						//m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_nCovers += nBlockCovers;
					}
				}
				else
				{
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_pItems -> Consolidate( ConsolInfo );
						//m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_nCovers += nBlockCovers;
					}
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();

	if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
	{
		CReportCostFinder CostFinder;

		__int64 nProgressTarget = 0;
		__int64 nProgressCount = 0;

		for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
		{
			nProgressTarget += m_arrayReportBlocks[n].m_pSales -> GetSize();
			nProgressTarget += m_arrayReportBlocks[n].m_pItems -> GetSize();
		}

		StatusProgress.Lock( TRUE, "Calculating cost prices" );

		m_ReportFilters.ResetDbIdx();
		for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
		{
			int nDbIdx = m_arrayReportBlocks[n].m_nDbIdx;
			
			if ( m_ReportFilters.SetDatabase( nDbIdx ) == TRUE )
				CostFinder.Init();

			for ( int s = 0; s < m_arrayReportBlocks[n].m_pSales -> GetSize(); s++ )
			{
				StatusProgress.SetPos( nProgressCount++, nProgressTarget );

				CEposReportConsolPluByPriceBandBase Sales;
				m_arrayReportBlocks[n].m_pSales -> GetAt( s, Sales );
				Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
				m_arrayReportBlocks[n].m_pSales -> SetAt( s, Sales );
			}

			for ( int s = 0; s < m_arrayReportBlocks[n].m_pItems -> GetSize(); s++ )
			{
				StatusProgress.SetPos( nProgressCount++, nProgressTarget );

				CEposReportConsolPluByPriceBandBase Sales;
				m_arrayReportBlocks[n].m_pItems -> GetAt( s, Sales );
				Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
				m_arrayReportBlocks[n].m_pItems -> SetAt( s, Sales );
			}
		}

		StatusProgress.Unlock();
	}
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::CreateSalesSection( CEposReportPluByPriceBandBlock& Block )
{
	for ( int s = Block.m_pItems -> GetSize() - 1; s >= 0; s-- )
	{
		CEposReportConsolPluByPriceBandBase ConsolInfo;
		Block.m_pItems -> GetAt( s, ConsolInfo );
		UpdateCreateReportProgress();
		
		m_PluInfoFinder.SetPluNo( ConsolInfo.m_nPluNo );
		ConsolInfo.m_nTaxBand = m_PluInfoFinder.GetModifierTaxBand( ConsolInfo.m_nTaxBand );
		
		Block.m_pSales -> Consolidate( ConsolInfo );
		Block.m_pItems -> RemoveAt( s );
	}

	Block.m_nCurrentPluNo = -1;
	Block.m_nCurrentDeptNo = -1;

	int nCurrentItemStart = -1;
	CEposReportConsolPluByPriceBandBase CurrentPluDeptTax;

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolPluByPriceBandBase Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( CurrentPluDeptTax.ComparePluDeptTax( Sales ) == FALSE )
		{
			if (nCurrentItemStart != -1)
			{
				if (CalculatePluItemTotals(Block, CurrentPluDeptTax.m_nTaxBand, nCurrentItemStart, s - 1) == TRUE)
				{
					WritePluItemTotals(Block, CurrentPluDeptTax.m_nTaxBand);
				}
			}
			
			nCurrentItemStart = s;
			CurrentPluDeptTax = Sales;
		}
	}

	if (nCurrentItemStart != -1)
	{
		if (CalculatePluItemTotals(Block, CurrentPluDeptTax.m_nTaxBand, nCurrentItemStart, Block.m_pSales->GetSize() - 1) == TRUE)
		{
			WritePluItemTotals(Block, CurrentPluDeptTax.m_nTaxBand);
		}
	}
}

/**********************************************************************/

bool CEposReportPluByPriceBandBase::CalculatePluItemTotals( CEposReportPluByPriceBandBlock& Block, int nTaxBand, int nStart, int nEnd )
{
	Block.m_CurrentPluTotals.Clear();

	if ((nTaxBand < 0) || (nTaxBand > MAX_TAX_BANDS))
	{
		nTaxBand = 0;
	}

	CEposReportConsolPluByPriceBandBase ConsolInfoStart;
	Block.m_pSales -> GetAt( nStart, ConsolInfoStart );	
	
	if ((TRUE == ConsolInfoStart.m_bDummyItem) && (ConsolInfoStart.m_nPluNo == Block.m_nCurrentPluNo))
	{
		return FALSE;
	}
	
	Block.m_nCurrentPluNo = ConsolInfoStart.m_nPluNo;
	Block.m_nCurrentDeptNo = ConsolInfoStart.m_nDeptNo;

	//FETCH PER ITEM COST IF AT DATABASE LEVEL
	double dItemCost = 0.0;
	bool bReportCost = DealerFlags.GetReportCostPriceFlag();
	if ( FALSE == bReportCost )
	{
		m_PluInfoFinder.SetPluNo( Block.m_nCurrentPluNo );
		dItemCost = m_PluInfoFinder.GetCost();
	}

	for ( int i = nStart; i <= nEnd; i++ )
	{		
		CEposReportConsolPluByPriceBandBase ConsolInfoLine;
		Block.m_pSales -> GetAt( i, ConsolInfoLine );

		if (TRUE == bReportCost)
		{
			dItemCost = ConsolInfoLine.m_dItemCost;
		}

		double dNet = ConsolInfoLine.m_dValBeforeDiscount - ConsolInfoLine.m_dDiscountVal + ConsolInfoLine.m_dPremiumVal;
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( ConsolInfoLine.m_nTaxDateCode, nTaxBand );
		double dTax = dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate );		
		double dCost = dItemCost * ConsolInfoLine.m_dTotalQty;

		CEposReportPluByPriceBandInfo PluSalesInfo;
		PluSalesInfo.m_dQty = ConsolInfoLine.m_dTotalQty;
		PluSalesInfo.m_dValBeforeDiscount = ConsolInfoLine.m_dValBeforeDiscount;
		PluSalesInfo.m_dDiscountAmount = ConsolInfoLine.m_dDiscountVal;
		PluSalesInfo.m_dPremiumAmount = ConsolInfoLine.m_dPremiumVal;
		PluSalesInfo.m_dTax = dTax;
		PluSalesInfo.m_dCost = dCost;

		for (int n = 0; n < MAX_PLU_PRICES; n++)
		{
			PluSalesInfo.m_dPriceBandQty[n] = ConsolInfoLine.m_dPriceBandQty[n];
			PluSalesInfo.m_dPriceBandVal[n] = ConsolInfoLine.m_dPriceBandVal[n];
		}
		
		Block.m_CurrentPluTotals.Consolidate( PluSalesInfo );
		Block.m_BlockTotals.Consolidate(PluSalesInfo );

		if (TRUE == Block.m_bAddToTotal)
		{
			m_Totals1.Consolidate(PluSalesInfo);
		}
	}

	return TRUE;
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::WritePluItemTotals(CEposReportPluByPriceBandBlock& Block, int nTaxBand)
{
	m_PluInfoFinder.SetPluNo(Block.m_nCurrentPluNo);

	CEposReportPluByPriceBandInfo PluSalesInfo = Block.m_CurrentPluTotals;
	
	CCSV csv('\t');
	csv.Add(ReportHelpers.GetDisplayPluNo(Block.m_nCurrentPluNo, TRUE));
	csv.Add(m_PluInfoFinder.GetPluInfoRepText(TRUE));
	csv.Add(Block.m_nCurrentDeptNo);

	double dQty = PluSalesInfo.m_dQty;
	double dVal = PluSalesInfo.m_dValBeforeDiscount;
	double dDiscount = PluSalesInfo.m_dDiscountAmount;
	double dPremium = PluSalesInfo.m_dPremiumAmount;
	double dTax = PluSalesInfo.m_dTax;
	double dCost = PluSalesInfo.m_dCost;
	double dNet = dVal - dDiscount + dPremium;

	for (int n = 0; n < MAX_PLU_PRICES; n++)
	{
		if (m_ReportSettings.IsPriceBandEnabled(n) == TRUE)
		{
			csv.Add(PluSalesInfo.m_dPriceBandQty[n], SysInfo.GetDPQty());

			if (TRUE == m_bAllowValue)
			{
				csv.Add(PluSalesInfo.m_dPriceBandVal[n], SysInfo.GetDPValue());
			}
		}
	}

	csv.Add(dQty, SysInfo.GetDPQty());

	if (TRUE == m_bAllowValue)
	{
		if (TRUE == m_bDiscountDetail)
		{
			csv.Add(dVal, SysInfo.GetDPValue());

			if (TRUE == m_bPremiumDetail)
			{
				csv.Add(dDiscount, SysInfo.GetDPValue());
				csv.Add(dPremium, SysInfo.GetDPValue());
			}
			else
			{
				csv.Add(dDiscount - dPremium, SysInfo.GetDPValue());
			}
		}

		csv.Add(dNet, SysInfo.GetDPValue());
		csv.Add(ReportHelpers.CalcAverage(dQty, dNet), SysInfo.GetDPValue());
		csv.Add(CTaxArray::GetTaxBandFromNumber(nTaxBand));
		csv.Add(dTax, SysInfo.GetDPValue());

		if (TRUE == m_bNetOfTax)
		{
			csv.Add(dNet - dTax, SysInfo.GetDPValue());
		}
	}

	csv.Add(dCost, SysInfo.GetDPValue());

	m_ReportFile.WriteReportDataLine(csv.GetLine());
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::WriteBlockTotals( CEposReportPluByPriceBandBlock& Block )
{
	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType );
	WriteConsolidatedTotals( Block.m_BlockTotals, strType, Block.m_nCovers, FALSE );
}

/**********************************************************************/

void CEposReportPluByPriceBandBase::WriteConsolidatedTotals(CEposReportPluByPriceBandInfo& Totals, const char* szType, int nCovers, bool bGrandTotal)
{
	CString strFullLine = "<..>";

	CString strType = szType;
	if (strType != "") strType += " ";
	strType += "Total";

	if (FALSE == bGrandTotal)
	{
		m_ReportFile.WriteReportMiscLine(m_UnderlineMaker.GetLine());
	}

	double dQty = Totals.m_dQty;
	double dVal = Totals.m_dValBeforeDiscount;
	double dDiscount = Totals.m_dDiscountAmount;
	double dPremium = Totals.m_dPremiumAmount;
	double dTax = Totals.m_dTax;
	double dCost = Totals.m_dCost;
	double dNet = dVal - dDiscount + dPremium;

	CCSVReportLine csv;
	csv.AppendBlanks(1);
	csv.AppendString(strFullLine + strType);
	csv.AppendBlanks(1);

	for (int n = 0; n < MAX_PLU_PRICES; n++)
	{
		if (m_ReportSettings.IsPriceBandEnabled(n) == TRUE)
		{
			csv.Add(Totals.m_dPriceBandQty[n], SysInfo.GetDPQty());

			if (TRUE == m_bAllowValue)
			{
				csv.Add(Totals.m_dPriceBandVal[n], SysInfo.GetDPValue());
			}
		}
	}

	csv.AppendLineQty(dQty);

	if (TRUE == m_bAllowValue)
	{
		if (TRUE == m_bDiscountDetail)
		{
			csv.AppendVal(dVal);

			if (TRUE == m_bPremiumDetail)
			{
				csv.AppendVal(dDiscount);
				csv.AppendVal(dPremium);
			}
			else
			{
				csv.AppendVal(dDiscount - dPremium);
			}
		}

		csv.AppendVal(dNet);
		csv.AppendVal(ReportHelpers.CalcAverage(dQty, dNet));
		csv.AppendBlanks(1);
		csv.AppendVal(dTax);

		if (TRUE == m_bNetOfTax)
		{
			csv.AppendVal(dNet - dTax);
		}
	}

	csv.AppendVal(dCost);
	m_ReportFile.WriteReportMiscLine(csv.GetLine());
}
	

/**********************************************************************/

