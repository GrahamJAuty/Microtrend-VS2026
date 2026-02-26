/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSOptions.h"
/**********************************************************************/
#include "PMSTariffMap.h"
/**********************************************************************/

CPMSTariffMapItem::CPMSTariffMapItem()
{
	Reset();
}

/**********************************************************************/

void CPMSTariffMapItem::Reset()
{
	m_nTariffType = 0;
	m_nRoomRateTypes = 0;
}

/**********************************************************************/

void CPMSTariffMapItem::Add (CPMSTariffMapItem& source )
{
	m_nRoomRateTypes |= source.m_nRoomRateTypes;
}

/**********************************************************************/

int CPMSTariffMapItem::Compare( CPMSTariffMapItem& source, int nHint )
{
	if ( m_nTariffType != source.m_nTariffType )
		return ( ( m_nTariffType > source.m_nTariffType ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

CPMSTariffMap::CPMSTariffMap()
{
}

/**********************************************************************/

void CPMSTariffMap::BuildMap( CPMSBookingCSVRecord& Booking )
{
	m_Map.RemoveAll();
	BuildMapInternal( Booking );
}

/**********************************************************************/

void CPMSTariffMap::BuildMap( CPMSRoomCSVRecord& Room )
{
	m_Map.RemoveAll();
	BuildMapInternal( Room );
}

/**********************************************************************/

void CPMSTariffMap::BuildMap( CPMSBookingCSVRecord& Booking, CPMSRoomCSVRecord& Room )
{
	m_Map.RemoveAll();
	BuildMapInternal( Booking );
	BuildMapInternal( Room );
}

/**********************************************************************/

void CPMSTariffMap::BuildMapInternal( CPMSBookingCSVRecord& Booking )
{
	CPMSTariffMapItem item;
	item.m_nTariffType = Booking.GetTariffType();
	item.m_nRoomRateTypes = 0;

	switch( Booking.GetRoomRateType() )
	{
	case PMS_BOOKING_RATE_ROOM:		item.m_nRoomRateTypes = 1;	break;
	case PMS_BOOKING_RATE_BB:		item.m_nRoomRateTypes = 2;	break;
	case PMS_BOOKING_RATE_DBB:		item.m_nRoomRateTypes = 4;	break;
	case PMS_BOOKING_RATE_FREE:		item.m_nRoomRateTypes = 8;	break;
	}

	if ( item.m_nRoomRateTypes != 0 )
		m_Map.Consolidate( item );
}

/**********************************************************************/

void CPMSTariffMap::BuildMapInternal( CPMSRoomCSVRecord& Room )
{
	BuildMapInternalRoom(0);

	if ( Room.GetRoomSingleRateFlag() == TRUE )
		BuildMapInternalRoom(1);

	if ( Room.GetRoomAllowExtraBedFlag() == TRUE )
		BuildMapInternalRoom(2);

	if ( Room.GetRoomAllowExtraCotFlag() == TRUE )
		BuildMapInternalRoom(3);
}

/**********************************************************************/

void CPMSTariffMap::BuildMapInternalRoom( int nTariffType )
{
	CPMSTariffMapItem item;
	item.m_nTariffType = nTariffType;

	if ( PMSOptions.GetMiscEnableRateRoom() == TRUE )
	{
		item.m_nRoomRateTypes = 1;
		m_Map.Consolidate( item );
	}

	if ( PMSOptions.GetMiscEnableRateBB() == TRUE )
	{
		item.m_nRoomRateTypes = 2;
		m_Map.Consolidate( item );
	}

	if ( PMSOptions.GetMiscEnableRateDBB() == TRUE )
	{
		item.m_nRoomRateTypes = 4;
		m_Map.Consolidate( item );
	}

	//FREE ROOM (E.G. FOR STAFF)
	if ( 0 == nTariffType )
	{
		if ( PMSOptions.GetMiscEnableFreeRoom() == TRUE )
		{
			item.m_nRoomRateTypes = 8;
			m_Map.Consolidate( item );
		}
	}
}

/**********************************************************************/

void CPMSTariffMap::GetTariffTypeList( CArray<int,int>& arrayTariffs )
{
	arrayTariffs.RemoveAll();

	for ( int n = 0; n < m_Map.GetSize(); n++ )
	{
		CPMSTariffMapItem item;
		m_Map.GetAt( n, item );
		arrayTariffs.Add( item.m_nTariffType );
	}

	if ( arrayTariffs.GetSize() == 0 )
		arrayTariffs.Add(0);
}

/**********************************************************************/

void CPMSTariffMap::GetRoomRateTypeList( int nTariff, CArray<int,int>& arrayRoomRates )
{
	arrayRoomRates.RemoveAll();

	CPMSTariffMapItem item;
	item.m_nTariffType = nTariff;
	
	int nPos;
	if ( m_Map.Find( item, nPos ) == TRUE )
	{
		m_Map.GetAt( nPos, item );	
		if ( ( item.m_nRoomRateTypes & 8 ) == 8 )	arrayRoomRates.Add( PMS_BOOKING_RATE_FREE );
		if ( ( item.m_nRoomRateTypes & 1 ) == 1 )	arrayRoomRates.Add( PMS_BOOKING_RATE_ROOM );
		if ( ( item.m_nRoomRateTypes & 2 ) == 2 )	arrayRoomRates.Add( PMS_BOOKING_RATE_BB );
		if ( ( item.m_nRoomRateTypes & 4 ) == 4 )	arrayRoomRates.Add( PMS_BOOKING_RATE_DBB );
	}

	if ( arrayRoomRates.GetSize() == 0 )
		arrayRoomRates.Add( PMS_BOOKING_RATE_ROOM );
}

/**********************************************************************/
#endif
/**********************************************************************/
