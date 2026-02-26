/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSCalendarTable.h"
#include "PMSBookEditDlg.h"
#include "PMSRoomStatusTable.h"
#include "PMSStatusHelpers.h"
/**********************************************************************/
#include "PMSAccountListBookingDlg.h"
/**********************************************************************/

CPMSAccountListBookingDlg::CPMSAccountListBookingDlg( int nRoomIdx, CWnd* pParent /*=NULL*/) :
	CPMSAccountListDlg( nRoomIdx, pParent )
{
	m_strTitle = "Edit Booking";
}

/**********************************************************************/

void CPMSAccountListBookingDlg::SetButtonControls()
{
	m_button1.SetWindowText ( "Add\nAccount" );
	m_button2.SetWindowText ( "Edit\nAccount" );

	if ( MAX_PMS_ROOMS == m_nRoomIdx )
	{
		m_button1.ShowWindow( SW_HIDE );
		m_button1.EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CPMSAccountListBookingDlg::HandleButton1()
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );

	CPMSBookingCSVRecord BookingRecordOld, BookingRecordNew;

	int nBookingIdx;
	int nBookingId = infoRoom.GetBookingId();
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
	{
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordOld );
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordNew );
	}

	if ( BookingRecordOld.GetPartCount() == 0 )
		return;

	int nNewAccountIdx = infoRoom.GetSafeAccountCount();
	if ( nNewAccountIdx >= MAX_PMS_ACCOUNTS_PER_ROOM )
	{
		Prompter.Error ( "You have already created the maximum number\nof accounts for this booking." );
		return;
	}

	int nBaseCustomerId = BookingRecordNew.GetCustomerId();
	
	PMSCalendarTable.SetEditMarkers( BookingRecordOld );
	
	CPMSCustomerCSVRecord CustomerRecord;
	CPMSBookEditDlg dlgEdit( PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW, MAX_PMS_ROOMS, nNewAccountIdx, BookingRecordNew, CustomerRecord, this );

	if ( dlgEdit.DoModal() == IDOK )
	{
		DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

		CPMSRoomAccountInfo infoAccount;
		PMSRoomStatusTable.GetAccountInfo( m_nRoomIdx, nNewAccountIdx, infoAccount );
		infoAccount.SetCustomerId( nBaseCustomerId + nNewAccountIdx );
		infoAccount.ResetCustomerName();
		infoAccount.SetGuestCount( dlgEdit.m_nCovers );
		PMSRoomStatusTable.SetAccountInfo( m_nRoomIdx, nNewAccountIdx, infoAccount );
		
		infoRoom.SetAccountCount( nNewAccountIdx + 1 );
		PMSRoomStatusTable.SetRoomInfo( m_nRoomIdx, infoRoom );

		CPMSStatusHelpers StatusHelpers;
		BookingRecordNew.SetMultipleAccountsFlag( TRUE );
		BookingRecordNew.SetAccountCount( nNewAccountIdx + 1 );
		BookingRecordNew.SetAccountCovers( nNewAccountIdx + 1, dlgEdit.m_nCovers );
		StatusHelpers.UpdateActiveBooking( m_nRoomIdx, BookingRecordOld, BookingRecordNew, nNewAccountIdx );

		m_listAccounts.SetItemCountEx( nNewAccountIdx + 1 );
		m_listAccounts.Invalidate();
		SelectLine( nNewAccountIdx );
	}

	PMSCalendarTable.ClearEditMarkers();
}

/**********************************************************************/

void CPMSAccountListBookingDlg::HandleButton2()
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );

	int nAccountIdx = m_listAccounts.GetCurSel();
	if ( ( nAccountIdx < 0 ) || ( nAccountIdx >= infoRoom.GetSafeAccountCount() ) )
		return;

	CPMSRoomAccountInfo infoAccount;
	PMSRoomStatusTable.GetAccountInfo( m_nRoomIdx, nAccountIdx, infoAccount );

	int nBookingId = infoRoom.GetBookingId();
	CPMSBookingCSVRecord BookingRecordOld, BookingRecordNew;

	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
	{
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordOld );
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordNew );
	}

	if ( BookingRecordOld.GetPartCount() == 0 )
		return;

	if ( MAX_PMS_ROOMS == m_nRoomIdx )
	{
		CPMSCustomerCSVRecord CustomerRecord;
		CPMSBookEditDlg dlg ( PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT, MAX_PMS_ROOMS, nAccountIdx, BookingRecordNew, CustomerRecord, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			if ( dlg.WasBookingEdited() == TRUE )
			{
				BookingRecordNew.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );
				CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecordNew );
		
				PMSCalendarTable.SetEditMarkers( BookingRecordOld );
				PMSCalendarTable.DeleteMarkedBookings();
				PMSCalendarTable.ApplyBooking( BookingRecordNew );
				PMSCalendarTable.ClearEditMarkers();
			}

			DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

			infoAccount.ResetCustomerName();
			PMSRoomStatusTable.SetAccountInfo( MAX_PMS_ROOMS, nAccountIdx, infoAccount );
		}
		
		m_listAccounts.Invalidate();
		SelectLine( nAccountIdx );
	}
	else
	{
		CPMSCustomerCSVRecord CustomerRecord;
		CPMSBookEditDlg dlgEdit( PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT, m_nRoomIdx, nAccountIdx, BookingRecordNew, CustomerRecord, this );
		
		if ( dlgEdit.DoModal() == IDOK )
		{
			DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

			infoAccount.ResetCustomerName();
			infoAccount.SetGuestCount( dlgEdit.m_nCovers );
			PMSRoomStatusTable.SetAccountInfo( m_nRoomIdx, nAccountIdx, infoAccount );

			CPMSStatusHelpers StatusHelpers;
			BookingRecordNew.SetMultipleAccountsFlag( TRUE );
			BookingRecordNew.SetAccountCovers( nAccountIdx, dlgEdit.m_nCovers );
			StatusHelpers.UpdateActiveBooking( m_nRoomIdx, BookingRecordOld, BookingRecordNew, nAccountIdx );

			m_listAccounts.Invalidate();
			SelectLine( nAccountIdx );
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/



