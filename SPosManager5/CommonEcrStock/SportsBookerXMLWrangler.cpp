/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "SportsBookerXMLWrangler.h"
/**********************************************************************/

CSportsBookerXMLWrangler::CSportsBookerXMLWrangler( int nNetworkIdx, CSSXMLParser& Parser ) : CXMLWrangler( nNetworkIdx, Parser, "SB" )
{
	m_nTransactionCount = m_Parser.GetNode ( NULL, m_arTransaction, "xml.Transactions.Transaction" );
}

/**********************************************************************/

void CSportsBookerXMLWrangler::GetTransaction( int nTranIdx, CSportsBookerTransaction& Transaction )
{
	Transaction.Reset();

	if ( ( nTranIdx >= 0 ) && ( nTranIdx < m_nTransactionCount ) )
	{
		CXMLNodeArray* pTransactionXML = m_arTransaction.GetAt(nTranIdx);

		Transaction.SetDate			( m_Parser.GetValue( pTransactionXML, "CreatedDate" ) );
		Transaction.SetTranId		( m_Parser.GetValue( pTransactionXML, "Id" ) );
		Transaction.SetAmount		( m_Parser.GetValue( pTransactionXML, "Amount" ) );
		Transaction.SetType			( GetStringValue( pTransactionXML, "TransactionType", "Id" ) );
		Transaction.SetPriceBand	( GetStringValue( pTransactionXML, "User.PriceBand", "Level" ) );
		Transaction.SetUserId		( GetStringValue( pTransactionXML, "User", "Id" ) );
		Transaction.SetPaymentId	( GetStringValue( pTransactionXML, "PaymentMethod", "Id" ) );
		Transaction.SetPaymentName	( GetStringValue( pTransactionXML, "PaymentMethod", "Name" ) );

		CXMLNodeArray arOrderItem;		
		int nOrderItemCount = m_Parser.GetNode ( pTransactionXML, arOrderItem, "OrderItems.OrderItem" );

		for ( int nItemIdx = 0; nItemIdx < nOrderItemCount; nItemIdx++ )
		{
			CXMLNodeArray* pOrderItemXML = arOrderItem.GetAt( nItemIdx );

			CSportsBookerOrderItem orderItem;
			orderItem.SetPluNo			( m_Parser.GetValue( pOrderItemXML, "PriceCode" ) );
			orderItem.SetPriceBand		( GetStringValue( pOrderItemXML, "PriceBand", "Level" ) );
			orderItem.SetDeptNo			( GetStringValue( pOrderItemXML, "Department", "Code" ) );
			orderItem.SetTaxBand		( GetStringValue( pOrderItemXML, "Tax", "Code" ) );
			orderItem.SetQty			( atoi ( m_Parser.GetValue( pOrderItemXML, "Quantity" ) ) );
			orderItem.SetCostPerItem	( m_Parser.GetValue( pOrderItemXML, "Cost" ) );
			
			Transaction.AddOrderItem( orderItem );
		}
	}
}

/**********************************************************************/

void CSportsBookerXMLWrangler::GetDeptAndGroupNo( const char* szXMLDeptNo, int& nDeptNo, int& nGroupNo )
{
	nDeptNo = atoi(szXMLDeptNo);
	nGroupNo = 0;

	if ( nDeptNo <= 0 )
		nDeptNo = DataManager.Department.GetSportsBookerDeptNo();

	int nDeptIdx;
	if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == TRUE )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt( nDeptIdx, DeptRecord );
		nGroupNo = DeptRecord.GetEposGroup();
	}

	if ( nGroupNo <= 0 )
		nGroupNo = DataManager.EposGroup.GetSportsBookerGroupNo();
}

/**********************************************************************/

void CSportsBookerXMLWrangler::WriteCashRSPFile( CSportsBookerTransaction& Transaction )
{
	if ( FALSE == m_bCashRSPFileOpen )
		return;

	m_fileCashRSP.WriteLine ( "$CASHRSP_VERSION 21" );
	m_fileCashRSP.WriteLine ( "$APPL 1" );

	int nSBTranType = atoi( Transaction.GetType() );
	switch( nSBTranType )
	{
	//BOOKING OF ACTIVITY
	//MEMBERSHIP
	//OPTIONAL EXTRAS
	case 1:
	case 11:
	case 14:
		{
			CString strAmount = GetPennyString( Transaction.GetAmount(), TRUE );

			{
				CCSV csv;
				csv.Add( "$TRAN" );
				csv.Add( Transaction.GetTranId() );
				csv.Add( 0 );											//TABLE NUMBER
				csv.Add( Transaction.GetQty() );						//QTY
				csv.Add( 0 );											//COVERS
				csv.Add( Transaction.GetItemCount() );					//NUMBER OF ITEMS
				csv.Add( strAmount );									//VALUE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( 0 );											//SERVER NUMBER
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			for ( int nItemIdx = 0; nItemIdx < Transaction.GetItemCount(); nItemIdx++ )
			{
				CSportsBookerOrderItem item;
				Transaction.GetOrderItem( nItemIdx, item );

				CString strPriceBand = item.GetPriceBand();
				if ( strPriceBand == "" )
					strPriceBand = Transaction.GetPriceBand();

				int nQty = item.GetQty();
				int nCostPerItem = atoi ( GetPennyString( item.GetCostPerItem(), FALSE ) );
				int nTotalCost = nCostPerItem * nQty;

				CString strTaxBand = "0";
				{
					CString strTaxTemp = item.GetTaxBand();
					if ( strTaxTemp.GetLength() == 1 )
					{
						strTaxBand = CTaxArray::GetTaxBandFromNumber(strTaxTemp.GetAt(0), 1, "0");
					}
				}

				int nDeptNo, nGroupNo;
				GetDeptAndGroupNo( item.GetDeptNo(), nDeptNo, nGroupNo );

				CCSV csv;
				csv.Add( "$ITEM" );
				csv.Add( item.GetPluNo() );								//PLU NUMBER
				csv.Add( nDeptNo );										//DEPT NUMBER
				csv.Add( nQty );										//QUANTITY
				csv.Add( strPriceBand );								//PRICE BAND
				csv.Add( nCostPerItem );								//UNIT SELLING PRICE
				csv.Add( strTaxBand );									//TAX BAND
				csv.Add( "" );											//REFUND FLAG
				csv.Add( "" );											//VOID FLAG
				csv.Add( nGroupNo );									//GROUP NUMBER
				csv.Add( "0" );											//PRICE OVERRIDE FLAG
				csv.Add( nCostPerItem );								//UNIT PRICE FROM PRODUCT FILE
				csv.Add( nTotalCost );									//TOTAL RETAIL PRICE
				csv.Add( "" );											//BARCODE
				csv.Add( "0" );											//DEPOSIT ID
				csv.Add( "0" );											//SUBTOTAL DISCOUNT
				csv.Add( "0" );											//ITEM TYPE
				csv.Add( "0" );											//ITEM ID
				csv.Add( "0" );											//SERVER NUMBER
				csv.Add( "" );											//ANALYSIS CATEGORY
				csv.Add( "0" );											//MIX MATCH NUMBER
				csv.Add( "0" );											//MIX MATCH TYPE
				csv.Add( "B" );											//VOID TYPE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( "0" );											//REASON CODE
				csv.Add( "''" );										//REASON TEXT
				csv.Add( "" );											//ITEM DESCRIPTION
				csv.Add( "0" );											//PRODUCT TYPE
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			{
				CCSV csv;
				csv.Add( "$PAYM" );
				csv.Add( 15 );											//METHOD NUMBER
				csv.Add( "Charge to Account" );							//METHOD NAME
				csv.Add( strAmount );									//AMOUNT
				csv.Add( strAmount );									//TENDER
				csv.Add( "" );											//GRATUITY
				csv.Add( "" );											//CASHBACK
				csv.Add( 0 );											//DEPOSIT ID
				csv.Add( 20 );											//PAYMENT TYPE
				csv.Add( Transaction.GetUserId() );						//ACCOUNT ID
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}
		}
		break;

	//REFUND
	case 2:
		{
			CString strAmount = GetPennyString( Transaction.GetAmount(), FALSE );

			int nDeptNo, nGroupNo;
			GetDeptAndGroupNo( "0", nDeptNo, nGroupNo );

			{
				CCSV csv;
				csv.Add( "$TRAN" );
				csv.Add( Transaction.GetTranId() );
				csv.Add( 0 );											//TABLE NUMBER
				csv.Add( 1 );											//QTY
				csv.Add( 0 );											//COVERS
				csv.Add( 1 );											//NUMBER OF ITEMS
				csv.Add( strAmount );									//VALUE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( 0 );											//SERVER NUMBER
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			{
				CCSV csv;
				csv.Add( "$ITEM" );
				csv.Add( "100000000090001" );							//PLU NUMBER
				csv.Add( nDeptNo );										//DEPT NUMBER
				csv.Add( 1 );											//QUANTITY
				csv.Add( Transaction.GetPriceBand() );					//PRICE BAND
				csv.Add( strAmount );									//UNIT SELLING PRICE
				csv.Add( "0" );											//TAX BAND
				csv.Add( "R" );											//REFUND FLAG
				csv.Add( "" );											//VOID FLAG
				csv.Add( nGroupNo );									//GROUP NUMBER
				csv.Add( "0" );											//PRICE OVERRIDE FLAG
				csv.Add( "0" );											//UNIT PRICE FROM PRODUCT FILE
				csv.Add( strAmount );									//TOTAL RETAIL PRICE
				csv.Add( "" );											//BARCODE
				csv.Add( "0" );											//DEPOSIT ID
				csv.Add( "0" );											//SUBTOTAL DISCOUNT
				csv.Add( "0" );											//ITEM TYPE
				csv.Add( "0" );											//ITEM ID
				csv.Add( "0" );											//SERVER NUMBER
				csv.Add( "1" );											//ANALYSIS CATEGORY
				csv.Add( "0" );											//MIX MATCH NUMBER
				csv.Add( "0" );											//MIX MATCH TYPE
				csv.Add( "B" );											//VOID TYPE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( "0" );											//REASON CODE
				csv.Add( "''" );										//REASON TEXT
				csv.Add( Transaction.GetDescription() );				//ITEM DESCRIPTION
				csv.Add( "0" );											//PRODUCT TYPE
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			{
				CCSV csv;
				csv.Add( "$PAYM" );
				csv.Add( 15 );											//METHOD NUMBER
				csv.Add( "Charge to Account" );							//METHOD NAME
				csv.Add( strAmount );									//AMOUNT
				csv.Add( strAmount );									//TENDER
				csv.Add( "" );											//GRATUITY
				csv.Add( "" );											//CASHBACK
				csv.Add( 0 );											//DEPOSIT ID
				csv.Add( 20 );											//PAYMENT TYPE
				csv.Add( Transaction.GetUserId() );						//ACCOUNT ID
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}
		}
		break;

	//SUPPLEMENT
	//DISCOUNT
	//ADMISSION
	case 3:
	case 5:
	case 13:
		{
			CString strAmount = GetPennyString( Transaction.GetAmount(), TRUE );

			int nDeptNo, nGroupNo;
			GetDeptAndGroupNo( "0", nDeptNo, nGroupNo );

			{
				CCSV csv;
				csv.Add( "$TRAN" );
				csv.Add( Transaction.GetTranId() );
				csv.Add( 0 );											//TABLE NUMBER
				csv.Add( 1 );											//QTY
				csv.Add( 0 );											//COVERS
				csv.Add( 1 );											//NUMBER OF ITEMS
				csv.Add( strAmount );									//VALUE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( 0 );											//SERVER NUMBER
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			{
				CString strPluNo = "";
				switch( nSBTranType )
				{
				case 3:	 strPluNo = "100000000090003";	break;
				case 5:	 strPluNo = "100000000090002";	break;
				case 13: strPluNo = "100000000090004";	break;
				}

				CCSV csv;
				csv.Add( "$ITEM" );
				csv.Add( strPluNo );									//PLU NUMBER
				csv.Add( nDeptNo );										//DEPT NUMBER
				csv.Add( 1 );											//QUANTITY
				csv.Add( Transaction.GetPriceBand() );					//PRICE BAND
				csv.Add( strAmount );									//UNIT SELLING PRICE
				csv.Add( "0" );											//TAX BAND
				csv.Add( "" );											//REFUND FLAG
				csv.Add( "" );											//VOID FLAG
				csv.Add( nGroupNo );									//GROUP NUMBER
				csv.Add( "0" );											//PRICE OVERRIDE FLAG
				csv.Add( "0" );											//UNIT PRICE FROM PRODUCT FILE
				csv.Add( strAmount );									//TOTAL RETAIL PRICE
				csv.Add( "" );											//BARCODE
				csv.Add( "0" );											//DEPOSIT ID
				csv.Add( "0" );											//SUBTOTAL DISCOUNT
				csv.Add( "0" );											//ITEM TYPE
				csv.Add( "0" );											//ITEM ID
				csv.Add( "0" );											//SERVER NUMBER
				csv.Add( "1" );											//ANALYSIS CATEGORY
				csv.Add( "0" );											//MIX MATCH NUMBER
				csv.Add( "0" );											//MIX MATCH TYPE
				csv.Add( "B" );											//VOID TYPE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( "0" );											//REASON CODE
				csv.Add( "''" );										//REASON TEXT
				csv.Add( Transaction.GetDescription() );				//ITEM DESCRIPTION
				csv.Add( "0" );											//PRODUCT TYPE
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			{
				CCSV csv;
				csv.Add( "$PAYM" );
				csv.Add( 15 );											//METHOD NUMBER
				csv.Add( "Charge to Account" );							//METHOD NAME
				csv.Add( strAmount );									//AMOUNT
				csv.Add( strAmount );									//TENDER
				csv.Add( "" );											//GRATUITY
				csv.Add( "" );											//CASHBACK
				csv.Add( 0 );											//DEPOSIT ID
				csv.Add( 20 );											//PAYMENT TYPE
				csv.Add( Transaction.GetUserId() );						//ACCOUNT ID
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}
		}
		break;
	
	//PAYMENT
	//VOUCHER
	//COUPON
	//CREDIT
	case 4:
	case 6:
	case 8:
	case 16:
		{
			CString strAmount = GetPennyString( Transaction.GetAmount(), FALSE );

			int nDeptNo, nGroupNo;
			GetDeptAndGroupNo( "0", nDeptNo, nGroupNo );

			{
				CCSV csv;
				csv.Add( "$TRAN" );
				csv.Add( Transaction.GetTranId() );
				csv.Add( 0 );											//TABLE NUMBER
				csv.Add( 1 );											//QTY
				csv.Add( 0 );											//COVERS
				csv.Add( 1 );											//NUMBER OF ITEMS
				csv.Add( strAmount );									//VALUE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( 0 );											//SERVER NUMBER
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			{
				CCSV csv;
				csv.Add( "$ITEM" );
				csv.Add( "100000000010030" );							//PLU NUMBER
				csv.Add( nDeptNo );										//DEPT NUMBER
				csv.Add( 1 );											//QUANTITY
				csv.Add( 0 );											//PRICE BAND
				csv.Add( strAmount );									//UNIT SELLING PRICE
				csv.Add( "0" );											//TAX BAND
				csv.Add( "" );											//REFUND FLAG
				csv.Add( "" );											//VOID FLAG
				csv.Add( nGroupNo );									//GROUP NUMBER
				csv.Add( "0" );											//PRICE OVERRIDE FLAG
				csv.Add( "0" );											//UNIT PRICE FROM PRODUCT FILE
				csv.Add( strAmount );									//TOTAL RETAIL PRICE
				csv.Add( "" );											//BARCODE
				csv.Add( "0" );											//DEPOSIT ID
				csv.Add( "0" );											//SUBTOTAL DISCOUNT
				csv.Add( "6" );											//ITEM TYPE
				csv.Add( Transaction.GetUserId() );						//ITEM ID
				csv.Add( "0" );											//SERVER NUMBER
				csv.Add( "1" );											//ANALYSIS CATEGORY
				csv.Add( "0" );											//MIX MATCH NUMBER
				csv.Add( "0" );											//MIX MATCH TYPE
				csv.Add( "B" );											//VOID TYPE
				csv.Add( Transaction.GetCashRSPDate() );				//DATE
				csv.Add( Transaction.GetCashRSPTime() );				//TIME
				csv.Add( "0" );											//REASON CODE
				csv.Add( "''" );										//REASON TEXT
				csv.Add( "" );											//ITEM DESCRIPTION
				csv.Add( "0" );											//PRODUCT TYPE
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}

			CString strPaymentId = "";
			CString strPaymentName = "";

			switch( nSBTranType )
			{
			case 4:
			case 6:
			case 8:		
				strPaymentId = Transaction.GetPaymentId();	
				strPaymentName = Transaction.GetPaymentName();
				break;

			case 16:	
				strPaymentId = "999";
				strPaymentName = CSportsBookerPaymentMapCSVArray::GetSportsBookerText(999);
				break;
			}

			{
				CCSV csv;
				csv.Add( "$PAYM" );
				csv.Add( strPaymentId );								//METHOD NUMBER
				csv.Add( strPaymentName );								//METHOD NAME
				csv.Add( strAmount );									//AMOUNT
				csv.Add( strAmount );									//TENDER
				csv.Add( 0 );											//GRATUITY
				csv.Add( 0 );											//CASHBACK
				csv.Add( 0 );											//DEPOSIT ID
				csv.Add( 99 );											//PAYMENT TYPE
				csv.Add( 0 );											//ACCOUNT ID
				m_fileCashRSP.WriteLine( csv.GetLine() );
			}
		}
		break;
	}
}

/**********************************************************************/