/**********************************************************************/
/**********************************************************************/
#include "CommsHandshaker.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "LockManager.h"
#include "NetworkCSVArray.h"
#include "..\CommonEcrStock\PMSCalendarDlg.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSZeroNightStayArray.h"
#include "PosTrayOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "MainFrmStockman.h"
/**********************************************************************/

void CMainFrameStockman::OnPMSCalendar() 
{
#ifdef COMPILE_PMS
	if ( SysInfo.IsPMSSystem() == TRUE )
	{
		PMSOptions.CreatePMSFolders();

		CString strError = "";

		CSSFile filePMSLock;
		CFilenameUpdater FnUp( SysFiles::PMSLock );
		if ( filePMSLock.Open( FnUp.GetFilenameToUse(), "wb" ) == FALSE )
			Prompter.Error( "The Guest Account system is being modified by another user.\n\nPlease try again later." );
		else
		{
			CFilenameUpdater FnUp1( SysFiles::PMSRooms );
			DataManagerNonDb.PMSRoom.Read( FnUp1.GetFilenameToUse() );

			CFilenameUpdater FnUp2( SysFiles::PMSBooking );
			DataManagerNonDb.PMSBooking.Read( FnUp2.GetFilenameToUse() );

			CFilenameUpdater FnUp3( SysFiles::PMSClosure );
			DataManagerNonDb.PMSClosure.Read( FnUp3.GetFilenameToUse() );

			CFilenameUpdater FnUp4( SysFiles::PMSRoomType );
			DataManagerNonDb.PMSRoomType.Read( FnUp4.GetFilenameToUse() );

			CFilenameUpdater FnUp5( SysFiles::PMSRoomGroup );
			DataManagerNonDb.PMSRoomGroup.Read( FnUp5.GetFilenameToUse() );

			DataManagerNonDb.PMSCustomer.ReadAndUpdateCustomerDatabase();

			switch( ManagePMSInternal() )
			{
			case 1:
				strError = "You must create a list of rooms to manage";
				break;

			case 2:
				strError = "You must configure an SPOS network";
				break;

			case 3:
				strError = "You must create a system database";
				break;
			}

			if ( strError != "" )
				Prompter.Error( strError + "\nbefore you can use the guest account system." );
		}
	}
#endif
}

/**********************************************************************/

int CMainFrameStockman::ManagePMSInternal() 
{
#ifdef COMPILE_PMS

	if ( DataManagerNonDb.PMSRoom.GetSize() == 0 )
		return 1;

	if ( dbNetwork.GetSize() == 0 )
		return 2;

	if ( dbDatabase.GetSize() == 0 )
		return 3;
	
	PMSCalendarTable.RemoveAll();
	PMSZeroNightStayArray.RemoveAll();
	DataManagerNonDb.PMSClosure.BuildInternalTables();
	DataManagerNonDb.PMSBooking.BuildInternalTables();

	DataManagerNonDb.PMSBooking.ValidateBookingDatabase();
	DataManagerNonDb.PMSCustomer.ValidateCustomerDatabase();

	CDataManagerInfo info;
	DataManagerNonDb.OpenPMSClosureReasons( DB_READONLY, info );

	bool bPosTrayFiddle = FALSE;
	if ( ( SysInfo.IsPosTraySystem() == TRUE ) && ( PosTrayOptions.GetPosTrayRealTimeFlag() == TRUE ) )
	{
		bPosTrayFiddle = TRUE;
		PosTrayOptions.SetPosTrayRealTimeFlag( FALSE );
		CommsHandshaker.BeginComms();
	}

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = FALSE;
	infoLock.m_nCommsAction = COMMS_ACTION_PAUSE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Manage guest accounts";
	infoLock.m_bStopPosTray = TRUE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		CDataManagerInfo info;
		DataManagerNonDb.OpenReportSession( DB_READONLY, info );

		CPMSCalendarDlg dlg( this );
		dlg.DoModal();
	}

	if ( TRUE == bPosTrayFiddle )
	{
		PosTrayOptions.SetPosTrayRealTimeFlag( TRUE );
		CommsHandshaker.BeginComms();
	}

	DataManagerNonDb.ClosePMSClosureReasons( info );

	return 0;

#else

	return 0;

#endif
}

/**********************************************************************/
