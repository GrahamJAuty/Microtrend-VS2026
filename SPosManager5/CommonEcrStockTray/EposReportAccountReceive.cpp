/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "PriceHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportAccountReceive.h"
/**********************************************************************/

CEposReportAccountReceiveInfo::CEposReportAccountReceiveInfo()
{
	m_strEventDate = "";
	m_strEventTime = "";
	m_strPurchaseDate = "";
	m_strPurchaseTime = "";
	COleDateTime m_oleDate = COleDateTime::GetCurrentTime();
	m_nLocIdx = 0;
	m_nAccountID = 0;
	m_nSelectArrayIdx = 0;
	m_nAmount = 0;
	m_nTender = 0;
	m_dReceiveVal = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportAccountReceive::CEposReportAccountReceive( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
	
	m_nReportType = REPORT_TYPE_DEPOSIT_PURCHASED;
	m_strUnderline = "";
	m_nColumnsBeforeTotal = 0;

	m_ReportSettings.SetShowPaymentFlag( TRUE );
	m_ReportSettings.SetShowRedeemFlag( TRUE );
	m_ReportSettings.SetShowRefundFlag( TRUE );
	m_ReportSettings.SetEndDateCutOffFlag( FALSE );
}

/**********************************************************************/

void CEposReportAccountReceive::Consolidate()
{
	CSortedDateTimeItem PurchaseCheckDateTo = DataManagerNonDb.SessionDateTimeFilter.GetCheckDateTo();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	GetConsolidationFileList( arrayFileInfo, nTotalFileSize );

	CEposReportAccountReceiveInfo infoReceive;
	infoReceive.m_strPurchaseDate = "";
	infoReceive.m_strPurchaseTime = "";
	infoReceive.m_strEventDate = "";
	infoReceive.m_strEventTime = "";
	infoReceive.m_oleDate = COleDateTime::GetCurrentTime();
	infoReceive.m_nLocIdx = 0;
	infoReceive.m_nAccountID = 0;
	infoReceive.m_nSelectArrayIdx = 0;
	infoReceive.m_nTender = 0;
	infoReceive.m_nAmount = 0;
	infoReceive.m_dReceiveVal = 0.0;

	m_arrayPaymentBuffer.RemoveAll();
	m_arrayReceiveBuffer.RemoveAll();
	m_arrayRefundBuffer.RemoveAll();
	m_bMixedTransaction = FALSE;

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	bool bInclude = FALSE;
	bool bGotDepositNames = FALSE;

	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );
		
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		SetConsolidationLocation( infoFile, infoReceive, bGotDepositNames );

		COleDateTime timeTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		COleDateTime dateTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		CString strPurchaseDate = "";
		CString strPurchaseTime = "";
		bool bValidPurchaseTime = FALSE;
		bool bValidEventTime = FALSE;

		CString strBuffer;
		while ( fileSales.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVAccountHistory csvIn ( strBuffer );

			int nAccountLineType = csvIn.GetAccountLineType();
			switch( nAccountLineType )
			{
			case ACCOUNT_LINETYPE_HEADER:
				ConsolidatePaymentBuffer( bValidEventTime );
				strPurchaseDate = infoFile.m_strDateTran;
				strPurchaseTime = csvIn.GetHeaderLineTime();
				ValidatePurchaseTime( infoFile.m_nLocIdx, strPurchaseDate, strPurchaseTime, timeTransaction, dateTransaction, bValidPurchaseTime, bValidEventTime );
				bValidPurchaseTime &= ( strPurchaseDate <= PurchaseCheckDateTo.m_strDateTime );
				break;

			case ACCOUNT_LINETYPE_RECEIVE:
				if ( ( TRUE == bValidPurchaseTime ) && ( CheckAccountType ( csvIn.GetReceiveType() ) ) )
				{
					if ( m_SelectArray.GetOriginalSelectionStatus( infoFile.m_nLocIdx, infoFile.m_nTermIdx ) == TRUE )
					{
						CString strEventDate = "";
						CString strEventTime = "";
						if ( ValidateEventTime( infoFile.m_nLocIdx, csvIn.GetReceiveID(), strEventDate, strEventTime ) == TRUE )
						{
							infoReceive.m_nSelectArrayIdx = infoFile.m_nSelectArrayIdx;
							infoReceive.m_oleDate = dateTransaction;
							infoReceive.m_strEventDate = strEventDate;
							infoReceive.m_strEventTime = strEventTime;
							infoReceive.m_strPurchaseDate = strPurchaseDate;
							infoReceive.m_strPurchaseTime = strPurchaseTime;
							infoReceive.m_nAccountID = csvIn.GetReceiveID();
							infoReceive.m_dReceiveVal = csvIn.GetReceiveValue();
							
							if (REPORT_TYPE_DEPOSIT_PURCHASED == m_nReportType)
							{
								if ((DEPOSIT_ADJUST_ID == infoReceive.m_nAccountID) && (infoReceive.m_dReceiveVal < 0.0))
								{
									infoReceive.m_strEventDate = "99999999";
									infoReceive.m_strEventTime = "999999";
									infoReceive.m_strPurchaseDate = "99999999";
									infoReceive.m_strPurchaseTime = "999999";
								}
							}
							
							m_arrayReceiveBuffer.Add( infoReceive );
							bValidEventTime = TRUE;
						}
					}
				}
				break;

			case ACCOUNT_LINETYPE_REFUND:
				if ( ( TRUE == bValidPurchaseTime ) && ( CheckAccountType ( csvIn.GetRefundType() ) ) )
				{
					//PURCHASED DEPOSITS ARE ONLY CHECKED AGAINST REQUESTED TERMINALS
					if ( m_SelectArray.GetOriginalSelectionStatus( infoFile.m_nLocIdx, infoFile.m_nTermIdx ) == TRUE )
					{
						CString strEventDate = "";
						CString strEventTime = "";
						if ( ValidateEventTime( infoFile.m_nLocIdx, csvIn.GetRefundID(), strEventDate, strEventTime ) == TRUE )
						{
							infoReceive.m_strEventDate = strEventDate;
							infoReceive.m_strEventTime = strEventTime;
							infoReceive.m_nSelectArrayIdx = infoFile.m_nSelectArrayIdx;
							infoReceive.m_oleDate = dateTransaction;
							infoReceive.m_strPurchaseDate = strPurchaseDate;
							infoReceive.m_strPurchaseTime = strPurchaseTime;
							infoReceive.m_nAccountID = csvIn.GetRefundID();
							infoReceive.m_dReceiveVal = csvIn.GetRefundValue();
							m_arrayRefundBuffer.Add( infoReceive );
							bValidEventTime = TRUE;
						}
					}
				}
				break;

			case ACCOUNT_LINETYPE_PAYMENT_RA:
				if ( TRUE == bValidPurchaseTime )
					m_arrayPaymentBuffer.Add( strBuffer );
				break;

			case ACCOUNT_LINETYPE_PAYMENT_MIXED:
				if ( TRUE == bValidPurchaseTime )
				{
					m_arrayPaymentBuffer.Add( strBuffer );
					m_bMixedTransaction = TRUE;
				}
				break;
			}

			switch( nAccountLineType )
			{
			case ACCOUNT_LINETYPE_CHARGE:
			case ACCOUNT_LINETYPE_REFUND:
				{
					if ( ProcessCharge( infoFile.m_nLocIdx, nAccountLineType, csvIn ) == TRUE )
					{
						m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, dateTransaction );
						m_SelectArray.ProcessEndDate ( infoFile.m_nSelectArrayIdx, dateTransaction );
					}
				}
				break;
			}
		}

		ConsolidatePaymentBuffer( TRUE );
				
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();

	if ( TRUE == bGotDepositNames )
	{
		CDataManagerInfo infoDM;
		DataManagerNonDb.CloseDepositNames( infoDM );
	}

	ProcessChargedAccounts();
}

/**********************************************************************/

void CEposReportAccountReceive::ConsolidatePaymentBuffer( bool bProcessTransaction )
{
	if ( TRUE == bProcessTransaction )
	{
		int nReceiveSize = m_arrayReceiveBuffer.GetSize();
		int nRefundSize = m_arrayRefundBuffer.GetSize();

		if ( ( nReceiveSize != 0 ) && ( nRefundSize != 0 ) )
		{
			ConsolidateReceiptsToPaymentType( FALSE, 996 );
			ConsolidateReceiptsToPaymentType( TRUE, 996 );
		}
		else if ( nReceiveSize != 0 )
		{
			ConsolidatePaymentBufferInternal( FALSE );
		}
		else if ( nRefundSize != 0 )
		{
			ConsolidatePaymentBufferInternal( TRUE );
		}
	}

	m_arrayReceiveBuffer.RemoveAll();
	m_arrayRefundBuffer.RemoveAll();
	m_arrayPaymentBuffer.RemoveAll();
	m_bMixedTransaction = FALSE;
}

/**********************************************************************/

void CEposReportAccountReceive::ConsolidatePaymentBufferInternal( bool bRefund )
{
	if ( ( FALSE == bRefund ) || ( m_ReportSettings.GetShowRefundFlag() ) )
	{
		CArray<CEposReportAccountReceiveInfo,CEposReportAccountReceiveInfo>* pReceipts = ( bRefund ) ? &m_arrayRefundBuffer : &m_arrayReceiveBuffer;

		//IF NO DEPOSITS HAVE BEEN PURCHASED THEN NOTHING NEEDS TO BE DONE
		if ( pReceipts -> GetSize() == 0 )
			return;

		//IF NO PAYMENT TYPE HAS BEEN USED THEN ASSIGN ALL DEPOSITS AS UNKNOWN PAYMENT
		if ( m_arrayPaymentBuffer.GetSize() == 0 )
		{
			ConsolidateReceiptsToPaymentType( bRefund, 999);
			return;
		}

		//CALCULATE TOTAL DEPOSIT AMOUNT
		double dTotalReceived = 0.0;
		for ( int d = 0; d < pReceipts -> GetSize(); d++ )
		{
			CEposReportAccountReceiveInfo infoReceive = pReceipts -> GetAt(d);
			dTotalReceived += infoReceive.m_dReceiveVal;
		}

		//CALCULATE TOTAL PAYMENT AMOUNT
		double dTotalPayment = 0.0;
		for ( int n = 0; n < m_arrayPaymentBuffer.GetSize(); n++ )
		{
			CCSVAccountHistory csvIn ( m_arrayPaymentBuffer.GetAt(n) );
			dTotalPayment += csvIn.GetPaymentAmount();
		}

		//FLAG ZERO PAYMENT
		if ( CPriceHelpers::CompareDoubles( dTotalPayment, 0.0, 3 ) == 0 )
		{
			ConsolidateReceiptsToPaymentType(bRefund, 994);
			return;
		}

		//FLAG INSUFFICIENT PAYMENT
		if ( FALSE == bRefund )
		{
			if ( CPriceHelpers::CompareDoubles( dTotalReceived, dTotalPayment, 3 ) == 1 )
			{
				ConsolidateReceiptsToPaymentType( bRefund, 998 );
				return;
			}
		}
		else
		{
			if ( CPriceHelpers::CompareDoubles( dTotalReceived, ( dTotalPayment * -1 ), 3 ) == 1 )
			{
				ConsolidateReceiptsToPaymentType( bRefund, 997 );
				return;
			}
		}

		//FLAG IF MULTIPLE PAYMENT TYPES IN MIXED TRANSACTION
		if ( TRUE == m_bMixedTransaction )
		{
			if ( m_arrayPaymentBuffer.GetSize() > 1 )
			{
				ConsolidateReceiptsToPaymentType( FALSE, 995 );
				return;
			}
		}

		//IF ONLY ONE PAYMENT TYPE HAS BEEN USED THEN ASSIGN ALL DEPOSITS TO THAT TYPE
		if ( m_arrayPaymentBuffer.GetSize() == 1 )
		{
			CCSVAccountHistory csvIn ( m_arrayPaymentBuffer.GetAt(0) );
			int nPaymentType = csvIn.GetPaymentNumber();
			ConsolidateReceiptsToPaymentType( bRefund, nPaymentType);
			return;
		}

		//ALLOCATE SPLIT PAYMENTS TO DEPOSITS
		CEposReportAccountReceiveInfo infoReceipt = pReceipts -> GetAt(0);

		for ( int d = 0; d < pReceipts -> GetSize(); d++ )
		{
			CEposReportAccountReceiveInfo infoReceipt = pReceipts -> GetAt(d);

			for ( int p = 0; p < m_arrayPaymentBuffer.GetSize(); p++ )
			{
				CCSVAccountHistory csvIn ( m_arrayPaymentBuffer.GetAt(p) );

				int nPaymentType = csvIn.GetPaymentNumber();
				double dPaymentAmount = csvIn.GetPaymentAmount();
				double dReceiveAmount = ( infoReceipt.m_dReceiveVal * dPaymentAmount ) / dTotalPayment; 

				//INDIVIDUAL PURCHASE OR REFUND LINE
				CEposReportConsolAccountReceive infoConsol;
				infoConsol.m_nLocSort = dbLocation.GetSortIdxFromLocIdx( infoReceipt.m_nLocIdx );
				infoConsol.m_nType = ( bRefund ) ? 2 : 1;
				infoConsol.m_strEventDate = infoReceipt.m_strEventDate;
				infoConsol.m_strEventTime = infoReceipt.m_strEventTime;
				infoConsol.m_strPurchaseDate = infoReceipt.m_strPurchaseDate;
				infoConsol.m_strPurchaseTime = infoReceipt.m_strPurchaseTime;
				infoConsol.m_nAccountID = infoReceipt.m_nAccountID;
				infoConsol.m_nPaymentType = nPaymentType;
				infoConsol.m_dVal = dReceiveAmount;
				m_ConsolArray.Consolidate( infoConsol );

				m_SelectArray.ProcessStartDate ( infoReceipt.m_nSelectArrayIdx, infoReceipt.m_oleDate );
				m_SelectArray.ProcessEndDate( infoReceipt.m_nSelectArrayIdx, infoReceipt.m_oleDate );
			}
		}
	}
}

/**********************************************************************/

void CEposReportAccountReceive::ConsolidateReceiptsToPaymentType( bool bRefund, int nPaymentType )
{
	if ( ( FALSE == bRefund ) || ( m_ReportSettings.GetShowRefundFlag() ) )
	{
		CArray<CEposReportAccountReceiveInfo,CEposReportAccountReceiveInfo>* pReceipts = ( bRefund ) ? &m_arrayRefundBuffer : &m_arrayReceiveBuffer;

		for ( int n = 0; n < pReceipts -> GetSize(); n++ )
		{
			CEposReportAccountReceiveInfo infoReceipt = pReceipts -> GetAt(n);

			//INDIVIDUAL PURCHASE OR REFUND LINE
			CEposReportConsolAccountReceive infoConsol;
			infoConsol.m_nLocSort = dbLocation.GetSortIdxFromLocIdx( infoReceipt.m_nLocIdx );
			infoConsol.m_nType = ( bRefund ) ? 2 : 1;
			infoConsol.m_strEventDate = infoReceipt.m_strEventDate;
			infoConsol.m_strEventTime = infoReceipt.m_strEventTime;
			infoConsol.m_strPurchaseDate = infoReceipt.m_strPurchaseDate;
			infoConsol.m_strPurchaseTime = infoReceipt.m_strPurchaseTime;
			infoConsol.m_nAccountID = infoReceipt.m_nAccountID;
			infoConsol.m_nPaymentType = nPaymentType;
			infoConsol.m_dVal = infoReceipt.m_dReceiveVal;
			m_ConsolArray.Consolidate( infoConsol );

			m_SelectArray.ProcessStartDate ( infoReceipt.m_nSelectArrayIdx, infoReceipt.m_oleDate );
			m_SelectArray.ProcessEndDate( infoReceipt.m_nSelectArrayIdx, infoReceipt.m_oleDate );
		}
	}
}

/**********************************************************************/

void CEposReportAccountReceive::ProcessChargedAccounts()
{
	if ( IsDepositReport() == TRUE )
	{
		bool bKeepCharge = FALSE;
		for ( int nConsolPos = 0; nConsolPos < m_ConsolArray.GetSize(); nConsolPos++ )
		{
			CEposReportConsolAccountReceive infoReceive;
			m_ConsolArray.GetAt( nConsolPos, infoReceive );

			if ( infoReceive.m_nType == 1 )
			{
				CEposReportConsolAccountCharged infoRedeem;
				infoRedeem.m_nLocSort = infoReceive.m_nLocSort;
				infoRedeem.m_nAccountID = infoReceive.m_nAccountID;
				
				int nRedeemPos;
				if ( m_arrayAccountCharged.Find( infoRedeem, nRedeemPos ) == TRUE )
				{
					m_arrayAccountCharged.GetAt( nRedeemPos, infoRedeem );
					infoReceive.m_nNewDepositStatus = infoRedeem.m_nNewDepositStatus;
					infoRedeem.m_bKeepMe = TRUE;
					m_ConsolArray.SetAt( nConsolPos, infoReceive );
					m_arrayAccountCharged.SetAt( nRedeemPos, infoRedeem );
					bKeepCharge = TRUE;
				}
			}
		}

		if (FALSE == bKeepCharge)
		{
			m_arrayAccountCharged.RemoveAll();
		}
		else
		{
			for ( int nRedeemPos = m_arrayAccountCharged.GetSize() - 1; nRedeemPos >=0; nRedeemPos-- )
			{
				CEposReportConsolAccountCharged infoRedeem;
				m_arrayAccountCharged.GetAt( nRedeemPos, infoRedeem );

				if (FALSE == infoRedeem.m_bKeepMe)
				{
					m_arrayAccountCharged.RemoveAt(nRedeemPos);
				}
			}
		}
	}
}

/**********************************************************************/

bool CEposReportAccountReceive::ProcessCharge( int nLocIdx, int nAccountLineType, CCSVAccountHistory& csvIn )
{
	bool bResult = FALSE;
	if ( IsDepositReport() == TRUE )
	{
		if ( CheckAccountType( csvIn.GetChargeType() == TRUE ) )
		{
			CEposReportConsolAccountCharged info;
			info.m_nLocSort = dbLocation.GetSortIdxFromLocIdx( nLocIdx );
			info.m_bKeepMe = FALSE;

			if ( ACCOUNT_LINETYPE_REFUND == nAccountLineType )
			{
				info.m_nAccountID = csvIn.GetRefundID();
				info.m_nNewDepositStatus = NEWDEPOSIT_REFUNDED;
			}
			else
			{
				info.m_nAccountID = csvIn.GetChargeID();
				info.m_nNewDepositStatus = NEWDEPOSIT_REDEEMED;
			}

			m_arrayAccountCharged.Consolidate( info );
			bResult = TRUE;
		}
	}
	return bResult;
}

/**********************************************************************/

bool CEposReportAccountReceive::CheckAccountType( int nType )
{
	if ( IsDepositReport() == TRUE )
		return ( CASHRSP_ITEMPAYTYPE_DEPOSIT == nType );
	else
		return ( CASHRSP_ITEMPAYTYPE_CUSTOMER == nType );
}

/**********************************************************************/

void CEposReportAccountReceive::SetConsolidationLocation( CTermFileListInfo& infoFile, CEposReportAccountReceiveInfo& infoReceive, bool& bGotDepositNames )
{
	infoReceive.m_nLocIdx = infoFile.m_nLocIdx;
}

/**********************************************************************/

bool CEposReportAccountReceive::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );

	AddColumns();

	int nReceivePos = 0;
	int nNextBlockPos = 0;
	int nCurrentDbNo = -1;
	
	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );
	
	while ( nReceivePos < m_ConsolArray.GetSize() )
	{
		CEposReportConsolAccountReceive infoThisBlock;
		m_ConsolArray.GetAt( nReceivePos, infoThisBlock );
		
		int nLocIdx = dbLocation.GetLocIdxFromSortIdx ( infoThisBlock.m_nLocSort );
		int nDbNo = dbLocation.GetDbNo( nLocIdx );

		int nRefundPos = FindReportItem( infoThisBlock.m_nLocSort, 2 );
		//int nPaymentPos = FindReportItem( infoThisBlock.m_nLocSort, 3 );
		nNextBlockPos = FindReportItem( infoThisBlock.m_nLocSort + 1, 1 );

		if ( nDbNo != nCurrentDbNo )
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			}

			nCurrentDbNo = nDbNo;
		}
		
		if ( 0 != nReceivePos )
			m_ReportFile.RequestBlankLines(1);

		CDataManagerInfo infoDM;
		DataManagerNonDb.OpenDepositNames( DB_READONLY, nLocIdx, infoDM );

		m_PaymentArray.RemoveAll();

		{
			CArray<int,int> arrayLineIdx;
			ShowReceiveBlock( nLocIdx, nReceivePos, nRefundPos, arrayLineIdx, FALSE, NEWDEPOSIT_AVAILABLE );
			
			if ( m_ReportSettings.GetShowRedeemFlag() )
				ShowReceiveBlock( nLocIdx, 0, 0, arrayLineIdx, FALSE, NEWDEPOSIT_REDEEMED );
			
			if ( m_ReportSettings.GetShowRefundFlag() )
				ShowReceiveBlock( nLocIdx, 0, 0, arrayLineIdx, FALSE, NEWDEPOSIT_REFUNDED );
		}

		if ( m_ReportSettings.GetShowRefundFlag() )
		{
			//SEPARATE BLOCK TO CREATE NEW EMPTY LINE IDX ARRAY
			CArray<int,int> arrayLineIdx;
			ShowReceiveBlock( nLocIdx, nRefundPos, nNextBlockPos, arrayLineIdx, TRUE, NEWDEPOSIT_AVAILABLE );
		}
	
		if ( m_ReportSettings.GetShowPaymentFlag() )
			ShowPaymentBlock( nLocIdx );
	
		DataManagerNonDb.CloseDepositNames( infoDM );

		nReceivePos = nNextBlockPos;
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportAccountReceive::ShowReceiveBlock( int nLocIdx, int nStartPos, int nEndPos, CArray<int, int>& arrayLineIdx, bool bRefund, int nNewDepositStatus )
{
	bool bFirstPass = ( arrayLineIdx.GetSize() == 0 );

	if ( FALSE == bFirstPass )
	{
		nStartPos = 0;
		nEndPos = arrayLineIdx.GetSize();
	}

	if ( nEndPos > nStartPos )
	{
		bool bFirstLineThisBlock = TRUE;
		
		double dTotal = 0.0;
		CEposReportConsolAccountReceive CurrentReceive;
		CurrentReceive.m_nAccountID = -1;

		for ( int nPos = nStartPos; nPos < nEndPos; nPos++ )
		{
			int nArrayPos = 0;
			if (FALSE == bFirstPass)
			{
				nArrayPos = arrayLineIdx.GetAt(nPos);
			}
			else
			{
				nArrayPos = nPos;
			}

			CEposReportConsolAccountReceive infoLine;
			m_ConsolArray.GetAt( nArrayPos, infoLine );

			//SKIP REFUNDS THAT ARE ALREADY LISTED AS PURCHASES
			if ( TRUE == bRefund )
			{
				CEposReportConsolAccountCharged infoRedeem;
				infoRedeem.m_nLocSort = infoLine.m_nLocSort;
				infoRedeem.m_nAccountID = infoLine.m_nAccountID;

				int nChartPos;
				if (m_arrayAccountCharged.Find(infoRedeem, nChartPos) == TRUE)
				{
					continue;
				}
			}

			if ( infoLine.m_nNewDepositStatus == nNewDepositStatus )
			{
				if ( TRUE == bFirstLineThisBlock )
				{
					m_ReportFile.RequestBlankLines(1);
					m_ReportFile.WriteReportMiscLine( GetReceiveBlockHeaderLine( nLocIdx, bRefund, nNewDepositStatus ) );
					m_ReportFile.WriteReportMiscLine( "<LI>" );
					bFirstLineThisBlock = FALSE;
				}

				dTotal += infoLine.m_dVal;

				CCSV csv( '\t' );

				bool bRecycledDeposits = FALSE;

				if ( infoLine.CompareReceive( CurrentReceive ) == FALSE )			
				{
					bRecycledDeposits = CreateReceiveLine( nLocIdx, infoLine, csv );
					CurrentReceive = infoLine;
				}
				else
				{
					for (int n = 0; n < m_nColumnsBeforeTotal - 1; n++)
					{
						csv.Add("");
					}

					csv.Add( GetPaymentLabel( infoLine.m_nPaymentType, "" ) );
					csv.Add( infoLine.m_dVal, 2 );
				}

				if (TRUE == bRecycledDeposits)
				{
					m_ReportFile.WriteReportMiscLine(m_strUnderline);
				}

				m_ReportFile.WriteReportDataLine( csv.GetLine() );

				//PAYMENT LOGIC
				if ( m_ReportSettings.GetShowPaymentFlag() == TRUE )
				{
					if ( FALSE == bRefund )
					{
						switch( nNewDepositStatus )
						{
						case NEWDEPOSIT_AVAILABLE:
						case NEWDEPOSIT_REDEEMED:
							{
								//PAYMENT TYPE TOTAL
								CEposReportConsolAccountPayment infoPayment;
								infoPayment.m_nLineType = 1;
								infoPayment.m_nPaymentType = infoLine.m_nPaymentType;
								infoPayment.m_dVal = infoLine.m_dVal;
								m_PaymentArray.Consolidate( infoPayment );
								//PAYMENT TOTAL
								infoPayment.m_nLineType = 3;
								infoPayment.m_nPaymentType = 0;
								m_PaymentArray.Consolidate( infoPayment );
								//NET TOTAL
								infoPayment.m_nLineType = 5;
								m_PaymentArray.Consolidate( infoPayment );
							}
							break;

						case NEWDEPOSIT_REFUNDED:
							{
								//PAYMENT TYPE TOTAL
								CEposReportConsolAccountPayment infoPayment;
								infoPayment.m_nLineType = 1;
								infoPayment.m_nPaymentType = infoLine.m_nPaymentType;
								infoPayment.m_dVal = infoLine.m_dVal;
								m_PaymentArray.Consolidate( infoPayment );
								//PAYMENT TOTAL
								infoPayment.m_nLineType = 3;
								infoPayment.m_nPaymentType = 0;
								m_PaymentArray.Consolidate( infoPayment );
								//REFUND TYPE TOTAL
								infoPayment.m_nLineType = 2;
								infoPayment.m_nPaymentType = infoLine.m_nPaymentType;
								infoPayment.m_dVal = infoLine.m_dVal;
								m_PaymentArray.Consolidate( infoPayment );
								//PAYMENT TOTAL
								infoPayment.m_nLineType = 4;
								infoPayment.m_nPaymentType = 0;
								m_PaymentArray.Consolidate( infoPayment );
								//GRAND TOTAL
								infoPayment.m_nLineType = 5;
								infoPayment.m_nPaymentType = 0;
								infoPayment.m_dVal = 0.0;
								m_PaymentArray.Consolidate( infoPayment );
							}
							break;
						}
					}
					else
					{
						//REFUND TYPE TOTAL
						CEposReportConsolAccountPayment infoPayment;
						infoPayment.m_nLineType = 2;
						infoPayment.m_nPaymentType = infoLine.m_nPaymentType;
						infoPayment.m_dVal = infoLine.m_dVal;
						m_PaymentArray.Consolidate( infoPayment );
						//REFUND TOTAL
						infoPayment.m_nLineType = 4;
						infoPayment.m_nPaymentType = 0;
						m_PaymentArray.Consolidate( infoPayment );
						//NET TOTAL
						infoPayment.m_nLineType = 5;
						infoPayment.m_dVal *= -1;
						m_PaymentArray.Consolidate( infoPayment );
					}
				}

				if ( FALSE == bFirstPass )
				{
					arrayLineIdx.RemoveAt( nPos );
					nPos--;
					nEndPos--;
				}
			}
			else
			{
				if (TRUE == bFirstPass)
				{
					arrayLineIdx.Add(nPos);
				}
			}
		}

		if (FALSE == bFirstLineThisBlock)
		{
			WriteTotal(dTotal);
		}
	}
}

/**********************************************************************/

void CEposReportAccountReceive::ShowPaymentBlock( int nLocIdx )
{
	if ( m_PaymentArray.GetSize() != 0 )
	{
		CString strLocation = "";
		strLocation.Format( "<..>%s", dbLocation.GetName( nLocIdx ) );

		m_ReportFile.RequestBlankLines( 1 );
		m_ReportFile.WriteReportMiscLine( strLocation + ", Payment Summary" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		int nLastLineType = 0;
		bool bGotPayment = FALSE;
		bool bGotRefund = FALSE;

		for ( int nPos = 0; nPos < m_PaymentArray.GetSize(); nPos++ )
		{
			CEposReportConsolAccountPayment infoLine;
			m_PaymentArray.GetAt( nPos, infoLine );

			bool bWantSkip = FALSE;
			bool bWantUnderline = FALSE;
			switch( infoLine.m_nLineType )
			{
			case 1:
				bWantSkip = FALSE;
				bWantUnderline = FALSE;
				bGotPayment = TRUE;
				break;

			case 2:
				bWantSkip = FALSE;
				bWantUnderline = bGotPayment && ( FALSE == bGotRefund );
				bGotRefund = TRUE;
				break;

			case 3:
				bWantSkip = ( FALSE == bGotRefund );
				bWantUnderline = TRUE;
				break;

			case 4:
				bWantSkip = ( FALSE == bGotPayment );
				bWantUnderline = FALSE;
				break;

			case 5:
				bWantSkip = FALSE;
				bWantUnderline = TRUE;
				break;
			}

			if ( TRUE == bWantSkip )
				continue;

			if (TRUE == bWantUnderline)
			{
				m_ReportFile.WriteReportMiscLine(m_strUnderline);
			}

			CCSV csv( '\t' );

			for (int n = 0; n < m_nColumnsBeforeTotal - 2; n++)
			{
				csv.Add("");
			}

			CString str = "<..>";
			switch( infoLine.m_nLineType )
			{
			case 1:		str += GetPaymentLabel( infoLine.m_nPaymentType, "Payment" );	break;
			case 2:		str += GetPaymentLabel( infoLine.m_nPaymentType, "Refund" );	break;
			case 3:		str += "Payment Total";	break;
			case 4:		str += "Refund Total";	break;
			default:	str += "";				break;
			}
			csv.Add( str );

			switch( infoLine.m_nLineType )
			{
			case 1:
			case 2:
				switch( infoLine.m_nPaymentType )
				{
				case 999:	csv.Add( "(No payment)" );									break;
				case 998:	csv.Add( "(Under payment)" );								break;
				case 997:	csv.Add( "(Under refund)" );								break;
				case 996:	csv.Add( "(Payment with Refund)" );							break;
				case 995:	csv.Add( "(Mixed transaction)" );							break;
				case 994:	csv.Add( "(Zero payment)" );								break;
				default:	csv.Add( GetPaymentLabel( infoLine.m_nPaymentType, "" ) );	break;
				}
				break;

			case 5:		
				csv.Add ( ( bGotPayment && bGotRefund ) ? "Net Total" : "Total" ); break;
			
			default:
				csv.Add( "" );
				break;
			}

			csv.Add( infoLine.m_dVal, 2 );
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}
	}
}

/**********************************************************************/

int CEposReportAccountReceive::FindReportItem( int nLocSort, int nType )
{
	CEposReportConsolAccountReceive infoConsol;
	infoConsol.m_nLocSort = nLocSort;
	infoConsol.m_nType = nType;

	int nPos;
	m_ConsolArray.Find( infoConsol, nPos );
	
	return nPos;
}

/**********************************************************************/

void CEposReportAccountReceive::WriteTotal( double dTotal )
{
	m_ReportFile.WriteReportMiscLine( m_strUnderline );

	CCSV csv( '\t' );
	
	for (int n = 0; n < m_nColumnsBeforeTotal - 1; n++)
	{
		csv.Add("");
	}

	csv.Add( "Total" );
	csv.Add( dTotal, 2 );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/

const char* CEposReportAccountReceive::GetReceiveBlockHeaderLine( int nLocIdx, bool bRefund, int nNewDepositStatus )
{
	m_strReceiveBlockHeaderLine.Format( "<..>%s", dbLocation.GetName( nLocIdx ) );

	if ( IsDepositReport() == TRUE )
	{
		if (TRUE == bRefund)
		{
			m_strReceiveBlockHeaderLine += ", Previous Deposits Refunded";
		}
		else 
		{
			switch( nNewDepositStatus )
			{
			case NEWDEPOSIT_REDEEMED:
				m_strReceiveBlockHeaderLine += ", Deposits Purchased (Redeemed)";
				break;

			case NEWDEPOSIT_REFUNDED:
				m_strReceiveBlockHeaderLine += ", Deposits Purchased (Refunded)";
				break;

			case NEWDEPOSIT_AVAILABLE:
			default:
				m_strReceiveBlockHeaderLine += ", Deposits Purchased (Available)";
				break;
			}
		}
	}
	else
	{
		m_strReceiveBlockHeaderLine += ", Customer Payments (RA)";
	}
		
	return m_strReceiveBlockHeaderLine;
}

/**********************************************************************/

const char* CEposReportAccountReceive::GetPaymentLabel( int nPaymentType, const char* szDefault )
{
	switch( nPaymentType )
	{
	case 999:
	case 998:
	case 997:
	case 996:
	case 995:
	case 994:
		m_strPaymentLabel.Format( "Error %d", nPaymentType );
		break;

	default:
		{
			CString strDefault = szDefault;
			if (strDefault != "")
			{
				m_strPaymentLabel = strDefault;
			}
			else
			{
				m_strPaymentLabel = DataManager.Payment.GetDisplayName(nPaymentType);
			}
		}
		break;
	}

	return m_strPaymentLabel;
}

/**********************************************************************/

void CEposReportAccountReceive::ValidatePurchaseTime( int nLocIdx, CString& strPurchaseDate, CString& strPurchaseTime, COleDateTime& timeTransaction, COleDateTime& dateTransaction, bool& bValidPurchaseTime, bool& bValidEventTime )
{
	bValidPurchaseTime = ValidateAccountDate( nLocIdx, strPurchaseDate, strPurchaseTime, timeTransaction, dateTransaction );
	bValidEventTime = TRUE;
}

/**********************************************************************/

bool CEposReportAccountReceive::ValidateEventTime( int nLocIdx, int nAccountID, CString& strEventDate, CString& strEventTime )
{
	strEventDate = "";
	strEventTime = "";
	return TRUE;
}

/**********************************************************************/