/**********************************************************************/
 
/**********************************************************************/
#include "SportsBookerPaymentMapArray.h"
/**********************************************************************/

CSportsBookerPaymentMapCSVArray::CSportsBookerPaymentMapCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

CSportsBookerPaymentMapCSVArray::~CSportsBookerPaymentMapCSVArray()
{
}

/**********************************************************************/

int CSportsBookerPaymentMapCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	m_mapPayment.RemoveAll();

	CSortedIntByInt item;
	item.m_nVal = 1;

	for ( int n = 1; n <= 17; n++ )
	{
		item.m_nKey = n;
		m_mapPayment.DirectAdd( item );
	}

	item.m_nKey = 999;
	m_mapPayment.DirectAdd( item );
	
	for ( int n = 0; n < GetSize(); n++ )
	{
		CSportsBookerPaymentMapCSVRecord Record;
		GetAt( n, Record );

		CSortedIntByInt item;
		item.m_nKey = Record.GetSportsBookerPayNo();
		item.m_nVal = Record.GetSharpPosPayNo();
		UpdateMapItem( item );
	}

	RemoveAt( 0, GetSize() );

	return nReply;
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < m_mapPayment.GetSize(); n++ )
	{
		CSortedIntByInt item;
		m_mapPayment.GetAt( n, item );

		CSportsBookerPaymentMapCSVRecord Record;
		Record.SetSportsBookerPayNo( item.m_nKey );
		Record.SetSharpPosPayNo( item.m_nVal );
		Add( Record );
	}
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVArray::GetMapItem( int n, CSortedIntByInt& item )
{
	item.Reset();
	if ( ( n >= 0 ) && ( n < m_mapPayment.GetSize() ) )
		m_mapPayment.GetAt( n, item );
}

/**********************************************************************/

void CSportsBookerPaymentMapCSVArray::UpdateMapItem( CSortedIntByInt& item )
{
	int nPos;
	if ( m_mapPayment.Find( item, nPos ) == TRUE )
		m_mapPayment.SetAt( nPos, item );
}

/**********************************************************************/

int CSportsBookerPaymentMapCSVArray::GetSharpPosPaymentNo( int nSBPaymentNo )
{
	int nResult = 0;

	CSortedIntByInt item;
	item.m_nKey = nSBPaymentNo;

	int nPos;
	if ( m_mapPayment.Find( item, nPos ) == TRUE )
	{
		m_mapPayment.GetAt( nPos, item );
		nResult = item.m_nVal;
	}

	return nResult;
}

/**********************************************************************/

CString CSportsBookerPaymentMapCSVArray::GetSportsBookerText( int nPayNo )
{
	CString strSportsBookerText = "";

	switch( nPayNo )
	{
	case 1:		strSportsBookerText = "Credit Note";	break;
	case 2:		strSportsBookerText = "EPOS";			break;
	case 3:		strSportsBookerText = "Standing Order";	break;
	case 4:		strSportsBookerText = "Cash";			break;
	case 5:		strSportsBookerText = "Cheque";			break;
	case 6:		strSportsBookerText = "Voucher";		break;
	case 7:		strSportsBookerText = "Coupon";			break;
	case 8:		strSportsBookerText = "Card";			break;
	case 9:		strSportsBookerText = "Direct Debit";	break;
	case 10:	strSportsBookerText = "PayPal";			break;
	case 11:	strSportsBookerText = "SagePay";		break;
	case 12:	strSportsBookerText = "Realex";			break;
	case 13:	strSportsBookerText = "Bank Transfer";	break;
	case 14:	strSportsBookerText = "Invoice";		break;
	case 15:	strSportsBookerText = "On Account";		break;
	case 16:	strSportsBookerText = "GoCardless";		break;
	case 17:	strSportsBookerText = "Secure Trading";	break;
	case 999:	strSportsBookerText = "Account Credit";	break;
	default:	strSportsBookerText.Format( "Unknown (%d)", nPayNo ); break;
	}

	return strSportsBookerText;
}

/**********************************************************************/
