/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportDiscountBase.h"
/**********************************************************************/

CEposReportDiscountBase::CEposReportDiscountBase( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nBlankColumnsBeforeDate = 1;
	m_nAuditLevel = NODE_LOCATION_TERMINAL;
	ClearGrandTotals();
}

/**********************************************************************/

void CEposReportDiscountBase::ClearGrandTotals()
{
	m_dGrandTotalDiscount = 0.0;
	m_dGrandTotalMarkdown = 0.0;
	m_dGrandTotalPremium = 0.0;
	m_dGrandTotalMixMatch = 0.0;
	m_dGrandTotalPMS = 0.0;
}

/**********************************************************************/

CEposReportDiscountBase::~CEposReportDiscountBase()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pDiscount -> RemoveAll();
		m_arrayReportBlocks[n].m_pTotals -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pDiscount );
		delete ( m_arrayReportBlocks[n].m_pTotals );
	}
}

/**********************************************************************/

void CEposReportDiscountBase::InitialiseConsolidationBlocksStageOne()
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

	if ( TRUE == Params.m_bConsolTerm )
		m_nAuditLevel = NODE_LOCATION_TERMINAL;
	else if ( TRUE == Params.m_bConsolLoc )
		m_nAuditLevel = NODE_LOCATION;
	else
		m_nAuditLevel = NODE_DATABASE;
}

/**********************************************************************/

void CEposReportDiscountBase::InitialiseConsolidationBlocksStageTwo()
{
	int nLastDbIdx = -1;

	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportDiscountBlock block;
		block.Copy( entity );
		block.m_pDiscount = new CReportConsolidationArray<CEposReportConsolDiscountNew>;
		block.m_pTotals = new CReportConsolidationArray<CEposReportConsolDiscountNew>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportDiscountBase::Consolidate()
{
	m_nLineNo = 0;

	switch( m_nReportType )
	{
	case REPORT_TYPE_TRANSERVER_DISCOUNT:
	case REPORT_TYPE_ITEMSERVER_DISCOUNT:
		m_nReportType = ( m_bItemServer ) ? REPORT_TYPE_ITEMSERVER_DISCOUNT : REPORT_TYPE_TRANSERVER_DISCOUNT;
		break;
	}

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
				if ( m_PMSModes.CheckPMSTimeLine( bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude ) == FALSE )
					continue;
						
				csv.ParseLine( strBuffer );

				//FILTER FOR POST TO ROOM
				if ( CONNECTION_TYPE_STANDARD_NONE == nConnectionType )
					if ( SysInfo.IsPMSSystem() == TRUE )
						if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
							if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
								continue;

				//SET CONSOLIDATION SERVER
				int nServerNo = ( m_bItemServer ) ? csv.GetItemServerNo() : csv.GetTranServerNo();
				SetConsolidationServer( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nServerNo );

				//CHECK SALE DATE AND TIME
				if ( CheckPluSaleDateTime( bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime ) == FALSE )
					continue;
			
				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				CEposReportConsolDiscountNew ConsolInfo;
				ConsolInfo.m_strDate = strThisSaleDate;
				ConsolInfo.m_strTime = strThisSaleTime;
				ConsolInfo.m_nLineNo = 0;
				ConsolInfo.m_nMixMatchType = -1;
				ConsolInfo.m_nDiscountMode = 0;
				ConsolInfo.m_nDiscountType = 0;
				ConsolInfo.m_strText = "";
				ConsolInfo.m_dValue = 0.0;
				ConsolInfo.m_nRate = 0;

				bool bProcessLine = TRUE;
				int nLineType = csv.GetLineType();

				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					ConsolInfo.m_nDiscountMode = csv.GetDiscountMode();
					ConsolInfo.m_nOfferNo = csv.GetPromoNoSeparate();
					ConsolInfo.m_nPluNo = GetSeparatedPromoPluNo( ConsolInfo.m_nOfferNo );
					ConsolInfo.m_nRate = csv.GetDiscountRate();
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_nMixMatchType = csv.GetMixMatchType();
					ConsolInfo.m_nDiscountMode = DISCOUNT_MODE_MIXMATCH;
					ConsolInfo.m_nOfferNo = csv.GetMixMatchNo();
					ConsolInfo.m_nPluNo = GetSeparatedMixMatchPluNo( ConsolInfo.m_nOfferNo );
					break;

				case CSVPLUDATA_PLU_DISCOUNT:
					ConsolInfo.m_nDiscountMode = csv.GetDiscountMode();
					ConsolInfo.m_nOfferNo = csv.GetPromoNoForPlu();
					ConsolInfo.m_nPluNo = csv.GetPluNoNew();
					ConsolInfo.m_nRate = csv.GetDiscountRate();
					break;

				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_nMixMatchType = csv.GetMixMatchType();
					ConsolInfo.m_nDiscountMode = DISCOUNT_MODE_MIXMATCH;
					ConsolInfo.m_nOfferNo = csv.GetMixMatchNo();
					ConsolInfo.m_nPluNo = csv.GetPluNoNew();
					break;

				default:
					bProcessLine = FALSE;
					break;
				}

				if ( TRUE == bProcessLine )
				{
					int nMask = m_ReportSettings.GetOfferFilter();
					switch( ConsolInfo.m_nMixMatchType )
					{
					case -1:	bProcessLine = ( ( nMask & 0x001 ) != 0 );	break;
					case 1:		bProcessLine = ( ( nMask & 0x002 ) != 0 );	break;
					case 2:		bProcessLine = ( ( nMask & 0x004 ) != 0 );	break;
					case 3:		bProcessLine = ( ( nMask & 0x008 ) != 0 );	break;
					case 4:		bProcessLine = ( ( nMask & 0x010 ) != 0 );	break;
					case 5:		bProcessLine = ( ( nMask & 0x020 ) != 0 );	break;
					case 6:		bProcessLine = ( ( nMask & 0x040 ) != 0 );	break;
					case 7:		bProcessLine = ( ( nMask & 0x080 ) != 0 );	break;
					case 8:		bProcessLine = ( ( nMask & 0x100 ) != 0 );	break;
					}
				}

				if ( TRUE == bProcessLine )
				{
					ConsolInfo.m_dValue = csv.GetValue();

					switch( nLineType )
					{
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_PLU_DISCOUNT:
						{
							CString strDiscType = csv.GetDiscountType();
							if ( strDiscType == "M" )		ConsolInfo.m_nDiscountType = DISCOUNT_TYPE_MARKDOWN;
							else if ( strDiscType == "D" )	ConsolInfo.m_nDiscountType = DISCOUNT_TYPE_DISCOUNT;
							else if ( strDiscType == "P" )	ConsolInfo.m_nDiscountType = DISCOUNT_TYPE_PREMIUM;
							else if ( TRUE == bIsPMSLocation ) ConsolInfo.m_nDiscountType = DISCOUNT_TYPE_PMS;
							else bProcessLine = FALSE;

							if ( DISCOUNT_TYPE_PREMIUM == ConsolInfo.m_nDiscountType )
								ConsolInfo.m_dValue *= -1;
						}
						break;
					}
				}

				if ( TRUE == bProcessLine )
				{
					if ( ( DISCOUNT_MODE_ITEM == ConsolInfo.m_nDiscountMode ) || ( TRUE == m_ReportSettings.GetSubtotalItemsFlag() ) )
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = ConsolInfo.m_nPluNo;
						::ProcessPluNo( infoPluNo );

						m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo );
						ConsolInfo.m_strText = m_PluInfoFinder.GetPluInfoRepText( TRUE, infoPluNo.m_nModifier, TRUE);
						
						switch( nLineType )
						{
						case CSVPLUDATA_PLU_DISCOUNT:
						case CSVPLUDATA_PLU_MIXMATCH:
							ConsolInfo.m_nLineNo = m_nLineNo++;
							break;
						}
					}
					else
					{
						ConsolInfo.m_nPluNo = 0;
					}

					CEposReportConsolDiscountNew infoTotal = ConsolInfo;
					infoTotal.m_strDate = "";
					infoTotal.m_strTime = "";
					infoTotal.m_nPluNo = 0;
					infoTotal.m_nLineNo = 0;
					infoTotal.m_strText = "";

					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						
						if ( m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_nEntityType == m_nAuditLevel )
							m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_pDiscount -> Consolidate( ConsolInfo );
						
						m_arrayReportBlocks[ nBlock + m_nServerBlockIdxStart ].m_pTotals -> Consolidate( infoTotal );
					}
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

void CEposReportDiscountBase::CreateDiscountSection( CEposReportDiscountBlock& Block, bool bBlockTotals )
{
	CReportConsolidationArray<CEposReportConsolDiscountNew>* pDiscounts;

	if ( TRUE == bBlockTotals )
		pDiscounts = Block.m_pTotals;
	else
		pDiscounts = Block.m_pDiscount;

	double dTotalMarkdown = 0.0;
	double dTotalDiscount = 0.0;
	double dTotalMixMatch = 0.0;
	double dTotalPremium = 0.0;
	double dTotalPMS = 0.0;

	for ( int s = 0; s < pDiscounts -> GetSize(); s++ )
	{
		CEposReportConsolDiscountNew infoConsol;
		pDiscounts -> GetAt( s, infoConsol );
		UpdateCreateReportProgress();

		CString strLineDate = "";
		CString strLineTime = "";

		if ( FALSE == bBlockTotals )
		{
			strLineDate.Format ( "%s/%s/%s",
				(const char*) infoConsol.m_strDate.Right(2),
				(const char*) infoConsol.m_strDate.Mid(4,2),
				(const char*) infoConsol.m_strDate.Left(4) );

			strLineTime.Format ( "%s:%s:%s",
				(const char*) infoConsol.m_strTime.Left(2),
				(const char*) infoConsol.m_strTime.Mid(2,2),
				(const char*) infoConsol.m_strTime.Right(2) );
		}

		CString strText = "";
		if ( infoConsol.m_nMixMatchType != -1 )
		{
			::GetMixMatchOfferTypeName( infoConsol.m_nMixMatchType, strText, TRUE, "Unknown Mix Match Type" );
			dTotalMixMatch += infoConsol.m_dValue;
		}
		else
		{
			strText = "<..>";

			switch( infoConsol.m_nDiscountMode )
			{
			case DISCOUNT_MODE_SUBTOTAL:	strText += "Subtotal ";	break;
			case DISCOUNT_MODE_LOYALTY:		strText += "Loyalty ";	break;
			case DISCOUNT_MODE_ITEM:		strText += "Item ";		break;
			}

			switch( infoConsol.m_nDiscountType )
			{
			case DISCOUNT_TYPE_MARKDOWN:
				dTotalMarkdown += infoConsol.m_dValue;
				strText += "Markdown";
				break;

			case DISCOUNT_TYPE_DISCOUNT:
				dTotalDiscount += infoConsol.m_dValue;
				strText += "Discount";
				break;

			case DISCOUNT_TYPE_PREMIUM:
				dTotalPremium += infoConsol.m_dValue;
				strText += "Premium";
				break;

			case DISCOUNT_TYPE_PMS:
				dTotalPMS += infoConsol.m_dValue;
				strText += "PMS";
				break;
			}
		}

		CCSV csvPromo;
		if ( infoConsol.m_nMixMatchType != -1 )
		{
			CString strOfferNo = "";
			CString strOfferName = "";

			if ( infoConsol.m_nOfferNo == 0 )
			{
				strOfferName = "Unknown Offer";
			}
			else
			{
				strOfferNo.Format ( "M%2.2d", infoConsol.m_nOfferNo );

				int nMixMatchIdx;
				if ( DataManager.MixMatch.FindTableByNumber( infoConsol.m_nOfferNo, nMixMatchIdx ) == FALSE )
				{
					strOfferName.Format( "Mix Match %d", infoConsol.m_nOfferNo );
				}
				else
				{
					CMixMatchCSVRecord MixMatch;
					DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
					strOfferName = MixMatch.GetDisplayName();
				}
			}
			
			if ( strOfferNo != "" ) csvPromo.Add( strOfferNo );
			csvPromo.Add( strOfferName );
		}
		else
		{
			CString strPromoNo = "";
			CString strPromoName = "";
			CString strRate = "";

			if (DISCOUNT_TYPE_PMS == infoConsol.m_nDiscountType)
			{
				strPromoName = "PMS Discount";
			}
			else if ( infoConsol.m_nOfferNo == 0 )
			{
				strPromoName = "Unknown Promo";
			}
			else
			{
				strPromoNo.Format ( "P%2.2d", infoConsol.m_nOfferNo );

				int nPromoIdx;
				if ( DataManager.Promotion.FindTableByNumber( infoConsol.m_nOfferNo, nPromoIdx ) == FALSE )
				{
					strPromoName.Format( "Promo %d", infoConsol.m_nOfferNo );
				}
				else
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt( nPromoIdx, Promo );
					strPromoName = Promo.GetDisplayName();
				}
				
				if ( infoConsol.m_nDiscountType != DISCOUNT_TYPE_MARKDOWN )
					if ( infoConsol.m_nRate != 0 )
						strRate.Format ( "%.2f%%", ( (double) infoConsol.m_nRate ) / 100.0 );
			}
			
			if ( strPromoNo != "" ) csvPromo.Add( strPromoNo );
			csvPromo.Add( strPromoName );
			if ( strRate != "" ) csvPromo.Add( strRate );

		}

		CCSV csvPluNo;
		if ( infoConsol.m_nPluNo != 0 )
		{		
			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = infoConsol.m_nPluNo;
			::ProcessPluNo( infoPluNo );

			if ( TRUE == infoPluNo.m_bValid )
			{
				CString strPluNo = ReportHelpers.GetDisplayPluNo( infoPluNo.m_nBasePluNo, TRUE );
				
				if ( infoPluNo.m_nModifier != 0 )
				{
					CString strExtra;
					strExtra.Format( " + %d",
						infoPluNo.m_nModifier );

					strPluNo += strExtra;
				}
				
				csvPluNo.Add( strPluNo );
				csvPluNo.Add( infoConsol.m_strText );
			}
		}
		
		CCSV csvReport ( '\t' );
		
		csvReport.Add( strLineDate );
		csvReport.Add( strLineTime );
		csvReport.Add( strText );
		csvReport.Add( csvPromo.GetLine() );
		csvReport.Add( infoConsol.m_dValue, SysInfo.GetDPValue() );
		csvReport.Add( csvPluNo.GetLine() );
		
		m_ReportFile.WriteReportDataLine ( csvReport.GetLine() );
	}

	if ( TRUE == bBlockTotals )
	{
		WriteTotals( FALSE, Block.m_nEntityType, dTotalDiscount, dTotalMarkdown, dTotalMixMatch, dTotalPremium, dTotalPMS );

		if ( TRUE == Block.m_bAddToTotal )
		{
			m_dGrandTotalDiscount += dTotalDiscount;
			m_dGrandTotalMarkdown += dTotalMarkdown;
			m_dGrandTotalMixMatch += dTotalMixMatch;
			m_dGrandTotalPremium += dTotalPremium;
			m_dGrandTotalPMS += dTotalPMS;
		}
	}
}

/**********************************************************************/

void CEposReportDiscountBase::WriteUnderline()
{
	m_ReportFile.WriteReportMiscLine( "\t\t\t\t<LI>" );
}

/**********************************************************************/

void CEposReportDiscountBase::WriteGrandTotals()
{
	WriteTotals( TRUE, 0, m_dGrandTotalDiscount, m_dGrandTotalMarkdown, m_dGrandTotalMixMatch, m_dGrandTotalPremium, m_dGrandTotalPMS );
}

/**********************************************************************/

void CEposReportDiscountBase::WriteTotals(bool bGrandTotal, int nEntityType, double dDiscount, double dMarkdown, double dMixMatch, double dPremium, double dPMS)
{
	CString strType = "";

	if (bGrandTotal == TRUE)
	{
		strType = "Total";
	}
	else
	{
		WriteUnderline();

		if (m_nReportType == REPORT_TYPE_NOSERVER_DISCOUNT)
		{
			switch (nEntityType)
			{
			case NODE_DATABASE:				strType = "Database";		break;
			case NODE_LOCATION:				strType = "Location";		break;
			case NODE_REPORTPOINT:			strType = "Report Point";	break;
			case NODE_LOCATION_TERMINAL:	strType = "Terminal";		break;
			default:						strType = "Total";			break;
			}
		}
		else
		{
			strType = "Server";
		}
	}

	bool bGotPMS = (CPriceHelpers::CompareDoubles(dPMS, 0.0, 3) != 0);
	bool bGotDiscount = (CPriceHelpers::CompareDoubles(dDiscount, 0.0, 3) != 0);
	bool bGotMarkdown = (CPriceHelpers::CompareDoubles(dMarkdown, 0.0, 3) != 0);
	bool bGotMixMatch = (CPriceHelpers::CompareDoubles(dMixMatch, 0.0, 3) != 0);
	bool bGotPremium = (CPriceHelpers::CompareDoubles(dPremium, 0.0, 3) != 0);
	bool bShowDiscountTypeTotal = TRUE;

	if (FALSE == bGrandTotal)
	{
		if (SysInfo.IsPMSSystem() == TRUE)
		{
			if ((TRUE == bGotPMS) && (FALSE == bGotDiscount) && (FALSE == bGotMarkdown) && (FALSE == bGotMixMatch))
			{
				bShowDiscountTypeTotal = FALSE;
			}
		}
	}

	CString strFullLine = "<..>";
	CCSV csv('\t');

	if (TRUE == bShowDiscountTypeTotal)
	{
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add(strFullLine + strType + " Discount");
		csv.Add(dDiscount, SysInfo.GetDPValue());
		m_ReportFile.WriteReportMiscLine(csv.GetLine());

		csv.RemoveAll();
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add(strFullLine + strType + " Markdown");
		csv.Add(dMarkdown, SysInfo.GetDPValue());
		m_ReportFile.WriteReportMiscLine(csv.GetLine());

		csv.RemoveAll();
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add(strFullLine + strType + " Mix Match");
		csv.Add(dMixMatch, SysInfo.GetDPValue());
		m_ReportFile.WriteReportMiscLine(csv.GetLine());

		if ((SysInfo.IsPMSSystem() == TRUE) && (TRUE == bGotPMS))
		{
			csv.RemoveAll();
			csv.Add("");
			csv.Add("");
			csv.Add("");
			csv.Add(strFullLine + strType + " PMS Discount");
			csv.Add(dPMS, SysInfo.GetDPValue());
			m_ReportFile.WriteReportMiscLine(csv.GetLine());
		}

		WriteUnderline();

	}

	if ((TRUE == bShowDiscountTypeTotal) || (TRUE == bGotPremium))
	{
		csv.RemoveAll();
		csv.Add("");
		csv.Add("");
		csv.Add("");
		csv.Add(strFullLine + strType + " Premium");
		csv.Add(dPremium, SysInfo.GetDPValue());
		m_ReportFile.WriteReportMiscLine(csv.GetLine());

		WriteUnderline();
	}

	csv.RemoveAll();
	csv.Add("");
	csv.Add("");
	csv.Add("");
	csv.Add(strFullLine + strType + " Net Discount");
	csv.Add(dDiscount + dMarkdown + dMixMatch + dPMS - dPremium, SysInfo.GetDPValue());
	m_ReportFile.WriteReportMiscLine(csv.GetLine());
}

/**********************************************************************/



