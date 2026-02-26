/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSAccountListBookingDlg.h"
#include "PMSBookingCSVArray.h"
#include "PMSBookEditDlg.h"
#include "PMSCalendarTable.h"
#include "PMSCloseRoomDlg.h"
#include "PMSInvoiceArray.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PMSStatusHelpers.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionBookingPending()
{
	EditBookingPending( m_nMenuBookingId, this );	
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionBookingActive()
{
	EditBookingActive( m_nMenuRoomIdx, m_nMenuBookingId, this, this );	
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionBookingComplete()
{
	EditBookingComplete( m_nMenuBookingId, this, this );	
}

/**********************************************************************/

void CPMSCalendarDlg::EditBookingPending( int nBookingId, CWnd* pParentWnd )
{
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_PENDING )
		return;

	PMSCalendarTable.SetEditMarkers( BookingRecord );

	CPMSCustomerCSVRecord CustomerRecord;
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, FALSE );
	CPMSBookEditDlg dlg ( PMS_EDITBOOKING_PENDING_SINGLE_EDIT, MAX_PMS_ROOMS, 0, BookingRecord, CustomerRecord, pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

		if ( dlg.GetDeleteBookingFlag() == TRUE )
		{
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_CANCELLED );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
			
			PMSCalendarTable.DeleteMarkedBookings();
		}
		else if ( dlg.WasBookingEdited() == TRUE )
		{
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
			
			PMSCalendarTable.DeleteMarkedBookings();
			PMSCalendarTable.ApplyBooking( BookingRecord );
		}
		else if ( dlg.WasBookingEmailed() == TRUE )
		{
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
		}
	}
		
	PMSCalendarTable.ClearEditMarkers();
}

/**********************************************************************/

void CPMSCalendarDlg::EditBookingCancelled( int nBookingId, CWnd* pParentWnd )
{
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_CANCELLED )
		return;

	CPMSCustomerCSVRecord CustomerRecord;
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, FALSE );
	CPMSBookEditDlg dlg ( PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT, MAX_PMS_ROOMS, 0, BookingRecord, CustomerRecord, pParentWnd );
		
	if ( dlg.DoModal() == IDOK )
	{
		DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

		if ( dlg.GetRestoreBookingFlag() == TRUE )
		{
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
			PMSCalendarTable.ApplyBooking( BookingRecord );
		}
		else if ( dlg.WasBookingEdited() == TRUE )
		{
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_CANCELLED );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::EditBookingActive( int nRoomIdx, int nBookingId, CWnd* pParentWnd, CPMSCalendarDlg* pCalendarDlg )
{
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_ACTIVE )
		return;

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	if ( infoRoom.GetBookingId() != BookingRecord.GetBookingId() )
		return;

	if ( ( infoRoom.GetMultipleAccountsFlag() == TRUE ) || ( infoRoom.GetSafeAccountCount() > 1 ) )
	{
		CPMSAccountListBookingDlg dlgAccount( nRoomIdx, pParentWnd );
		dlgAccount.SetCalendarDlg( pCalendarDlg );
		dlgAccount.DoModal();
	}
	else
	{
		CPMSBookingCSVRecord BookingRecordOld, BookingRecordNew;

		int nBookingIdx;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == TRUE )
		{
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordOld );
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordNew );
		}

		if ( BookingRecordNew.GetPartCount() > 0 )
		{
			CPMSCustomerCSVRecord CustomerRecord;
			PMSCalendarTable.SetEditMarkers( BookingRecordOld );
			CPMSBookEditDlg dlgEdit( PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT, nRoomIdx, 0, BookingRecordNew, CustomerRecord, pParentWnd );
			BookingRecordNew.SetAccountCovers( 0, BookingRecordNew.GetGuests() );

			if ( dlgEdit.DoModal() == IDOK )
			{
				DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

				if ( dlgEdit.GetDeleteBookingFlag() == TRUE )
					CancelCheckIn( nRoomIdx, BookingRecordOld );
				else
				{
					BookingRecordNew.SetMultipleAccountsFlag ( dlgEdit.GetMultipleAccountsFlag() );
					CPMSStatusHelpers::UpdateActiveBooking( nRoomIdx, BookingRecordOld, BookingRecordNew, 0 );
					
					if ( BookingRecordNew.GetMultipleAccountsFlag() == TRUE )
					{
						CPMSAccountListBookingDlg dlgAccount( nRoomIdx, pParentWnd );
						dlgAccount.SetCalendarDlg( pCalendarDlg );
						dlgAccount.DoModal();
					}
				}
			}

			PMSCalendarTable.ClearEditMarkers();
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::EditBookingComplete( int nBookingId, CWnd* pParentWnd, CPMSCalendarDlg* pCalendarDlg )
{		
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );
	
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

	if ( infoRoom.GetBookingId() != BookingRecord.GetBookingId() )
		return;

	bool bTrainingModeOld = BookingRecord.GetTrainingFlag();

	if ( infoRoom.GetSafeAccountCount() > 1 )
	{
		CPMSAccountListBookingDlg dlgAccount( MAX_PMS_ROOMS, pParentWnd );
		dlgAccount.SetCalendarDlg( pCalendarDlg );
		dlgAccount.DoModal();
	}
	else
	{
		CPMSCustomerCSVRecord CustomerRecord;
		PMSCalendarTable.SetEditMarkers( BookingRecord );
		CPMSBookEditDlg dlg ( PMS_EDITBOOKING_CLOSED_SINGLE_EDIT, MAX_PMS_ROOMS, 0, BookingRecord, CustomerRecord, pParentWnd );
		
		if ( dlg.DoModal() == IDOK )
		{
			DataManagerNonDb.PMSCustomer.InsertAndSaveCustomer( CustomerRecord );

			if ( dlg.GetDeleteBookingFlag() == TRUE )
			{
				BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_CANCELLED );
				BookingRecord.SetAccountCovers( 0, BookingRecord.GetGuests() );
				CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
				PMSCalendarTable.DeleteMarkedBookings();
			}
			else if ( dlg.WasBookingEdited() == TRUE )
			{
				BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );
				BookingRecord.SetAccountCovers( 0, BookingRecord.GetGuests() );
				CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
			
				PMSCalendarTable.DeleteMarkedBookings();
				PMSCalendarTable.ApplyBooking( BookingRecord );
			}
		}
		
		PMSCalendarTable.ClearEditMarkers();
	}

	//LOOK UP BOOKING RECORD AGAIN IN CASE IT HAS BEEN DELETED
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );
	
	PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );
	if ( infoRoom.GetBookingId() != BookingRecord.GetBookingId() )
		return;

	//ACCOUNT BALANCE MAY HAVE CHANGED DUE TO ROOM RATE
	bool bDummy1, bDummy2;
	UpdateCompleteAccounts( nBookingId, bDummy1, bDummy2 );

	bool bTrainingModeNew = BookingRecord.GetTrainingFlag();

	if ( bTrainingModeNew != bTrainingModeOld )
	{
		if ( TRUE == bTrainingModeNew )
		{
			for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
			{
				CPMSInvoiceArray arrayInvoice;
				arrayInvoice.DeleteEPOSReportFile( MAX_PMS_ROOMS, nAccountIdx );
			}
		}
		else
		{
			for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
			{
				//MUST UPDATE ACCOUNT BALANCE BEFORE CREATING EPOS REPORT
				PMSRoomStatusTable.UpdateAccountBalance( MAX_PMS_ROOMS, nAccountIdx, TRUE );

				CPMSInvoiceArray arrayInvoice;
				bool bDBBCredit = ( BookingRecord.GetRoomRateType() == PMS_BOOKING_RATE_DBB ) && ( 0 == nAccountIdx );
				arrayInvoice.BuildInvoiceArray( BookingRecord, nAccountIdx + 1, PMS_INVOICETYPE_EPOS_REPORT, bDBBCredit ); 
				arrayInvoice.CreateEPOSReportFile( MAX_PMS_ROOMS, nAccountIdx );
			}
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
