/**********************************************************************/
#include "CashRSPCSVLines.h"
#include "CashRSPImporterDefines.h"
#include "CashRSPImporterTransaction.h"
#include "PromotionsCSVArray.h"
/**********************************************************************/

CCashRSPImporterTransaction::CCashRSPImporterTransaction()
{
	ResetTransaction( CASHRSP_TRANSACTION_TYPE_NONE, "" );
}

/**********************************************************************/

void CCashRSPImporterTransaction::ResetTransaction( int nType, const char* szHeader )
{
	m_bProcessTransaction = FALSE;
	m_bGotDuplicate = FALSE;
	m_nTransactionType = nType;
	m_strTransactionHeader = szHeader;
	m_strVersionHeader = "";
	m_nSeqNo = 0;
	m_nTableNo = 0;
	m_bIsPaidIO = FALSE;
	m_bIsDelivery = FALSE;
	m_strCSVDate = "";
	m_strDateTran = "";
	m_strDateItem = "";
	m_strTimeTran = "";
	m_strTNoSalesHistory = "";
	m_nCovers = 0;
	m_nCustomerNo = 0;
	m_nServerNo = 0;
	m_nPaymentNo = 0;
	m_nItemPayRAType = CASHRSP_ITEMPAYTYPE_NORMAL;
	m_bGotNormalItem = FALSE;
	m_nHeaderValue = 0;
	m_dTotalSalesValue = 0.0;
	m_dSubtotalDiscountValue = 0.0;
	m_dLoyaltyDiscountValue = 0.0;
	m_bManualMode = FALSE;
	m_strLoyaltyID = "";

	//ALLOCATING PROMOS AS PLU SALES
	m_nCurrentPromoPluType = PROMO_REPORTASPLU_NONVATABLE;
	m_nCurrentPromoNo = 0;
	m_nCurrentPromoDeptNo = 0;
	m_nCurrentPromoSalesQty = 0;
	m_bCurrentPromoSCTaxFix = FALSE;
	m_bCurrentPromoDiscountForceTaxable = FALSE;

	//INTERMEDIATE BUFFERS FOR PROCESSING
	m_arrayLinePosDiscountItem.RemoveAll();
	m_arrayLinePosDiscountSubtotal.RemoveAll();
	m_arrayLinePosDiscountLoyalty.RemoveAll();
	m_arrayLinePosDiscountPromo.RemoveAll();
	m_arrayLinePosDiscountMixMatch.RemoveAll();
	m_arrayLinePosDiscountVoid.RemoveAll();
	m_arrayLinePosItem.RemoveAll();
	m_arrayLinePosPayment.RemoveAll();
	m_bufferItems.RemoveAll();
	
	//HELPER ARRAYS FOR ASSIGNING ITEM DISCOUNTS
	m_arrayDiscountHelperSales.RemoveAll();
	m_arrayMMPromoDiscountHelperSales.RemoveAll();
	m_arrayDiscountHelperPendingItem.RemoveAll();
	m_arrayDiscountHelperPendingSubtotal.RemoveAll();
	m_arrayDiscountHelperToWrite.RemoveAll();
	m_arrayCheapestOneFree.RemoveAll();
	m_arrayMixMatchTotals.RemoveAll();
	m_arrayMixMatchItems.RemoveAll();
	m_arrayPromoTotals.RemoveAll();
	m_arrayPromoItems.RemoveAll();
	
	//BUFFERS FOR LINES TO WRITE TO OUTPUT FILES
	m_bufferDailyLogTerm.RemoveAll();
	m_bufferDailyLogSales.RemoveAll();
	m_bufferDailyLogMixMatchUsage.RemoveAll();
	m_bufferDailyLogReasons.RemoveAll();
	m_bufferDailyLogAccounts.RemoveAll();
	m_bufferDailyLogAccountsPay.RemoveAll();
	m_bufferPendingStockSales.RemoveAll();
	m_bufferPendingStockDelivery.RemoveAll();
}

/**********************************************************************/

void CCashRSPImporterTransaction::ProcessTransactionHeader( CCSVEposTermLine& csvIn, CCashRSPImporterBatchInfo& BatchInfo, CCashRSPSpooler& CashRSPSpooler, bool bRemoveDuplicates )
{
	m_bProcessTransaction = FALSE;
	m_bGotDuplicate = FALSE;
	m_strDateTran = "";
	m_strCSVDate = "";
	m_strTimeTran = "";
	m_nHeaderValue = 0;
	m_bManualMode = CashRSPSpooler.IsManualMode();
	m_strLoyaltyID = "";
	m_nLineTNo = 0;

	CString strCSVDate = "";
	CString strCSVTime = "";
	bool bStandardTranHeader = FALSE;

	switch( csvIn.GetLineTypeInt() )
	{
	case CASHRSP_LINETYPE_TRAN_NORMAL:
	case CASHRSP_LINETYPE_TRAN_VOID:
	case CASHRSP_LINETYPE_WASTAGE:
		strCSVDate = csvIn.GetTransactionDate();	
		strCSVTime = csvIn.GetTransactionTime();
		m_nHeaderValue = csvIn.GetTransactionValue();
		m_strLoyaltyID = csvIn.GetTransactionLoyaltyIDAsString(m_CashRSPVersionChecker.GetCurrentTransactionVersionNo());
		bStandardTranHeader = TRUE;
		break;

	case CASHRSP_LINETYPE_NOSALE:
		strCSVDate = csvIn.GetNoSaleDate();
		strCSVTime = csvIn.GetNoSaleTime();
		break;

	case CASHRSP_LINETYPE_AGEVERIFY:
		strCSVDate = csvIn.GetAgeVerifyDate();	
		strCSVTime = csvIn.GetAgeVerifyTime();
		break;

	case CASHRSP_LINETYPE_EODHEADER:
		{
			CString strLine = csvIn.GetLine();
			if ( strLine.GetLength() >= 19 )
			{
				strCSVDate.Format( "%s %s %s",
					(const char*) strLine.Mid(10,2),
					(const char*) strLine.Mid(8,2),
					(const char*) strLine.Mid(6,2) );

				strCSVTime.Format( "%s %s %s",
					(const char*) strLine.Mid(13,2),
					(const char*) strLine.Mid(15,2),
					(const char*) strLine.Mid(17,2) );
			}
		}
		break;
	}

	if ( strCSVDate.GetLength() == 8 )
	{
		m_strCSVDate.Format ( "%s%s20%s",
				(const char*) strCSVDate.Left(2),
				(const char*) strCSVDate.Mid(3,2),
				(const char*) strCSVDate.Right(2) );

		m_strDateTran.Format ( "20%s%s%s",
			(const char*) strCSVDate.Right(2),
			(const char*) strCSVDate.Mid(3,2),
			(const char*) strCSVDate.Left(2) );

		m_bProcessTransaction = FALSE;
		m_bProcessTransaction |= ( CASHRSP_PROCESSTYPE_NEWSALE == BatchInfo.m_nProcessType );
		m_bProcessTransaction |= ( CASHRSP_PROCESSTYPE_RETRY == BatchInfo.m_nProcessType );
		m_bProcessTransaction |= ( ( m_strDateTran >= BatchInfo.m_strRebuildDateFrom ) && ( m_strDateTran <= BatchInfo.m_strRebuildDateTo ) );
	}

	if ( CASHRSP_PROCESSTYPE_REBUILD == BatchInfo.m_nProcessType )
	{
		if (TRUE == bRemoveDuplicates)
		{
			int nPos = 0;
			CSortedStringItem item;

			CString strIime = csvIn.GetTransactionTime();
			if ( (DealerFlags.GetDuplicateCheckIgnoreTimeFlag() == TRUE) && ( TRUE == bStandardTranHeader ) )
			{
				csvIn.SetAt(8, "");
			}

			item.m_strItem = csvIn.GetLine();
			if (BatchInfo.m_arrayRebuildHeaders.Find(item, nPos) == FALSE)
			{
				BatchInfo.m_arrayRebuildHeaders.InsertAt(nPos, item);
			}
			else
			{
				m_bGotDuplicate = TRUE;

				/*
				CSSFile fileShowDupes;
				fileShowDupes.Open("E:\\dupes.txt", "ab");
				fileShowDupes.WriteLine(csvIn.GetLine());
				*/
			}

			if ((DealerFlags.GetDuplicateCheckIgnoreTimeFlag() == TRUE) && (TRUE == bStandardTranHeader))
			{
				csvIn.SetAt(8, strIime);
			}
		}
	}

	m_strTimeTran = "000000";
	if ( strCSVTime.GetLength() == 8 )
	{
		m_strTimeTran.Format ( "%s%s%s",
			(const char*) strCSVTime.Left(2),
			(const char*) strCSVTime.Mid(3,2),
			(const char*) strCSVTime.Right(2) );
	}

	int nTillSeqNo = csvIn.GetTransactionNumber();

	m_nSeqNo = nTillSeqNo % 1000000;
	m_nLineTNo = nTillSeqNo / 1000000;
	m_nTableNo = csvIn.GetTransactionTable();
	m_nServerNo = csvIn.GetTransactionServer();
	m_nCovers = csvIn.GetTransactionCovers();

	m_strTNoSalesHistory = BatchInfo.m_strTNoOriginal;

	if (DealerFlags.GetUseCashRSPTillNoFlag() == TRUE)
	{
		m_strTNoSalesHistory.Format("%3.3d", m_nLineTNo);
	}
}

/**********************************************************************/

void CCashRSPImporterTransaction::SetCurrentPromoNo( int nPromoNo, CPromotionsCSVArray& arrayPromotions )
{
	int nPromoIdx;
	m_nCurrentPromoNo = nPromoNo;
	if ( arrayPromotions.FindTableByNumber( nPromoNo, nPromoIdx ) == TRUE )
	{
		CPromotionsCSVRecord Promo;
		arrayPromotions.GetAt( nPromoIdx, Promo );
		Promo.SetServiceChargeVATFixFields();
		m_nCurrentPromoPluType = Promo.GetReportAsPluType();
		m_nCurrentPromoDeptNo = Promo.GetDeptNoReport();
		m_nCurrentPromoSalesQty = Promo.GetReportAsZeroQtyFlag() ? 0 : 1;
		m_bCurrentPromoSCTaxFix = Promo.GetServiceChargeVATFixFlag();
		m_bCurrentPromoDiscountForceTaxable = Promo.GetDiscountAlwaysVATableFlag();
	}
	else
	{
		m_nCurrentPromoPluType = PROMO_REPORTASPLU_NONVATABLE;
		m_nCurrentPromoDeptNo = 0;
		m_nCurrentPromoSalesQty = 0;
		m_bCurrentPromoSCTaxFix = FALSE;
		m_bCurrentPromoDiscountForceTaxable = FALSE;
	}
}

/**********************************************************************/
