/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSInvoiceDlg.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSAccountListInvoiceDlg.h"
/**********************************************************************/

CPMSAccountListInvoiceDlg::CPMSAccountListInvoiceDlg( int nRoomIdx, CWnd* pParent /*=NULL*/) :
	CPMSAccountListDlg( nRoomIdx, pParent )
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	CPMSBookingCSVRecord BookingRecordInvoice;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecordInvoice );
	
	if ( BookingRecordInvoice.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
		m_strTitle = "Print Interim Invoice";
	else if ( BookingRecordInvoice.GetInvoiceNum() == 0 )
		m_strTitle = "Print Invoice";
	else 
		m_strTitle.Format( "Print Invoice %8.8d", BookingRecordInvoice.GetInvoiceNum() );

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecord );
	m_bTraining = BookingRecord.GetTrainingFlag();

	m_bInvoicePrint = TRUE;
}

/**********************************************************************/

void CPMSAccountListInvoiceDlg::SetButtonControls()
{
	m_button1.SetWindowText( "Print\nInvoice" );
	m_button2.EnableWindow( FALSE );
	m_button2.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CPMSAccountListInvoiceDlg::HandleButton1()
{
	int nAccountIdx = m_listAccounts.GetCurSel();
	CPMSInvoiceDlg dlgProcess( m_nRoomIdx, nAccountIdx, FALSE, FALSE, FALSE, this );
	dlgProcess.DoModal();
	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateBalance( nAccountIdx );
	m_listAccounts.Invalidate();
	SelectLine( nAccountIdx );
}

/**********************************************************************/
#endif
/**********************************************************************/



