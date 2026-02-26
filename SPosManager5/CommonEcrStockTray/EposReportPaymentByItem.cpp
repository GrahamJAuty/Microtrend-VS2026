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
#include "EposReportPaymentByItem.h"
/**********************************************************************/

CEposReportPaymentByItem::CEposReportPaymentByItem( CEposSelectArray& SelectArray, int nReportType ) : CEposReport( SelectArray )
{
	m_bTransactionCashbackFlag = FALSE;
	m_nTransactionRAType = 0;
	m_bGotPayMethGoodLine = FALSE;
	m_bGotPayMethFilteredItem = FALSE;
	m_bGotNormalBlockGoodLine = FALSE;
	m_bGotNormalBlockFilteredItem = FALSE;
	m_bGotChargeBlockGoodLine = FALSE;
	m_bGotChargeBlockFilteredItem = FALSE;

	switch( nReportType )
	{
	case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
	case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
	case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
		m_nReportType = nReportType;
		break;

	default:
		m_nReportType = REPORT_TYPE_PAYMENT_ESTIMATE_GROUP;
		break;
	}
		
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
	SetPMSDepositsFlag( TRUE );
}

/**********************************************************************/

CEposReportPaymentByItem::~CEposReportPaymentByItem()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pPaymentsNormal -> RemoveAll();
		m_arrayReportBlocks[n].m_pTotalsNormal -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsCharge -> RemoveAll();
		m_arrayReportBlocks[n].m_pTotalsCharge -> RemoveAll();
		
		delete ( m_arrayReportBlocks[n].m_pPaymentsNormal );
		delete ( m_arrayReportBlocks[n].m_pTotalsNormal );
		delete ( m_arrayReportBlocks[n].m_pPaymentsCharge );
		delete ( m_arrayReportBlocks[n].m_pTotalsCharge );
	}
}

/**********************************************************************/

void CEposReportPaymentByItem::InitialiseConsolidationBlocks()
{
	bool bAllowGlobal = ( ( NODE_SYSTEM == m_nConLevel ) && ( EcrmanOptions.GetGlobalPaymentFlag() ) );

	switch( m_nReportType )
	{
	case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
		bAllowGlobal &= EcrmanOptions.GetGlobalDepartmentFlag();
		break;

	case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
		bAllowGlobal &= EcrmanOptions.GetGlobalDeptGroupEposFlag();
		break;

	case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
		bAllowGlobal = FALSE;
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

		CEposReportPaymentByItemBlock block;
		block.Copy( entity );
		block.m_pPaymentsNormal = new CReportConsolidationArray<CEposReportConsolPBI_MainBlock>;
		block.m_pTotalsNormal = new CReportConsolidationArray<CEposReportConsolPBI_MainBlock>;
		block.m_pPaymentsCharge = new CReportConsolidationArray<CEposReportConsolPBI_MainBlock>;
		block.m_pTotalsCharge = new CReportConsolidationArray<CEposReportConsolPBI_MainBlock>;
		
		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPaymentByItem::Consolidate()
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

void CEposReportPaymentByItem::ProcessPMSFile( CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize )
{
	ResetTransaction();
	
	bool bFail = FALSE;
	int nFileSize = 0;
		
	{
		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			bFail = TRUE;
		}
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
					if (m_PMSModes.CheckPMSTimeLine(TRUE, strBuffer, strDummy1, strDummy2, oleDateTimeDummy, bDummy) == FALSE)
					{
						continue;
					}

					csv.ParseLine( strBuffer );

					switch( csv.GetLineType() )
					{
					case CSVPLUDATA_PLU_SALE:
						ConsolidatePluNo( csv.GetPluNoNew(), csv.GetDeptNo(), csv.GetGroupNo(), csv.GetValue() );
						break;
					
					case CSVPLUDATA_PLU_DISCOUNT:
						ConsolidatePluNo( csv.GetPluNoNew(), csv.GetDeptNo(), csv.GetGroupNo(), -csv.GetValue() );
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
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			bFail = TRUE;
		}
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
							CPaymentByItemBufferPayment info;
							info.m_nPaymentNo = csvIn.GetPaymentNumber();
							info.m_bIsRedemptionBlock = FALSE;
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

void CEposReportPaymentByItem::ProcessEPOSFile( CTermFileListInfo& infoFile, int& nProgress, int nTotalFileSize )
{
	CSSFile fileSales;
	if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
	{
		return;
	}

	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	{
		int nDummy = 0;
		dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
	}

	ResetTransaction();
	bool bInclude = FALSE;

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

				if ( nPluNo != -1 )
				{
					ConsolidatePluNo( nPluNo, nDeptNo, nGroupNo, dVal );
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
					if (SysInfo.IsPMSSystem() == TRUE)
					{
						if (m_PMSModes.GetEposFlag() == FALSE)
						{
							if (DataManager.Payment.IsPMSPaymentType(nPaymentNo) == TRUE)
							{
								bInclude = FALSE;
							}
						}
					}
					break;
				}

				if (TRUE == bInclude)
				{
					double dAmount = ((double)csvPayment.GetPaymentAmount()) / 100.0;

					int nType, nDummy32;
					__int64 nDummy64;
					bool bIsCash;
					csvPayment.GetPaymentAccountTypeAndID(m_CashRSPVersionChecker, nType, nDummy32, nDummy64, bIsCash);

					if ((TRUE == bIsCash) && (EcrmanOptions.GetReportsMultiCashMethod() == REPORTCASH_METHOD1_CONSOLIDATE))
					{
						nPaymentNo = 1;
					}

					CPaymentByItemBufferPayment info;
					info.m_nPaymentNo = nPaymentNo;
					info.m_bIsRedemptionBlock = (CASHRSP_ITEMPAYTYPE_NORMAL != nType);
					info.m_dVal = dAmount;
					m_ConsolBufferPayment.Consolidate(info);

					if (csvPayment.GetPaymentCashback() != 0)
					{
						m_bTransactionCashbackFlag = TRUE;
					}
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

void CEposReportPaymentByItem::ConsolidatePluNo( __int64 nPluNo, int nDeptNo, int nGroupNo, double dVal )
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
	
	CPaymentByItemBufferSale infoItem;

	switch( m_nReportType )
	{
	case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
		infoItem.m_nPluNo = 0;
		infoItem.m_nAuxNo = ( bFilter ) ? -1 : nDeptNo;
		break;

	case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
		infoItem.m_nPluNo = ( bFilter ) ? -1 : nPluNo;
		infoItem.m_nAuxNo = 0;
		break;

	case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
	default:
		infoItem.m_nPluNo = 0;
		infoItem.m_nAuxNo = ( bFilter ) ? -1 : nGroupNo;
		break;
	}

	infoItem.m_dVal = dVal;
	
	m_ConsolBufferSale.Consolidate( infoItem );
}

/**********************************************************************/

void CEposReportPaymentByItem::ResetTransaction()
{
	m_ConsolBufferPayment.RemoveAll();
	m_ConsolBufferSale.RemoveAll();
	m_bTransactionCashbackFlag = FALSE;
	m_nTransactionRAType = CASHRSP_ITEMPAYTYPE_NORMAL;

	m_bGotPayMethGoodLine = FALSE;
	m_bGotPayMethFilteredItem = FALSE;
	m_bGotNormalBlockGoodLine = FALSE;
	m_bGotNormalBlockFilteredItem = FALSE;
	m_bGotChargeBlockGoodLine = FALSE;
	m_bGotChargeBlockFilteredItem = FALSE;

	m_arrayTransactionItemsNormal.RemoveAll();
	m_arrayTransactionItemsCharge.RemoveAll();
}

/**********************************************************************/

void CEposReportPaymentByItem::ProcessTransaction()
{
	m_bGotPayMethGoodLine = FALSE;
	m_bGotPayMethFilteredItem = FALSE;
	m_bGotNormalBlockGoodLine = FALSE;
	m_bGotNormalBlockFilteredItem = FALSE;
	m_bGotChargeBlockGoodLine = FALSE;
	m_bGotChargeBlockFilteredItem = FALSE;

	m_arrayTransactionItemsNormal.RemoveAll();
	m_arrayTransactionItemsCharge.RemoveAll();

	if (m_ConsolBufferPayment.GetSize() == 0)
	{
		return;
	}

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
		}

		if ( nExceptionType != 0 )
		{
			ProcessTransactionException( nExceptionType );
		}

		return;
	}

	//HANDLE AS EXCEPTION IF TRANSACTION INCLUDES CASHBACK
	if ( TRUE == m_bTransactionCashbackFlag )
	{
		ProcessTransactionException( 8 );
		return;
	}

	//CALCULATE ITEM TOTAL
	double dItemTotal = 0.0;
	for ( int n = 0; n < m_ConsolBufferSale.GetSize(); n++ )
	{
		CPaymentByItemBufferSale item;
		m_ConsolBufferSale.GetAt( n, item );
		dItemTotal += item.m_dVal;
	}

	//CALCULATE PAYMENT TOTAL
	double dPaymentTotal = 0.0;
	for ( int n = 0; n < m_ConsolBufferPayment.GetSize(); n++ )
	{
		CPaymentByItemBufferPayment item;
		m_ConsolBufferPayment.GetAt( n, item );
		dPaymentTotal += item.m_dVal;
	}

	//HANDLE AS EXCEPTION IF ITEM TOTAL DIFFERS FROM PAYMENT TOTAL
	if ( CPriceHelpers::CompareDoubles( dItemTotal, dPaymentTotal, 3 ) != 0 )
	{
		ProcessTransactionException( 9 );
		return;
	}

	//IF ITEM TOTAL IS ZERO THEN HANDLE AS UNASSIGNED PAYMENT
	if ( CPriceHelpers::CompareDoubles( dItemTotal, 0.0, 3 ) == 0 )
	{
		return;
	}

	//ASSIGN PAYMENTS TO GROUPS BY RATIO TO GROUP TOTAL
	for ( int p = 0; p < m_ConsolBufferPayment.GetSize(); p++ )
	{
		CPaymentByItemBufferPayment infoPayment;
		m_ConsolBufferPayment.GetAt( p, infoPayment );

		m_bGotPayMethGoodLine = FALSE;
		m_bGotPayMethFilteredItem = FALSE;

		for ( int g = 0; g < m_ConsolBufferSale.GetSize(); g++ )
		{
			CPaymentByItemBufferSale infoSale;
			m_ConsolBufferSale.GetAt( g, infoSale );
				
			double dGroupPayment = 0.0;
			
			if ( 1 == m_ConsolBufferSale.GetSize() )
			{
				dGroupPayment = infoPayment.m_dVal;
			}
			else 	
			{
				dGroupPayment = ( infoPayment.m_dVal * infoSale.m_dVal ) / dItemTotal;
			}
				
			__int64 nItemNo = ( REPORT_TYPE_PAYMENT_ESTIMATE_ITEM == m_nReportType ) ? infoSale.m_nPluNo : infoSale.m_nAuxNo;
			
			if ( nItemNo != -1 )
			{
				bool bIsZeroItem = FALSE;
				switch( m_nReportType )
				{
				case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
				case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
					bIsZeroItem = ( 0 == nItemNo );
					break;
				}

				ConsolidatePayment( infoPayment, bIsZeroItem, nItemNo, dGroupPayment );
			}
			else
			{
				ConsolidateException( infoPayment, 10, dGroupPayment );
			}
		}
	}
}

/**********************************************************************/

void CEposReportPaymentByItem::ProcessTransactionException( int nType )
{
	for ( int p = 0; p < m_ConsolBufferPayment.GetSize(); p++ )
	{
		m_bGotPayMethGoodLine = FALSE;
		m_bGotPayMethFilteredItem = FALSE;
		CPaymentByItemBufferPayment infoPayment;
		m_ConsolBufferPayment.GetAt( p, infoPayment );
		ConsolidateException( infoPayment, nType, infoPayment.m_dVal );
	}
}

/**********************************************************************/

void CEposReportPaymentByItem::ConsolidatePayment( CPaymentByItemBufferPayment& infoPayment, bool bZeroItem, __int64 nItem, double dVal)
{
	if (CPriceHelpers::CompareDoubles(infoPayment.m_dVal, 0.0, 5) == 0)
	{
		return;
	}

	bool bNewTranForPayMethAllocated = (FALSE == m_bGotPayMethGoodLine);
	bool bNewTranForPayMethOverall = (FALSE == m_bGotPayMethGoodLine) && (FALSE == m_bGotPayMethFilteredItem);
	m_bGotPayMethGoodLine = TRUE;

	bool bNewTranForTotalAllocated = TRUE;
	bool bNewTranForTotalOverall = TRUE;
	if (FALSE == infoPayment.m_bIsRedemptionBlock)
	{
		bNewTranForTotalAllocated = (FALSE == m_bGotNormalBlockGoodLine);
		bNewTranForTotalOverall = (FALSE == m_bGotNormalBlockGoodLine) && (FALSE == m_bGotNormalBlockFilteredItem);
		m_bGotNormalBlockGoodLine = TRUE;
	}
	else
	{
		bNewTranForTotalAllocated = (FALSE == m_bGotChargeBlockGoodLine);
		bNewTranForTotalOverall = (FALSE == m_bGotChargeBlockGoodLine) && (FALSE == m_bGotChargeBlockFilteredItem);
		m_bGotChargeBlockGoodLine = TRUE;
	}

	CEposReportConsolPBI_MainBlock infoPaymentItem;
	infoPaymentItem.m_nPaymentNo = infoPayment.m_nPaymentNo;
	infoPaymentItem.m_nPaymentBlock = PBI_MAINBLOCK_LINE_ITEM;
	infoPaymentItem.m_bIsZeroItem = bZeroItem;
	infoPaymentItem.m_nItemNumber = nItem;
	infoPaymentItem.CountAsTran(TRUE);
	infoPaymentItem.m_dVal = dVal;

	CEposReportConsolPBI_MainBlock infoPaymentTotal1;
	infoPaymentTotal1.m_nPaymentNo = infoPayment.m_nPaymentNo;
	infoPaymentTotal1.m_nPaymentBlock = PBI_MAINBLOCK_TOTAL_ITEM;
	infoPaymentTotal1.m_bIsZeroItem = FALSE;
	infoPaymentTotal1.m_nItemNumber = 0;
	infoPaymentTotal1.CountAsTran(bNewTranForPayMethAllocated);
	infoPaymentTotal1.m_dVal = dVal;

	CEposReportConsolPBI_MainBlock infoPaymentTotal2;
	infoPaymentTotal2.m_nPaymentNo = infoPayment.m_nPaymentNo;
	infoPaymentTotal2.m_nPaymentBlock = PBI_MAINBLOCK_TOTAL_ALL;
	infoPaymentTotal2.m_bIsZeroItem = FALSE;
	infoPaymentTotal2.m_nItemNumber = 0;
	infoPaymentTotal2.CountAsTran(bNewTranForPayMethOverall);
	infoPaymentTotal2.m_dVal = dVal;

	bool bNewTransactionItem = TRUE;

	if (FALSE == infoPayment.m_bIsRedemptionBlock)
	{
		CSortedInt64Item item;
		item.m_nItem = nItem;

		int nPos = 0;
		if (m_arrayTransactionItemsNormal.Find(item, nPos) == TRUE)
		{
			bNewTransactionItem = FALSE;
		}
		else
		{
			m_arrayTransactionItemsNormal.InsertAt(nPos, item);
		}
	}
	else
	{
		CSortedInt64Item item;
		item.m_nItem = nItem;

		int nPos = 0;
		if (m_arrayTransactionItemsCharge.Find(item, nPos) == TRUE)
		{
			bNewTransactionItem = FALSE;
		}
		else
		{
			m_arrayTransactionItemsCharge.InsertAt(nPos, item);
		}
	}

	for (int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++)
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		if (FALSE == infoPayment.m_bIsRedemptionBlock)
		{
			m_arrayReportBlocks[nBlock].m_pPaymentsNormal->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pPaymentsNormal->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pPaymentsNormal->Consolidate(infoPaymentTotal2);
		}
		else
		{
			m_arrayReportBlocks[nBlock].m_pPaymentsCharge->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pPaymentsCharge->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pPaymentsCharge->Consolidate(infoPaymentTotal2);
		}

		{
			infoPaymentItem.m_nPaymentNo = 0;
			infoPaymentTotal1.m_nPaymentNo = 0;
			infoPaymentTotal2.m_nPaymentNo = 0;		
			infoPaymentItem.CountAsTran(bNewTransactionItem);
			infoPaymentTotal1.CountAsTran(bNewTranForTotalAllocated);
			infoPaymentTotal2.CountAsTran(bNewTranForTotalOverall);
		}

		if (FALSE == infoPayment.m_bIsRedemptionBlock)
		{
			m_arrayReportBlocks[nBlock].m_pTotalsNormal->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pTotalsNormal->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pTotalsNormal->Consolidate(infoPaymentTotal2);
		}
		else
		{
			m_arrayReportBlocks[nBlock].m_pTotalsCharge->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pTotalsCharge->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pTotalsCharge->Consolidate(infoPaymentTotal2);
		}

		{
			infoPaymentItem.m_nPaymentNo = infoPayment.m_nPaymentNo;
			infoPaymentTotal1.m_nPaymentNo = infoPayment.m_nPaymentNo;
			infoPaymentTotal2.m_nPaymentNo = infoPayment.m_nPaymentNo;
			infoPaymentItem.CountAsTran(TRUE);
			infoPaymentTotal1.CountAsTran(bNewTranForPayMethAllocated);
			infoPaymentTotal2.CountAsTran(bNewTranForPayMethOverall);
		}
	}
}

/**********************************************************************/

void CEposReportPaymentByItem::ConsolidateException(CPaymentByItemBufferPayment& infoPayment, int nExceptionType, double dVal)
{
	if (CPriceHelpers::CompareDoubles(infoPayment.m_dVal, 0.0, 5) == 0)
	{
		return;
	}

	bool bNewPaymentTran = TRUE;
	bool bNewOverallTran = TRUE;

	if (10 == nExceptionType)
	{
		bNewPaymentTran = (FALSE == m_bGotPayMethFilteredItem);
		m_bGotPayMethFilteredItem = TRUE;

		if (FALSE == infoPayment.m_bIsRedemptionBlock)
		{
			bNewOverallTran = ( FALSE == m_bGotNormalBlockGoodLine ) && (FALSE == m_bGotNormalBlockFilteredItem);
			m_bGotNormalBlockFilteredItem = TRUE;
		}
		else
		{
			bNewOverallTran = (FALSE == m_bGotChargeBlockGoodLine) && (FALSE == m_bGotChargeBlockFilteredItem);
			m_bGotChargeBlockFilteredItem = TRUE;
		}
	}

	CEposReportConsolPBI_MainBlock infoPaymentItem;
	infoPaymentItem.m_nPaymentNo = infoPayment.m_nPaymentNo;
	infoPaymentItem.m_nPaymentBlock = PBI_MAINBLOCK_LINE_EXCEPTION;
	infoPaymentItem.m_bIsZeroItem = FALSE;
	infoPaymentItem.m_nItemNumber = nExceptionType;
	infoPaymentItem.CountAsTran(bNewPaymentTran);
	infoPaymentItem.m_dVal = dVal;

	CEposReportConsolPBI_MainBlock infoPaymentTotal1;
	infoPaymentTotal1.m_nPaymentNo = infoPayment.m_nPaymentNo;
	infoPaymentTotal1.m_nPaymentBlock = PBI_MAINBLOCK_TOTAL_EXCEPTION;
	infoPaymentTotal1.m_bIsZeroItem = FALSE;
	infoPaymentTotal1.m_nItemNumber = 0;
	infoPaymentTotal1.CountAsTran(bNewPaymentTran);
	infoPaymentTotal1.m_dVal = dVal;

	CEposReportConsolPBI_MainBlock infoPaymentTotal2;
	infoPaymentTotal2.m_nPaymentNo = infoPayment.m_nPaymentNo;
	infoPaymentTotal2.m_nPaymentBlock = PBI_MAINBLOCK_TOTAL_ALL;
	infoPaymentTotal2.m_bIsZeroItem = FALSE;
	infoPaymentTotal2.m_nItemNumber = 0;
	infoPaymentTotal2.CountAsTran(bNewPaymentTran);
	infoPaymentTotal2.m_dVal = dVal;

	for (int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++)
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n);

		if (FALSE == infoPayment.m_bIsRedemptionBlock)
		{
			m_arrayReportBlocks[nBlock].m_pPaymentsNormal->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pPaymentsNormal->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pPaymentsNormal->Consolidate(infoPaymentTotal2);
		}
		else
		{
			m_arrayReportBlocks[nBlock].m_pPaymentsCharge->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pPaymentsCharge->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pPaymentsCharge->Consolidate(infoPaymentTotal2);
		}

		{
			infoPaymentItem.m_nPaymentNo = 0;
			infoPaymentTotal1.m_nPaymentNo = 0;
			infoPaymentTotal2.m_nPaymentNo = 0;
			infoPaymentItem.CountAsTran(bNewOverallTran);
			infoPaymentTotal1.CountAsTran(bNewOverallTran);
			infoPaymentTotal2.CountAsTran(bNewOverallTran);
		}

		if (FALSE == infoPayment.m_bIsRedemptionBlock)
		{
			m_arrayReportBlocks[nBlock].m_pTotalsNormal->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pTotalsNormal->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pTotalsNormal->Consolidate(infoPaymentTotal2);
		}
		else
		{
			m_arrayReportBlocks[nBlock].m_pTotalsCharge->Consolidate(infoPaymentItem);
			m_arrayReportBlocks[nBlock].m_pTotalsCharge->Consolidate(infoPaymentTotal1);
			m_arrayReportBlocks[nBlock].m_pTotalsCharge->Consolidate(infoPaymentTotal2);
		}

		{
			infoPaymentItem.m_nPaymentNo = infoPayment.m_nPaymentNo;
			infoPaymentTotal1.m_nPaymentNo = infoPayment.m_nPaymentNo;
			infoPaymentTotal2.m_nPaymentNo = infoPayment.m_nPaymentNo;
			infoPaymentItem.CountAsTran(bNewPaymentTran);
			infoPaymentTotal1.CountAsTran(bNewPaymentTran);
			infoPaymentTotal2.CountAsTran(bNewPaymentTran);
		}
	}
}

/**********************************************************************/

bool CEposReportPaymentByItem::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
		return FALSE;

	m_ReportFile.SetStyle1(GetReportTitle(FALSE));
	m_ReportFile.AddColumn("Name", TA_LEFT, 300);

	switch (m_nReportType)
	{
	case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
		m_ReportFile.AddColumn("Plu Number", TA_LEFT, 350);
		m_ReportFile.AddColumn("Description", TA_LEFT, 500);
		break;

	case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
		m_ReportFile.AddColumn("Dept", TA_LEFT, 700);
		break;

	case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
	default:
		m_ReportFile.AddColumn("Group", TA_LEFT, 700);
		break;
	}

	m_ReportFile.AddColumn("TranQty", TA_RIGHT, 250);
	m_ReportFile.AddColumn("Value", TA_RIGHT, 250);

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for (int n = 0; n < m_arrayReportBlocks.GetSize(); n++)
	{
		CEposReportPaymentByItemBlock ReportBlock = m_arrayReportBlocks.GetAt(n);
		m_nCreateReportTarget += ReportBlock.GetLineCount();
	}

	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if (m_nCreateReportMiniTarget < 1)
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock(TRUE, "Creating report");

	m_BlockMap.BuildSortArray();
	for (int n = 0; n < m_BlockMap.GetSortArraySize(); n++)
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPaymentByItemBlock ReportBlock = m_arrayReportBlocks.GetAt(nIdx);

		if (ReportBlock.GotData() == FALSE)
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly(ReportBlock.m_nDbIdx, info, FALSE);

		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText(strHeader);
		m_ReportFile.WriteReportMiscLine(strHeader);
		m_ReportFile.WriteReportMiscLine("<LI>");
		m_ReportFile.LockBlankLines(1);

		CreateSalesSection(strHeader, ReportBlock);
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportPaymentByItem::CreateSalesSection( CString& strHeader, CEposReportPaymentByItemBlock& Block )
{
	//NORMAL PAYMENTS
	{
		int nPaymentTypes = 0;
		int nCurrentPaymentNo = -1;
		int nMiniBlockStart = 0;
		
		int nSize = Block.m_pPaymentsNormal -> GetSize();
		for ( int s = 0; s < nSize; s++ )
		{
			CEposReportConsolPBI_MainBlock infoPayment;
			Block.m_pPaymentsNormal -> GetAt( s, infoPayment );
			UpdateCreateReportProgress();

			if ( infoPayment.m_nPaymentNo != nCurrentPaymentNo )
			{
				if ( nCurrentPaymentNo != -1 )
				{
					CreatePaymentSection( strHeader, Block.m_pPaymentsNormal, nMiniBlockStart, s - 1, FALSE );
					nPaymentTypes++;
				}
				
				nCurrentPaymentNo = infoPayment.m_nPaymentNo;
				nMiniBlockStart = s;
			}
		}

		if ( nMiniBlockStart < nSize )
		{
			CreatePaymentSection( strHeader, Block.m_pPaymentsNormal, nMiniBlockStart, nSize - 1, FALSE );
			nPaymentTypes++;
		}
	
		if ( nPaymentTypes >= 2 )
			CreatePaymentSection( strHeader, Block.m_pTotalsNormal, 0, Block.m_pTotalsNormal -> GetSize() - 1, FALSE );
	}

	//REDEMPTION & CHARGE
	{
		int nPaymentTypes = 0;
		int nCurrentPaymentNo = -1;
		int nMiniBlockStart = 0;
		
		int nSize = Block.m_pPaymentsCharge -> GetSize();
		for ( int s = 0; s < nSize; s++ )
		{
			CEposReportConsolPBI_MainBlock infoPayment;
			Block.m_pPaymentsCharge -> GetAt( s, infoPayment );
			UpdateCreateReportProgress();

			if ( infoPayment.m_nPaymentNo != nCurrentPaymentNo )
			{
				if ( nCurrentPaymentNo != -1 )
				{
					CreatePaymentSection( strHeader, Block.m_pPaymentsCharge, nMiniBlockStart, s - 1, TRUE );
					nPaymentTypes++;
				}

				nCurrentPaymentNo = infoPayment.m_nPaymentNo;
				nMiniBlockStart = s;
			}
		}

		if ( nMiniBlockStart < nSize )
		{
			CreatePaymentSection( strHeader, Block.m_pPaymentsCharge, nMiniBlockStart, nSize - 1, TRUE );
			nPaymentTypes++;
		}
	
		if ( nPaymentTypes >= 2 )
			CreatePaymentSection( strHeader, Block.m_pTotalsCharge, 0, Block.m_pTotalsCharge -> GetSize() - 1 , TRUE);
	}
}

/**********************************************************************/

void CEposReportPaymentByItem::CreatePaymentSection(CString& strHeader, CReportConsolidationArray<CEposReportConsolPBI_MainBlock>* pPayments, int nBlockStart, int nBlockEnd, bool bCharge)
{
	CEposReportConsolPBI_MainBlock infoPayment;
	pPayments->GetAt(nBlockStart, infoPayment);

	CString strPayNo;
	CString strPayNameHeader;
	CString strPayNameLine;

	if (infoPayment.m_nPaymentNo != 0)
	{
		strPayNo.Format(", P%3.3d, ", infoPayment.m_nPaymentNo);
		strPayNameHeader = DataManager.Payment.GetDisplayName(infoPayment.m_nPaymentNo);
		strPayNameLine = strPayNameHeader;
	}
	else
	{
		strPayNo = " ";
		strPayNameHeader = "Total ";
		strPayNameHeader += (bCharge) ? "Charged to Account / Paid by Redemption" : "Money Received";
		strPayNameLine = "Total";
	}

	CString strLine = "";
	strLine += strHeader;
	strLine += strPayNo;
	strLine += strPayNameHeader;

	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine(strLine);
	m_ReportFile.WriteReportMiscLine("<LI>");

	bool bGotItem = FALSE;
	bool bGotException = FALSE;

	for (int s = nBlockStart; s <= nBlockEnd; s++)
	{
		CEposReportConsolPBI_MainBlock infoPayment;
		pPayments->GetAt(s, infoPayment);

		CCSV csv('\t');
		csv.Add(strPayNameLine);

		bool bShowLine = FALSE;

		switch (infoPayment.m_nPaymentBlock)
		{
		case PBI_MAINBLOCK_LINE_ITEM:

			bGotItem = TRUE;
			bShowLine = TRUE;

			switch (m_nReportType)
			{
			case REPORT_TYPE_PAYMENT_ESTIMATE_ITEM:
			{
				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nEposPluNo = infoPayment.m_nItemNumber;
				::ProcessPluNo(infoPluNo, FALSE, TRUE);

				m_PluInfoFinder.SetPluNo(infoPluNo.m_nBasePluNo);

				csv.Add(ReportHelpers.GetDisplayPluNo(infoPluNo.m_nBasePluNo, TRUE));
				csv.Add(m_PluInfoFinder.GetPluInfoRepText(TRUE));
			}
			break;

			case REPORT_TYPE_PAYMENT_ESTIMATE_DEPT:
			{
				CString strDeptText = "";

				int nDeptIdx = 0;
				if (DataManager.Department.FindDeptByNumber((int)infoPayment.m_nItemNumber, nDeptIdx) == TRUE)
				{
					CDepartmentCSVRecord dept;
					DataManager.Department.GetAt(nDeptIdx, dept);
					strDeptText = dept.GetReportText();
				}
				else
				{
					strDeptText.Format("Dept %d", (int)infoPayment.m_nItemNumber);
				}

				csv.Add(strDeptText);
			}
			break;

			case REPORT_TYPE_PAYMENT_ESTIMATE_GROUP:
			default:
			{
				CString strGroupText = "";

				int nGroupIdx = 0;
				if (DataManager.EposGroup.FindGroupByNumber((int)infoPayment.m_nItemNumber, nGroupIdx) == TRUE)
				{
					CGroupCSVRecordEpos group;
					DataManager.EposGroup.GetAt(nGroupIdx, group);
					strGroupText = group.GetReportText();
				}
				else
				{
					strGroupText.Format("Group %d", (int)infoPayment.m_nItemNumber);
				}

				csv.Add(strGroupText);
			}
			break;
			}

			break;

		case PBI_MAINBLOCK_LINE_EXCEPTION:

			if ((TRUE == bGotItem) && (FALSE == bGotException))
			{
				m_ReportFile.WriteReportMiscLine("<LI>");
			}

			bGotException = TRUE;
			bShowLine = TRUE;

			switch ((int)infoPayment.m_nItemNumber)
			{
			case 1:		csv.Add("<..>Transactions with deposit purchases");						break;
			case 2:		csv.Add("<..>Transactions with customer payment on account");				break;
			case 3:		csv.Add("<..>Transactions with room settlement");							break;
			case 4:		csv.Add("<..>Transactions with Loyalty top-up");							break;
			case 5:		csv.Add("<..>Transactions with SmartPay top-up");							break;
			case 6:		csv.Add("<..>Transactions with mySmartPhoneMoney top-up");				break;
			case 7:		csv.Add("<..>Transactions with Sports Booker payment on account");		break;
			case 8:		csv.Add("<..>Transactions with cashback");								break;
			case 9:		csv.Add("<..>Payment total not equal to item total");						break;
			case 10:	csv.Add("<..>Transactions with filtered items");							break;
			default:	csv.Add("<..>Unknown exception type");									break;
			}

			if (REPORT_TYPE_PAYMENT_ESTIMATE_ITEM == m_nReportType)
			{
				csv.Add("");
			}

			break;

		case PBI_MAINBLOCK_TOTAL_ITEM:
			if (TRUE == bGotException)
			{
				bShowLine = TRUE;

				m_ReportFile.WriteReportMiscLine("<LI>");

				csv.Add("<..>Total (Allocated)");

				if (REPORT_TYPE_PAYMENT_ESTIMATE_ITEM == m_nReportType)
				{
					csv.Add("");
				}
			}
			break;

		case PBI_MAINBLOCK_TOTAL_EXCEPTION:
			if (TRUE == bGotItem)
			{
				bShowLine = TRUE;

				csv.Add("<..>Total (Exceptions)");

				if (REPORT_TYPE_PAYMENT_ESTIMATE_ITEM == m_nReportType)
				{
					csv.Add("");
				}
			}
			break;

		case PBI_MAINBLOCK_TOTAL_ALL:
		{
			bShowLine = TRUE;

			m_ReportFile.WriteReportMiscLine("<LI>");

			if ((TRUE == bGotException) && (FALSE == bGotItem))
			{
				csv.Add("<..>Total (Exceptions)");
			}
			else
			{
				csv.Add("<..>Total (Payment)");
			}

			if (REPORT_TYPE_PAYMENT_ESTIMATE_ITEM == m_nReportType)
			{
				csv.Add("");
			}
		}

		break;
		}

		if (TRUE == bShowLine)
		{
			csv.Add(infoPayment.m_nTranQty);
			csv.Add(infoPayment.m_dVal, SysInfo.GetDPValue());
			m_ReportFile.WriteReportDataLine(csv.GetLine());
		}
	}
}


/**********************************************************************/
