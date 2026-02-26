#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSBookingCSVArray.h"
/**********************************************************************/

class CPMSStatusHelpers
{
public:
	CPMSStatusHelpers(){}

public:
	static void UpdateActiveBooking( int nRoomIdx, CPMSBookingCSVRecord& BookingRecordOld, CPMSBookingCSVRecord& BookingRecordNew, int nAccountIdx );
	static void ImportPMSSales();
};

/**********************************************************************/
#endif
/**********************************************************************/
