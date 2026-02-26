/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSInvoiceDlg.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSAccountListCheckoutDlg.h"
/**********************************************************************/

CPMSAccountListCheckoutDlg::CPMSAccountListCheckoutDlg( int nRoomIdx, CWnd* pParent /*=NULL*/) :
	CPMSAccountListDlg( nRoomIdx, pParent )
{
	m_strTitle = "Check Out";
	
	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSInvoiceBookingBuffer.GetInvoiceBooking( BookingRecord );
	m_bTraining = BookingRecord.GetTrainingFlag();

	m_bInvoicePrint = TRUE;
}

/**********************************************************************/

void CPMSAccountListCheckoutDlg::SetButtonControls()
{
	m_button1.SetWindowText( "Process\nPayment" );
	m_button2.SetWindowText( "Cancel\nCheckout" );
	m_buttonOK.SetWindowText( "Continue" );
}

/**********************************************************************/

void CPMSAccountListCheckoutDlg::HandleButton1()
{
	int nAccountIdx = m_listAccounts.GetCurSel();
	CPMSInvoiceDlg dlgProcess( m_nRoomIdx, nAccountIdx, TRUE, FALSE, FALSE, this );
	dlgProcess.DoModal();
	DataManagerNonDb.PMSInvoiceBookingBuffer.UpdateBalance( nAccountIdx );
	m_listAccounts.Invalidate();
	SelectLine( nAccountIdx );
}

/**********************************************************************/

void CPMSAccountListCheckoutDlg::HandleButton2()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/


