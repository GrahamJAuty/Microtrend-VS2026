/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSAccountFindDlg.h"
#include "PMSAccountListCheckoutDlg.h"
#include "PMSBatchInvoiceDlg.h"
#include "PMSCalendarTable.h"
#include "PMSCheckOutDlg.h"
#include "PMSCheckOutListDlg.h"
#include "PMSHighlightDlg.h"
#include "PMSInvoiceDlg.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PMSStatusHelpers.h"
#include "PriceHelpers.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionCheckOutActive()
{
	ProcessPrebookedCheckOut( 0 );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionCheckOutUnpaidDebit()
{
	CPMSAccountFindDlg dlg( IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT, this );
	dlg.DoModal();
	FindAndSelectBooking( dlg.GetLastEditedBookingId() );
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionCheckOutUnpaidCredit()
{
	CPMSAccountFindDlg dlg( IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT, this );
	dlg.DoModal();
	FindAndSelectBooking( dlg.GetLastEditedBookingId() );
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionCheckOut()
{
	ProcessCheckOut( m_nMenuRoomIdx, FALSE );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessPrebookedCheckOut( int nRoomNo )
{
	CPMSCheckOutListDlg dlg( nRoomNo, this );
	if ( dlg.DoModal() != IDOK )
		return;

	int nRoomIdx = dlg.GetSelectedRoomIdx();
	if ( ( nRoomIdx < 0 ) || ( nRoomIdx >= DataManagerNonDb.PMSRoom.GetSize() ) )
		return;

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	int nBookingId = infoRoom.GetBookingId();
	if ( nBookingId == 0 )
		return;

	ProcessCheckOut( nRoomIdx, TRUE );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessCheckOut( int nRoomIdx, bool bHighlight )
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecordDatabase, BookingRecordInvoice;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordDatabase );
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecordInvoice );
	BookingRecordInvoice.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );
	
	PMSCalendarTable.SetEditMarkers( BookingRecordInvoice );	
	CPMSCheckoutDlg dlg( nRoomIdx, BookingRecordInvoice, this );
	bool bContinue = ( dlg.DoModal() == IDOK );
	PMSCalendarTable.ClearEditMarkers();
	
	if ( FALSE == bContinue )
		return;

	DataManagerNonDb.PMSInvoiceBookingBuffer.SetDatabaseBooking( BookingRecordDatabase );
	DataManagerNonDb.PMSInvoiceBookingBuffer.SetInvoiceBooking( BookingRecordInvoice );
	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateAllBalances();
	
	PMSOptions.BuildValidatedPaymentTypeArray();

	bool bCheckout = FALSE;
	bool bCredit = FALSE;
	bool bDebit = FALSE;

	while ( TRUE == bContinue )
	{
		if ( infoRoom.GetSafeAccountCount() > 1 )
		{
			CPMSAccountListCheckoutDlg dlgInvoice( nRoomIdx, this );
			
			if ( dlgInvoice.DoModal() == IDCANCEL )
			{
				bCheckout = FALSE;
				bContinue = FALSE;
			}
		}
		else
		{
			CPMSInvoiceDlg dlgInvoice( nRoomIdx, 0, TRUE, TRUE, FALSE, this );
			
			if ( dlgInvoice.DoModal() == IDCANCEL )
			{
				bCheckout = FALSE;
				bContinue = FALSE;
			}
		}

		if ( TRUE == bContinue )
		{
			DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateAllBalances();

			bDebit = FALSE;
			bCredit = FALSE;

			for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
			{
				double dBalance = DataManagerNonDb.PMSInvoiceBookingBuffer.GetBalance( nAccountIdx );
				int nBalanceType = CPriceHelpers::CompareDoubles( dBalance, 0.0, 3 );
				bDebit |= ( nBalanceType == 1 );
				bCredit |= ( nBalanceType == -1 );
			}

			if ( FALSE == bDebit )
			{
				bCheckout = TRUE;
				bContinue = FALSE;
			}
			else
			{
				CString strMsg = "";
				strMsg += "Payment is still owing for one or more accounts in this booking.\n\n";
				strMsg += "Do you wish to allow checkout now and collect payment later.";
				
				int nResult = Prompter.YesNo( strMsg, FALSE );

				if( IDYES == nResult )
				{
					bCheckout = TRUE;
					bContinue = FALSE;
				}
				else
				{
					bCheckout = FALSE;
					bContinue = TRUE;
				}
			}
		}
	}

	if ( TRUE == bCheckout )
	{
		//BOOKING FILE
		int nBookingIdx;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == TRUE )
		{
			int nNewBalanceType;
			if ( TRUE == bDebit )
				nNewBalanceType = ( TRUE == bCredit ) ? PMS_CHECKOUT_BALANCE_MIXED : PMS_CHECKOUT_BALANCE_DEBIT;
			else
				nNewBalanceType = ( TRUE == bCredit ) ? PMS_CHECKOUT_BALANCE_CREDIT : PMS_CHECKOUT_BALANCE_ZERO;

			CPMSBookingCSVRecord BookingRecordInvoice, BookingRecordDatabase;
			DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );
			DataManagerNonDb.PMSInvoiceBookingBuffer.GetDatabaseBooking( BookingRecordDatabase );

			BookingRecordInvoice.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );
			BookingRecordInvoice.SetCheckoutBalanceType( nNewBalanceType );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecordInvoice );

			//CALENDAR STATUS (MEMORY)
			PMSCalendarTable.SetEditMarkers( BookingRecordDatabase );
			PMSCalendarTable.DeleteMarkedBookings();
			PMSCalendarTable.ApplyBooking( BookingRecordInvoice );
		
			//EPOS REPORT FILE
			for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
			{
				//MUST UPDATE ACCOUNT BALANCE BEFORE CREATING EPOS REPORT
				PMSRoomStatusTable.UpdateAccountBalance( nRoomIdx, nAccountIdx, TRUE );
				
				CPMSInvoiceArray arrayInvoice;
				
				if ( BookingRecordInvoice.GetTrainingFlag() == FALSE )
				{
					bool bDBBCredit = ( BookingRecordInvoice.GetRoomRateType() == PMS_BOOKING_RATE_DBB ) && ( 0 == nAccountIdx );		 
					arrayInvoice.BuildInvoiceArray( BookingRecordInvoice, nAccountIdx + 1, PMS_INVOICETYPE_EPOS_REPORT, bDBBCredit );
					arrayInvoice.CreateEPOSReportFile( nRoomIdx, nAccountIdx );
				}
				else
					arrayInvoice.DeleteEPOSReportFile( nRoomIdx, nAccountIdx );
			}

			//ROOM STATUS GRID (MEMORY)
			PMSRoomStatusTable.CheckOut( nRoomIdx );
			PMSRoomStatusTable.WriteDownloadFileRoom( nRoomIdx );

			if ( TRUE == bHighlight )
				ScrollToBooking( BookingRecordInvoice );

			CStringArray arrayMsg;
			if ( TRUE == bDebit )
			{
				arrayMsg.Add( "One or more accounts on this booking has a new debit balance." );
				arrayMsg.Add( "This booking will be added to the unpaid accounts debit list." );
				arrayMsg.Add( "" );
			}

			if ( TRUE == bCredit )
			{
				arrayMsg.Add( "One or more accounts on this booking has a new credit balance." );
				arrayMsg.Add( "This booking will be added to the unpaid accounts credit list." );
				arrayMsg.Add( "" );
			}

			if ( arrayMsg.GetSize() > 0 )
			{
				CString strMsg;
				for ( int n = 0; n < arrayMsg.GetSize() - 1; n++ )
				{
					strMsg += arrayMsg.GetAt(n);
					strMsg += "\n";
				}

				Prompter.Info( strMsg );
			}
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionBatchInvoices()
{
	CPMSBatchInvoiceDlg dlg( this );
	dlg.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::MenuBarOptionHighlightAccounts()
{
	int nHighlightMode = m_PMSStaticCalendar.GetSpecialHighlightMode();

	CPMSHighlightDlg dlg( nHighlightMode, this );

	if ( dlg.DoModal() == IDOK )
	{
		nHighlightMode = dlg.GetHighlightMode();
		m_PMSStaticCalendar.SetSpecialHighlightMode( nHighlightMode );
		m_PMSStaticCalendar.RedrawWindow();

		CString strText = "Guest Accounts Calendar";

		switch( nHighlightMode )
		{
		case PMS_SPECIAL_HIGHLIGHT_DEBIT:	strText += " (Completed Bookings with Accounts in Debit)";			break;
		case PMS_SPECIAL_HIGHLIGHT_CREDIT:	strText += " (Completed Bookings with Accounts in Credit)";			break;
		case PMS_SPECIAL_HIGHLIGHT_BOTH:	strText += " (Completed Bookings with Accounts in Debit or Credit)";	break;
		}

		SetWindowText( strText );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
