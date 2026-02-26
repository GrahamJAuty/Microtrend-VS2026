/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSReportRoomList.h"
/**********************************************************************/

CPMSReportRoomList::CPMSReportRoomList()
{
	CFilenameUpdater FnUp( SysFiles::PMSRoomListPrm );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_bOccupied1 = FALSE;
	m_bOccupied2 = FALSE;
	m_bOccupied3 = FALSE;
	m_bVacant1 = FALSE;
	m_bVacant2 = FALSE;
	m_bVacant3 = FALSE;
	m_bBalance = FALSE;
	m_bLines = FALSE;
	m_bTraining = FALSE;
}

/**********************************************************************/

void CPMSReportRoomList::CreateReport()
{
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = CreateRoomListReport();
	StatusProgress.Unlock();

	CString strKey = ( m_bOccupied1 | m_bOccupied2 | m_bOccupied3 ) ? "1" : "0";
	
	if ( TRUE == bResult )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, strKey, NULL );
	}
	else
		Prompter.Error( "Unable to create report." );
}

/**********************************************************************/
void CPMSReportRoomList::AddColumnRoomNo(){ m_ReportFile.AddColumn ( "No", TA_LEFT, 120 ); }
void CPMSReportRoomList::AddColumnRoomName(){ m_ReportFile.AddColumn ( "Room", TA_LEFT, 250 ); }
void CPMSReportRoomList::AddColumnCheckIn(){ m_ReportFile.AddColumn ( "Check In", TA_LEFT, 220 ); }
void CPMSReportRoomList::AddColumnNights(){ m_ReportFile.AddColumn ( "Nights", TA_LEFT, 150 ); }
void CPMSReportRoomList::AddColumnCheckOut(){ m_ReportFile.AddColumn ( "Check Out", TA_LEFT, 220 ); }
void CPMSReportRoomList::AddColumnGuests(){ m_ReportFile.AddColumn ( "Guests", TA_LEFT, 150 ); }
void CPMSReportRoomList::AddColumnGuestName(){ m_ReportFile.AddColumn ( "Guest Name", TA_LEFT, 290 ); }
void CPMSReportRoomList::AddColumnBillingName(){ m_ReportFile.AddColumn ( "Billing Name", TA_LEFT, 290 ); }
void CPMSReportRoomList::AddColumnBalance(){ m_ReportFile.AddColumn ( "Balance", TA_RIGHT, 170 ); }
void CPMSReportRoomList::AddColumnCR(){ m_ReportFile.AddColumn ( "", TA_LEFT, 100 ); }
/**********************************************************************/

bool CPMSReportRoomList::IsRoomBooked( int nRoomNo, int nDayNumber )
{
	bool bResult = FALSE;

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = nRoomNo;
	Query.m_nStartDay = nDayNumber;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = 1;

	CPMSBookingResponseInfo Response;
	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( Response.m_nBookingClashId != 0 )
	{
		CPMSCalendarBooking CalendarBooking;
		PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

		if ( CalendarBooking.m_nStatus != PMS_BOOKING_STATUS_CLOSED )
			bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/
				
bool CPMSReportRoomList::CreateRoomListReport()
{
	int nRooms = DataManagerNonDb.PMSRoom.GetSize();

	if ( nRooms > SysInfo.GetMaxPMSRooms() )
		nRooms = SysInfo.GetMaxPMSRooms();

	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;
	
	m_ReportFile.SetStyle1 ( "Guest Account Room List" );
		
	AddColumnRoomNo();
	AddColumnRoomName();

	if ( m_bOccupied1 | m_bOccupied2 | m_bOccupied3 )
	{
		AddColumnCheckIn();
		AddColumnNights();
		AddColumnCheckOut();
		AddColumnGuests();
		AddColumnBillingName();
		AddColumnGuestName();
		
		if ( TRUE == m_bBalance )
		{
			AddColumnBalance();
			AddColumnCR();
		}
	}

	int nTotalNights = 0;
	int nTotalGuests = 0;
	double dTotalBalance = 0.0;
	
	COleDateTime dateNow = COleDateTime::GetCurrentTime();

	for ( int nRoomIdx = 0; nRoomIdx < nRooms; nRoomIdx++ )
	{
		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( nRoomIdx, infoRoom );

		int nBookingId = infoRoom.GetBookingId();
		
		if ( PMSRoomStatusTable.GetClosureFlag( nRoomIdx ) == TRUE )
			nBookingId = 0;

		//FILTER TRAINING BOOKINGS
		if ( ( nBookingId != 0 ) && ( FALSE == m_bTraining ) )
		{
			int nBookingIdx;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
			{
				CPMSBookingCSVRecord BookingRecord;
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

				if ( BookingRecord.GetTrainingFlag() == TRUE )
					continue;
			}
		}

		bool bInclude = TRUE;
		bool bChecked = FALSE;

		if ( 0 == nBookingId )
		{
			if ( ( m_bVacant1 & m_bVacant2 & m_bVacant3 ) == FALSE )
			{
				if ( IsRoomBooked ( infoRoom.GetRoomNo(), PMSOptions.GetPMSDayNumber() ) )
					bInclude = m_bVacant1;
				else if ( IsRoomBooked ( infoRoom.GetRoomNo(), PMSOptions.GetPMSDayNumber() + 1 ) )
					bInclude = m_bVacant2;
				else
					bInclude = m_bVacant3;
			}
		}
		else
		{
			if ( ( m_bOccupied1 & m_bOccupied2 & m_bOccupied3 ) == FALSE )
			{
				bInclude = FALSE;

				int nBookingIdx;
				if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
				{
					CPMSBookingCSVRecord BookingRecord;
					DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

					CPMSBookingPartInfo infoPart;
					BookingRecord.GetFinalPartInfo( infoPart );

					int nCheckOutDay = infoPart.GetStartDay() + infoPart.GetNights();
					int nPMSDay = PMSOptions.GetPMSDayNumber();

					if ( nCheckOutDay <= nPMSDay )
						bInclude = m_bOccupied1;
					else if ( nCheckOutDay == nPMSDay + 1 )
						bInclude = m_bOccupied2;
					else
						bInclude = m_bOccupied3;
				}
			}
		}
	
		if ( FALSE == bInclude )
			continue;

		COleDateTime dateCheckIn( 1980, 1, 1, 0, 0, 0 );
		COleDateTime dateCheckOut( 1980, 1, 2, 0, 0, 0 );
		int nNights = 1;

		//VACANT ROOMS
		if ( nBookingId == 0 )
		{
			CCSV csvOut( '\t' );
			csvOut.Add( infoRoom.GetRoomNo() );
			csvOut.Add( infoRoom.GetRoomName() );
			m_ReportFile.WriteLine( csvOut.GetLine() );
		}
		//OCCUPIED ROOMS
		else
		{
			int nCheckInDate = infoRoom.GetStartDay();
			GetOleDateSince2011( nCheckInDate, dateCheckIn );

			int nCheckOutDate = infoRoom.GetStartDay() + infoRoom.GetNights();
			GetOleDateSince2011( nCheckOutDate, dateCheckOut );

			nNights = infoRoom.GetNights(); 

			CString strDateCheckIn;
			strDateCheckIn.Format( "%2.2d/%2.2d/%4.4d",
				dateCheckIn.GetDay(),
				dateCheckIn.GetMonth(),
				dateCheckIn.GetYear() );

			CString strDateCheckOut;
			strDateCheckOut.Format( "%2.2d/%2.2d/%4.4d",
				dateCheckOut.GetDay(),
				dateCheckOut.GetMonth(),
				dateCheckOut.GetYear() );

			int nGuests = infoRoom.GetGuestCount();
			nTotalGuests += nGuests;
			nTotalNights += nNights;
			
			for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
			{
				CCSVReportLine ReportLine;
				
				if ( nAccountIdx == 0 )
				{
					ReportLine.AppendInt( infoRoom.GetRoomNo() );
					ReportLine.AppendString( infoRoom.GetRoomName() );
					ReportLine.AppendString( strDateCheckIn );
					ReportLine.AppendInt( nNights );
					ReportLine.AppendString( strDateCheckOut );
					ReportLine.AppendInt( nGuests );
				}
				else
					ReportLine.AppendBlanks(6);

				CPMSRoomAccountInfo infoAccount;
				PMSRoomStatusTable.GetAccountInfo( nRoomIdx, nAccountIdx, infoAccount );

				ReportLine.AppendString( infoAccount.GetBillingName() );
				ReportLine.AppendString( infoAccount.GetGuestName() );

				if ( TRUE == m_bBalance )
				{
					double dBalance = infoAccount.CalculateBalance();
					
					switch ( CPriceHelpers::CompareDoubles( dBalance, 0.0, 2 ) )
					{
					case 1:
						ReportLine.AppendVal( dBalance, 2 );
						break;

					case -1:
						ReportLine.AppendVal( -dBalance, 2 );
						ReportLine.AppendString( "CR" );
						break;

					case 0:
					default:
						ReportLine.AppendVal( 0.0, 2 );
					}
		
					dTotalBalance += dBalance;
				}

				m_ReportFile.WriteLine( ReportLine.GetLine() );
			}
		}

		if ( TRUE == m_bLines )
			m_ReportFile.WriteLine( "<LI>" );
	}

	if ( m_bOccupied1 | m_bOccupied2 | m_bOccupied3 )
	{
		if ( FALSE == m_bLines )
		{
			CString strLine = "\t\t\t<LI>\t\t<LI>\t\t";
			
			if ( TRUE == m_bBalance )
				strLine += "\t<LI>";

			m_ReportFile.WriteLine( strLine );
		}

		CCSVReportLine ReportLine;
		ReportLine.AppendBlanks(2);
		ReportLine.AppendString( "Total" );
		ReportLine.AppendInt( nTotalNights );
		ReportLine.AppendString( "" );
		ReportLine.AppendInt( nTotalGuests );
		ReportLine.AppendBlanks(2);
		
		if ( TRUE == m_bBalance )
		{
			switch ( CPriceHelpers::CompareDoubles( dTotalBalance, 0.0, 3 ) )
			{
			case 1:
				ReportLine.AppendVal( dTotalBalance );
				break;

			case -1:
				ReportLine.AppendVal( -dTotalBalance );
				ReportLine.AppendString( "CR" );
				break;

			case 0:
			default:
				ReportLine.AppendVal( 0.0 );
				break;
			}
		}

		m_ReportFile.WriteLine( ReportLine.GetLine() );
	}

	m_ReportFile.WriteLine ( "" );	
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/