/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSInvoiceBookingBuffer.h"
/**********************************************************************/

CPMSInvoiceBookingBuffer::CPMSInvoiceBookingBuffer()
{
	CPMSBookingCSVRecord BookingRecord;
	m_arrayBuffer.Add( BookingRecord );
	m_arrayBuffer.Add( BookingRecord );
	ClearBalance();
}

/**********************************************************************/

void CPMSInvoiceBookingBuffer::ClearBalance()
{
	for ( int n = 0; n < MAX_PMS_ACCOUNTS_PER_ROOM; n++ )
	{
		m_dAccomSpend[n] = 0.0;
		m_dExtraBedSpend[n] = 0.0;
		m_dItemSpend[n] = 0.0;
		m_dDepositPaid[n] = 0.0;
		m_dPostCheckInPaid[n] = 0.0;
		m_dAllowance[n] = 0.0;
	}
}

/**********************************************************************/

void CPMSInvoiceBookingBuffer::UpdateAllBalances()
{
	for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
		UpdateBalance( nAccountIdx );
}

/**********************************************************************/

void CPMSInvoiceBookingBuffer::UpdateBalance( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) && ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
	{
		CPMSBookingCSVRecord BookingRecord;
		GetInvoiceBooking( BookingRecord );

		m_dAccomSpend[ nAccountIdx ] = 0.0;
		m_dExtraBedSpend[ nAccountIdx ] = 0.0;
		m_dItemSpend[ nAccountIdx ] = 0.0;
		m_dDepositPaid[ nAccountIdx ] = 0.0;
		m_dPostCheckInPaid[ nAccountIdx ] = 0.0;
		m_dAllowance[ nAccountIdx ] = 0.0;

		CTime timeDummy;
		PMSRoomStatusTable.GetBalanceForBooking( 
			BookingRecord, 
			nAccountIdx, 
			m_dAccomSpend[ nAccountIdx ],
			m_dExtraBedSpend[ nAccountIdx ],
			m_dItemSpend[ nAccountIdx ], 
			m_dDepositPaid[ nAccountIdx ],
			m_dPostCheckInPaid[ nAccountIdx ],
			m_dAllowance[ nAccountIdx ], 
			timeDummy, 
			TRUE );
	}
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetAccomSpend( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dAccomSpend[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetExtraBedSpend( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dExtraBedSpend[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetItemSpend( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dItemSpend[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetDepositPaid( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dDepositPaid[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetPostCheckInPaid( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dPostCheckInPaid[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetAllowance( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dAllowance[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetTotalSpend( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dAccomSpend[ nAccountIdx ] + m_dItemSpend[ nAccountIdx ] + m_dExtraBedSpend[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetTotalPaid( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_dDepositPaid[ nAccountIdx ] + m_dPostCheckInPaid[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/

double CPMSInvoiceBookingBuffer::GetBalance( int nAccountIdx )
{
	if ( ( nAccountIdx >= 0 ) || ( nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return GetTotalSpend( nAccountIdx ) - GetTotalPaid( nAccountIdx ) - m_dAllowance[ nAccountIdx ];
	else
		return 0.0;
}

/**********************************************************************/
#endif
/**********************************************************************/