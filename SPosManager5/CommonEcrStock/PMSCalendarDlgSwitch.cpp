/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "ListDataSwitchRoomDlg.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionSwitchRoomPending()
{
	int nSourceRoomIdx = m_nMenuRoomIdx;

	CPMSRoomCSVRecord RoomRecord;
	DataManagerNonDb.PMSRoom.GetAt( nSourceRoomIdx, RoomRecord );
	CString strSourceRoomName = RoomRecord.GetRoomName();

	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( m_nMenuBookingId, nBookingIdx ) == FALSE )
		return;

	CPMSBookingCSVRecord BookingRecord;
	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

	if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_PENDING )
		return;

	if ( BookingRecord.GetPartCount() != 1 )
		return;

	int nGuests = BookingRecord.GetGuests();
	bool bTraining = BookingRecord.GetTrainingFlag();
	int nTariffType = BookingRecord.GetTariffType();

	CPMSBookingPartInfo infoPart;
	BookingRecord.GetFinalPartInfo( infoPart );

	int nCheckinDay = infoPart.GetStartDay();
	int nCheckoutDay = infoPart.GetStartDay() + infoPart.GetNights();

	//CREATE A LIST OF ROOMS THAT COULD ACCOMODATE THESE GUESTS
	CWordArray arrayRoomIdx;	
	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		if ( nRoomIdx == nSourceRoomIdx )
			continue;
		
		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
		
		int nMaxGuests = Room.GetSleeps();

		if ( PMS_TARIFFTYPE_EXTRACOT == nTariffType )
		{
			if ( Room.GetRoomAllowExtraCotFlag() )
				nMaxGuests++;
			else
				continue;
		}
		else if ( PMS_TARIFFTYPE_EXTRABED == nTariffType )
		{
			if ( Room.GetRoomAllowExtraBedFlag() )
				nMaxGuests++;
		}

		if ( nMaxGuests < nGuests )
			continue;

		CPMSBookingQueryInfo infoQuery;
		infoQuery.m_nStartDay = infoPart.GetStartDay();
		infoQuery.m_nMinNights = infoPart.GetNights();
		infoQuery.m_nMaxNights = infoPart.GetNights();
		infoQuery.m_nRoomNo = Room.GetRoomNo();

		CPMSBookingResponseInfo infoResponse;
		PMSCalendarTable.CheckRoomAvailability( infoQuery, infoResponse );
		if ( FALSE == infoResponse.m_bSuccess )
			continue;

		arrayRoomIdx.Add( nRoomIdx );
	}
	
	if ( arrayRoomIdx.GetSize() == 0 )
	{
		Prompter.Error( GetSwitchError( nCheckinDay, nCheckoutDay, nGuests ) );
		return;
	}

	CListDataSwitchRoomDlg dlg( strSourceRoomName, nGuests, bTraining, arrayRoomIdx, this );
	if ( dlg.DoModal() != IDOK )
		return;

	CPMSRoomStatusInfo infoDest;
	int nDestRoomIdx = dlg.GetDestRoomIdx();
	PMSRoomStatusTable.GetRoomInfo( nDestRoomIdx, infoDest );

	//REMOVE THE ORIGINAL BOOKING FROM THE CALENDAR TABLE
	PMSCalendarTable.SetEditMarkers( BookingRecord );
	PMSCalendarTable.DeleteMarkedBookings();

	//CHANGE ROOM NUMBER AS REQUESTED
	infoPart.SetRoomNo ( infoDest.GetRoomNo() );
	BookingRecord.SetFinalPartInfo( infoPart );
	
	CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );

	//UPDATE THE CALENDAR TABLE
	PMSCalendarTable.ApplyBooking( BookingRecord );
}

/**********************************************************************/

void CPMSCalendarDlg::ContextMenuOptionSwitchRoomActive()
{
	int nSourceRoomIdx = m_nMenuRoomIdx;

	CPMSRoomStatusInfo infoSource;
	PMSRoomStatusTable.GetRoomInfo( nSourceRoomIdx, infoSource );

	int nBookingId = infoSource.GetBookingId();
	if ( 0 == nBookingId )
		return;

	bool bTraining = FALSE;
	int nTariffType = PMS_TARIFFTYPE_STANDARD;
	
	int nBookingIdx;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
	{
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
		bTraining = BookingRecord.GetTrainingFlag();
		nTariffType = BookingRecord.GetTariffType();
	}

	int nGuests = infoSource.GetGuestCount();
	if ( nGuests < 1 )
		return;

	int nCheckoutDay = infoSource.GetStartDay() + infoSource.GetNights();

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

		int nMaxGuests = Room.GetSleeps();
		
		if ( PMS_TARIFFTYPE_EXTRACOT == nTariffType )
		{
			if ( Room.GetRoomAllowExtraCotFlag() )
				nMaxGuests++;
			else
				continue;
		}
		else if ( PMS_TARIFFTYPE_EXTRABED == nTariffType )
		{
			if ( Room.GetRoomAllowExtraBedFlag() )
				nMaxGuests++;
			else
				continue;
		}

		if ( nMaxGuests < nGuests )
			continue;

		CPMSBookingQueryInfo infoQuery;
		infoQuery.m_nStartDay = PMSOptions.GetPMSDayNumber();
		infoQuery.m_nMinNights = nCheckoutDay - PMSOptions.GetPMSDayNumber();
		infoQuery.m_nMaxNights = nCheckoutDay - PMSOptions.GetPMSDayNumber();
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

	CPMSBookingPartInfo infoPart;
	BookingRecord.GetFinalPartInfo( infoPart );

	//REMOVE THE ORIGINAL BOOKING FROM THE CALENDAR TABLE
	PMSCalendarTable.SetEditMarkers( BookingRecord );
	PMSCalendarTable.DeleteMarkedBookings();

	//SWITCHING ON CHECK IN DAY, SO JUST CHANGE ROOM FOR ORIGINAL BOOKING
	if ( infoPart.GetStartDay() == PMSOptions.GetPMSDayNumber() )
	{
		infoPart.SetRoomNo ( infoDest.GetRoomNo() );
		BookingRecord.SetFinalPartInfo( infoPart );

		if ( BookingRecord.GetPartCount() > 1 )
		{
			CPMSBookingPartInfo infoPartPrevious;
			BookingRecord.GetPartInfo( BookingRecord.GetPartCount() - 2, infoPartPrevious );

			if ( infoPartPrevious.GetRoomNo() == infoPart.GetRoomNo() )
			{
				infoPartPrevious.SetNights( infoPartPrevious.GetNights() + infoPart.GetNights() );
				BookingRecord.RemoveFinalPartInfo();
				BookingRecord.SetFinalPartInfo( infoPartPrevious );
			}
		}
	}
	//SWITCHING AFTER CHECK IN DAY, SO MODIFY ORIGINAL BOOKING AND ADD NEW ONE
	else
	{
		infoPart.SetNights ( PMSOptions.GetPMSDayNumber() - infoPart.GetStartDay() );
		BookingRecord.SetFinalPartInfo( infoPart );

		CPMSBookingPartInfo infoPartNew;
		infoPartNew.SetRoomNo( infoDest.GetRoomNo() );
		infoPartNew.SetStartDay( PMSOptions.GetPMSDayNumber() );
		infoPartNew.SetNights( nCheckoutDay - PMSOptions.GetPMSDayNumber() );
		BookingRecord.AddPartInfo( infoPartNew );
	}

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

/**********************************************************************/

const char* CPMSCalendarDlg::GetSwitchError( int nCheckinDay, int nCheckoutDay, int nGuests )
{
	COleDateTime dateCheckin;
	GetOleDateSince2011( nCheckinDay, dateCheckin );

	CString strDateIn;
	strDateIn.Format( "%2.2d/%2.2d/%4.4d",
		dateCheckin.GetDay(),
		dateCheckin.GetMonth(),
		dateCheckin.GetYear() );

	COleDateTime dateCheckout;
	GetOleDateSince2011( nCheckoutDay, dateCheckout );

	CString strDateOut;
	strDateOut.Format( "%2.2d/%2.2d/%4.4d",
		dateCheckout.GetDay(),
		dateCheckout.GetMonth(),
		dateCheckout.GetYear() );

	CString str;
	if ( nGuests == 1 )
		m_strSwitchError = "There are no other rooms available from\n";
	else
		m_strSwitchError.Format( "There are no other rooms available to sleep %d guests\nfrom ", nGuests );

	m_strSwitchError += strDateIn;
	m_strSwitchError += " to ";
	m_strSwitchError += strDateOut;

	return m_strSwitchError;
}

/**********************************************************************/
#endif
/**********************************************************************/
