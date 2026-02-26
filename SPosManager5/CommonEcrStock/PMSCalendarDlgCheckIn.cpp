/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSAccountListBookingDlg.h"
#include "PMSBookingCSVArray.h"
#include "PMSBookEditDlg.h"
#include "PMSCalendarTable.h"
#include "PMSCheckInListDlg.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PMSStatusHelpers.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionCheckIn()
{
	CPMSCheckInListDlg dlg( 0, this );
	
	if ( dlg.DoModal() == IDOK )
		ProcessPrebookedCheckInByRoomIdx( dlg.GetSelectedRoomIdx() );
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionCheckIn()
{
	ProcessPrebookedCheckInByRoomIdx( m_nMenuRoomIdx );
}

/**********************************************************************/

void CPMSCalendarDlg::CancelCheckIn( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord )
{
	BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );
	CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );

	//CALENDAR STATUS (MEMORY)
	PMSCalendarTable.SetEditMarkers( BookingRecord );
	PMSCalendarTable.DeleteMarkedBookings();
	PMSCalendarTable.ApplyBooking( BookingRecord );

	//ROOM STATUS GRID (MEMORY)
	PMSRoomStatusTable.CheckOut( nRoomIdx );
	PMSRoomStatusTable.WriteDownloadFileRoom( nRoomIdx );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessPrebookedCheckInByRoomIdx( int nRoomIdx )
{
	if ( ( nRoomIdx < 0 ) || ( nRoomIdx >= DataManagerNonDb.PMSRoom.GetSize() ) )
		return;

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	if ( infoRoom.GetBookingId() != 0 )
	{
		CString strMsg = "";
		strMsg += "You cannot check a new guest in to this room because the\n";
		strMsg += "previous guest has not checked out.\n\n";
		strMsg += "Would you like to complete the check out process for the\n";
		strMsg += "previous guest now.";

		if ( Prompter.YesNo( strMsg, FALSE ) != IDYES )
		{
			FindAndSelectBooking( PMSRoomStatusTable.GetExpectedBookingId( nRoomIdx ) );
			return;
		}

		ProcessCheckOut( nRoomIdx, FALSE );
		
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );
		if ( infoRoom.GetBookingId() != 0 )
			return;
	}

	int nExpectedBookingId = PMSRoomStatusTable.GetExpectedBookingId( nRoomIdx );
	if ( 0 == nExpectedBookingId )
		return;

	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nExpectedBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

	FinaliseCheckIn( nRoomIdx, BookingRecord, TRUE );
}

/**********************************************************************/

bool CPMSCalendarDlg::FinaliseCheckIn( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord, bool bHighlight )
{
	BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_ACTIVE );

	CPMSCustomerCSVRecord CustomerRecord;
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, FALSE );
	CPMSBookEditDlg dlgEdit( PMS_EDITBOOKING_ACTIVE_SINGLE_NEW, MAX_PMS_ROOMS, 0, BookingRecord, CustomerRecord, this );
	
	if ( dlgEdit.DoModal() != IDOK )
		return FALSE;

	if ( BookingRecord.GetPartCount() == 0 )
		return FALSE;

	DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

	BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_ACTIVE );
	BookingRecord.SetMultipleAccountsFlag( dlgEdit.GetMultipleAccountsFlag() );
	BookingRecord.SetAccountCovers( 0, BookingRecord.GetGuests() );

	PMSCalendarTable.SetEditMarkers( BookingRecord );
	PMSCalendarTable.DeleteMarkedBookings();
	
	CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
	PMSCalendarTable.ApplyBooking( BookingRecord );

	PMSRoomStatusTable.CheckIn( nRoomIdx, BookingRecord );
	PMSRoomStatusTable.UpdateAccountBalance( nRoomIdx, TRUE );
	PMSRoomStatusTable.WriteDownloadFileRoom( nRoomIdx );
	
	if ( BookingRecord.GetMultipleAccountsFlag() == TRUE )
	{
		CPMSAccountListBookingDlg dlgAccount( nRoomIdx, this );
		dlgAccount.SetCalendarDlg( this );
		dlgAccount.DoModal();
	}

	if ( TRUE == bHighlight )
		ScrollToBooking( BookingRecord );

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
