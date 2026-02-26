#pragma once
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSDefines.h"
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

class CPMSInvoiceBookingBuffer
{
public:
	CPMSInvoiceBookingBuffer();

public:
	void ClearBalance();
	void UpdateAllBalances();
	void UpdateBalance( int nAccountIdx );
	double GetAccomSpend( int nAccountIdx );
	double GetExtraBedSpend( int nAccountIdx );
	double GetItemSpend( int nAccountIdx );
	double GetDepositPaid( int nAccountIdx );
	double GetPostCheckInPaid( int nAccountIdx );
	double GetAllowance( int nAccountIdx );
	double GetTotalSpend( int nAccountIdx );
	double GetTotalPaid( int nAccountIdx );
	double GetBalance( int nAccountIdx );

public:
	void GetInvoiceBooking( CPMSBookingCSVRecord& Booking ){ m_arrayBuffer.GetAt( 0, Booking ); }
	void GetDatabaseBooking( CPMSBookingCSVRecord& Booking ){ m_arrayBuffer.GetAt( 1, Booking ); }

public:
	void SetInvoiceBooking( CPMSBookingCSVRecord& Booking ){ m_arrayBuffer.SetAt( 0, Booking ); }
	void SetDatabaseBooking( CPMSBookingCSVRecord& Booking ){ m_arrayBuffer.SetAt( 1, Booking ); }

private:
	CPMSBookingCSVArray m_arrayBuffer;

private:
	double m_dAccomSpend[ MAX_PMS_ACCOUNTS_PER_ROOM ];
	double m_dExtraBedSpend[ MAX_PMS_ACCOUNTS_PER_ROOM ];
	double m_dItemSpend[ MAX_PMS_ACCOUNTS_PER_ROOM ];
	double m_dDepositPaid[ MAX_PMS_ACCOUNTS_PER_ROOM ];
	double m_dPostCheckInPaid[ MAX_PMS_ACCOUNTS_PER_ROOM ];
	double m_dAllowance[ MAX_PMS_ACCOUNTS_PER_ROOM ];
};

/**********************************************************************/
#endif
/**********************************************************************/
