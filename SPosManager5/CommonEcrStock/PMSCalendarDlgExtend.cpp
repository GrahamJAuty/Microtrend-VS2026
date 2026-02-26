/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "ListDataSwitchRoomDlg.h"
#include "PMSBookEditExtendDlg.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSRoomStatusInfo.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionExtendBookingToday()
{
	int nSourceRoomIdx = m_nMenuRoomIdx;

	CPMSRoomStatusInfo infoSource;
	PMSRoomStatusTable.GetRoomInfo( nSourceRoomIdx, infoSource );

	int nBookingId = infoSource.GetBookingId();
	if ( infoSource.GetBookingId() == 0 )
		return;

	int nGuests = infoSource.GetGuestCount();
	if ( nGuests < 1 )
		return;

	//READ ORIGINAL BOOKING FROM DATABASE
	int nBookingIdx;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	bool bTraining = BookingRecord.GetTrainingFlag();

	CPMSBookEditExtendDlg dlgExtend( nSourceRoomIdx, this );
	if ( dlgExtend.DoModal() != IDOK )
		return;

	CPMSBookingQueryInfo infoQuery;
	infoQuery.m_nStartDay = PMSOptions.GetPMSDayNumber();
	infoQuery.m_nMinNights = dlgExtend.m_nNights;
	infoQuery.m_nMaxNights = dlgExtend.m_nNights;
	infoQuery.m_nRoomNo = infoSource.GetRoomNo();

	CPMSBookingResponseInfo infoResponse;
	PMSCalendarTable.CheckRoomAvailability( infoQuery, infoResponse );
	
	if ( TRUE == infoResponse.m_bSuccess )
	{
		PMSCalendarTable.SetEditMarkers( BookingRecord );
		PMSCalendarTable.DeleteMarkedBookings();
		
		CPMSBookingPartInfo infoPart;
		BookingRecord.GetFinalPartInfo( infoPart );
		infoPart.SetNights( PMSOptions.GetPMSDayNumber() + dlgExtend.m_nNights - infoPart.GetStartDay() );
		BookingRecord.SetFinalPartInfo( infoPart );
		
		DataManagerNonDb.PMSBooking.InsertAndSaveBooking( BookingRecord );
		PMSCalendarTable.ApplyBooking( BookingRecord );
		PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecord, FALSE );
	}
	else
	{
		CString strMsg = "";
		if ( dlgExtend.m_nNights == 1 )
		{
			strMsg.Format ( "Room %d is already booked for the following night.",
				infoSource.GetRoomNo() );
		}
		else
		{
			strMsg.Format( "Room %d is already booked during the next %d nights.",
				infoSource.GetRoomNo(),
				dlgExtend.m_nNights );
		}

		strMsg += "\n\nWould you like to search for an alternative room.";

		if ( Prompter.YesNo( strMsg ) != IDYES )
			return;

		int nCheckoutDay = PMSOptions.GetPMSDayNumber() + dlgExtend.m_nNights;

		//CREATE A LIST OF ROOMS THAT COULD ACCOMODATE THESE GUESTS
		CWordArray arrayRoomIdx;	
		for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
		{
			if ( nRoomIdx == nSourceRoomIdx )
				continue;

			CPMSRoomStatusInfo infoDest;
			PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoDest );

			if ( infoDest.GetBookingId() != 0 )
				continue;
			
			CPMSRoomCSVRecord Room;
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
				
			if ( Room.GetSleeps() < nGuests )
				continue;

			CPMSBookingQueryInfo infoQuery;
			infoQuery.m_nStartDay = PMSOptions.GetPMSDayNumber();
			infoQuery.m_nMinNights = dlgExtend.m_nNights;
			infoQuery.m_nMaxNights = dlgExtend.m_nNights;
			infoQuery.m_nRoomNo = Room.GetRoomNo();

			CPMSBookingResponseInfo infoResponse;
			PMSCalendarTable.CheckRoomAvailability( infoQuery, infoResponse );
			if ( FALSE == infoResponse.m_bSuccess )
				continue;

			arrayRoomIdx.Add( nRoomIdx );
		}
	
		if ( arrayRoomIdx.GetSize() == 0 )
		{
			Prompter.Error( GetSwitchError( PMSOptions.GetPMSDayNumber(), nCheckoutDay, nGuests ) );
			return;
		}

		CListDataSwitchRoomDlg dlg( infoSource.GetRoomName(), nGuests, bTraining, arrayRoomIdx, this );
		if ( dlg.DoModal() != IDOK )
			return;

		CPMSRoomStatusInfo infoDest;
		int nDestRoomIdx = dlg.GetDestRoomIdx();
		PMSRoomStatusTable.GetRoomInfo( nDestRoomIdx, infoDest );

		BookingRecord.ValidateForEditing( PMSOptions.GetPMSDayNumber() );
		if ( BookingRecord.GetValidationError() != 0 )
		{
			CString str;
			str.Format( "Unable to split booking\n\nError code %d", BookingRecord.GetValidationError() );
			Prompter.Error( str );
			return;
		}

		//REMOVE THE ORIGINAL BOOKING FROM THE CALENDAR TABLE
		PMSCalendarTable.SetEditMarkers( BookingRecord );
		PMSCalendarTable.DeleteMarkedBookings();

		CPMSBookingPartInfo infoPart;
		infoPart.SetRoomNo ( infoDest.GetRoomNo() );
		infoPart.SetStartDay( PMSOptions.GetPMSDayNumber() );
		infoPart.SetNights( dlgExtend.m_nNights );

		BookingRecord.AddPartInfo( infoPart );
		
		CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );

		//UPDATE THE CALENDAR TABLE
		PMSCalendarTable.ApplyBooking( BookingRecord );
	
		//UPDATE THE ROOM STATUS ARRAY IN MEMORY
		PMSRoomStatusTable.CheckOut( nSourceRoomIdx );	
		PMSRoomStatusTable.CheckIn( nDestRoomIdx, BookingRecord );
		
		PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecord, TRUE );

		PMSRoomStatusTable.WriteDownloadFileRoom( nSourceRoomIdx );
		PMSRoomStatusTable.UpdateAccountBalance( nSourceRoomIdx, TRUE );
		
		PMSRoomStatusTable.WriteDownloadFileRoom( nDestRoomIdx );
		PMSRoomStatusTable.UpdateAccountBalance( nDestRoomIdx, TRUE );

		ScrollToRoom( nDestRoomIdx );
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionExtendBookingTomorrow()
{
	int nSourceRoomIdx = m_nMenuRoomIdx;

	CPMSRoomStatusInfo infoSource;
	PMSRoomStatusTable.GetRoomInfo( nSourceRoomIdx, infoSource );

	int nBookingId = infoSource.GetBookingId();
	if ( infoSource.GetBookingId() == 0 )
		return;

	int nGuests = infoSource.GetGuestCount();
	if ( nGuests < 1 )
		return;

	//READ ORIGINAL BOOKING FROM DATABASE
	int nBookingIdx;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	
	CPMSBookingQueryInfo infoQuery;
	infoQuery.m_nStartDay = PMSOptions.GetPMSDayNumber() + 1;
	infoQuery.m_nMinNights = 1;
	infoQuery.m_nMaxNights = 14;
	infoQuery.m_nRoomNo = infoSource.GetRoomNo();

	CPMSBookingResponseInfo infoResponse;
	PMSCalendarTable.CheckRoomAvailability( infoQuery, infoResponse );
	
	if ( FALSE == infoResponse.m_bSuccess )
		Prompter.Error ( "This room is already booked for tomorrow night." );
	else
	{
		CPMSBookEditExtendDlg dlgExtend( nSourceRoomIdx, this );
		dlgExtend.SetMaxNights( infoResponse.m_nMaxNights );
		
		if ( dlgExtend.DoModal() == IDOK )
		{
			PMSCalendarTable.SetEditMarkers( BookingRecord );
			PMSCalendarTable.DeleteMarkedBookings();
		
			CPMSBookingPartInfo infoPart;
			BookingRecord.GetFinalPartInfo( infoPart );
			infoPart.SetNights( PMSOptions.GetPMSDayNumber() + 1 + dlgExtend.m_nNights - infoPart.GetStartDay() );
			BookingRecord.SetFinalPartInfo( infoPart );
		
			DataManagerNonDb.PMSBooking.InsertAndSaveBooking( BookingRecord );
			PMSCalendarTable.ApplyBooking( BookingRecord );
			PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecord, FALSE );
		}
	}
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionExtendBookingComplete()
{
	int nSourceRoomIdx = m_nMenuRoomIdx;

	//FIND BOOKING IN DATABASE
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( m_nMenuBookingId, nBookingIdx ) == FALSE )
		return;

	//BOOKING MUST BE CURRENTLY CHECKED OUT
	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
		return;

	bool bTraining = BookingRecord.GetTrainingFlag();
	int nGuests = BookingRecord.GetGuests();

	//FIND OUT IF LAST ROOM USED BEFORE CHECKOUT IS STILL VACANT
	CPMSRoomStatusInfo infoSource;
	PMSRoomStatusTable.GetRoomInfo( nSourceRoomIdx, infoSource );
	bool bOriginalRoomVacant = ( infoSource.GetBookingId() == 0 );
	
	//FIND OUT HOW MANY NIGHTS TO EXTEND BOOKING FOR
	CPMSBookEditExtendDlg dlgExtend( nSourceRoomIdx, this );
	if ( dlgExtend.DoModal() != IDOK )
		return;

	//NOTE ORIGINAL AND EXTENDED CHECKOUT DATES
	CPMSBookingPartInfo infoPart;
	BookingRecord.GetFinalPartInfo( infoPart );
	int nOriginalCheckoutDay = infoPart.GetStartDay() + infoPart.GetNights();
	int nExtendedCheckoutDay = nOriginalCheckoutDay + dlgExtend.m_nNights;
	
	//IF ORIGINAL ROOM IS VACANT, THEN SEE IF WE CAN EXTEND STAY FROM THERE
	if ( TRUE == bOriginalRoomVacant )
	{
		CPMSBookingQueryInfo infoQuery;
		infoQuery.m_nStartDay = nOriginalCheckoutDay;
		infoQuery.m_nMinNights = dlgExtend.m_nNights;
		infoQuery.m_nMaxNights = dlgExtend.m_nNights;
		infoQuery.m_nRoomNo = infoSource.GetRoomNo();

		CPMSBookingResponseInfo infoResponse;
		PMSCalendarTable.CheckRoomAvailability( infoQuery, infoResponse );
	
		//ORIGINAL ROOM WAS AVAILABLE, SO CHECK BACK IN TO IT
		if ( TRUE == infoResponse.m_bSuccess )
		{
			PMSCalendarTable.SetEditMarkers( BookingRecord );
			PMSCalendarTable.DeleteMarkedBookings();
		
			infoPart.SetNights( infoPart.GetNights() + dlgExtend.m_nNights );
			BookingRecord.SetFinalPartInfo( infoPart );
		
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_ACTIVE );
			DataManagerNonDb.PMSBooking.InsertAndSaveBooking( BookingRecord );
			PMSCalendarTable.ApplyBooking( BookingRecord );

			PMSRoomStatusTable.CheckIn( nSourceRoomIdx, BookingRecord );

			PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecord, TRUE );
		
			PMSRoomStatusTable.WriteDownloadFileRoom( nSourceRoomIdx );
			PMSRoomStatusTable.UpdateAccountBalance( nSourceRoomIdx, TRUE );

			ScrollToRoom( nSourceRoomIdx );
			return;
		}
	}

	//ORIGINAL ROOM WAS NOT AVAILABLE FOR REQUESTED PERIOD
	CString strMsg = "";

	if ( FALSE == bOriginalRoomVacant )
	{
		strMsg.Format ( "Room %d is already occupied by another booking.",
			infoSource.GetRoomNo() );
	}
	else
	{
		strMsg.Format ( "Room %d is already booked for the requested period.",
			infoSource.GetRoomNo() );
	}
	
	strMsg += "\n\nWould you like to search for an alternative room.";

	if ( Prompter.YesNo( strMsg ) != IDYES )
		return;

	//CREATE A LIST OF ROOMS THAT ARE AVAILABLE FOR THE REQUIRED PARTY SIZE AND TIME
	CWordArray arrayRoomIdx;	
	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		if ( nRoomIdx == nSourceRoomIdx )
			continue;

		CPMSRoomStatusInfo infoDest;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoDest );

		if ( infoDest.GetBookingId() != 0 )
			continue;
			
		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
				
		if ( Room.GetSleeps() < nGuests )
			continue;

		CPMSBookingQueryInfo infoQuery;
		infoQuery.m_nStartDay = nOriginalCheckoutDay;
		infoQuery.m_nMinNights = dlgExtend.m_nNights;
		infoQuery.m_nMaxNights = dlgExtend.m_nNights;
		infoQuery.m_nRoomNo = Room.GetRoomNo();

		CPMSBookingResponseInfo infoResponse;
		PMSCalendarTable.CheckRoomAvailability( infoQuery, infoResponse );
		if ( FALSE == infoResponse.m_bSuccess )
			continue;

		arrayRoomIdx.Add( nRoomIdx );
	}

	//ERROR IF NO ROOMS AVAILABLE FOR THE REQUIRED PARTY SIZE AND TIME
	if ( arrayRoomIdx.GetSize() == 0 )
	{
		Prompter.Error( GetSwitchError( nOriginalCheckoutDay, nExtendedCheckoutDay, nGuests ) );
		return;
	}

	//PROMPT TO SELECT FROM THE LIST OF AVAILABLE ROOMS
	CListDataSwitchRoomDlg dlg( infoSource.GetRoomName(), nGuests, bTraining, arrayRoomIdx, this );
	if ( dlg.DoModal() != IDOK )
		return;

	CPMSRoomStatusInfo infoDest;
	int nDestRoomIdx = dlg.GetDestRoomIdx();
	PMSRoomStatusTable.GetRoomInfo( nDestRoomIdx, infoDest );

	BookingRecord.ValidateForEditing( PMSOptions.GetPMSDayNumber(), TRUE );
	if ( BookingRecord.GetValidationError() != 0 )
	{
		CString str;
		str.Format( "Unable to split booking\n\nError code %d", BookingRecord.GetValidationError() );
		Prompter.Error( str );
		return;
	}

	//REMOVE THE ORIGINAL BOOKING FROM THE CALENDAR TABLE
	PMSCalendarTable.SetEditMarkers( BookingRecord );
	PMSCalendarTable.DeleteMarkedBookings();

	//EXTEND THE BOOKING INTO THE NEW ROOM
	infoPart.SetRoomNo ( infoDest.GetRoomNo() );
	infoPart.SetStartDay( PMSOptions.GetPMSDayNumber() );
	infoPart.SetNights( dlgExtend.m_nNights );
	BookingRecord.AddPartInfo( infoPart );

	//MARK THE BOOKING AS ACTIVE AND SAVE IT
	BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_ACTIVE );
	CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );

	//UPDATE THE CALENDAR TABLE
	PMSCalendarTable.ApplyBooking( BookingRecord );
	
	//UPDATE THE ROOM STATUS ARRAY IN MEMORY
	PMSRoomStatusTable.CheckIn( nDestRoomIdx, BookingRecord );
		
	PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecord, TRUE );

	PMSRoomStatusTable.WriteDownloadFileRoom( nDestRoomIdx );
	PMSRoomStatusTable.UpdateAccountBalance( nDestRoomIdx, TRUE );

	ScrollToRoom( nDestRoomIdx );
}

/**********************************************************************/
#endif
/**********************************************************************/
