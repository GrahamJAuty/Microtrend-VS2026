/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSAccountListPaymentDlg.h"
#include "PMSManualSalesDatabaseDlg.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PMSStatusHelpers.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionPaymentsActive()
{
	ProcessPaymentsActive( m_nMenuRoomIdx );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessPaymentsActive( int nRoomIdx )
{
	PMSOptions.BuildValidatedPaymentTypeArray();
	if ( PMSOptions.GetValidatedPaymentTypeArraySize() == 0 )
	{
		PMSOptions.NoPaymentTypeError();
		return;
	}

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

	if ( infoRoom.GetBookingId() == 0 )
		return;

	bool bTraining = FALSE;

	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( infoRoom.GetBookingId(), nBookingIdx ) == TRUE )
	{
		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
		bTraining = BookingRecord.GetTrainingFlag();
	}

	if ( infoRoom.GetSafeAccountCount() > 1 )
	{
		CPMSAccountListPaymentDlg dlgPayment( nRoomIdx, bTraining, this );
		dlgPayment.DoModal();
	}
	else
	{
		CPMSManualSalesDatabaseDlg dlg( nRoomIdx, 0, PMS_MANUAL_PAYMENT, bTraining, this );
		dlg.DoModal();
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionPaymentsComplete()
{
	if ( ProcessPaymentsComplete( m_nMenuBookingId, this ) == TRUE )
	{
		bool bDummy1, bDummy2;
		UpdateCompleteAccounts( m_nMenuBookingId, bDummy1, bDummy2 );
	}
}

/**********************************************************************/

bool CPMSCalendarDlg::ProcessPaymentsComplete( int nBookingId, CWnd* pParentWnd )
{
	int nBookingIdx;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return FALSE;

	PMSOptions.BuildValidatedPaymentTypeArray();
	if ( PMSOptions.GetValidatedPaymentTypeArraySize() == 0 )
	{
		PMSOptions.NoPaymentTypeError();
		return FALSE;
	}

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

	bool bTraining = BookingRecord.GetTrainingFlag();

	if ( BookingRecord.GetAccountCount() > 1 )
	{
		CPMSAccountListPaymentDlg dlgPayment( MAX_PMS_ROOMS, bTraining, pParentWnd );
		dlgPayment.DoModal();
	}
	else
	{
		CPMSManualSalesDatabaseDlg dlg( MAX_PMS_ROOMS, 0, PMS_MANUAL_PAYMENT, bTraining, pParentWnd );
		dlg.DoModal();
	}
	
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
