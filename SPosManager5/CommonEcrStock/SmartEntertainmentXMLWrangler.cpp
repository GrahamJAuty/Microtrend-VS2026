/**********************************************************************/
#include "SmartEntertainmentXMLWrangler.h"
/**********************************************************************/

CSmartEntertainmentXMLWrangler::CSmartEntertainmentXMLWrangler( int nNetworkIdx, CSSXMLParser& Parser ) : CXMLWrangler( nNetworkIdx, Parser, "SE" )
{
	m_nTransactionCount = m_Parser.GetNode ( NULL, m_arTransaction, "Transactions.Transaction" );
	m_nPaymentCount = 0;
}

/**********************************************************************/

int CSmartEntertainmentXMLWrangler::GetPaymentCount( int nTranIdx )
{
	m_nPaymentCount = 0;

	if ( ( nTranIdx >= 0 ) && ( nTranIdx < m_nTransactionCount ) )
	{ 
		m_arPayment.RemoveAll();
		CXMLNodeArray* pTransactionXML = m_arTransaction.GetAt(nTranIdx);
		m_nPaymentCount = m_Parser.GetNode ( pTransactionXML, m_arPayment, "Payments.Payment" );
	}

	return m_nPaymentCount;
}

/**********************************************************************/

void CSmartEntertainmentXMLWrangler::GetTransaction( int nTranIdx, int nPayIdx, CSmartEntertainmentTransaction& Transaction )
{
	Transaction.Reset();

	if ( ( nTranIdx >= 0 ) && ( nTranIdx < m_nTransactionCount ) && ( nPayIdx >= 0 ) && ( nPayIdx < m_nPaymentCount ) )
	{
		CXMLNodeArray* pTransactionXML = m_arTransaction.GetAt(nTranIdx);
		CXMLNodeArray* pPaymentXML = m_arPayment.GetAt( nPayIdx );
		
		Transaction.SetTranId			( m_Parser.GetValue( pTransactionXML, "Id" ) );
		Transaction.SetSaleDate			( m_Parser.GetValue( pTransactionXML, "CreatedDate" ) );
		Transaction.SetPaymentDate		( m_Parser.GetValue( pPaymentXML, "CreatedDate" ) );
		
		CSmartEntertainmentPayment Payment;
		Payment.SetID					( atoi ( GetStringValue( pPaymentXML, "PaymentMethod", "Id" ) ) );
			
		{
			CString strAmount = m_Parser.GetValue( pPaymentXML, "Amount" );
			Transaction.SetAmount( strAmount );
			Payment.SetAmount( strAmount );
		}

		Transaction.AddPayment( Payment );

		{
			CXMLNodeArray arItem;		
			int nItemCount = m_Parser.GetNode ( pPaymentXML, arItem, "OrderItems.OrderItem" );

			for ( int nItemIdx = 0; nItemIdx < nItemCount; nItemIdx++ )
			{
				CXMLNodeArray* pItemXML = arItem.GetAt( nItemIdx );

				CSmartEntertainmentItem Item;
				Item.SetPluNo		( m_Parser.GetValue( pItemXML, "PriceCode" ) );
				Item.SetDeptNo		( atoi ( m_Parser.GetValue( pItemXML, "Department" ) ) );
				Item.SetQty			( atoi ( m_Parser.GetValue( pItemXML, "Quantity" ) ) );
				Item.SetLineCost	( m_Parser.GetValue( pItemXML, "LineCost" ) );
				Item.SetDescription	( m_Parser.GetValue( pItemXML, "Description" ) );
				
				Transaction.AddItem( Item );
			}
		}
	}
}

/**********************************************************************/

void CSmartEntertainmentXMLWrangler::WriteCashRSPFile( CSmartEntertainmentTransaction& Transaction, CReportConsolidationArray<CSortedStringItem>& arrayTrans, bool& bDuplicate )
{
	bDuplicate = FALSE;

	if ( FALSE == m_bCashRSPFileOpen )
		return;

	{
		CString strAmount = GetPennyString( Transaction.GetAmount(), FALSE );

		CCSV csv;
		csv.Add( "$TRAN" );
		csv.Add( Transaction.GetTranId() );
		csv.Add( 0 );											//TABLE NUMBER
		csv.Add( Transaction.GetQty() );						//QTY
		csv.Add( 0 );											//COVERS
		csv.Add( Transaction.GetItemCount() );					//NUMBER OF ITEMS
		csv.Add( strAmount );									//VALUE
		csv.Add( Transaction.GetCashRSPPaymentDate() );			//DATE
		csv.Add( Transaction.GetCashRSPPaymentTime() );			//TIME
		csv.Add( 0 );											//SERVER NUMBER

		CString strTran = csv.GetLine();

		int nPos;
		CSortedStringItem item;
		item.m_strItem = strTran;
		if ( arrayTrans.Find( item, nPos ) == FALSE )
		{
			m_fileCashRSP.WriteLine ( "$CASHRSP_VERSION 21" );
			m_fileCashRSP.WriteLine ( "$APPL 1" );
			m_fileCashRSP.WriteLine( csv.GetLine() );
		}
		else
		{
			bDuplicate = TRUE;
			return;
		}
	}

	for ( int nItemIdx = 0; nItemIdx < Transaction.GetItemCount(); nItemIdx++ )
	{
		CSmartEntertainmentItem item;
		Transaction.GetItem( nItemIdx, item );

		int nLineCost = atoi ( GetPennyString( item.GetLineCost(), FALSE ) );
		
		int nDeptNo = item.GetDeptNo();
		if ( nDeptNo < 1 )
			nDeptNo = 1;

		CCSV csv;
		csv.Add( "$ITEM" );
		csv.Add( item.GetPluNo() );								//PLU NUMBER
		csv.Add( nDeptNo );										//DEPT NUMBER
		csv.Add( item.GetQty() );								//QUANTITY
		csv.Add( 0 );											//PRICE BAND
		csv.Add( 0 );											//UNIT SELLING PRICE
		csv.Add( "A" );											//TAX BAND
		csv.Add( "" );											//REFUND FLAG
		csv.Add( "" );											//VOID FLAG
		csv.Add( 1 );											//GROUP NUMBER
		csv.Add( "0" );											//PRICE OVERRIDE FLAG
		csv.Add( 0 );											//UNIT PRICE FROM PRODUCT FILE
		csv.Add( nLineCost );									//TOTAL RETAIL PRICE
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
		csv.Add( Transaction.GetCashRSPSaleDate() );			//DATE
		csv.Add( Transaction.GetCashRSPSaleTime() );			//TIME
		csv.Add( "0" );											//REASON CODE
		csv.Add( "''" );										//REASON TEXT
		csv.Add( item.GetDescription() );						//ITEM DESCRIPTION
		csv.Add( "0" );											//PRODUCT TYPE
		m_fileCashRSP.WriteLine( csv.GetLine() );
	}

	for ( int nPaymentIdx = 0; nPaymentIdx < Transaction.GetPaymentCount(); nPaymentIdx++ )
	{
		CSmartEntertainmentPayment Payment;
		Transaction.GetPayment( nPaymentIdx, Payment );

		int nAmount = atoi ( GetPennyString( Payment.GetAmount(), FALSE ) );

		int nPayID = Payment.GetID();
		int nPayNo = DataManager.SEPaymentMap.GetSharpPosPaymentNo( nPayID );
		CString strPayName = DataManager.Payment.GetDisplayName( nPayNo );

		CCSV csv;
		csv.Add( "$PAYM" );
		csv.Add( nPayID );										//METHOD NUMBER
		csv.Add( strPayName );									//METHOD NAME
		csv.Add( nAmount );										//AMOUNT
		csv.Add( nAmount );										//TENDER
		csv.Add( 0 );											//GRATUITY
		csv.Add( 0 );											//CASHBACK
		csv.Add( 0 );											//DEPOSIT ID
		csv.Add( 99 );											//PAYMENT TYPE
		csv.Add( 0 );											//ACCOUNT ID
		m_fileCashRSP.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/
