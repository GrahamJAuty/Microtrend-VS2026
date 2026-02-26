/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSOptions.h"
#include "PMSManualSalesDatabaseDlg.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSAccountListPaymentDlg.h"
/**********************************************************************/

CPMSAccountListPaymentDlg::CPMSAccountListPaymentDlg( int nRoomIdx, bool bTraining, CWnd* pParent /*=NULL*/) :
	CPMSAccountListDlg( nRoomIdx, pParent )
{
	m_strTitle = "Edit Guest Payments";
	m_bTraining = bTraining;
}

/**********************************************************************/

void CPMSAccountListPaymentDlg::SetButtonControls()
{
	m_button1.SetWindowText( "Edit\nPayments" );
	m_button2.EnableWindow( FALSE );
	m_button2.ShowWindow( SW_HIDE );
}

/**********************************************************************/

void CPMSAccountListPaymentDlg::HandleButton1()
{
	int nAccountIdx = m_listAccounts.GetCurSel();

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );

	if ( infoRoom.GetBookingId() == 0 )
		return;

	CPMSManualSalesDatabaseDlg dlg( m_nRoomIdx, nAccountIdx, PMS_MANUAL_PAYMENT, m_bTraining, this );
	dlg.DoModal();

	PMSRoomStatusTable.UpdateAccountBalance( m_nRoomIdx, nAccountIdx, TRUE );
	m_listAccounts.Invalidate();
	SelectLine( nAccountIdx );
}

/**********************************************************************/
#endif
/**********************************************************************/



