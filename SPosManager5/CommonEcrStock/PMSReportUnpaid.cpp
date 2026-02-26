/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "CCSVReportLine.h"
#include "PMSBookingCSVArray.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PMSReportUnpaid.h"
/**********************************************************************/

CPMSReportUnpaid::CPMSReportUnpaid()
{
	CFilenameUpdater FnUp( SysFiles::PMSUnpaidPrm );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_bIncludePending = TRUE;
	m_bIncludeActive = TRUE;
	m_bIncludeComplete = FALSE;
	m_bIncludeCancelled = FALSE;
	m_bLines = FALSE;
	m_bIncludeTraining = FALSE;
}

/**********************************************************************/

void CPMSReportUnpaid::CreateReport()
{
	bool bResult = FALSE;

	StatusProgress.Lock( TRUE, "Creating report" );

	bResult = CreateReportInternal();
	
	StatusProgress.Unlock();
	
	if ( TRUE == bResult )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, "", NULL );
	}
	else
		Prompter.Error( "Unable to create report." );
}

/**********************************************************************/
void CPMSReportUnpaid::AddColumnInvoiceNum(){ m_ReportFile.AddColumn ( "Invoice", TA_LEFT, 200 ); }
void CPMSReportUnpaid::AddColumnBookingRef(){ m_ReportFile.AddColumn ( "Reference", TA_LEFT, 200 ); }
void CPMSReportUnpaid::AddColumnRoomNo(){ m_ReportFile.AddColumn ( "No", TA_LEFT, 120 ); }
void CPMSReportUnpaid::AddColumnRoomName(){ m_ReportFile.AddColumn ( "Room", TA_LEFT, 250 ); }
void CPMSReportUnpaid::AddColumnCheckIn(){ m_ReportFile.AddColumn ( "Check In", TA_LEFT, 220 ); }
void CPMSReportUnpaid::AddColumnNights(){ m_ReportFile.AddColumn ( "Nights", TA_LEFT, 150 ); }
void CPMSReportUnpaid::AddColumnCheckOut(){ m_ReportFile.AddColumn ( "Check Out", TA_LEFT, 220 ); }
void CPMSReportUnpaid::AddColumnGuests(){ m_ReportFile.AddColumn ( "Guests", TA_LEFT, 150 ); }
void CPMSReportUnpaid::AddColumnGuestName(){ m_ReportFile.AddColumn ( "Guest Name", TA_LEFT, 290 ); }
void CPMSReportUnpaid::AddColumnBillingName(){ m_ReportFile.AddColumn ( "Billing Name", TA_LEFT, 290 ); }
void CPMSReportUnpaid::AddColumnBalance(){ m_ReportFile.AddColumn ( "Balance", TA_RIGHT, 170 ); }
void CPMSReportUnpaid::AddColumnCR(){ m_ReportFile.AddColumn ( "", TA_LEFT, 100 ); }
/**********************************************************************/

bool CPMSReportUnpaid::CreateReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;
	
	m_ReportFile.SetStyle1 ( "Unpaid Account Report" );

	AddColumnInvoiceNum();
	AddColumnBookingRef();
	AddColumnRoomNo();
	AddColumnRoomName();
	AddColumnCheckIn();
	AddColumnNights();
	AddColumnCheckOut();
	AddColumnGuests();
	AddColumnBillingName();
	AddColumnGuestName();
	AddColumnBalance();
	AddColumnCR();

	int nTotalNights = 0;
	int nTotalGuests = 0;
	double dTotalDebit = 0.0;
	double dTotalCredit = 0.0;

	StatusProgress.Lock( TRUE, "Creating Report" );
	
	int nSize = DataManagerNonDb.PMSBooking.GetSize();
	for ( int nBookingIdx = 0; nBookingIdx < nSize; nBookingIdx++ )
	{
		StatusProgress.SetPos( nBookingIdx, nSize );

		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

		if ( FALSE == m_bIncludeTraining )
			if ( BookingRecord.GetTrainingFlag() == TRUE )
				continue;

		int nStatus = BookingRecord.GetBookingStatus();

		bool bInclude = FALSE;
		switch( nStatus )
		{
		case PMS_BOOKING_STATUS_PENDING:		bInclude = m_bIncludePending;		break;
		case PMS_BOOKING_STATUS_ACTIVE:			bInclude = m_bIncludeActive;		break;
		case PMS_BOOKING_STATUS_COMPLETE:		bInclude = m_bIncludeComplete;		break;
		case PMS_BOOKING_STATUS_CANCELLED:		bInclude = m_bIncludeCancelled;		break;
		}		

		if ( FALSE == bInclude )
			continue;

		PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

		int nBookingId = infoRoom.GetBookingId();

		COleDateTime dateCheckIn( 1980, 1, 1, 0, 0, 0 );
		COleDateTime dateCheckOut( 1980, 1, 1, 0, 0, 0 );

		int nCheckInDate = infoRoom.GetStartDay();
		GetOleDateSince2011( nCheckInDate, dateCheckIn );

		int nCheckOutDate = infoRoom.GetStartDay() + infoRoom.GetNights();
		GetOleDateSince2011( nCheckOutDate, dateCheckOut );
	
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
		nTotalNights += infoRoom.GetNights();

		CString strBookingId;
		strBookingId.Format( "%8.8d", infoRoom.GetBookingId() );

		CString strInvoiceNum = "";
		if ( BookingRecord.GetInvoiceNum() != 0 )
			strInvoiceNum.Format( "%8.8d", BookingRecord.GetInvoiceNum() );
		
		bool bWrittenBookingLine = FALSE;

		for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
		{
			CPMSRoomAccountInfo infoAccount;
			PMSRoomStatusTable.GetAccountInfo( MAX_PMS_ROOMS, nAccountIdx, infoAccount );

			double dBalance = infoAccount.CalculateBalance();
			if ( CPriceHelpers::CompareDoubles( dBalance, 0.0, 3 ) == 0 )
				continue;
			
			CCSVReportLine ReportLine;
			
			if ( FALSE == bWrittenBookingLine )
			{
				ReportLine.AppendString( strInvoiceNum );
				ReportLine.AppendString( strBookingId );
				ReportLine.AppendInt( infoRoom.GetRoomNo() );
				ReportLine.AppendString( infoRoom.GetRoomName() );
				ReportLine.AppendString( strDateCheckIn );
				ReportLine.AppendInt( infoRoom.GetNights() );
				ReportLine.AppendString( strDateCheckOut );
				ReportLine.AppendInt( nGuests );
				bWrittenBookingLine = TRUE;
			}
			else
				ReportLine.AppendBlanks(8);

			ReportLine.AppendString( infoAccount.GetBillingName() );
			ReportLine.AppendString( infoAccount.GetGuestName() );

			switch ( CPriceHelpers::CompareDoubles( dBalance, 0.0, 2 ) )
			{
			case 1:
				ReportLine.AppendVal( dBalance, 2 );
				dTotalDebit += dBalance;
				break;

			case -1:
				ReportLine.AppendVal( -dBalance, 2 );
				ReportLine.AppendString( "CR" );
				dTotalCredit -= dBalance;
				break;

			case 0:
			default:
				ReportLine.AppendVal( 0.0, 2 );
				break;
			}
			
			m_ReportFile.WriteLine( ReportLine.GetLine() );
		}

		if ( ( TRUE == m_bLines ) && ( TRUE == bWrittenBookingLine ) )
			m_ReportFile.WriteLine( "<LI>" );
	}

	StatusProgress.Unlock();
	
	if ( FALSE == m_bLines )
		m_ReportFile.WriteLine( "\t\t\t\t\t\t\t\t\t\t<LI>" );
		
	CCSVReportLine ReportLine;
	ReportLine.AppendBlanks(9);
	ReportLine.AppendString( "Total Debit" );
	ReportLine.AppendVal( dTotalDebit );
	m_ReportFile.WriteLine( ReportLine.GetLine() );
	
	ReportLine.RemoveAll();
	ReportLine.AppendBlanks(9);
	ReportLine.AppendString( "Total Credit" );
	ReportLine.AppendVal( dTotalCredit );
	m_ReportFile.WriteLine( ReportLine.GetLine() );
	
	m_ReportFile.WriteLine ( "\t\t\t\t\t\t\t\t\t\t<LI>" );

	ReportLine.RemoveAll();
	ReportLine.AppendBlanks(9);
	ReportLine.AppendString( "Balance" );

	switch( CPriceHelpers::CompareDoubles( dTotalDebit, dTotalCredit, 2 ) )
	{
	case 1:
		ReportLine.AppendVal( dTotalDebit - dTotalCredit, 2 );
		break;

	case -1:
		ReportLine.AppendVal( dTotalCredit - dTotalDebit, 2 );
		ReportLine.AppendString( " CR" );
		break;

	case 0:
	default:
		ReportLine.AppendVal( 0.0, 2 );
		break;
	}

	m_ReportFile.WriteLine( ReportLine.GetLine() );	
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
