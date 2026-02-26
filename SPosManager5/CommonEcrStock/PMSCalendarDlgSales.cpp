/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "CommsHandshaker.h"
#include "PMSAccountListSalesDlg.h"
#include "PMSManualSalesDatabaseDlg.h"
#include "PMSOptions.h"
#include "PMSQuickPluDlg.h"
#include "PMSRoomStatusTable.h"
#include "PMSStatusHelpers.h"
#include "..\SPosEcrManager5\ZSalesImporterEcrman.h"
#include "..\SPosStockManager5\ZSalesImporterStockman.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionSalesActive()
{
	ProcessSalesActive( m_nMenuRoomIdx );
}

/**********************************************************************/

void CPMSCalendarDlg::ProcessSalesActive( int nRoomIdx )
{	
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
		CPMSAccountListSalesDlg dlgSales( nRoomIdx, bTraining, this );
		dlgSales.DoModal();
	}
	else
	{
		if ( ProcessQuickPluSales( nRoomIdx, 0, bTraining, this ) == TRUE )
		{
			CPMSManualSalesDatabaseDlg dlg( nRoomIdx, 0, PMS_MANUAL_SALES, bTraining, this );
			dlg.DoModal();
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionSalesComplete()
{
	if ( ProcessSalesComplete( m_nMenuBookingId, this ) == TRUE )
	{
		bool bDummy1, bDummy2;
		UpdateCompleteAccounts( m_nMenuBookingId, bDummy1, bDummy2 );
	}
}

/**********************************************************************/

bool CPMSCalendarDlg::ProcessSalesComplete( int nBookingId, CWnd* pParentWnd )
{
	int nBookingIdx;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return FALSE;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

	bool bTraining = BookingRecord.GetTrainingFlag();

	if ( BookingRecord.GetAccountCount() > 1 )
	{
		CPMSAccountListSalesDlg dlgSales( MAX_PMS_ROOMS, bTraining, pParentWnd );
		dlgSales.DoModal();
	}
	else
	{
		if ( ProcessQuickPluSales( MAX_PMS_ROOMS, 0, bTraining, pParentWnd ) == TRUE )
		{
			CPMSManualSalesDatabaseDlg dlg( MAX_PMS_ROOMS, 0, PMS_MANUAL_SALES, bTraining, pParentWnd );
			dlg.DoModal();
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CPMSCalendarDlg::ProcessQuickPluSales( int nRoomIdx, int nAccountIdx, bool bTraining, CWnd* pParentWnd )
{
	bool bResult = TRUE;

	if ( PMSOptions.AreAnyQuickSalesEnabled() == TRUE )
	{
		CPMSQuickPluDlg dlgQuick( nRoomIdx, nAccountIdx, bTraining, pParentWnd );

		dlgQuick.DoModal();
		if ( dlgQuick.GetAction() == PMS_QUICKPLU_CANCEL )
			bResult = FALSE;
		else
		{	
			CPMSInvoiceArray arrayNewSales;
			dlgQuick.GetPMSInvoiceSales( arrayNewSales );

			if ( arrayNewSales.GetInvoiceItemCount() != 0 )
			{
				CPMSRoomStatusInfo infoRoom;
				PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

				arrayNewSales.WriteInvoice( infoRoom.GetBookingId(), nAccountIdx + 1, TRUE );

#ifndef STOCKMAN_SYSTEM
				CZSalesImporterEcrman ZSalesImporter;
				ZSalesImporter.LookForPMSSales();
#else
				CZSalesImporterStockman ZSalesImporter;
				ZSalesImporter.LookForPMSSales();
#endif
				CDataManagerInfo infoDb;
				DataManager.OpenDatabaseReadOnly( 0, infoDb );
			}
		}
	}

	return bResult;
}

/**********************************************************************/
#endif
/**********************************************************************/
