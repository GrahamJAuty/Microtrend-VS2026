/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "RealTimeCommsThread.h"
/**********************************************************************/
#include "PMSStatusHelpers.h"
/**********************************************************************/

void CPMSStatusHelpers::UpdateActiveBooking( int nRoomIdx, CPMSBookingCSVRecord& BookingRecordOld, CPMSBookingCSVRecord& BookingRecordNew, int nAccountIdx )
{
	CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecordNew );

	PMSCalendarTable.SetEditMarkers( BookingRecordOld );
	PMSCalendarTable.DeleteMarkedBookings();
	PMSCalendarTable.ApplyBooking( BookingRecordNew );

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );
	infoRoom.SetMultipleAccountsFlag( BookingRecordNew.GetMultipleAccountsFlag() );
	PMSRoomStatusTable.SetRoomInfo( nRoomIdx, infoRoom );
	
	PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecordNew, FALSE );

	CPMSRoomAccountInfo infoAccount;
	PMSRoomStatusTable.GetAccountInfo( nRoomIdx, 0, infoAccount );
	infoAccount.ResetCustomerName();
	PMSRoomStatusTable.SetAccountInfo( nRoomIdx, 0, infoAccount );

	PMSRoomStatusTable.WriteDownloadFileAccount( nRoomIdx, nAccountIdx );
}

/**********************************************************************/

void CPMSStatusHelpers::ImportPMSSales()
{
	CFilenameUpdater FnUp( SysFiles::PMSLockFile );
	
	CRealTimeCommsThreadInfo info;
	info.m_strPMSDownloadFolder = PMSOptions.GetPMSFolderDownload();
	info.m_strPMSRoomFolder = PMSOptions.GetEposLinkRoomPath();
	info.m_strPMSLockFile = FnUp.GetFilenameToUse();
	info.m_strPMSPostPath = PMSOptions.GetEposLinkPostPath();
	info.m_strPMSFolderPending = PMSOptions.GetPMSFolderPending();
	info.m_strPMSFolderSales = PMSOptions.GetPMSFolderSales();
	info.m_bPMS = TRUE;
	info.m_bAllPMS = TRUE;
	info.m_bCASHRSP = FALSE;
	info.m_bLogAllCASHRSPFiles = FALSE;
	info.m_nZeroFileCount = 0;
	info.m_bProducts = FALSE;
	info.m_bCustomer = FALSE;

	//FIDDLE : NOT ACTUALLY RUNNING IN A SEPARATE THREAD
	CRealTimeCommsThread::DoComms( &info );
}

/**********************************************************************/
#endif
/**********************************************************************/