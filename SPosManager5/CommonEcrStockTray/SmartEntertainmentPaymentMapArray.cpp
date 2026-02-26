/**********************************************************************/
 
/**********************************************************************/
#include "SmartEntertainmentPaymentMapArray.h"
/**********************************************************************/

CSmartEntertainmentPaymentMapCSVArray::CSmartEntertainmentPaymentMapCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

CSmartEntertainmentPaymentMapCSVArray::~CSmartEntertainmentPaymentMapCSVArray()
{
}

/**********************************************************************/

int CSmartEntertainmentPaymentMapCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	m_mapPayment.RemoveAll();
	
	DirectAddMapKey( 2178 );
	DirectAddMapKey( 2179 );
	
	for ( int n = 0; ( n < GetSize() ) && ( GetMapSize() < 10 ); n++ )
	{
		CSmartEntertainmentPaymentMapCSVRecord Record;
		GetAt( n, Record );

		CSmartEntPaymentMapEntry item;
		item.m_nSmartEntPayNo = Record.GetSmartEntPayNo();
		item.m_strSmartEntPayName = Record.GetSmartEntPayName();
		item.m_nSharpPosPayNo = Record.GetSharpPosPayNo();
		UpdateMapItem( item );
	}

	SetMapKeyName( 2178, "Online" );
	SetMapKeyName( 2179, "Online" );

	RemoveAt( 0, GetSize() );

	return nReply;
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < m_mapPayment.GetSize(); n++ )
	{
		CSmartEntPaymentMapEntry item;
		m_mapPayment.GetAt( n, item );

		CSmartEntertainmentPaymentMapCSVRecord Record;
		Record.SetSmartEntPayNo( item.m_nSmartEntPayNo );
		Record.SetSmartEntPayName( item.m_strSmartEntPayName );
		Record.SetSharpPosPayNo( item.m_nSharpPosPayNo );
		Add( Record );
	}
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVArray::GetMapItem( int n, CSmartEntPaymentMapEntry& item )
{
	item.Reset();
	if ( ( n >= 0 ) && ( n < m_mapPayment.GetSize() ) )
		m_mapPayment.GetAt( n, item );
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVArray::UpdateMapItem( CSmartEntPaymentMapEntry& item )
{
	int nPos;
	if ( m_mapPayment.Find( item, nPos ) == TRUE )
		m_mapPayment.SetAt( nPos, item );
	else
		m_mapPayment.InsertAt( nPos, item );
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVArray::DirectAddMapKey( int nSmartEntPayNo )
{
	CSmartEntPaymentMapEntry item;
	item.m_nSmartEntPayNo = nSmartEntPayNo;
	item.m_nSharpPosPayNo = 1;

	if ( m_mapPayment.GetSize() == 0 )
	{
		m_mapPayment.DirectAdd( item );
	}
	else
	{
		CSmartEntPaymentMapEntry itemLast;
		m_mapPayment.GetAt( m_mapPayment.GetSize() - 1, itemLast );

		if ( item.m_nSmartEntPayNo > itemLast.m_nSmartEntPayNo ) 
			m_mapPayment.DirectAdd( item );
	}
}

/**********************************************************************/

void CSmartEntertainmentPaymentMapCSVArray::SetMapKeyName( int nSmartEntPayNo, const char* szSmartEntPayName )
{
	CSmartEntPaymentMapEntry item;
	item.m_nSmartEntPayNo = nSmartEntPayNo;

	int nPos;
	if ( m_mapPayment.Find( item, nPos ) == TRUE )
	{
		m_mapPayment.GetAt( nPos, item );
		item.m_strSmartEntPayName = szSmartEntPayName;
		m_mapPayment.SetAt( nPos, item );
	}
}

/**********************************************************************/

int CSmartEntertainmentPaymentMapCSVArray::GetSharpPosPaymentNo( int nSEPaymentNo )
{
	int nResult = 0;

	CSmartEntPaymentMapEntry item;
	item.m_nSmartEntPayNo = nSEPaymentNo;

	int nPos;
	if ( m_mapPayment.Find( item, nPos ) == TRUE )
	{
		m_mapPayment.GetAt( nPos, item );
		nResult = item.m_nSharpPosPayNo;
	}

	return nResult;
}

/**********************************************************************/

const char* CSmartEntertainmentPaymentMapCSVArray::GetSmartEntPaymentName( int nSEPaymentNo )
{
	CSmartEntPaymentMapEntry item;
	item.m_nSmartEntPayNo = nSEPaymentNo;

	int nPos;
	if ( m_mapPayment.Find( item, nPos ) == TRUE )
	{
		m_mapPayment.GetAt( nPos, item );
		m_strSmartEntPaymentName = item.m_strSmartEntPayName;
	}
	else
	{
		m_strSmartEntPaymentName.Format( "#%4.4d", nSEPaymentNo );
	}

	return m_strSmartEntPaymentName;
}

/**********************************************************************/

