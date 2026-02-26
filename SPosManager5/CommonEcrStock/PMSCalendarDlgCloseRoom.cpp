/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CommsHandshaker.h"
#include "LockManager.h"
#include "PMSCalendarTable.h"
#include "PMSCloseRoomDlg.h"
#include "PMSClosedRoomCSVArray.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionCloseRoom()
{
	CPMSRoomCSVRecord Room;
	DataManagerNonDb.PMSRoom.GetAt( m_nMenuRoomIdx, Room );

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = Room.GetRoomNo();
	Query.m_nStartDay = m_nMenuDay;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = 1;
	
	CPMSBookingResponseInfo Response;
	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( 0 == Response.m_nBookingClashId )
		CloseRoomFromCalendar( m_nMenuRoomIdx, Response );
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionCloseEdit()
{
	EditClosure( m_nMenuRoomIdx, m_nMenuClosureId );
}

/**********************************************************************/

void CPMSCalendarDlg::CloseRoomFromCalendar( int nRoomIdx, CPMSBookingResponseInfo& Response )
{
	CPMSRoomCSVRecord RoomRecord;
	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, RoomRecord );

	CPMSClosedRoomCSVRecord ClosureRecord;
	ClosureRecord.SetStartDay( Response.m_nStartDay );
	ClosureRecord.SetNights(1);

	CPMSCloseRoomDlg dlgClose( FALSE, nRoomIdx, Response, ClosureRecord, this );
		
	if ( dlgClose.DoModal() == IDOK )
	{	
		int nClosureId = DataManagerNonDb.PMSClosure.FindFirstFreeId();
		
		ClosureRecord.SetClosureId( nClosureId );
		ClosureRecord.SetRoomNo( RoomRecord.GetRoomNo() );
		
		DataManagerNonDb.PMSClosure.InsertClosure( ClosureRecord );	
		PMSCalendarTable.ApplyClosure( ClosureRecord );
		MergeClosure( ClosureRecord );

		CFilenameUpdater FnUp( SysFiles::PMSClosure );
		DataManagerNonDb.PMSClosure.Write( FnUp.GetFilenameToUse() );

		m_CalendarHelpers.SetSelectedBookingId( 0 );
		m_CalendarHelpers.SetSelectedClosureId( ClosureRecord.GetClosureId() );
	}
}

/**********************************************************************/

void CPMSCalendarDlg::EditClosure( int nRoomIdx, int nClosureId )
{
	int nClosureIdx;
	CPMSClosedRoomCSVRecord ClosureRecord;
	if ( DataManagerNonDb.PMSClosure.FindClosureById( nClosureId, nClosureIdx ) == FALSE )
		return;

	DataManagerNonDb.PMSClosure.GetAt( nClosureIdx, ClosureRecord );

	CPMSRoomCSVRecord RoomRecord;
	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, RoomRecord );

	BlockTimer( TRUE );

	m_CalendarHelpers.SetSelectedClosureId( nClosureId );
	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();

	CLockManagerInfo infoLock;
	infoLock.m_bLockAllUsers = FALSE;
	infoLock.m_nCommsAction = COMMS_ACTION_NONE;
	infoLock.m_bStopComms = TRUE;
	infoLock.m_bStopCommsPMSExemption = FALSE;
	infoLock.m_strStopCommsPrompt = "Edit room closure";
	infoLock.m_bStopPosTray = FALSE;
	infoLock.m_pParentWnd = this;

	CLockManager LockManager( infoLock );

	if ( LockManager.GetLock() == TRUE )
	{
		PMSCalendarTable.SetEditMarkers( ClosureRecord );

		CPMSBookingQueryInfo Query;
		Query.m_nRoomNo = ClosureRecord.GetRoomNo();
		Query.m_nStartDay = ClosureRecord.GetStartDay();
		Query.m_nMinNights = ClosureRecord.GetNights();
		Query.m_nMaxNights = ClosureRecord.GetNights();
	
		CPMSBookingResponseInfo Response;
		PMSCalendarTable.CheckRoomAvailability( Query, Response );

		CPMSCloseRoomDlg dlgClose( TRUE, nRoomIdx, Response, ClosureRecord, this );
		
		if ( dlgClose.DoModal() == IDOK )
		{
			if ( dlgClose.m_nAction == 1 )
			{
				DataManagerNonDb.PMSClosure.RemoveClosureById( ClosureRecord.GetClosureId() );

				CFilenameUpdater FnUp( SysFiles::PMSClosure );
				DataManagerNonDb.PMSClosure.Write( FnUp.GetFilenameToUse() );

				PMSCalendarTable.DeleteMarkedBookings();
			}
			else
			{
				ClosureRecord.SetRoomNo ( RoomRecord.GetRoomNo() );
				DataManagerNonDb.PMSClosure.InsertClosure( ClosureRecord );

				PMSCalendarTable.DeleteMarkedBookings();
				PMSCalendarTable.ApplyClosure( ClosureRecord );
				MergeClosure( ClosureRecord );

				CFilenameUpdater FnUp( SysFiles::PMSClosure );
				DataManagerNonDb.PMSClosure.Write( FnUp.GetFilenameToUse() );
			}
		}

		PMSCalendarTable.ClearEditMarkers();
	}
	
	m_PMSStaticDate.RedrawWindow();
	m_PMSStaticCalendar.RedrawWindow();

	BlockTimer( FALSE );
}

/**********************************************************************/

void CPMSCalendarDlg::MergeClosure( CPMSClosedRoomCSVRecord& ClosureRecord )
{
	//CHECK PREVIOUS RECORD
	if ( ClosureRecord.GetStartDay() > 0 )
	{
		CPMSBookingQueryInfo Query;
		Query.m_nRoomNo = ClosureRecord.GetRoomNo();
		Query.m_nStartDay = ClosureRecord.GetStartDay() - 1;
		Query.m_nMinNights = 1;
		Query.m_nMaxNights = 1;

		CPMSBookingResponseInfo Response;

		PMSCalendarTable.CheckRoomAvailability( Query, Response );

		CPMSCalendarBooking CalendarBooking;
		PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

		if ( PMS_BOOKING_STATUS_CLOSED == CalendarBooking.m_nStatus )
		{
			int nClosureIdx;
			if ( DataManagerNonDb.PMSClosure.FindClosureById( CalendarBooking.m_nBookingId, nClosureIdx ) == TRUE )
			{
				CPMSClosedRoomCSVRecord ClosurePrevious;
				DataManagerNonDb.PMSClosure.GetAt( nClosureIdx, ClosurePrevious );
				MergeClosures( ClosurePrevious, ClosureRecord );
			}
		}
	}

	{
		CPMSBookingQueryInfo Query;
		Query.m_nRoomNo = ClosureRecord.GetRoomNo();
		Query.m_nStartDay = ClosureRecord.GetStartDay() + ClosureRecord.GetNights();
		Query.m_nMinNights = 1;
		Query.m_nMaxNights = 1;

		CPMSBookingResponseInfo Response;

		PMSCalendarTable.CheckRoomAvailability( Query, Response );

		CPMSCalendarBooking CalendarBooking;
		PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

		if ( PMS_BOOKING_STATUS_CLOSED == CalendarBooking.m_nStatus )
		{
			int nClosureIdx;
			if ( DataManagerNonDb.PMSClosure.FindClosureById( CalendarBooking.m_nBookingId, nClosureIdx ) == TRUE )
			{
				CPMSClosedRoomCSVRecord ClosureNext;
				DataManagerNonDb.PMSClosure.GetAt( nClosureIdx, ClosureNext );
				MergeClosures( ClosureRecord, ClosureNext );
			}
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::MergeClosures( CPMSClosedRoomCSVRecord& Closure1, CPMSClosedRoomCSVRecord& Closure2 )
{
	if ( Closure1.GetStartDay() + Closure1.GetNights() != Closure2.GetStartDay() )
		return;

	if ( Closure1.GetRoomNo() != Closure2.GetRoomNo() )
		return;

	PMSCalendarTable.SetEditMarkers( Closure1, FALSE );
	PMSCalendarTable.SetEditMarkers( Closure2, TRUE );

	DataManagerNonDb.PMSClosure.RemoveClosureById( Closure2.GetClosureId() );

	Closure2.SetClosureId	( Closure1.GetClosureId() );
	Closure2.SetStartDay	( Closure1.GetStartDay() );
	Closure2.SetNights		( Closure1.GetNights() + Closure2.GetNights() );

	DataManagerNonDb.PMSClosure.InsertClosure( Closure2 );
				
	PMSCalendarTable.DeleteMarkedBookings();
	PMSCalendarTable.ApplyClosure( Closure2 );
}

/**********************************************************************/
#endif
/**********************************************************************/
