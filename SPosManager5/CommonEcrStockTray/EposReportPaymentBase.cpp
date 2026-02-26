/**********************************************************************/
#include "EposReportConsolParams.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPaymentBase.h"
/**********************************************************************/

CEposReportPaymentBase::CEposReportPaymentBase( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nServerBlockIdxStart = 0;

	SetPMSDepositsFlag( TRUE );

	if ( EcrmanOptions.GetReportsShowPaymentQuantityFlag() )
		m_strSimpleUnderline = "\t\t\t<LI>";
	else
		m_strSimpleUnderline = "\t\t<LI>";
	
	int nTotals = 5;
	
	if ( TRUE == EcrmanOptions.GetReportsShowPaymentQuantityFlag() )
		nTotals++;

	if ( TRUE == EcrmanOptions.GetReportsShowCashChangeFlag() )
		nTotals++;
	
	m_strDetailUnderline = "\t";
	for ( int n = 0; n < nTotals; n++ )
		m_strDetailUnderline += "\t<LI>";
}

/**********************************************************************/

CEposReportPaymentBase::~CEposReportPaymentBase()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pPaymentsAll -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsItem -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsAccount -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsDeposit -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsCustomer -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsRoom -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsLoyalty -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsSmartPay -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsSmartPhone -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsSptBook -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaymentsMixed -> RemoveAll();		
		m_arrayReportBlocks[n].m_pPaidIn -> RemoveAll();
		m_arrayReportBlocks[n].m_pPaidOut -> RemoveAll();
		m_arrayReportBlocks[n].m_pNet -> RemoveAll();
		
		delete ( m_arrayReportBlocks[n].m_pPaymentsAll );
		delete ( m_arrayReportBlocks[n].m_pPaymentsItem );
		delete ( m_arrayReportBlocks[n].m_pPaymentsAccount );
		delete ( m_arrayReportBlocks[n].m_pPaymentsDeposit );
		delete ( m_arrayReportBlocks[n].m_pPaymentsCustomer );
		delete ( m_arrayReportBlocks[n].m_pPaymentsRoom );
		delete ( m_arrayReportBlocks[n].m_pPaymentsLoyalty );
		delete ( m_arrayReportBlocks[n].m_pPaymentsSmartPay );
		delete ( m_arrayReportBlocks[n].m_pPaymentsSmartPhone );
		delete ( m_arrayReportBlocks[n].m_pPaymentsSptBook );
		delete ( m_arrayReportBlocks[n].m_pPaymentsMixed );
		delete ( m_arrayReportBlocks[n].m_pPaidIn );
		delete ( m_arrayReportBlocks[n].m_pPaidOut );
		delete ( m_arrayReportBlocks[n].m_pNet );
	}
}

/**********************************************************************/

void CEposReportPaymentBase::InitialiseConsolidationBlocksStageOne()
{
	bool bAllowGlobal = ( NODE_SYSTEM == m_nConLevel ) && ( EcrmanOptions.GetGlobalDepartmentFlag() ) && ( EcrmanOptions.GetGlobalPaidInOutFlag() ) && ( EcrmanOptions.GetGlobalPaymentFlag() ); 

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
}

/**********************************************************************/

void CEposReportPaymentBase::InitialiseConsolidationBlocksStageTwo()
{
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportPaySumBlock block{};
		block.Copy( entity );

		block.m_pPaymentsAll = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsItem = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsAccount = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsDeposit = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsCustomer = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsRoom = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsLoyalty = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsSmartPay = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsSmartPhone = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsSptBook = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaymentsMixed = new CReportConsolidationArray<CEposReportConsolPaySum>;
		block.m_pPaidIn = new CReportConsolidationArray<CEposReportConsolPaySumOneVal>;
		block.m_pPaidOut = new CReportConsolidationArray<CEposReportConsolPaySumOneVal>;
		block.m_pNet = new CReportConsolidationArray<CEposReportConsolPaySumOneVal>;
		
		block.m_bGotPaidIOTotal = FALSE;
		block.m_nPaidInTotalCash = 0;
		block.m_nPaidInTotalNonCash = 0;
		block.m_nPaidOutTotalCash = 0;
		block.m_nPaidOutTotalNonCash = 0;

		block.m_bGotDepositRA = FALSE;
		block.m_bGotCustomerRA = FALSE;
		block.m_bGotRoomRA = FALSE;
		block.m_bGotLoyaltyRA = FALSE;
		block.m_dDepositRA = 0.0;
		block.m_dDepositRefund = 0.0;
		block.m_dCustomerRA = 0.0;
		block.m_dRoomRA = 0.0;
		block.m_dLoyaltyRA = 0.0;
		block.m_dSmartPayRA = 0.0;
		block.m_dSmartPhoneRA = 0.0;
		block.m_dSptBookRA = 0.0;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPaymentBase::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	bool bInclude = FALSE;
	bool bGotDepositItem = FALSE;
	bool bGotNormalItem = FALSE;
	bool bGotCustomerRAItem = FALSE;
	bool bGotRoomRAItem = FALSE;
	bool bGotLoyaltyRAItem = FALSE;
	bool bGotSmartPayRAItem = FALSE;
	bool bGotSmartPhoneRAItem = FALSE;
	bool bGotSptBookRAItem = FALSE;
	m_CashRSPVersionChecker.ResetTransactionVersions();

	m_arrayPaymentBuffer.RemoveAll();
	m_arrayPaidIOBuffer.RemoveAll();

	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ); nFileIdx++ )
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

		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			nDbIdx = 0;

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

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				CCSVEposTermLine csvIn ( strBuffer );

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				if (m_PMSModes.CheckPMSTransactionType(bIsPMSLocation, nCashRSPLineType) == FALSE)
				{
					continue;
				}

				switch( nCashRSPLineType )
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
					{
						ConsolidatePaymentBuffer( bGotDepositItem, bGotNormalItem, bGotCustomerRAItem, bGotRoomRAItem, bGotLoyaltyRAItem, bGotSmartPayRAItem, bGotSmartPhoneRAItem, bGotSptBookRAItem );
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
							
						bInclude = FALSE;
						bGotDepositItem = FALSE;
						bGotNormalItem = FALSE;
						bGotCustomerRAItem = FALSE;
						bGotRoomRAItem = FALSE;
						bGotLoyaltyRAItem = FALSE;
						bGotSmartPayRAItem = FALSE;
						bGotSmartPhoneRAItem = FALSE;
						bGotSptBookRAItem = FALSE;

						SetConsolidationServer( nDbIdx, infoFile.m_nLocIdx, csvIn.GetTransactionServer() );

						CString strCheckDate;
						bool bAcceptDateTime = FALSE;

						if ( ( TRUE == bIsPMSLocation ) && ( m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT ) )
						{
							strCheckDate = infoFile.m_strDateTran;
							bAcceptDateTime = TRUE;
						}
						else
						{
							CString strDate = csvIn.GetTransactionDate();
							CString strTime = csvIn.GetTransactionTime();

							if (strDate.GetLength() != 8 || strTime.GetLength() != 8)
							{
								continue;
							}

							strCheckDate.Format ( "20%s%s%s",
								(const char*) strDate.Right(2),
								(const char*) strDate.Mid(3,2),
								(const char*) strDate.Left(2) );

							CString strCheckTime;
							strCheckTime.Format ( "%s%s%s",
								(const char*) strTime.Left(2),
								(const char*) strTime.Mid(3,2),
								(const char*) strTime.Right(2) );

							bAcceptDateTime = SimpleTimeCheck ( infoFile.m_nLocIdx, strCheckDate, strCheckTime );
						}

						if ( TRUE == bAcceptDateTime )
						{
							bInclude = TRUE;

							COleDateTime oleDate = COleDateTime (
								atoi(strCheckDate.Left(4)),
								atoi(strCheckDate.Mid(4,2)),
								atoi(strCheckDate.Right(2)),
								0, 0, 0 );

							m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleDate );
							m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleDate );
						}
					}
					break;

				case CASHRSP_LINETYPE_TRAN_VOID:
				case CASHRSP_LINETYPE_WASTAGE:
				case CASHRSP_LINETYPE_NOSALE:
				case CASHRSP_LINETYPE_AGEVERIFY:
					ConsolidatePaymentBuffer( bGotDepositItem, bGotNormalItem, bGotCustomerRAItem, bGotRoomRAItem, bGotLoyaltyRAItem, bGotSmartPayRAItem, bGotSmartPhoneRAItem, bGotSptBookRAItem );
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
					bInclude = FALSE;
					bGotDepositItem = FALSE;
					bGotNormalItem = FALSE;
					bGotCustomerRAItem = FALSE;
					bGotRoomRAItem = FALSE;
					bGotLoyaltyRAItem = FALSE;
					bGotSmartPayRAItem = FALSE;
					bGotSmartPhoneRAItem = FALSE;
					bGotSptBookRAItem = FALSE;
					break;

				case CASHRSP_LINETYPE_ITEM:
					if ( TRUE == bInclude )
					{
						int nType, nDummy32;
						__int64 nDummy64;
						csvIn.GetItemTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64 );

						switch( nType )
						{
						case CASHRSP_ITEMPAYTYPE_DEPOSIT:
						case CASHRSP_ITEMPAYTYPE_CUSTOMER:
						case CASHRSP_ITEMPAYTYPE_ROOM:
						case CASHRSP_ITEMPAYTYPE_LOYALTY:
						case CASHRSP_ITEMPAYTYPE_SMARTPAY:
						case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
						case CASHRSP_ITEMPAYTYPE_SPTBOOK:
							{
								CString strVoid = csvIn.GetItemVoidFlag();
							
								if ( strVoid != "V" )
								{		
									switch( nType )
									{
									case CASHRSP_ITEMPAYTYPE_DEPOSIT:
										{
											CString strRefund = csvIn.GetItemRefundFlag();
											
											if ( strRefund == "R" )
												ConsolidateDepositRefund ( -csvIn.GetItemLineValueReport() );
											else			
												ConsolidateDepositRA ( csvIn.GetItemLineValueReport() );
											
											bGotDepositItem = TRUE;
										}
										break;

									case CASHRSP_ITEMPAYTYPE_CUSTOMER:
										ConsolidateCustomerRA ( csvIn.GetItemLineValueReport() );
										bGotCustomerRAItem = TRUE;
										break;

									case CASHRSP_ITEMPAYTYPE_ROOM:
										ConsolidateRoomRA ( csvIn.GetItemLineValueReport() );	
										bGotRoomRAItem = TRUE;
										break;

									case CASHRSP_ITEMPAYTYPE_LOYALTY:
										ConsolidateLoyaltyRA ( csvIn.GetItemLineValueReport() );	
										bGotLoyaltyRAItem = TRUE;
										break;

									case CASHRSP_ITEMPAYTYPE_SMARTPAY:
										ConsolidateSmartPayRA ( csvIn.GetItemLineValueReport() );	
										bGotSmartPayRAItem = TRUE;
										break;

									case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
										ConsolidateSmartPhoneRA ( csvIn.GetItemLineValueReport() );	
										bGotSmartPhoneRAItem = TRUE;
										break;

									case CASHRSP_ITEMPAYTYPE_SPTBOOK:
										ConsolidateSptBookRA ( csvIn.GetItemLineValueReport() );	
										bGotSptBookRAItem = TRUE;
										break;
									}
								}
							}
							break;

						case CASHRSP_ITEMPAYTYPE_NORMAL:
							{
								CString strVoid = csvIn.GetItemVoidFlag();

								if (strVoid != "V")
								{
									bGotNormalItem = TRUE;
								}
							}
							break;
						}
					}
					break;

				case CASHRSP_LINETYPE_PAYMENT:
				case CASHRSP_LINETYPE_DEPOSIT_PMS:
					if ( TRUE == bInclude )
					{
						int nPaymentNo = csvIn.GetPaymentNumber();

						switch( nConnectionType )
						{
						case CONNECTION_TYPE_SPTBOOK_NONE:
							csvIn.SetPaymentNumber( DataManager.SBPaymentMap.GetSharpPosPaymentNo( nPaymentNo ) );
							strBuffer = csvIn.GetLine();
							break;

						case CONNECTION_TYPE_SMARTENT_NONE:
							csvIn.SetPaymentNumber( DataManager.SEPaymentMap.GetSharpPosPaymentNo( nPaymentNo ) );
							strBuffer = csvIn.GetLine();
							break;

						case CONNECTION_TYPE_STANDARD_NONE:
						default:
							{
								if ( SysInfo.IsPMSSystem() == TRUE )
									if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
										if ( DataManager.Payment.IsPMSPaymentType( nPaymentNo ) == TRUE )
											bInclude = FALSE;
							}
							break;
						}
						
						if ( TRUE == bInclude )
							m_arrayPaymentBuffer.Add( strBuffer );
					}
					break;

				case CASHRSP_LINETYPE_PAIDIO:
					if ( TRUE == bInclude )
						m_arrayPaidIOBuffer.Add( strBuffer );
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

		ConsolidatePaymentBuffer( bGotDepositItem, bGotNormalItem, bGotCustomerRAItem, bGotRoomRAItem, bGotLoyaltyRAItem, bGotSmartPayRAItem, bGotSmartPhoneRAItem, bGotSptBookRAItem  );

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CEposReportPaymentBase::ConsolidatePaymentBuffer( bool bGotDepositItem, bool bGotNormalItem, bool bGotCustomerRAItem, bool bGotRoomRAItem, bool bGotLoyaltyRAItem, bool bGotSmartPayRAItem, bool bGotSmartPhoneRAItem, bool bGotSptBookRAItem )
{		
	if ( m_arrayPaidIOBuffer.GetSize() != 0 )
	{
		CCSVEposTermLine csvIn ( m_arrayPaidIOBuffer.GetAt(0) );
		bool bPaidOut = ( csvIn.GetPaidIODirection() == 0 );
				
		for ( int n = 0; n < m_arrayPaidIOBuffer.GetSize(); n++ )
		{
			CCSVEposTermLine csvIn ( m_arrayPaidIOBuffer.GetAt(n) );

			CString strVoid = csvIn.GetPaidIOVoidFlag();
			if ( strVoid != "V" )
			{
				int nAmount = csvIn.GetPaidIOValue();

				if ( TRUE == bPaidOut )
				{
					nAmount *= -1;
					ConsolidatePaidOut( csvIn.GetPaidIONumber(), nAmount );
				}
				else
					ConsolidatePaidIn( csvIn.GetPaidIONumber(), nAmount );
			}
		}
	
		for ( int n = 0; n < m_arrayPaymentBuffer.GetSize(); n++ )
		{
			CCSVEposTermLine csvIn ( m_arrayPaymentBuffer.GetAt(n) );
			int nAmount = csvIn.GetPaymentTender();

			if ( TRUE == bPaidOut )
				nAmount *= -1;
			
			if ( csvIn.GetPaymentNumber() == 1 )
			{
				if ( TRUE == bPaidOut )
					ConsolidatePaidOutTotalCash( nAmount );
				else
					ConsolidatePaidInTotalCash( nAmount );
			}
			else
			{
				if ( TRUE == bPaidOut )
					ConsolidatePaidOutTotalNonCash( nAmount );
				else
					ConsolidatePaidInTotalNonCash( nAmount );
			}
					
			if ( TRUE == bPaidOut )
				nAmount *= -1;

			ConsolidateNet( csvIn.GetPaymentNumber(), nAmount );
		}
	}
	else if ( m_arrayPaymentBuffer.GetSize() != 0 )
	{
		int nConsolidationType = 0;
		if ( EcrmanOptions.GetReportsShowRAPaymentsFlag() == TRUE )
		{
			if ( EcrmanOptions.GetReportsSeparateRAPaymentsFlag() == TRUE )
				nConsolidationType = 2;
			else
				nConsolidationType = 1;
		}

		int nItemTypes = 0;
		int nBlock = 0;

		if ( TRUE == bGotDepositItem )		
		{
			nItemTypes++;
			nBlock = 3;
		}

		if ( TRUE == bGotCustomerRAItem )	
		{
			nItemTypes++;
			nBlock = 4;
		}

		if ( TRUE == bGotRoomRAItem )		
		{
			nItemTypes++;
			nBlock = 5;
		}

		
		if ( TRUE == bGotLoyaltyRAItem )		
		{
			nItemTypes++;
			nBlock = 6;
		}

		if ( TRUE == bGotSmartPayRAItem )		
		{
			nItemTypes++;
			nBlock = 7;
		}

		if ( TRUE == bGotSmartPhoneRAItem )		
		{
			nItemTypes++;
			nBlock = 8;
		}

		if ( TRUE == bGotSptBookRAItem )		
		{
			nItemTypes++;
			nBlock = 9;
		}

		if ( TRUE == bGotNormalItem )		
		{
			nItemTypes++;
			nBlock = 10;
		}

		if ( 0 == nItemTypes )
		{
			//FIDDLE : ASSUME NORMAL ITEM TYPE IF NO ITEMS DETECTED
			//FIDDLE : THIS ALLOWS PMS DEPOSIT PAYMENTS FOR PENDING BOOKINGS TO APPEAR IN REPORT
			bGotNormalItem = TRUE;
			nItemTypes = 1;
			nBlock = 10;
		}

		switch( nConsolidationType )
		{
		case 1:
			{
				//CONSOLIDATED RA TYPES
				if ( FALSE == bGotNormalItem )
					nBlock = 2;

				//MIXED TRANSACTION TYPES
				else if ( nItemTypes > 1 )
					nBlock = 11;
			}
			break;

		case 2:
			{
				//MIXED TRANSACTION TYPES
				if ( nItemTypes > 1 )
					nBlock = 11;
			}
			break;
			
		case 0:
		default:
			//ALL TYPES CONSOLIDATED TOGETHER
			nBlock = 1;
			break;
		}

		for ( int n = 0; n < m_arrayPaymentBuffer.GetSize(); n++ )
		{
			CCSVEposTermLine csvIn ( m_arrayPaymentBuffer.GetAt(n) );

			int nPaymentNo = csvIn.GetPaymentNumber();
			int nTender = csvIn.GetPaymentTender();
			int nAmount = csvIn.GetPaymentAmount();
			int nGratuity = csvIn.GetPaymentGratuity();
			int nCashback = csvIn.GetPaymentCashback();
			int nSurplus = csvIn.GetPaymentSurplus( m_CashRSPVersionChecker.GetCurrentTransactionVersionNo() );
			int nChange = nTender - ( nAmount + nGratuity + nSurplus );

			//SKIP CUSTOMER MARKERS FOR TRANSACTIONS WITH NO POST TO ACCOUNT
			if ( ( 0 == nTender ) && ( 0 == nAmount ) && ( 0 == nGratuity ) && ( 0 == nSurplus ) && ( 0 == nCashback ) )
				continue;

			int nType, nDummy32;
			__int64 nDummy64;
			bool bIsCash;
			csvIn.GetPaymentAccountTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64, bIsCash );
			int nPaymentNoForChange = 1;

			if ( ( nPaymentNo != 1 ) && ( TRUE == bIsCash ) )
			{
				switch( EcrmanOptions.GetReportsMultiCashMethod() )
				{
				case REPORTCASH_ALL:
					nPaymentNoForChange = nPaymentNo;
					break;

				case REPORTCASH_METHOD1_CONSOLIDATE:
					nPaymentNo = 1;
					break;

				case REPORTCASH_METHOD1_CHANGE:
					break;

				case REPORTCASH_METHOD1_ONLY:
				default:
					bIsCash = FALSE;
					break;
				}
			}
	
			if ( ( EcrmanOptions.GetReportsShowCashChangeFlag() == FALSE ) && ( CASHRSP_ITEMPAYTYPE_DEPOSIT != nType ) )
				nChange = 0;

			CEposReportConsolPaySum ConsolLocal;
			ConsolLocal.m_bIsCash = bIsCash;
			ConsolLocal.m_nQty = 1;
			ConsolLocal.m_nTender = nTender;
			ConsolLocal.m_nAmount = nAmount;
			ConsolLocal.m_nGratuity = nGratuity;
			ConsolLocal.m_nCashback = nCashback;
			ConsolLocal.m_nSurplus = nSurplus;
			ConsolLocal.m_nChange = nChange;
				
			switch( nType )
			{
			case CASHRSP_ITEMPAYTYPE_DEPOSIT:
				ConsolLocal.m_nDepositSpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;

			case CASHRSP_ITEMPAYTYPE_CUSTOMER:
				ConsolLocal.m_nCustomerSpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;

			case CASHRSP_ITEMPAYTYPE_ROOM:
				ConsolLocal.m_nRoomSpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;

			case CASHRSP_ITEMPAYTYPE_LOYALTY:
				ConsolLocal.m_nLoyaltySpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;

			case CASHRSP_ITEMPAYTYPE_SMARTPAY:
				ConsolLocal.m_nSmartPaySpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;

			case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
				ConsolLocal.m_nSmartPhoneSpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;

			case CASHRSP_ITEMPAYTYPE_SPTBOOK:
				ConsolLocal.m_nSptBookSpend = nTender;
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				break;
			
			case CASHRSP_ITEMPAYTYPE_NORMAL:
			default:
				ConsolidatePayment( nBlock, nPaymentNo, ConsolLocal );
				ConsolidateNet( nPaymentNo, nTender );
				break;
			}
	
			//CHANGE REDUCES NET CASH
			if ( 0 != nChange )
				ConsolidateNet( nPaymentNoForChange, -nChange );
				
			//CASHBACK REDUCES NET CASH
			if ( 0 != nCashback )
				if ( EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE )
					ConsolidateNet( 1, -nCashback );
		}
	}

	m_arrayPaymentBuffer.RemoveAll();
	m_arrayPaidIOBuffer.RemoveAll();
}
		
/**********************************************************************/

void CEposReportPaymentBase::ConsolidatePayment( int nType, int nSubType, CEposReportConsolPaySum& infoConsol )
{
	infoConsol.m_nReportGroup = 0;
	if ( REPORT_TYPE_PAYMENT_GROUP_SUMMARY == m_nReportType )
	{
		int nPaymentIdx;
		if ( DataManager.Payment.FindPaymentByNumber( nSubType, nPaymentIdx ) == TRUE )
		{
			CPaymentCSVRecord Payment;
			DataManager.Payment.GetAt( nPaymentIdx, Payment );
			infoConsol.m_nReportGroup = Payment.GetReportGroup();
		}
	}

	infoConsol.m_nSubType = nSubType;
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n) + m_nServerBlockIdxStart;
		
		switch( nType )
		{
		case 1:		m_arrayReportBlocks[ nBlock ].m_pPaymentsAll ->			Consolidate( infoConsol );	break;
		case 2:		m_arrayReportBlocks[ nBlock ].m_pPaymentsAccount ->		Consolidate( infoConsol );	break;
		case 3:		m_arrayReportBlocks[ nBlock ].m_pPaymentsDeposit ->		Consolidate( infoConsol );	break;
		case 4:		m_arrayReportBlocks[ nBlock ].m_pPaymentsCustomer ->	Consolidate( infoConsol );	break;
		case 5:		m_arrayReportBlocks[ nBlock ].m_pPaymentsRoom ->		Consolidate( infoConsol );	break;
		case 6:		m_arrayReportBlocks[ nBlock ].m_pPaymentsLoyalty ->		Consolidate( infoConsol );	break;
		case 7:		m_arrayReportBlocks[ nBlock ].m_pPaymentsSmartPay ->	Consolidate( infoConsol );	break;
		case 8:		m_arrayReportBlocks[ nBlock ].m_pPaymentsSmartPhone ->	Consolidate( infoConsol );	break;
		case 9:		m_arrayReportBlocks[ nBlock ].m_pPaymentsSptBook ->		Consolidate( infoConsol );	break;
		case 10:	m_arrayReportBlocks[ nBlock ].m_pPaymentsItem ->		Consolidate( infoConsol );	break;
		case 11:	m_arrayReportBlocks[ nBlock ].m_pPaymentsMixed ->		Consolidate( infoConsol );	break;
		}
	}
}

/**********************************************************************/
void CEposReportPaymentBase::ConsolidatePaidIn( int nSubType, __int64 nValue ){ ConsolidateOneVal( 1, nSubType, nValue ); }
void CEposReportPaymentBase::ConsolidatePaidOut( int nSubType, __int64 nValue ){ ConsolidateOneVal( 2, nSubType, nValue ); }
void CEposReportPaymentBase::ConsolidateNet( int nSubType, __int64 nValue ){ ConsolidateOneVal( 3, nSubType, nValue ); }
/**********************************************************************/

void CEposReportPaymentBase::ConsolidateOneVal( int nType, int nSubType, __int64 nValue )
{
	CEposReportConsolPaySumOneVal infoConsol;
	infoConsol.m_nReportGroup = 0;
	infoConsol.m_nSubType = nSubType;
	infoConsol.m_nValue = nValue;

	if ( 3 == nType )
	{
		if ( REPORT_TYPE_PAYMENT_GROUP_SUMMARY == m_nReportType )
		{
			int nPaymentIdx;
			if ( DataManager.Payment.FindPaymentByNumber( nSubType, nPaymentIdx ) == TRUE )
			{
				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt( nPaymentIdx, Payment );
				infoConsol.m_nReportGroup = Payment.GetReportGroup();
			}
		}
	}

	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n) + m_nServerBlockIdxStart;
		
		switch( nType )
		{
		case 1:		m_arrayReportBlocks[ nBlock ].m_pPaidIn ->				Consolidate( infoConsol );	break;
		case 2:		m_arrayReportBlocks[ nBlock ].m_pPaidOut ->				Consolidate( infoConsol );	break;
		case 3:		m_arrayReportBlocks[ nBlock ].m_pNet ->					Consolidate( infoConsol );	break;
		}
	}
}

/**********************************************************************/
void CEposReportPaymentBase::ConsolidatePaidInTotalCash( __int64 nValue ){ ConsolidatePayIOTotal( 1, nValue ); }
void CEposReportPaymentBase::ConsolidatePaidInTotalNonCash( __int64 nValue ){ ConsolidatePayIOTotal( 2, nValue ); }
void CEposReportPaymentBase::ConsolidatePaidOutTotalCash( __int64 nValue ){ ConsolidatePayIOTotal( 3, nValue ); }
void CEposReportPaymentBase::ConsolidatePaidOutTotalNonCash( __int64 nValue ){ ConsolidatePayIOTotal( 4, nValue ); }
/**********************************************************************/

void CEposReportPaymentBase::ConsolidatePayIOTotal( int nType, __int64 nValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n) + m_nServerBlockIdxStart;
		m_arrayReportBlocks[ nBlock ].m_bGotPaidIOTotal = TRUE;

		switch( nType )
		{
		case 1:	m_arrayReportBlocks[ nBlock ].m_nPaidInTotalCash += nValue;			break;
		case 2:	m_arrayReportBlocks[ nBlock ].m_nPaidInTotalNonCash += nValue;		break;
		case 3:	m_arrayReportBlocks[ nBlock ].m_nPaidOutTotalCash += nValue;		break;
		case 4:	m_arrayReportBlocks[ nBlock ].m_nPaidOutTotalNonCash += nValue;		break;
		}
	}
}

/**********************************************************************/
void CEposReportPaymentBase::ConsolidateDepositRA( double dValue ){ ConsolidateRA( 1, dValue ); }
void CEposReportPaymentBase::ConsolidateCustomerRA( double dValue ){ ConsolidateRA( 2, dValue ); }
void CEposReportPaymentBase::ConsolidateRoomRA( double dValue ){ ConsolidateRA( 3, dValue ); }
void CEposReportPaymentBase::ConsolidateLoyaltyRA( double dValue ){ ConsolidateRA( 4, dValue ); }
void CEposReportPaymentBase::ConsolidateSmartPayRA( double dValue ){ ConsolidateRA( 5, dValue ); }
void CEposReportPaymentBase::ConsolidateSmartPhoneRA( double dValue ){ ConsolidateRA( 6, dValue ); }
void CEposReportPaymentBase::ConsolidateSptBookRA( double dValue ){ ConsolidateRA( 7, dValue ); }
/**********************************************************************/

void CEposReportPaymentBase::ConsolidateRA( int nType, double dValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n) + m_nServerBlockIdxStart;
		
		switch( nType )
		{
		case 1:
			m_arrayReportBlocks[ nBlock ].m_bGotDepositRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dDepositRA += dValue;
			break;

		case 2:
			m_arrayReportBlocks[ nBlock ].m_bGotCustomerRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dCustomerRA += dValue;
			break;

		case 3:
			m_arrayReportBlocks[ nBlock ].m_bGotRoomRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dRoomRA += dValue;
			break;

		case 4:
			m_arrayReportBlocks[ nBlock ].m_bGotLoyaltyRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dLoyaltyRA += dValue;
			break;

		case 5:
			m_arrayReportBlocks[ nBlock ].m_bGotSmartPayRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dSmartPayRA += dValue;
			break;

		case 6:
			m_arrayReportBlocks[ nBlock ].m_bGotSmartPhoneRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dSmartPhoneRA += dValue;
			break;

		case 7:
			m_arrayReportBlocks[ nBlock ].m_bGotSptBookRA = TRUE;
			m_arrayReportBlocks[ nBlock ].m_dSptBookRA += dValue;
			break;
		}
	}
}

/**********************************************************************/

void CEposReportPaymentBase::ConsolidateDepositRefund( double dValue )
{
	for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
	{
		int nBlock = m_arrayTerminalBlocks.GetAt(n) + m_nServerBlockIdxStart;	
		m_arrayReportBlocks[ nBlock ].m_bGotDepositRA = TRUE;
		m_arrayReportBlocks[ nBlock ].m_dDepositRefund += dValue;
	}
}

/**********************************************************************/

void CEposReportPaymentBase::CreateSalesSection( CEposReportPaySumBlock& Block )
{
	CEposReportConsolPaySum AllPaymentTotals, AccountTotals, DepositTotals, CustomerTotals, RoomTotals, LoyaltyTotals, SmartPayTotals, SmartPhoneTotals, SptBookTotals, ItemTotals, MixedTotals, NetTotals;
	
	WritePaymentBlock( "Payments", Block.m_pPaymentsAll, AllPaymentTotals );
	WritePaymentBlock( "Payment on Account", Block.m_pPaymentsAccount, AccountTotals );
	WritePaymentBlock( "Purchase of Deposits", Block.m_pPaymentsDeposit, DepositTotals );
	WritePaymentBlock( "Customer Payment Received on Account", Block.m_pPaymentsCustomer, CustomerTotals );
	WritePaymentBlock( "Settlement of Room Accounts", Block.m_pPaymentsRoom, RoomTotals );
	WritePaymentBlock( "Loyalty Top-ups", Block.m_pPaymentsLoyalty, LoyaltyTotals );
	WritePaymentBlock( "SmartPay Top-ups", Block.m_pPaymentsSmartPay, SmartPayTotals );
	WritePaymentBlock( "mySmartPhoneMoney Top-ups", Block.m_pPaymentsSmartPhone, SmartPhoneTotals );
	WritePaymentBlock( "Sports Booker Payment on Account", Block.m_pPaymentsSptBook, SptBookTotals );
	WritePaymentBlock( "Payment for Items", Block.m_pPaymentsItem, ItemTotals );
	WritePaymentBlock( "Transactions with Item Purchases and Payments on Account", Block.m_pPaymentsMixed, MixedTotals );

	CEposReportConsolPaySum DummyTotals;
	DummyTotals.Add( AllPaymentTotals );
	DummyTotals.Add( AccountTotals );
	DummyTotals.Add( DepositTotals );
	DummyTotals.Add( CustomerTotals );
	DummyTotals.Add( RoomTotals );
	DummyTotals.Add( LoyaltyTotals );
	DummyTotals.Add( SmartPayTotals );
	DummyTotals.Add( SmartPhoneTotals );
	DummyTotals.Add( SptBookTotals );
	DummyTotals.Add( ItemTotals );
	DummyTotals.Add( MixedTotals );

	int nAllCount = Block.m_pPaymentsAll -> GetSize();
	int nAccountCount = Block.m_pPaymentsAccount -> GetSize();
	int nDepositCount = Block.m_pPaymentsDeposit -> GetSize();
	int nCustomerCount = Block.m_pPaymentsCustomer -> GetSize();
	int nRoomCount = Block.m_pPaymentsRoom -> GetSize();
	int nLoyaltyCount = Block.m_pPaymentsLoyalty -> GetSize();
	int nSmartPayCount = Block.m_pPaymentsSmartPay -> GetSize();
	int nSmartPhoneCount = Block.m_pPaymentsSmartPhone -> GetSize();
	int nSptBookCount = Block.m_pPaymentsSptBook -> GetSize();
	int nMixedCount = Block.m_pPaymentsMixed -> GetSize();
	int nItemCount = Block.m_pPaymentsItem -> GetSize();

	int nPaymentCount = nAllCount + nAccountCount + nDepositCount + nCustomerCount + nRoomCount + nLoyaltyCount + nSmartPayCount + nSmartPhoneCount + nSptBookCount + nMixedCount + nItemCount;
	int nPaidInOutCount = Block.m_pPaidIn -> GetSize() + Block.m_pPaidOut -> GetSize();
	
	if ( EcrmanOptions.GetReportsShowRABalanceFlag() == TRUE )
	{
		if ( EcrmanOptions.GetReportsSeparateRABalanceFlag() == TRUE )
		{
			if ( ( nDepositCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nDepositSpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;
				double dRefundTotal = 0.0;

				if ( TRUE == Block.m_bGotDepositRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dDepositRA;
					dRefundTotal = Block.m_dDepositRefund;
				}

				if ( ( DummyTotals.m_nDepositSpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteDepositSummary( dRATotal, dRefundTotal, DummyTotals );
				}
			}
		
			if ( ( nCustomerCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nCustomerSpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;

				if ( TRUE == Block.m_bGotCustomerRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dCustomerRA;
				}

				if ( ( DummyTotals.m_nCustomerSpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteCustomerSummary( dRATotal, DummyTotals );
				}
			}
			
			if ( ( nRoomCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nRoomSpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;

				if ( TRUE == Block.m_bGotRoomRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dRoomRA;
				}

				if ( ( DummyTotals.m_nRoomSpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteRoomSummary( dRATotal, DummyTotals );
				}
			}

			if ( ( nLoyaltyCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nLoyaltySpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;

				if ( TRUE == Block.m_bGotLoyaltyRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dLoyaltyRA;
				}

				if ( ( DummyTotals.m_nLoyaltySpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteLoyaltySummary( dRATotal, DummyTotals );
				}
			}

			if ( ( nSmartPayCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nSmartPaySpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;

				if ( TRUE == Block.m_bGotSmartPayRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dSmartPayRA;
				}

				if ( ( DummyTotals.m_nSmartPaySpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteSmartPaySummary( dRATotal, DummyTotals );
				}
			}

			if ( ( nSmartPhoneCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nSmartPhoneSpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;

				if ( TRUE == Block.m_bGotSmartPhoneRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dSmartPhoneRA;
				}

				if ( ( DummyTotals.m_nSmartPhoneSpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteSmartPhoneSummary( dRATotal, DummyTotals );
				}
			}

			if ( ( nSptBookCount > 0 ) || ( nAccountCount > 0 ) || ( nMixedCount > 0 ) || ( DummyTotals.m_nSptBookSpend != 0 ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;

				if ( TRUE == Block.m_bGotSptBookRA )
				{
					bGotRA = TRUE;
					dRATotal = Block.m_dSptBookRA;
				}

				if ( ( DummyTotals.m_nSptBookSpend != 0 ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteSptBookSummary( dRATotal, DummyTotals );
				}
			}
		}
		else
		{
			bool bGotRedemption = ( DummyTotals.m_nDepositSpend != 0 ) || ( DummyTotals.m_nCustomerSpend != 0 ) || ( DummyTotals.m_nRoomSpend != 0 ) || ( DummyTotals.m_nLoyaltySpend != 0 ) || ( DummyTotals.m_nSmartPaySpend != 0 ) || ( DummyTotals.m_nSptBookSpend != 0 ) || ( DummyTotals.m_nSmartPhoneSpend != 0 );

			if ( ( nMixedCount > 0 ) || ( nAccountCount > 0 ) || ( nDepositCount > 0 ) || ( nRoomCount > 0 ) || ( nCustomerCount > 0 ) ||
				( nLoyaltyCount > 0 ) || ( nSmartPayCount > 0 ) || ( nSmartPhoneCount > 0 ) || ( nSptBookCount > 0 ) || ( TRUE == bGotRedemption ) )
			{
				bool bGotRA = FALSE;
				double dRATotal = 0.0;
				double dRefundTotal = 0.0;

				if ( TRUE == Block.m_bGotDepositRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dDepositRA;
					dRefundTotal += Block.m_dDepositRefund;
				}

				if ( TRUE == Block.m_bGotCustomerRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dCustomerRA;
				}

				if ( TRUE == Block.m_bGotRoomRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dRoomRA;
				}

				if ( TRUE == Block.m_bGotLoyaltyRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dLoyaltyRA;
				}

				if ( TRUE == Block.m_bGotSmartPayRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dSmartPayRA;
				}

				if ( TRUE == Block.m_bGotSmartPhoneRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dSmartPhoneRA;
				}

				
				if ( TRUE == Block.m_bGotSptBookRA )
				{
					bGotRA = TRUE;
					dRATotal += Block.m_dSptBookRA;
				}

				if ( ( TRUE == bGotRedemption ) || ( TRUE == bGotRA ) )
				{
					m_ReportFile.RequestBlankLines( 2 );
					WriteRASummary( dRATotal, dRefundTotal, DummyTotals );
				}
			}
		}
	}

	if ( TRUE == Block.m_bGotPaidIOTotal )
	{
		m_ReportFile.RequestBlankLines( 2 );
		WritePaidInOutSummary( Block );
	}

	if ( ( nPaymentCount > 0 ) || ( nPaidInOutCount > 0 ) || ( EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE ) )
	{
		//NET PAYMENT RECEIVED
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( "Net Payments Received" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		CEposReportConsolPaySumOneVal NetTotals;

		if ( SysInfo.IsPMSSystem() == FALSE )
			WriteNetLines( Block.m_pNet, NetTotals, TRUE, TRUE, TRUE );
		else
			WriteNetLines( Block.m_pNet, NetTotals, TRUE, FALSE, TRUE );

		WriteTotals( "Net Total", NetTotals );
		
		//CASH SUMMARY
		WriteCashSummary( Block );
	}
}

/**********************************************************************/

void CEposReportPaymentBase::WritePaymentBlock( const char* szTitle, CReportConsolidationArray<CEposReportConsolPaySum>* pPayments, CEposReportConsolPaySum& Totals )
{
	if ( pPayments -> GetSize() != 0 )
	{
		CString strTitle = szTitle;
		CString strHeader = "";

		if ( strTitle.Left(4) != "<..>" )
			strHeader = "<..>";

		strHeader += strTitle;

		m_ReportFile.RequestBlankLines( 2 );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );	

		if ( SysInfo.IsPMSSystem() == FALSE )
		{
			WritePaymentLines( pPayments, Totals, TRUE, TRUE, TRUE );
			WriteTotals( "Total", Totals );
		}
		else
		{
			WritePaymentLines( pPayments, Totals, TRUE, FALSE, TRUE );
			WriteTotals( "Total", Totals );

			m_ReportFile.RequestBlankLines(1);
			WritePaymentLines( pPayments, Totals, FALSE, TRUE, FALSE );
		}
	}
}

/**********************************************************************/

bool CEposReportPaymentBase::WritePaymentLines( CReportConsolidationArray<CEposReportConsolPaySum>* pPayments, CEposReportConsolPaySum& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate )
{
	bool bResult = FALSE;

	for ( int nPos = 0; nPos < pPayments -> GetSize(); nPos++ )
	{
		UpdateCreateReportProgress();

		CEposReportConsolPaySum ConsolInfo;
		pPayments -> GetAt( nPos, ConsolInfo );
		
		int nPaymentNo = ConsolInfo.m_nSubType;

		bool bShow = TRUE;
		if ( ( FALSE == bShowPTR ) || ( FALSE == bShowNonPTR ) )
		{
			int nPayIdx;
			if ( DataManager.Payment.FindPaymentByNumber( nPaymentNo, nPayIdx ) == TRUE )
			{
				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt( nPayIdx, Payment );

				switch( Payment.GetPaymentType() )
				{
				case PAYMENT_TYPE_ROOM_AUTO:
				case PAYMENT_TYPE_ROOM_MANUAL:
					bShow = bShowPTR;
					break;

				default:
					bShow = bShowNonPTR;
					break;
				}
			}
			else
				bShow = bShowNonPTR;
		}
	
		if (TRUE == bShow)
		{
			int nDP = SysInfo.GetDPValue();

			CCSV csv('\t');
			csv.Add(nPaymentNo);
			csv.Add(DataManager.Payment.GetDisplayName(nPaymentNo));

			if (EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE)
			{
				csv.Add(ConsolInfo.m_nQty);
			}

			csv.Add(ConsolInfo.GetReportTender(), nDP);
			csv.Add(ConsolInfo.GetReportAmount(), nDP);
			csv.Add(ConsolInfo.GetReportGratuity(), nDP);
			csv.Add(ConsolInfo.GetReportCashback(), nDP);
			csv.Add(ConsolInfo.GetReportSurplus(), nDP);

			if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
			{
				csv.Add(ConsolInfo.GetReportChange(), nDP);
			}

			m_ReportFile.WriteReportDataLine(csv.GetLine());
			bResult = TRUE;

			if (TRUE == bConsolidate)
			{
				Totals.Add(ConsolInfo);
			}
		}

	}

	return bResult;
}

/**********************************************************************/

bool CEposReportPaymentBase::WriteNetLines( CReportConsolidationArray<CEposReportConsolPaySumOneVal>* pPayments, CEposReportConsolPaySumOneVal& Totals, bool bShowNonPTR, bool bShowPTR, bool bConsolidate )
{
	bool bResult = FALSE;

	for ( int nPos = 0; nPos < pPayments -> GetSize(); nPos++ )
	{
		UpdateCreateReportProgress();

		CEposReportConsolPaySumOneVal ConsolInfo;
		pPayments -> GetAt( nPos, ConsolInfo );
		
		int nPaymentNo = ConsolInfo.m_nSubType;

		bool bShow = TRUE;
		if ((FALSE == bShowPTR) || (FALSE == bShowNonPTR))
		{
			int nPayIdx;
			if (DataManager.Payment.FindPaymentByNumber(nPaymentNo, nPayIdx) == TRUE)
			{
				CPaymentCSVRecord Payment;
				DataManager.Payment.GetAt(nPayIdx, Payment);

				switch (Payment.GetPaymentType())
				{
				case PAYMENT_TYPE_ROOM_AUTO:
				case PAYMENT_TYPE_ROOM_MANUAL:
					bShow = bShowPTR;
					break;

				default:
					bShow = bShowNonPTR;
					break;
				}
			}
			else
			{
				bShow = bShowNonPTR;
			}
		}
	
		if (TRUE == bShow)
		{
			int nDP = SysInfo.GetDPValue();

			CCSV csv('\t');
			csv.Add(nPaymentNo);
			csv.Add(DataManager.Payment.GetDisplayName(nPaymentNo));

			if (EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE)
			{
				csv.Add("");
			}

			csv.Add(ConsolInfo.GetReportValue(), nDP);

			m_ReportFile.WriteReportDataLine(csv.GetLine());
			bResult = TRUE;

			if (TRUE == bConsolidate)
			{
				Totals.Add(ConsolInfo);
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CEposReportPaymentBase::WriteDepositSummary( double dTotalRA, double dTotalRefund, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalDepositSpend = PaymentTotals.GetReportDepositSpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Deposit Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Deposits Purchased", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Deposits Refunded", FALSE, 0 );
	csv.Add( dTotalRefund, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Deposits Redeemed", FALSE, 0 );
	csv.Add( dTotalDepositSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - ( dTotalDepositSpend + dTotalRefund ), SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteCustomerSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = PaymentTotals.GetReportCustomerSpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Customer Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Paid on Account", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Charged to Account", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteRoomSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = PaymentTotals.GetReportRoomSpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Room Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Accounts Settled", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Charged to Room", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteLoyaltySummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = PaymentTotals.GetReportLoyaltySpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Loyalty Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Top-ups Purchased", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Value Redeemed", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteSmartPaySummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = PaymentTotals.GetReportSmartPaySpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "SmartPay Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Top-ups Purchased", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Value Redeemed", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteSmartPhoneSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = PaymentTotals.GetReportSmartPhoneSpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "mySmartPhoneMoney Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Top-ups Purchased", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Value Redeemed", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteSptBookSummary( double dTotalRA, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = PaymentTotals.GetReportSptBookSpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Sports Booker Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Paid On Account", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Charged to Account", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteRASummary( double dTotalRA, double dTotalRefund, CEposReportConsolPaySum& PaymentTotals )
{
	double dTotalSpend = 0.0;
	dTotalSpend += PaymentTotals.GetReportDepositSpend();
	dTotalSpend += PaymentTotals.GetReportCustomerSpend();
	dTotalSpend += PaymentTotals.GetReportRoomSpend();
	dTotalSpend += PaymentTotals.GetReportLoyaltySpend();
	dTotalSpend += PaymentTotals.GetReportSmartPaySpend();
	dTotalSpend += PaymentTotals.GetReportSmartPhoneSpend();
	dTotalSpend += PaymentTotals.GetReportSptBookSpend();
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Account Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Received on Account", FALSE, 0 );
	csv.Add( dTotalRA, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	if ( CPriceHelpers::CompareDoubles( dTotalRefund, 0.0, 3 ) != 0 )
	{
		m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );
		PrepareTotalLine( csv, 0, "Deposits Refunded", FALSE, 0 );
		csv.Add( dTotalRefund, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportDataLine( csv.GetLine() );
	}

	PrepareTotalLine( csv, 0, "Charged to Account", FALSE, 0 );
	csv.Add( dTotalSpend, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Balance", FALSE, 0 );
	csv.Add( dTotalRA - ( dTotalSpend + dTotalRefund ), SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WritePaidInOutSummary( CEposReportPaySumBlock& Block )
{
	double dPaidIn = double ( Block.m_nPaidInTotalCash + Block.m_nPaidInTotalNonCash ) / 100.0; 
	double dPaidOut = double ( Block.m_nPaidOutTotalCash + Block.m_nPaidOutTotalNonCash ) / 100.0; 
	
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Paid In/Out Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Paid In", FALSE, 0 );
	csv.Add( dPaidIn, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	PrepareTotalLine( csv, 0, "Paid Out", FALSE, 0 );
	csv.Add( dPaidOut, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );

	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

	PrepareTotalLine( csv, 0, "Net Paid In", FALSE, 0 );
	csv.Add( dPaidIn - dPaidOut, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::ConsolidatePaymentsForCashSummary( CReportConsolidationArray<CEposReportConsolPaySum>& source, CReportConsolidationArray<CEposReportConsolPaySum>& dest )
{
	for ( int n = 0; n < source.GetSize(); n++ )
	{
		CEposReportConsolPaySum ConsolInfo;
		source.GetAt( n, ConsolInfo );
		dest.Consolidate( ConsolInfo );
	}
}

/**********************************************************************/

void CEposReportPaymentBase::WriteCashSummary( CEposReportPaySumBlock& Block )
{
	m_ReportFile.RequestBlankLines(2);
	m_ReportFile.WriteReportMiscLine( "Cash Summary" );
	m_ReportFile.WriteReportMiscLine( "<LI>" );

	//CONSOLIDATE PAYMENT FOR DEPOSITS & PAYMENT FOR ITEMS
	CReportConsolidationArray<CEposReportConsolPaySum> arrayTemp;
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsAll, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsAccount, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsItem, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsDeposit, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsCustomer, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsRoom, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsLoyalty, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsSmartPay, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsSmartPhone, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsSptBook, arrayTemp );
	ConsolidatePaymentsForCashSummary( *Block.m_pPaymentsMixed, arrayTemp );
	
	//GET TOTAL CASH TENDER
	int nCashTypes = 0;
	double dCashTender = 0.0;
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{	
		CEposReportConsolPaySum ConsolInfo;
		arrayTemp.GetAt( n, ConsolInfo );

		int nPayNo = ConsolInfo.m_nSubType;
		double dPayVal = ConsolInfo.GetReportTender();

		if ( ( 1 == nPayNo ) || ( TRUE == ConsolInfo.m_bIsCash ) )
		{
			switch( EcrmanOptions.GetReportsMultiCashMethod() )
			{
			case REPORTCASH_ALL: 
			case REPORTCASH_METHOD1_CHANGE:
				{
					CCSV csv( '\t' );
					PrepareTotalLine( csv, nPayNo, DataManager.Payment.GetDisplayName( nPayNo ), FALSE, 0 );
					csv.Add( dPayVal, SysInfo.GetDPValue() );
					m_ReportFile.WriteReportDataLine( csv.GetLine() );
					nCashTypes++;
				}
				break;
			}

			dCashTender += dPayVal;
		}
	}

	if (nCashTypes >= 1)
	{
		m_ReportFile.WriteReportMiscLine(m_strSimpleUnderline);
	}

	//WRITE TOTAL TENDER
	CCSV csv( '\t' );
	PrepareTotalLine( csv, 0, "Total Tender", FALSE, 0 );
	csv.Add( dCashTender, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );

	//WRITE PAID IN / OUT
	double dPaidIn = 0.0;
	double dPaidOut = 0.0;
	bool bGotPaidIn = WritePaidInOutDetail( FALSE, Block, dPaidIn );
	bool bGotPaidOut = WritePaidInOutDetail( TRUE, Block, dPaidOut ); 

	//WRITE CHANGE
	bool bGotChange = FALSE;
	double dTotalChange = 0.0;
	for( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CEposReportConsolPaySum ConsolInfo;
		arrayTemp.GetAt( n, ConsolInfo );

		double dChange = ConsolInfo.GetReportChange();
		
		if ( 0.0 != dChange )
		{
			if ( FALSE == bGotChange )
			{
				m_ReportFile.RequestBlankLines( 1 );
				bGotChange = TRUE;
			}
		
			CCSV csv( '\t' );
			PrepareTotalLine( csv, 0, DataManager.Payment.GetDisplayName(ConsolInfo.m_nSubType), FALSE, 0 );
			csv.Add( dChange, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );

			dTotalChange += dChange;
		}
	}

	if ( TRUE == bGotChange )
	{
		m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );
		
		CCSV csv( '\t' );
		PrepareTotalLine( csv, 0, "Total Change", FALSE, 0 );
		csv.Add( dTotalChange, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine( csv.GetLine() );
	}

	//WRITE CASHBACK
	bool bGotCashback = FALSE;
	double dTotalCashback = 0.0;
	for( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CEposReportConsolPaySum ConsolInfo;
		arrayTemp.GetAt( n, ConsolInfo );

		double dCashback = ConsolInfo.GetReportCashback();

		if ( 0.0 != dCashback )
		{
			if ( FALSE == bGotCashback )
			{
				m_ReportFile.RequestBlankLines( 1 );
				bGotCashback = TRUE;
			}

			CCSV csv( '\t' );
			PrepareTotalLine( csv, 0, DataManager.Payment.GetDisplayName(ConsolInfo.m_nSubType), FALSE, 0 );
			csv.Add( dCashback, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );

			dTotalCashback += dCashback;
		}
	}

	if ( TRUE == bGotCashback )
	{
		m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );

		CCSV csv( '\t' );
		PrepareTotalLine( csv, 0, "Total Cashback", FALSE, 0 );
		csv.Add( dTotalCashback, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine( csv.GetLine() );
	}

	if ( ( TRUE == bGotCashback ) || ( TRUE == bGotChange ) || ( TRUE == bGotPaidIn ) || ( TRUE == bGotPaidOut ) )
	{
		m_ReportFile.RequestBlankLines( 1 );
		m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );
		
		CCSV csv( '\t' );
		PrepareTotalLine( csv, 0, "Net Cash", FALSE, 0 );
		csv.Add( dCashTender + dPaidIn - ( dTotalChange + dTotalCashback + dPaidOut ), SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine( csv.GetLine() );
	}
}

/**********************************************************************/

bool CEposReportPaymentBase::WritePaidInOutDetail( bool bOut, CEposReportPaySumBlock& Block, double& dAmount )
{
	bool bResult = FALSE;
	dAmount = 0.0;

	int nDP = SysInfo.GetDPValue();

	__int64 nPaidInOut = 0;

	CArray<int,int> arrayPaidInOutType;
	CArray<__int64,__int64> arrayPaidInOutValue;

	CReportConsolidationArray<CEposReportConsolPaySumOneVal>* pPayments = ( bOut ? Block.m_pPaidOut : Block.m_pPaidIn );

	for ( int nPos = 0; nPos < pPayments -> GetSize(); nPos++ )
	{
		CEposReportConsolPaySumOneVal ConsolInfo;
		pPayments -> GetAt( nPos, ConsolInfo );
		arrayPaidInOutType.Add( ConsolInfo.m_nSubType );
		arrayPaidInOutValue.Add( ConsolInfo.m_nValue );
		nPaidInOut += ConsolInfo.m_nValue;
	}

	__int64 nPaidInOutCash = ( bOut ? Block.m_nPaidOutTotalCash : Block.m_nPaidInTotalCash );
	__int64 nPaidInOutNonCash = ( bOut ? Block.m_nPaidOutTotalNonCash : Block.m_nPaidInTotalNonCash );

	if ( arrayPaidInOutType.GetSize() != 0 )
	{
		m_ReportFile.RequestBlankLines( 1 );

		for ( int n = 0; n < arrayPaidInOutType.GetSize(); n++ )
		{
			double dValue = ( (double) arrayPaidInOutValue.GetAt(n) ) / 100.0;

			int nNo = arrayPaidInOutType.GetAt(n);

			CPaidInOutTextCSVRecord Text;
			if (FALSE == bOut)
			{
				DataManager.PaidInText.GetReportPaidInOutText(nNo, Text);
			}
			else
			{
				DataManager.PaidOutText.GetReportPaidInOutText(nNo, Text);
			}

			CCSV csv( '\t' );
			PrepareTotalLine( csv, arrayPaidInOutType.GetAt(n), Text.GetEposText(), FALSE, 0 );
			csv.Add( dValue, nDP );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}

		if ( ( nPaidInOutNonCash != 0 ) || ( nPaidInOut != nPaidInOutCash ) )
		{
			m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );
		
			if ( nPaidInOutNonCash != 0 )
			{
				double dValue = ( (double) nPaidInOutNonCash ) / 100.0;

				CCSV csv( '\t' );
				PrepareTotalLine( csv, 0, "Non Cash", FALSE, 0 );
				csv.Add( dValue, nDP );
				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}

			if ( nPaidInOut != nPaidInOutCash + nPaidInOutNonCash )
			{
				double dValue = ( (double) nPaidInOut - ( nPaidInOutCash + nPaidInOutNonCash ) ) / 100.0;

				CCSV csv( '\t' );
				PrepareTotalLine( csv, 0, "Exceptions", FALSE, 0 );
				csv.Add( dValue, nDP );
				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
		}

		m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );
		
		dAmount = ( (double) nPaidInOutCash ) / 100.0;

		CCSV csv( '\t' );
		CString strText = ( bOut ? "Paid Out Cash" : "Paid In Cash" );
		PrepareTotalLine( csv, 0, strText, FALSE, 0 );
		csv.Add( dAmount, nDP );
		m_ReportFile.WriteReportMiscLine( csv.GetLine() );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CEposReportPaymentBase::PrepareTotalLine( CCSV& csv, int nNum, const char* szText, bool bShowQty, int nQty )
{
	csv.RemoveAll();

	if (0 == nNum)
	{
		csv.Add("");
	}
	else
	{
		csv.Add(nNum);
	}

	csv.Add ( szText );

	if (EcrmanOptions.GetReportsShowPaymentQuantityFlag() == TRUE)
	{
		if (TRUE == bShowQty)
		{
			csv.Add(nQty);
		}
		else
		{
			csv.Add("");
		}
	}
}

/**********************************************************************/

void CEposReportPaymentBase::WriteTotals( const char* szType, CEposReportConsolPaySum& Totals )
{
	m_ReportFile.WriteReportMiscLine( m_strDetailUnderline );
	
	int nDP = SysInfo.GetDPValue();
			
	CCSV csv ( '\t' );
	PrepareTotalLine( csv, 0, szType, TRUE, Totals.GetReportQty() );

	csv.Add ( Totals.GetReportTender(), nDP );
	csv.Add ( Totals.GetReportAmount(), nDP );
	csv.Add ( Totals.GetReportGratuity(), nDP );
	csv.Add ( Totals.GetReportCashback(), nDP );
	csv.Add ( Totals.GetReportSurplus(), nDP );

	if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
	{
		csv.Add(Totals.GetReportChange(), nDP);
	}

	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPaymentBase::WriteTotals( const char* szType, CEposReportConsolPaySumOneVal& Totals )
{
	m_ReportFile.WriteReportMiscLine( m_strSimpleUnderline );
	
	int nDP = SysInfo.GetDPValue();
			
	CCSV csv ( '\t' );
	PrepareTotalLine( csv, 0, szType, FALSE, 0 );

	csv.Add ( Totals.GetReportValue(), nDP );

	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
