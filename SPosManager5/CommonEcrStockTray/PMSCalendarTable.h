#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "..\CommonEcrStock\PMSCalendarDlg.h"
#include "PMSClosedRoomCSVArray.h"
#include "PMSStructs.h"
/**********************************************************************/

class CPMSCalendarTable 
{
public:
	CPMSCalendarTable(void);
	~CPMSCalendarTable(void);

public:
	int GetSize(){ return m_arrayBookings.GetSize(); }
	void GetAt( int nIdx, CPMSCalendarBooking& Booking );
	void SetAt( int nIdx, CPMSCalendarBooking& Booking );
	void InsertAt( int nIdx, CPMSCalendarBooking& Booking );
	void RemoveAt( int nIdx );
	void RemoveAll();

public:
	bool FindBooking( CPMSCalendarBooking& Booking, int& nPos );
	void GetBookingIndexRange( int nRoomNo, int nStartDay, int nEndDay, int& nStartIdx, int& nEndIdx );

public:
	bool ValidateRoomCheck( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response );
	void CheckRoomAvailability( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response );
	void BookRoom( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response );

private:
	void CheckRoomAvailabilityInternal( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response );
	
public:
	void ApplyBooking( CPMSBookingCSVRecord& BookingRecord );
	void ApplyClosure( CPMSClosedRoomCSVRecord& ClosureRecord );

public:
	void SetEditMarkers( CPMSClosedRoomCSVRecord& ClosureRecord, bool bKeepExisting = FALSE );
	void SetEditMarkers( CPMSBookingCSVRecord& BookingRecord );
	void ClearEditMarkers();
	void DeleteMarkedBookings();

private:
	bool Validate( int x, int y, int z );

private:
	CArray<CPMSCalendarBooking,CPMSCalendarBooking> m_arrayBookings;
	CArray<int,int> m_arrayEditingIdx;
};

/**********************************************************************/
extern CPMSCalendarTable PMSCalendarTable;
/**********************************************************************/
#endif
/**********************************************************************/