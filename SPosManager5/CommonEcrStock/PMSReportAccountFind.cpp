/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "CCSVReportLine.h"
#include "PMSRoomStatusTable.h"
#include "PriceHelpers.h"
#include "RepSpawn.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSReportAccountFind.h"
/**********************************************************************/

CPMSReportAccountFind::CPMSReportAccountFind( CArray<CPMSAccountFindInfo,CPMSAccountFindInfo>& arrayInfo, CArray<int,int>& arrayFilter, bool bUseFilter ) : 
	m_arrayInfoList( arrayInfo ), 
	m_arrayFilter( arrayFilter )
{
	m_bUseFilter = bUseFilter;

	CFilenameUpdater FnUp( SysFiles::PMSAccountFindPrm );
	m_strParamsFilename = FnUp.GetFilenameToUse();
	m_nSortType = 0;
	m_bRoom = FALSE;
	m_bBalance = FALSE;
	m_bLines = FALSE;
	m_bInvoiceNum = FALSE;
}

/**********************************************************************/

void CPMSReportAccountFind::SetSortKey( CPMSAccountFindInfo& info, int nIdx )
{
	CString strRoomNo = "";
	if ( TRUE == m_bRoom )
	{
		strRoomNo.Format( "%4.4d", info.m_nRoomNo );
		info.m_strGroupKey = strRoomNo;
	}
	
	switch( m_nSortType )
	{
	case 1:
		info.m_strSortKey.Format( "%s%8.8d%6.6d", 
			(const char*) strRoomNo,
			info.m_nBookingPartStartDay + info.m_nBookingPartNights, 
			nIdx );
		break;

	case 2:
		info.m_strSortKey.Format( "%s%8.8d%6.6d", 
			(const char*) strRoomNo,
			info.m_nBookingId, 
			nIdx );
		break;

	case 3:
		{
			if ( 0 == info.m_nInvoiceNum )
			{
				info.m_strSortKey.Format( "%s99999999%6.6d", 
					(const char*) strRoomNo,
					nIdx );
			}
			else
			{
				info.m_strSortKey.Format( "%s%8.8d%6.6d", 
					(const char*) strRoomNo,
					info.m_nInvoiceNum, 
					nIdx );
			}
		}
		break;

	case 0:
	default:
		info.m_strSortKey.Format( "%s%8.8d%6.6d", 
			(const char*) strRoomNo,
			info.m_nBookingPartStartDay, 
			nIdx );
		break;
	}

	if ( FALSE == m_bRoom )
		info.m_strGroupKey = info.m_strSortKey.Left(8);
}

/**********************************************************************/

bool CPMSReportAccountFind::FindAccount( CPMSAccountFindInfo& infoAccount, int nIdx, int& nPos )
{
	CString strKey = infoAccount.m_strSortKey;

	int nStart = 0;
	int nEnd = m_arrayInfoSorted.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		CPMSAccountFindInfo infoArray = m_arrayInfoSorted.GetAt( nPos );
		CString strArrayKey = infoArray.m_strSortKey;

		if ( strKey < strArrayKey )
			nEnd = nPos - 1;
		else if ( strKey > strArrayKey )
			nStart = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

void CPMSReportAccountFind::SortAccounts()
{
	if ( FALSE == m_bUseFilter )
	{
		for ( int nIdx = 0; nIdx < m_arrayInfoList.GetSize(); nIdx++ )
		{
			CPMSAccountFindInfo infoAccount = m_arrayInfoList.GetAt( nIdx );
			SetSortKey( infoAccount, nIdx );
		
			int nPos;
			if ( FindAccount( infoAccount, nIdx, nPos ) == FALSE )
				m_arrayInfoSorted.InsertAt( nPos, infoAccount );
		}
	}
	else
	{
		for ( int nFilterIdx = 0; nFilterIdx < m_arrayFilter.GetSize(); nFilterIdx++ )
		{
			int nIdx = m_arrayFilter.GetAt( nFilterIdx );

			if ( ( nIdx >= 0 ) && ( nIdx < m_arrayInfoList.GetSize() ) )
			{
				CPMSAccountFindInfo infoAccount = m_arrayInfoList.GetAt( nIdx );
				SetSortKey( infoAccount, nIdx );
		
				int nPos;
				if ( FindAccount( infoAccount, nIdx, nPos ) == FALSE )
					m_arrayInfoSorted.InsertAt( nPos, infoAccount );
			}
		}
	}
}

/**********************************************************************/

void CPMSReportAccountFind::CreateReport()
{
	m_arrayOptionalColumns.RemoveAll();

	if ( FALSE == m_bRoom )
	{
		switch( m_nSortType )
		{
		case 2:
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_BOOKINGREF );
			if ( TRUE == m_bInvoiceNum ) m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_INVOICENUM );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKIN );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_NIGHTS );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKOUT );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNO );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNAME );
			break;

		case 3:
			if ( TRUE == m_bInvoiceNum ) m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_INVOICENUM );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_BOOKINGREF );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKIN );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_NIGHTS );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKOUT );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNO );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNAME );
			break;

		case 0:
		case 1:
		default:
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKIN );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_NIGHTS );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKOUT );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_BOOKINGREF );
			if ( TRUE == m_bInvoiceNum ) m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_INVOICENUM );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNO );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNAME );
			break;
		}
	}
	else
	{
		switch( m_nSortType )
		{
		case 2:
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNO );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNAME );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_BOOKINGREF );
			if ( TRUE == m_bInvoiceNum ) m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_INVOICENUM );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKIN );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_NIGHTS );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKOUT );
			break;

		case 3:
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNO );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNAME );
			if ( TRUE == m_bInvoiceNum ) m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_INVOICENUM );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_BOOKINGREF );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKIN );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_NIGHTS );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKOUT );
			break;

		case 0:
		case 1:
		default:
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNO );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_ROOMNAME );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKIN );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_NIGHTS );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_CHECKOUT );
			m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_BOOKINGREF );
			if ( TRUE == m_bInvoiceNum ) m_arrayOptionalColumns.Add( PMS_ACCOUNTFIND_COLUMN_INVOICENUM );
			break;
		}
	}

	CString strKey;
	strKey.Format( "%d%d%d", m_bRoom, m_nSortType, m_bBalance );
	
	if ( CreateReportInternal() == TRUE )
	{
		CReportManager ReportManager;
		ReportManager.DisplayReport ( Super.ReportFilename(), m_strParamsFilename, strKey, NULL );
	}
	else
		Prompter.Error( "Unable to create report." );
}

/**********************************************************************/
void CPMSReportAccountFind::AddColumnBookingRef(){ m_ReportFile.AddColumn ( "Reference", TA_LEFT, 200 ); }
void CPMSReportAccountFind::AddColumnInvoiceNum(){ m_ReportFile.AddColumn ( "Invoice", TA_LEFT, 200 ); }
void CPMSReportAccountFind::AddColumnCheckIn(){ m_ReportFile.AddColumn ( "Check In", TA_LEFT, 220 ); }
void CPMSReportAccountFind::AddColumnNights(){ m_ReportFile.AddColumn ( "Nights", TA_LEFT, 150 ); }
void CPMSReportAccountFind::AddColumnCheckOut(){ m_ReportFile.AddColumn ( "Check Out", TA_LEFT, 220 ); }
void CPMSReportAccountFind::AddColumnRoomNo(){ m_ReportFile.AddColumn ( "No", TA_LEFT, 120 ); }
void CPMSReportAccountFind::AddColumnRoomName(){ m_ReportFile.AddColumn ( "Room", TA_LEFT, 250 ); }
void CPMSReportAccountFind::AddColumnGuests(){ m_ReportFile.AddColumn ( "Guests", TA_LEFT, 150 ); }
void CPMSReportAccountFind::AddColumnGuestName(){ m_ReportFile.AddColumn ( "Guest Name", TA_LEFT, 290 ); }
void CPMSReportAccountFind::AddColumnBillingName(){ m_ReportFile.AddColumn ( "Billing Name", TA_LEFT, 290 ); }
void CPMSReportAccountFind::AddColumnBalance(){ m_ReportFile.AddColumn ( "Balance", TA_RIGHT, 170 ); }
void CPMSReportAccountFind::AddColumnCR(){ m_ReportFile.AddColumn ( "", TA_LEFT, 100 ); }
/**********************************************************************/

bool CPMSReportAccountFind::CreateReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;
	
	m_ReportFile.SetStyle1 ( "Booking List Report" );

	for ( int n = 0; n < m_arrayOptionalColumns.GetSize(); n++ )
	{
		switch( m_arrayOptionalColumns.GetAt(n) )
		{
		case PMS_ACCOUNTFIND_COLUMN_BOOKINGREF:		
			AddColumnBookingRef();		
			break;

		case PMS_ACCOUNTFIND_COLUMN_INVOICENUM:		
			if ( TRUE == m_bInvoiceNum ) 
				AddColumnInvoiceNum();		
			break;

		case PMS_ACCOUNTFIND_COLUMN_CHECKIN:		
			AddColumnCheckIn();			
			break;

		case PMS_ACCOUNTFIND_COLUMN_NIGHTS:			
			AddColumnNights();			
			break;

		case PMS_ACCOUNTFIND_COLUMN_CHECKOUT:		
			AddColumnCheckOut();		
			break;

		case PMS_ACCOUNTFIND_COLUMN_ROOMNO:			
			AddColumnRoomNo();			
			break;

		case PMS_ACCOUNTFIND_COLUMN_ROOMNAME:		
			AddColumnRoomName();		
			break;
		}
	}

	AddColumnGuests();
	AddColumnBillingName();
	AddColumnGuestName();

	if ( TRUE == m_bBalance )
	{
		AddColumnBalance();
		AddColumnCR();
	}

	StatusProgress.Lock( TRUE, "Creating Report" );
	
	CString strCurrentGroupKey = "";

	int nTotalNights = 0;
	int nTotalGuests = 0;
	double dTotalBalance = 0.0;

	int nSize = m_arrayInfoSorted.GetSize();
	for ( int nIdx = 0; nIdx < nSize; nIdx++ )
	{
		StatusProgress.SetPos( nIdx, nSize );

		CPMSAccountFindInfo infoAccount = m_arrayInfoSorted.GetAt( nIdx );
		
		int nBookingId = infoAccount.m_nBookingId;
		int nInvoiceNum = infoAccount.m_nInvoiceNum;
		
		int nBookingIdx;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
			continue;

		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

		COleDateTime dateCheckIn( 1980, 1, 1, 0, 0, 0 );
		COleDateTime dateCheckOut( 1980, 1, 1, 0, 0, 0 );

		int nCheckInDate = infoAccount.m_nBookingPartStartDay;
		GetOleDateSince2011( nCheckInDate, dateCheckIn );

		int nCheckOutDate = infoAccount.m_nBookingPartStartDay + infoAccount.m_nBookingPartNights;
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

		CString strBookingId;
		strBookingId.Format( "%8.8d", nBookingId );

		CString strInvoiceNum = "";
		if ( nInvoiceNum != 0 )
			strInvoiceNum.Format( "%8.8d", nInvoiceNum );

		int nColumnSkip = ( /*m_bRoom ? 2 :*/ 0 );

		if ( infoAccount.m_strGroupKey != strCurrentGroupKey )
		{
			nColumnSkip = 0;

			if ( TRUE == m_bLines )
				if ( strCurrentGroupKey != "" )
					m_ReportFile.WriteLine( "<LI>" );
		}

		strCurrentGroupKey = infoAccount.m_strGroupKey;

		CCSVReportLine ReportLine;
	
		for ( int n = 0; n < m_arrayOptionalColumns.GetSize(); n++ )
		{
			if ( n < nColumnSkip )
				ReportLine.AppendString( "" );
			else
			{
				switch( m_arrayOptionalColumns.GetAt(n) )
				{
				case PMS_ACCOUNTFIND_COLUMN_BOOKINGREF:		
					ReportLine.AppendString( strBookingId );					
					break;

				case PMS_ACCOUNTFIND_COLUMN_INVOICENUM:		
					if ( TRUE == m_bInvoiceNum )
						ReportLine.AppendString( strInvoiceNum );					
					break;

				case PMS_ACCOUNTFIND_COLUMN_CHECKIN:		
					ReportLine.AppendString( strDateCheckIn );					
					break;

				case PMS_ACCOUNTFIND_COLUMN_NIGHTS:			
					ReportLine.AppendInt( infoAccount.m_nBookingPartNights );	
					break;

				case PMS_ACCOUNTFIND_COLUMN_CHECKOUT:		
					ReportLine.AppendString( strDateCheckOut );					
					break;

				case PMS_ACCOUNTFIND_COLUMN_ROOMNO:			
					ReportLine.AppendInt( infoAccount.m_nRoomNo );				
					break;

				case PMS_ACCOUNTFIND_COLUMN_ROOMNAME:		
					ReportLine.AppendString( infoAccount.m_strRoomName );		
					break;
				}
			}
		}

		ReportLine.AppendInt( infoAccount.m_nGuests );		

		nTotalNights += infoAccount.m_nBookingPartNights;
		nTotalGuests += infoAccount.m_nGuests;
			
		PMSRoomStatusTable.LoadRoomAccountSettingsBuffer( BookingRecord, TRUE );

		CPMSRoomStatusInfo infoRoom;
		PMSRoomStatusTable.GetRoomInfo( MAX_PMS_ROOMS, infoRoom );

		for ( int nAccountIdx = 0; nAccountIdx < infoRoom.GetSafeAccountCount(); nAccountIdx++ )
		{
			if ( nAccountIdx != 0 )
			{
				ReportLine.RemoveAll();
				ReportLine.AppendBlanks( m_arrayOptionalColumns.GetSize() + 1 );
			}

			CPMSRoomAccountInfo infoAccount;
			PMSRoomStatusTable.GetAccountInfo( MAX_PMS_ROOMS, nAccountIdx, infoAccount );

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
	
	StatusProgress.Unlock();
		
	if ( TRUE == m_bLines )
		if ( strCurrentGroupKey != "" )
			m_ReportFile.WriteLine( "<LI>" );

	if ( ( FALSE == m_bLines ) || ( strCurrentGroupKey == "" ) )
	{
		CCSVReportLine ReportLine;

		for ( int n = 0; n < m_arrayOptionalColumns.GetSize(); n++ )
		{
			if ( m_arrayOptionalColumns[n] == PMS_ACCOUNTFIND_COLUMN_NIGHTS )
				ReportLine.AppendString( "<LI>" );
			else
				ReportLine.AppendString( "" );
		}

		ReportLine.AppendString( "<LI>" );
		
		if ( TRUE == m_bBalance )
		{
			ReportLine.AppendBlanks( 2 );
			ReportLine.AppendString( "<LI>" );
		}

		m_ReportFile.WriteLine( ReportLine.GetLine() );
	}

	CCSVReportLine ReportLine;
	
	for ( int n = 0; n < m_arrayOptionalColumns.GetSize(); n++ )
	{
		if ( m_arrayOptionalColumns[n] == PMS_ACCOUNTFIND_COLUMN_NIGHTS )
		{
			ReportLine.AppendInt( nTotalNights );

			if ( n > 0 )
				ReportLine.SetAt( n - 1, "Total" );
		}
		else
			ReportLine.AppendString( "" );
	}

	ReportLine.AppendInt( nTotalGuests );
	
	if ( TRUE == m_bBalance )
	{
		ReportLine.AppendBlanks( 2 );
			
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
	m_ReportFile.WriteLine( "" );
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
