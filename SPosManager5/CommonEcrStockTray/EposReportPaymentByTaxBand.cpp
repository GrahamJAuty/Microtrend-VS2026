/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "ReportPluInfoFinder.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPaymentByTaxBand.h"
/**********************************************************************/

CEposReportPaymentByTaxBand::CEposReportPaymentByTaxBand( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_PAYMENT_ESTIMATE_TAX;	
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
	SetPMSDepositsFlag( TRUE );
}

/**********************************************************************/

CEposReportPaymentByTaxBand::~CEposReportPaymentByTaxBand()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pPayments -> RemoveAll();
		m_arrayReportBlocks[n].m_pTotals -> RemoveAll();
		
		delete ( m_arrayReportBlocks[n].m_pPayments );
		delete ( m_arrayReportBlocks[n].m_pTotals );
	}
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::InitialiseConsolidationBlocks()
{
	bool bAllowGlobal = ( NODE_SYSTEM == m_nConLevel );
	bAllowGlobal &= EcrmanOptions.GetGlobalPaymentFlag();
	bAllowGlobal &= EcrmanOptions.GetGlobalTaxFlag();
	
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
	
	if ( TRUE == Params.m_bConsolSys )
	{
		if ( ( NODE_SYSTEM != m_nConLevel ) || ( FALSE == bAllowGlobal ) )
		{
			Params.m_bConsolSys = FALSE;
			Params.m_bConsolDb = TRUE;
		}
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

		CEposReportPaymentByTaxBandBlock block;
		block.Copy( entity );
		block.m_pPayments = new CReportConsolidationArray<CEposReportConsolPBT_MainBlock>;
		block.m_pTotals = new CReportConsolidationArray<CEposReportConsolPBT_TotalBlock>;
		
		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, FALSE, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ITEMPAY, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

		switch ( m_PMSModes.GetPMSLocationStatus( infoFile ) )
		{
		case 1:	ProcessPMSFile	( infoFile, nProgress, nTotalFileSize );	break; 	
		case 2:	ProcessEPOSFile	( infoFile, nProgress, nTotalFileSize );	break;
		}
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ProcessPMSFile( CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize )
{
	ResetTransaction();
	
	bool bFail = FALSE;
	int nFileSize = 0;
	
	CString strThisSaleDate = "";
	CString strThisSaleTime = "";
	m_PMSModes.GetInitialSaleDateTime( TRUE, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
	int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );
		
	{
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			bFail = TRUE;
		else
		{
			CPluSalesLine csv ( infoFile.IsLegacySalesFolder() );

			int nLinesToRead;
			CString strBuffer;
			if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
			{
				do
				{
					StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

					bool bDummy; 
					CString strDummy1, strDummy2;
					COleDateTime oleDateTimeDummy;
					if ( m_PMSModes.CheckPMSTimeLine( TRUE, strBuffer, strDummy1, strDummy2, oleDateTimeDummy, bDummy ) == FALSE )
						continue;

					csv.ParseLine( strBuffer );

					int nTaxBand = 0;

					if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					{
						nTaxBand = ReportHelpers.GetNumericTaxBandFromString( csv.GetTaxBand() );
					}
					else
					{
						nTaxBand = REPORT_TAXBAND_UNKNOWN_PLU;

						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );

						if ( infoPluNo.m_bValid )
						{
							m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo  );

							if ( m_PluInfoFinder.PluExists() == TRUE )
							{
								nTaxBand = m_PluInfoFinder.GetModifierTaxBand( infoPluNo.m_nModifier );
							}
						}
					}
					
					switch( csv.GetLineType() )
					{
					case CSVPLUDATA_PLU_SALE:
						ConsolidatePluNo( csv.GetPluNoNew(), csv.GetDeptNo(), csv.GetGroupNo(), nTaxDateCode, nTaxBand, csv.GetValue() );
						break;
					
					case CSVPLUDATA_PLU_DISCOUNT:
						ConsolidatePluNo( csv.GetPluNoNew(), csv.GetDeptNo(), csv.GetGroupNo(), nTaxDateCode, nTaxBand, -csv.GetValue() );
						break;
					}

				}
				while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
			}
		}
	}

	if ( FALSE == bFail )
	{
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			bFail = TRUE;
		else
		{
			nFileSize = fileSales.GetLength();

			int nLinesToRead;
			CString strBuffer;
			if ( ::FindFirstTermLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
			{
				bool bInclude = FALSE;		

				do
				{
					StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

					CCSVEposTermLine csvIn ( strBuffer );

					switch( csvIn.GetLineTypeInt() )
					{
					case CASHRSP_LINETYPE_TRAN_NORMAL:
						{
							bInclude = TRUE;
							CString strCheckDate = infoFile.m_strDateTran;		
						
							COleDateTime oleDate = COleDateTime (
								atoi(infoFile.m_strDateTran.Left(4)),
								atoi(infoFile.m_strDateTran.Mid(4,2)),
								atoi(infoFile.m_strDateTran.Right(2)),
								0, 0, 0 );
						
							m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
							m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );
						}
						break;

					case CASHRSP_LINETYPE_PAYMENT:
					case CASHRSP_LINETYPE_DEPOSIT_PMS:
						if ( TRUE == bInclude )
						{
							CPaymentByTaxBandBufferPayment info;
							info.m_nPaymentNo = csvIn.GetPaymentNumber();
							info.m_dVal = ( ( double ) csvIn.GetPaymentAmount() ) / 100.0;
							m_ConsolBufferPayment.Consolidate( info );
						}
						break;
					}
				}
				while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );

				bFail = ( FALSE == bInclude );
			}
		}
	}

	nProgress += nFileSize;
	
	if ( FALSE == bFail )
	{
		ProcessTransaction();
	}
	
	ResetTransaction();
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ProcessEPOSFile( CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize )
{
	CSSFile fileSales;
	if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
		return;

	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	{
		int nDummy = 0;
		dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
	}

	ResetTransaction();
	bool bInclude = FALSE;

	int nTaxDateCode = 0;
	CString strLastCheckDate = "";

	CString strBuffer = "";
	while( fileSales.ReadString( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

		CCSV csvIn( strBuffer );

		switch( csvIn.GetInt(0) )
		{
		case 0:	//HEADER
			
			if ( TRUE == bInclude )
			{
				ProcessTransaction();
			}
			
			ResetTransaction();
			bInclude = FALSE;
					
			if ( csvIn.GetInt(1) == 1 )
			{
				CString strCheckDate = infoFile.m_strDateTran;
				CString strCheckTime = csvIn.GetString(2);
	
				if ( SimpleTimeCheck ( infoFile.m_nLocIdx, strCheckDate, strCheckTime ) == TRUE )
				{
					bInclude = TRUE;

					COleDateTime oleDate = COleDateTime (
					atoi(strCheckDate.Left(4)),
					atoi(strCheckDate.Mid(4,2)),
					atoi(strCheckDate.Right(2)),
					0, 0, 0 );

					m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
					m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );

					if ( strCheckDate != strLastCheckDate )
					{
						nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strCheckDate );
						strLastCheckDate = strCheckDate;
					}

					m_CashRSPVersionChecker.ForceSetVersionInfo( csvIn.GetInt(3), csvIn.GetInt(4) );
				}
			}
			break;
		
		case 1: //ITEM
			if ( TRUE == bInclude )
			{
				__int64 nPluNo = -1;
				int nDeptNo = 0;
				int nGroupNo = 0;
				double dVal = 0.0;
				int nTaxBand = 0;
				
				int nLineType = csvIn.GetInt(1);
				int nLineVer = csvIn.GetInt(2);

				if ( 1 == nLineVer )
				{
					switch( nLineType )
					{
					case CSVPLUDATA_PLU_SALE:
						nPluNo = csvIn.GetInt64(3);
						nDeptNo = csvIn.GetInt(4);
						nGroupNo = csvIn.GetInt(5);
						dVal = csvIn.GetDouble(6);
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
						nPluNo = csvIn.GetInt64(3);
						nDeptNo = csvIn.GetInt(4);
						nGroupNo = csvIn.GetInt(5);
						dVal = csvIn.GetDouble(6);
						dVal *= -1;
						break;

					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
						nPluNo = GetSeparatedPromoPluNo( csvIn.GetInt64(3) );
						nDeptNo = csvIn.GetInt(4);
						nGroupNo = csvIn.GetInt(5);
						dVal = csvIn.GetDouble(6);
						dVal *= -1;
						break;

					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						nPluNo = GetSeparatedMixMatchPluNo( csvIn.GetInt64(3) );
						nDeptNo = csvIn.GetInt(4);
						nGroupNo = csvIn.GetInt(5);
						dVal = csvIn.GetDouble(6);
						dVal *= -1;
						break;
					
					case CSVPLUDATA_PLU_MIXMATCH:
						nPluNo = csvIn.GetInt64(3);
						nDeptNo = csvIn.GetInt(4);
						nGroupNo = csvIn.GetInt(5);
						dVal = csvIn.GetDouble(6);
						dVal *= -1;
						break;
					}
				}

				if ( 1 == nLineVer )
				{
					switch( nLineType )
					{
					case CSVPLUDATA_PLU_SALE:
					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PLU_MIXMATCH:
					
						if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
						{
							nTaxBand = ReportHelpers.GetNumericTaxBandFromString( csvIn.GetString(8) );
						}
						else
						{
							CSQLPluNoInfo infoPluNo;
							infoPluNo.m_nEposPluNo = csvIn.GetInt64(3);
							::ProcessPluNo( infoPluNo, FALSE, TRUE );

							nTaxBand = REPORT_TAXBAND_UNKNOWN_PLU;

							if ( infoPluNo.m_bValid )
							{
								m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo  );
								
								if ( m_PluInfoFinder.PluExists() == TRUE )
								{
									nTaxBand = m_PluInfoFinder.GetModifierTaxBand( infoPluNo.m_nModifier );
								}
							}
						}

						break;

					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						nTaxBand = REPORT_TAXBAND_NONTAX_DISCOUNT;
						break;
					}
				}
			
				if ( nPluNo != -1 )
				{
					ConsolidatePluNo( nPluNo, nDeptNo, nGroupNo, nTaxDateCode, nTaxBand, dVal );
				}
			}
			break;

		case 2: //PAYMENT 
			if ( TRUE == bInclude )
			{
				CCSVEposTermLine csvPayment ( strBuffer );

				int nPaymentNo = csvPayment.GetPaymentNumber();

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
					if ( SysInfo.IsPMSSystem() == TRUE )
						if ( m_PMSModes.GetEposFlag() == FALSE )
							if ( DataManager.Payment.IsPMSPaymentType( nPaymentNo ) == TRUE )
								bInclude = FALSE;
					break;
				}

				if ( TRUE == bInclude )
				{
					double dAmount = ( ( double ) csvPayment.GetPaymentAmount() ) / 100.0;
							
					int nType, nDummy32;
					__int64 nDummy64;
					bool bIsCash;
					csvPayment.GetPaymentAccountTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64, bIsCash );

					if ( ( TRUE == bIsCash ) && ( EcrmanOptions.GetReportsMultiCashMethod() == REPORTCASH_METHOD1_CONSOLIDATE ) )
						nPaymentNo = 1;
						
					CPaymentByTaxBandBufferPayment info;
					info.m_nPaymentNo = nPaymentNo;
					info.m_dVal = dAmount;
					m_ConsolBufferPayment.Consolidate( info );

					if ( csvPayment.GetPaymentCashback() != 0 )
						m_bTransactionCashbackFlag = TRUE;
				}
			}
			break;

		case 3:
			if ( TRUE == bInclude )
			{
				if ( ( csvIn.GetInt(1) == 1 ) && ( CASHRSP_ITEMPAYTYPE_NORMAL == m_nTransactionRAType ) )
				{
					int nNewType = csvIn.GetInt(2);
					switch( nNewType )
					{
					case CASHRSP_ITEMPAYTYPE_DEPOSIT:	
					case CASHRSP_ITEMPAYTYPE_CUSTOMER:		
					case CASHRSP_ITEMPAYTYPE_ROOM:			
					case CASHRSP_ITEMPAYTYPE_LOYALTY:		
					case CASHRSP_ITEMPAYTYPE_SMARTPAY:		
					case CASHRSP_ITEMPAYTYPE_SMARTPHONE:	
					case CASHRSP_ITEMPAYTYPE_SPTBOOK:	
						m_nTransactionRAType = nNewType;
						break;
					}
				}
			}
			break;
		}
	}

	nProgress += fileSales.GetLength();
	fileSales.Close();

	if ( TRUE == bInclude )
	{
		ProcessTransaction();
	}
	
	ResetTransaction();
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ConsolidatePluNo( __int64 nPluNo, int nDeptNo, int nGroupNo, int nTaxDateCode, int nTaxBand, double dVal )
{
	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = nPluNo;
	::ProcessPluNo( infoPluNo, FALSE, TRUE );
		
	bool bFilter = FALSE;
	bFilter |= ( m_ReportFilters.CheckPlu( infoPluNo ) == FALSE );
	bFilter |= ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE );

	nGroupNo = nGroupNo - 1;
	if ( nGroupNo < 0 )
	{
		nGroupNo = 0;
	}
	
	CPaymentByTaxBandBufferSale infoItem;

	infoItem.m_nTaxBand = nTaxBand;
	infoItem.m_bFiltered = bFilter;
	infoItem.m_dVal = dVal;
	infoItem.m_dTax = 0.0;

	if ( ( nTaxBand >= 1 ) && ( nTaxBand <= MAX_TAX_BANDS ) )
	{
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, nTaxBand );
		infoItem.m_dTax = infoItem.m_dVal - ReportHelpers.CalcNonTax ( infoItem.m_dVal, dTaxRate );		
	}
	
	m_ConsolBufferSale.Consolidate( infoItem );
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ResetTransaction()
{
	m_ConsolBufferPayment.RemoveAll();
	m_ConsolBufferSale.RemoveAll();
	m_bTransactionCashbackFlag = FALSE;
	m_nTransactionRAType = CASHRSP_ITEMPAYTYPE_NORMAL;
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ProcessTransaction()
{
	//HANDLE AS EXCEPTION IF TRANSACTION HAS RA
	if ( CASHRSP_ITEMPAYTYPE_NORMAL != m_nTransactionRAType )
	{
		int nExceptionType = 0;
		switch( m_nTransactionRAType )
		{
		case CASHRSP_ITEMPAYTYPE_DEPOSIT:		nExceptionType = 1;	break;
		case CASHRSP_ITEMPAYTYPE_CUSTOMER:		nExceptionType = 2;	break;
		case CASHRSP_ITEMPAYTYPE_ROOM:			nExceptionType = 3;	break;
		case CASHRSP_ITEMPAYTYPE_LOYALTY:		nExceptionType = 4;	break;
		case CASHRSP_ITEMPAYTYPE_SMARTPAY:		nExceptionType = 5;	break;
		case CASHRSP_ITEMPAYTYPE_SMARTPHONE:	nExceptionType = 6;	break;
		case CASHRSP_ITEMPAYTYPE_SPTBOOK:		nExceptionType = 7;	break;
		default:								nExceptionType = 8;	break;
		}

		ProcessTransactionException( nExceptionType );
		return;
	}

	//HANDLE AS EXCEPTION IF TRANSACTION INCLUDES CASHBACK
	if ( TRUE == m_bTransactionCashbackFlag )
	{
		ProcessTransactionException( 9 );
		return;
	}

	//CALCULATE PAYMENT TOTAL
	double dPaymentTotal = 0.0;
	for ( int n = 0; n < m_ConsolBufferPayment.GetSize(); n++ )
	{
		CPaymentByTaxBandBufferPayment item;
		m_ConsolBufferPayment.GetAt( n, item );
		dPaymentTotal += item.m_dVal;
	}

	//CALCULATE ITEM TOTAL
	double dItemTotal = 0.0;
	double dTaxTotal = 0.0;
	for ( int n = 0; n < m_ConsolBufferSale.GetSize(); n++ )
	{
		CPaymentByTaxBandBufferSale item;
		m_ConsolBufferSale.GetAt( n, item );
		dItemTotal += item.m_dVal;
		dTaxTotal += item.m_dTax;
	}

	//HANDLE AS EXCEPTION IF ITEM TOTAL DIFFERS FROM PAYMENT TOTAL
	if ( CPriceHelpers::CompareDoubles( dItemTotal, dPaymentTotal, 3 ) != 0 )
	{
		ProcessTransactionException( 10 );
		return;
	}

	//HANDLE AS EXCEPTION IF ITEM TOTAL IS ZERO
	if ( CPriceHelpers::CompareDoubles( dItemTotal, 0.0, 3 ) == 0 )
	{
		ProcessTransactionException( 11 );
		return;
	}

	//ASSIGN PAYMENTS TO TAX BANDS BY RATIO TO TAX BAND TOTAL
	for ( int p = 0; p < m_ConsolBufferPayment.GetSize(); p++ )
	{
		CPaymentByTaxBandBufferPayment infoPayment;
		m_ConsolBufferPayment.GetAt( p, infoPayment );

		for ( int g = 0; g < m_ConsolBufferSale.GetSize(); g++ )
		{
			CPaymentByTaxBandBufferSale infoSale;
			m_ConsolBufferSale.GetAt( g, infoSale );
				
			double dTaxBandPayment = 0.0;
			double dTaxBandTax = 0.0;
			
			if ( 1 == m_ConsolBufferSale.GetSize() )
			{
				dTaxBandPayment = infoPayment.m_dVal;
			}
			else 	
			{
				dTaxBandPayment = ( infoPayment.m_dVal * infoSale.m_dVal ) / dItemTotal;
			}
				
			if ( CPriceHelpers::CompareDoubles( dTaxTotal, 0.0, 3 ) != 0 )
			{
				dTaxBandTax = ( infoPayment.m_dVal * infoSale.m_dTax ) / dItemTotal;
			}
			
			if ( FALSE == infoSale.m_bFiltered )
			{
				bool bPushDown = ( ( infoSale.m_nTaxBand < 1 ) || ( infoSale.m_nTaxBand > MAX_TAX_BANDS ) );
				ConsolidatePaymentTax( infoPayment.m_nPaymentNo, infoSale.m_nTaxBand, bPushDown, dTaxBandPayment, dTaxBandTax );
			}
		}
	}	
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ProcessTransactionException( int nType )
{
	for ( int n = 0; n < m_ConsolBufferSale.GetSize(); n++ )
	{
		CPaymentByTaxBandBufferSale item;
		m_ConsolBufferSale.GetAt( n, item );

		bool bInclude = ( 12 == nType ) ? ( TRUE == item.m_bFiltered ) : ( FALSE == item.m_bFiltered );

		if ( TRUE == bInclude )
		{
			CEposReportConsolPBT_TotalBlock Total;
			Total.m_dVal = item.m_dVal;
			Total.m_dTax = item.m_dTax;

			for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
			{
				int nBlock = m_arrayTerminalBlocks.GetAt(n);
			
				Total.m_nBlockType = nType;
				Total.m_nTaxBand = ( 12 == nType ) ? 0 : item.m_nTaxBand;
				m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( Total );
				
				if ( 12 != nType )
				{
					Total.m_nBlockType = PBT_TOTALBLOCK_TAXBAND;
					m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( Total );
				}

				Total.m_nBlockType = PBT_TOTALBLOCK_ALL;
				Total.m_nTaxBand = 0;
				m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( Total );			
			}
		}
	}
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::ConsolidatePaymentTax( int nPaymentNo, int nTaxBand, bool bPushDown, double dPayment, double dTax )
{
	CEposReportConsolPBT_MainBlock item;
	item.m_nPaymentNo = nPaymentNo;
	item.m_bPushDown = bPushDown;
	item.m_dVal = dPayment;
	item.m_dTax = dTax;
	
	CEposReportConsolPBT_TotalBlock total;
	total.m_dVal = dPayment;
	total.m_dTax = dTax;
	
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);
		
		item.m_bTotal = FALSE;
		item.m_nTaxBand = nTaxBand;
		m_arrayReportBlocks[ nBlock ].m_pPayments -> Consolidate( item );
			
		item.m_bTotal = TRUE;
		item.m_nTaxBand = 0;
		m_arrayReportBlocks[ nBlock ].m_pPayments -> Consolidate( item );

		total.m_nBlockType = PBT_TOTALBLOCK_PAYMENT;
		total.m_nTaxBand = nTaxBand;
		m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( total );

		total.m_nBlockType = PBT_TOTALBLOCK_TAXBAND;
		m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( total );

		total.m_nBlockType = PBT_TOTALBLOCK_ALL;
		total.m_nTaxBand = 0;
		m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( total );
	}
}

/**********************************************************************/

bool CEposReportPaymentByTaxBand::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;
	
	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Name", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( EcrmanOptions.GetTaxBandString(), TA_LEFT, 400 );	
	m_ReportFile.AddColumn ( "Value", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 800 );
	
	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPaymentByTaxBandBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.GetLineCount();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPaymentByTaxBandBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines(1);
		
		CreateSalesSection( strHeader, ReportBlock );
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;	
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::CreateSalesSection( CString& strHeader, CEposReportPaymentByTaxBandBlock& Block )
{
	//NORMAL PAYMENTS
	{
		int nPaymentTypes = 0;
		int nCurrentPaymentNo = -1;
		int nMiniBlockStart = 0;
		
		int nSize = Block.m_pPayments -> GetSize();
		for ( int s = 0; s < nSize; s++ )
		{
			CEposReportConsolPBT_MainBlock infoPayment;
			Block.m_pPayments -> GetAt( s, infoPayment );
			UpdateCreateReportProgress();

			if ( infoPayment.m_nPaymentNo != nCurrentPaymentNo )
			{
				if ( nCurrentPaymentNo != -1 )
				{
					CreatePaymentSection( strHeader, Block.m_pPayments, nMiniBlockStart, s - 1 );
					nPaymentTypes++;
				}
				
				nCurrentPaymentNo = infoPayment.m_nPaymentNo;
				nMiniBlockStart = s;
			}
		}

		if ( nMiniBlockStart < nSize )
		{
			CreatePaymentSection( strHeader, Block.m_pPayments, nMiniBlockStart, nSize - 1 );
			nPaymentTypes++;
		}

		CreateTotalsSection( strHeader, Block.m_pTotals );
	}
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::CreatePaymentSection( CString& strHeader, CReportConsolidationArray<CEposReportConsolPBT_MainBlock>* pPayments, int nBlockStart, int nBlockEnd )
{
	CEposReportConsolPBT_MainBlock infoPayment;
	pPayments -> GetAt( nBlockStart, infoPayment );

	CString	strPayNo;
	strPayNo.Format( ", P%3.3d, ", infoPayment.m_nPaymentNo );
	
	CString	strPayNameHeader = DataManager.Payment.GetDisplayName( infoPayment.m_nPaymentNo );
	CString	strPayNameLine = strPayNameHeader;
	
	CString strLine = "";
	strLine += strHeader;
	strLine += strPayNo;
	strLine += strPayNameHeader;

	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( strLine );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	for ( int s = nBlockStart; s <= nBlockEnd; s++ )
	{
		CEposReportConsolPBT_MainBlock infoPayment;
		pPayments -> GetAt( s, infoPayment );

		CCSV csv ('\t' );
		csv.Add ( strPayNameLine );

		bool bShowLine = FALSE;
		
		if ( FALSE == infoPayment.m_bTotal )
		{
			csv.Add( ReportHelpers.GetTaxBandString( infoPayment.m_nTaxBand ) );
		}
		else
		{
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			csv.Add( "Total" );
		}
	
		csv.Add( infoPayment.m_dVal, SysInfo.GetDPValue() );
		csv.Add( infoPayment.m_dTax, SysInfo.GetDPValue() );

		m_ReportFile.WriteReportDataLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CEposReportPaymentByTaxBand::CreateTotalsSection( CString& strHeader, CReportConsolidationArray<CEposReportConsolPBT_TotalBlock>* pTotals )
{
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( strHeader + ", Totals" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	int nLastBlockType = -1;
	int nBlockTypeCount = 0;
	CString strInfoPrefix = "<..>    ";

	for ( int s = 0; s < pTotals -> GetSize(); s++ )
	{
		CEposReportConsolPBT_TotalBlock infoTotal;
		pTotals -> GetAt( s, infoTotal );

		if  ( infoTotal.m_nBlockType != nLastBlockType )
		{
			if ( nLastBlockType != -1 )
			{
				if ( ( PBT_TOTALBLOCK_TAXBAND != infoTotal.m_nBlockType ) || ( nBlockTypeCount > 1 ) )
				{
					m_ReportFile.WriteReportMiscLine( "<LI>" );
				}
			}

			nLastBlockType = infoTotal.m_nBlockType;

			if ( PBT_TOTALBLOCK_TAXBAND != infoTotal.m_nBlockType )
			{
				nBlockTypeCount++;
			}
		}

		if ( ( PBT_TOTALBLOCK_TAXBAND == infoTotal.m_nBlockType ) && ( nBlockTypeCount < 2 ) )
		{
			continue;
		}

		CCSV csv ('\t' );
		csv.Add ( "" );

		switch( infoTotal.m_nBlockType )
		{
		case PBT_TOTALBLOCK_ALL:
			csv.Add( "Total" );
			break;

		case 12:
			csv.Add( EcrmanOptions.GetTaxString( "All %T Bands" ) );
			break;

		default:
			csv.Add( ReportHelpers.GetTaxBandString( infoTotal.m_nTaxBand ) );
			break;
		}
		
		csv.Add( infoTotal.m_dVal, SysInfo.GetDPValue() );
		csv.Add( infoTotal.m_dTax, SysInfo.GetDPValue() );

		{
			CString strInfo = "";

			switch( infoTotal.m_nBlockType )
			{
			case 1:		strInfo = "Transactions with deposit purchases";					break;
			case 2:		strInfo = "Transactions with customer payment on account";			break;
			case 3:		strInfo = "Transactions with room settlement";						break;
			case 4:		strInfo = "Transactions with Loyalty top-up";						break;
			case 5:		strInfo = "Transactions with SmartPay top-up";						break;
			case 6:		strInfo = "Transactions with mySmartPhoneMoney top-up";				break;
			case 7:		strInfo = "Transactions with Sports Booker payment on account";		break;
			case 8:		strInfo = "Transactions with unknown RA type";						break;
			case 9:		strInfo = "Transactions with cashback";								break;	
			case 10:	strInfo = "Payment total not equal to item total";					break;
			case 11:	strInfo = "Transactions with zero item value";						break;
			
			case PBT_TOTALBLOCK_PAYMENT:		
				strInfo = "Allocated to payment";										
				break;
			
			case PBT_TOTALBLOCK_TAXBAND:
				strInfo = EcrmanOptions.GetTaxString( "Total for %T Band" );
				break;
			}

			if ( strInfo != "" )
			{
				csv.Add( strInfoPrefix + strInfo );
			}
		}

		m_ReportFile.WriteReportMiscLine( csv.GetLine() );
	}
}

/**********************************************************************/
