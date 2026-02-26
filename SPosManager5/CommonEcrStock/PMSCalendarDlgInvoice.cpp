/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSAccountListInvoiceDlg.h"
#include "PMSInvoiceDlg.h"
#include "PMSOptions.h"
#include "PMSStatusHelpers.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionInvoiceActive()
{
	ProcessInvoiceActive( m_nMenuRoomIdx, FALSE, this );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessInvoiceActive( int nRoomIdx, bool bCheckout, CWnd* pParentWnd )
{
	PMSOptions.BuildValidatedPaymentTypeArray();

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );
	
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.SetDatabaseBooking( BookingRecord );

	if ( TRUE == bCheckout )
		BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_COMPLETE );

	DataManagerNonDb.PMSInvoiceBookingBuffer.SetInvoiceBooking( BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateAllBalances();
	
	if ( infoRoom.GetSafeAccountCount() > 1 )
	{
		CPMSAccountListInvoiceDlg dlgInvoice( nRoomIdx, pParentWnd );
		dlgInvoice.DoModal();
	}
	else
	{
		CPMSInvoiceDlg dlgProcess( nRoomIdx, 0, FALSE, FALSE, FALSE, pParentWnd );
		dlgProcess.DoModal();
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionInvoicePending()
{
	ProcessInvoicePending( m_nMenuRoomIdx, m_nMenuBookingId, this );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessInvoicePending( int nRoomIdx, int nBookingId, CWnd* pParentWnd )
{
	PMSOptions.BuildValidatedPaymentTypeArray();

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );
	
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.SetDatabaseBooking( BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.SetInvoiceBooking( BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateAllBalances();
	
	CPMSInvoiceDlg dlgProcess( nRoomIdx, 0, FALSE, FALSE, TRUE, pParentWnd );
	dlgProcess.DoModal();
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionInvoiceComplete()
{
	if ( ProcessInvoiceComplete( m_nMenuBookingId, this ) == TRUE )
	{
		bool bDummy1, bDummy2;
		UpdateCompleteAccounts( m_nMenuBookingId, bDummy1, bDummy2 );
	}
}

/**********************************************************************/

bool CPMSCalendarDlg::ProcessInvoiceComplete( int nBookingId, CWnd* pParentWnd )
{
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return FALSE;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.SetDatabaseBooking( BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.SetInvoiceBooking( BookingRecord );
	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateAllBalances();

	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

	PMSOptions.BuildValidatedPaymentTypeArray();

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

	if ( infoRoom.GetSafeAccountCount() > 1 )
	{
		CPMSAccountListInvoiceDlg dlgInvoice( MAX_PMS_ROOMS, pParentWnd );
		dlgInvoice.DoModal();
	}
	else
	{
		CPMSInvoiceDlg dlgProcess( MAX_PMS_ROOMS, 0, FALSE, FALSE, FALSE, pParentWnd );
		dlgProcess.DoModal();
	}
	
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
