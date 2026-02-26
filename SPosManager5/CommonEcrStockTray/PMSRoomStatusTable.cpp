/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSCalendarTable.h"
#include "PMSInvoiceArray.h"
#include "PMSOptions.h"
#include "PMSTermLineEdit.h"
#include "RepCSV.h"
/**********************************************************************/
#include "PMSRoomStatusTable.h"
/**********************************************************************/

CPMSRoomStatusTable::CPMSRoomStatusTable()
{
	m_nExpectedBookingDayNumberCheck = -1;
}

/**********************************************************************/

bool CPMSRoomStatusTable::ValidateRoomAndAccountIdx( int nRoomIdx, int nAccountIdx )
{
	if ( ( nRoomIdx < 0 ) || ( nRoomIdx > MAX_PMS_ROOMS ) )
		return FALSE;

	if ( ( nAccountIdx < 0 ) || ( nAccountIdx >= MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CPMSRoomStatusTable::GetRoomInfo( int nRoomIdx, CPMSRoomStatusInfo& info )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == TRUE )
		info = m_arrayRooms[ nRoomIdx ];
}

/**********************************************************************/

void CPMSRoomStatusTable::SetRoomInfo( int nRoomIdx, CPMSRoomStatusInfo& info )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == TRUE )
		m_arrayRooms[ nRoomIdx ] = info;
}

/**********************************************************************/

void CPMSRoomStatusTable::GetAccountInfo( int nRoomIdx, int nAccountIdx, CPMSRoomAccountInfo& info )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, nAccountIdx ) == TRUE )
		info = m_arrayAccounts[ nRoomIdx ][ nAccountIdx ];
}

/**********************************************************************/

void CPMSRoomStatusTable::SetAccountInfo( int nRoomIdx, int nAccountIdx, CPMSRoomAccountInfo& info )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, nAccountIdx ) == TRUE )
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ] = info;
}

/**********************************************************************/

void CPMSRoomStatusTable::InitialiseRooms()
{
	int nMaxRoomIdx = DataManagerNonDb.PMSRoom.GetSize() - 1;

	if ( nMaxRoomIdx >= MAX_PMS_ROOMS )
		nMaxRoomIdx = MAX_PMS_ROOMS - 1;

	for ( int nRoomIdx = 0; nRoomIdx <= nMaxRoomIdx; nRoomIdx++ )
	{
		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

		m_arrayRooms[ nRoomIdx ].Reset();
		m_arrayRooms[ nRoomIdx ].SetRoomNo ( Room.GetRoomNo() );
		m_arrayRooms[ nRoomIdx ].SetRoomName ( Room.GetRoomName() );
		m_arrayRooms[ nRoomIdx ].SetRoomOnlyRate( Room.GetRoomOnlyRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomBBRate( Room.GetRoomBBRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomDBBRate( Room.GetRoomDBBRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomOnlySingleRate( Room.GetRoomOnlySingleRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomBBSingleRate( Room.GetRoomBBSingleRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomDBBSingleRate( Room.GetRoomDBBSingleRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomExtraBedRate( Room.GetRoomExtraBedRate() );
		m_arrayRooms[ nRoomIdx ].SetRoomExtraCotRate( Room.GetRoomExtraCotRate() );

		for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].Reset();
	}
}

/**********************************************************************/

void CPMSRoomStatusTable::LoadRoomAccountSettingsLive( CPMSBookingCSVRecord& BookingRecord, bool bAccounts )
{
	int nPartCount = BookingRecord.GetPartCount();
	if ( nPartCount == 0 )
		return;

	CPMSBookingPartInfo infoPart;
	BookingRecord.GetFinalPartInfo( infoPart );
	int nRoomNo = infoPart.GetRoomNo();
	
	int nRoomIdx;
	if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNo, nRoomIdx ) == FALSE )
		return;
	
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == FALSE )
		return;

	m_arrayRooms[ nRoomIdx ].SetBookingId( BookingRecord.GetBookingId() );
	m_arrayRooms[ nRoomIdx ].SetStartDay( infoPart.GetStartDay() );
	m_arrayRooms[ nRoomIdx ].SetNights( infoPart.GetNights() );
	m_arrayRooms[ nRoomIdx ].SetGuestCount( BookingRecord.GetGuests() );

	if ( TRUE == bAccounts )
		LoadRoomAccountSettingsInternal( nRoomIdx, BookingRecord );
}

/**********************************************************************/

void CPMSRoomStatusTable::LoadRoomAccountSettingsBuffer( CPMSBookingCSVRecord& BookingRecord, bool bBalance )
{
	for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
	{
		m_arrayAccounts[ MAX_PMS_ROOMS ][ nAccountIdx ].SetTotalDepositPaid( 0.0 );
		m_arrayAccounts[ MAX_PMS_ROOMS ][ nAccountIdx ].SetTotalPostCheckInPaid( 0.0 );
		m_arrayAccounts[ MAX_PMS_ROOMS ][ nAccountIdx ].SetTotalAllowance( 0.0 );
		m_arrayAccounts[ MAX_PMS_ROOMS ][ nAccountIdx ].SetTotalAccomSpend( 0.0 );
		m_arrayAccounts[ MAX_PMS_ROOMS ][ nAccountIdx ].SetTotalItemSpend( 0.0 );
		m_arrayAccounts[ MAX_PMS_ROOMS ][ nAccountIdx ].SetBalanceTime( CTime( 1971, 6, 17, 16, 0, 0, -1 ) );
	}

	int nPartCount = BookingRecord.GetPartCount();
	if ( nPartCount == 0 )
		return;

	CPMSBookingPartInfo infoPart;
	BookingRecord.GetFinalPartInfo( infoPart );

	CPMSRoomCSVRecord Room;
		
	int nDbRoomIdx;
	int nRoomNo = infoPart.GetRoomNo();
	if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNo, nDbRoomIdx ) == TRUE )
		DataManagerNonDb.PMSRoom.GetAt( nDbRoomIdx, Room );
		
	CString strRoomName = Room.GetRoomName();

	if ( strRoomName == "" )
		strRoomName.Format( "Room %d", nRoomNo );

	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomNo( nRoomNo );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomName( strRoomName );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomOnlyRate( Room.GetRoomOnlyRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomBBRate( Room.GetRoomBBRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomDBBRate( Room.GetRoomDBBRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomOnlySingleRate( Room.GetRoomOnlySingleRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomBBSingleRate( Room.GetRoomBBSingleRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomDBBSingleRate( Room.GetRoomDBBSingleRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomExtraBedRate( Room.GetRoomExtraBedRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetRoomExtraCotRate( Room.GetRoomExtraCotRate() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetBookingId( BookingRecord.GetBookingId() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetStartDay( infoPart.GetStartDay() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetNights( infoPart.GetNights() );
	m_arrayRooms[ MAX_PMS_ROOMS ].SetGuestCount( BookingRecord.GetGuests() );

	LoadRoomAccountSettingsInternal( MAX_PMS_ROOMS, BookingRecord );

	if ( TRUE == bBalance )
		UpdateAccountBalance( MAX_PMS_ROOMS, TRUE );
}
		
/**********************************************************************/

void CPMSRoomStatusTable::LoadRoomAccountSettingsInternal( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord )
{
	m_arrayRooms[ nRoomIdx ].SetAccountCount( BookingRecord.GetAccountCount() );
	m_arrayRooms[ nRoomIdx ].SetMultipleAccountsFlag( BookingRecord.GetMultipleAccountsFlag() );
		
	int nAccounts = m_arrayRooms[ nRoomIdx ].GetSafeAccountCount();
	int nCustomerId = BookingRecord.GetCustomerId();

	for ( int nAccountIdx = 0; nAccountIdx < nAccounts; nAccountIdx++ )
	{
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetCustomerId( nCustomerId + nAccountIdx );
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].ResetCustomerName();
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetGuestCount( BookingRecord.GetAccountCovers( nAccountIdx ) );
	}
}

/**********************************************************************/

void CPMSRoomStatusTable::UpdateAccountBalance( int nRoomIdx, bool bForceCheck )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == TRUE )
		for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
			UpdateAccountBalance( nRoomIdx, nAccountIdx, bForceCheck );
}

/**********************************************************************/

bool CPMSRoomStatusTable::UpdateAccountBalance( int nRoomIdx, int nAccountIdx, bool bForceCheck )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, nAccountIdx ) == FALSE )
		return FALSE;

	int nBookingId = m_arrayRooms[ nRoomIdx ].GetBookingId();

	CPMSBookingCSVRecord BookingRecord;
	BookingRecord.SetBookingId( nBookingId );

	if ( 0 == nAccountIdx )
	{
		int nBookingIdx;
		if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );
	}

	double dAccomSpend = 0.0, dExtraBedSpend = 0.0, dItemSpend = 0.0, dDepositPaid = 0.0, dPostCheckInPaid = 0.0, dAllowance = 0.0;
	CTime timeCheck = m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].GetBalanceTime();
	
	int nResult = GetBalanceForBooking( BookingRecord, nAccountIdx, dAccomSpend, dExtraBedSpend, dItemSpend, dDepositPaid, dPostCheckInPaid, dAllowance, timeCheck, bForceCheck );

	if ( nResult != 0 )
	{
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalAccomSpend( dAccomSpend );
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalExtraBedSpend( dExtraBedSpend );
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalDepositPaid( dDepositPaid );
		
		if ( nResult == 2 )
		{
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalItemSpend( dItemSpend );
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalPostCheckInPaid( dPostCheckInPaid );
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalAllowance( dAllowance );

			if ( FALSE == bForceCheck )
				m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetBalanceTime ( timeCheck );
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CPMSRoomStatusTable::UpdateAccountBalance( int nRoomIdx, int nAccountIdx, CPMSBookingCSVRecord& BookingRecord )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, nAccountIdx ) == FALSE )
		return FALSE;

	double dAccomSpend = 0.0, dExtraBedSpend = 0.0, dItemSpend = 0.0, dDepositPaid = 0.0, dPostCheckInPaid = 0.0, dAllowance = 0.0;
	CTime timeCheck = m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].GetBalanceTime();
	
	int nResult = GetBalanceForBooking( BookingRecord, nAccountIdx, dAccomSpend, dExtraBedSpend, dItemSpend, dDepositPaid, dPostCheckInPaid, dAllowance, timeCheck, TRUE );

	if ( nResult != 0 )
	{
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalAccomSpend( dAccomSpend );
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalExtraBedSpend( dExtraBedSpend );
		m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalDepositPaid( dDepositPaid );
		
		if ( nResult == 2 )
		{
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalItemSpend( dItemSpend );
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalPostCheckInPaid( dPostCheckInPaid );
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetTotalAllowance( dAllowance );
			m_arrayAccounts[ nRoomIdx ][ nAccountIdx ].SetBalanceTime ( timeCheck );
		}
	}

	return TRUE;
}

/**********************************************************************/

int CPMSRoomStatusTable::GetBalanceForBooking( CPMSBookingCSVRecord& BookingRecord, int nAccountIdx, double& dAccomSpend, double& dExtraBedSpend, double& dItemSpend, double& dDepositPaid, double& dPostCheckInPaid, double& dAllowance, CTime& timeCheck, bool bForceCheck )
{
	dAccomSpend = 0.0;
	dExtraBedSpend = 0.0;
	dItemSpend = 0.0;
	dDepositPaid = 0.0;
	dPostCheckInPaid = 0.0;
	dAllowance = 0.0;

	CString strTransFile;
	strTransFile.Format( "%s\\%d.%d",
		PMSOptions.GetPMSFolderSales(),
		BookingRecord.GetBookingId(),
		nAccountIdx + 1 );

	if ( 0 == nAccountIdx )
	{
		int nStartDay = 0;
		int nStayLength = 0;
		BookingRecord.GetAccomodationSpend( dAccomSpend, dExtraBedSpend, nStartDay, nStayLength );
		dDepositPaid = BookingRecord.CalculateDepositValue();
	}

	if ( ::FileExists( strTransFile ) == FALSE )
		return 2;

	CFileStatus FileStatus;

	if ( FALSE == bForceCheck )
	{
		CFile::GetStatus( strTransFile, FileStatus );
		if ( FileStatus.m_mtime == timeCheck )
			return 1;
	}

	int nDinnerTaxBand = 0;
	bool bDBBCredit = FALSE;

	if ( BookingRecord.GetRoomRateType() == PMS_BOOKING_RATE_DBB )
		if ( 0 == nAccountIdx )
			if ( BookingRecord.CheckDinnerSettings( nDinnerTaxBand ) == TRUE )
				bDBBCredit = TRUE;

	if ( TRUE == bDBBCredit )
	{
		CPMSInvoiceArray InvoiceArray;
		InvoiceArray.BuildInvoiceArray( BookingRecord, 1, PMS_INVOICETYPE_ITEM );

		CReportConsolidationArray<CConsolidatedDoubleByString> arraySales;
		InvoiceArray.PrepareDBBCreditsItem( BookingRecord, nDinnerTaxBand, dItemSpend, dPostCheckInPaid, dAllowance, arraySales, FALSE );

		for ( int n = 0; n < arraySales.GetSize(); n++ )
		{
			CConsolidatedDoubleByString infoSales;
			arraySales.GetAt( n, infoSales );

			double dDinnerSales = infoSales.m_dVal;
			double dAllowance = BookingRecord.GetDinnerAllowance() * BookingRecord.GetGuests();

			if ( dDinnerSales >= dAllowance )
				dItemSpend -= dAllowance;
			else if ( dDinnerSales > 0.0 )
				dItemSpend -= dDinnerSales;
		}
	}
	else
	{
		CSSFile fileTrans;
		if ( fileTrans.Open( strTransFile, "rb" ) == FALSE )
			return 0;

		CString strBuffer;
		while( fileTrans.ReadString( strBuffer ) == TRUE )
		{
			if ( strBuffer.Left(6) == "$ITEM," )
			{
				CCSVPMSTermLine csvSale( strBuffer );
				dItemSpend += csvSale.GetItemLineValueReport();
			}
			else if ( strBuffer.Left(7) == "$IDISC," )
			{
				CCSVPMSTermLine csvSale( strBuffer );
				double dLineValue = ( (double) csvSale.GetItemDiscountAmount() );
				dItemSpend += ( dLineValue / 100.0 );
			}
			else if ( ( strBuffer.Left(7) == "$SDISC," ) || ( strBuffer.Left(7) == "$LDISC," ) )
			{
				CCSVPMSTermLine csvSale( strBuffer );
				double dLineValue = ( (double) csvSale.GetSubtotalDiscountAmount() );
				dItemSpend += ( dLineValue / 100.0 );
			}
			else if ( ( strBuffer.Left(11) == "$ITEM_EDIT," ) || ( strBuffer.Left(13) == "$REFUND_EDIT," ) )
			{
				CPMSTermLineEdit csvSale( strBuffer );
				double dLineValue = ( (double) csvSale.GetItemLinePrice() );
				dItemSpend += ( dLineValue / 100.0 );
			}
			else if ( strBuffer.Left(16) == "$ALLOWANCE_EDIT," )
			{
				CPMSTermLineEdit csvSale( strBuffer );
				double dLineValue = ( (double) csvSale.GetItemLinePrice() );
				dAllowance -= ( dLineValue / 100.0 );
			}
			else if ( strBuffer.Left(12) == "$IDISC_EDIT," )
			{
				CPMSTermLineEdit csvSale( strBuffer );
				double dLineValue = ( (double) csvSale.GetItemDiscountAmount() );
				dItemSpend += ( dLineValue / 100.0 );
			}
			else if ( ( strBuffer.Left(12) == "$SDISC_EDIT," ) || ( strBuffer.Left(12) == "$LDISC_EDIT," ) )
			{
				CPMSTermLineEdit csvSale( strBuffer );
				double dLineValue = ( (double) csvSale.GetSubtotalDiscountAmount() );
				dItemSpend += ( dLineValue / 100.0 );
			}
			else if ( strBuffer.Left(10) == "$PAYM_EDIT" )
			{
				CPMSTermLineEdit csvSale( strBuffer );
				double dPayment = ( (double) csvSale.GetPaymentAmount() );
				dPostCheckInPaid += ( dPayment / 100.0 );
			}
		}
	}

	timeCheck = FileStatus.m_mtime;
	return 2;
}

/**********************************************************************/

void CPMSRoomStatusTable::CheckIn( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == TRUE )
	{
		CPMSBookingPartInfo infoPart;
		BookingRecord.GetFinalPartInfo( infoPart );

		m_arrayRooms[nRoomIdx].SetBookingId( BookingRecord.GetBookingId() );
		m_arrayRooms[nRoomIdx].SetAccountCount(1);
		m_arrayRooms[nRoomIdx].SetStartDay( infoPart.GetStartDay() );
		m_arrayRooms[nRoomIdx].SetNights( infoPart.GetNights() );
		m_arrayRooms[nRoomIdx].SetGuestCount( BookingRecord.GetGuests() );
		m_arrayRooms[nRoomIdx].SetMultipleAccountsFlag ( BookingRecord.GetMultipleAccountsFlag() );

		CTime timeBalance = CTime ( 1980, 1, 1, 0, 0, 0, -1 );

		for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
		{
			if ( 0 == nAccountIdx )
			{
				m_arrayAccounts[nRoomIdx][nAccountIdx].SetGuestCount( BookingRecord.GetGuests() );
				m_arrayAccounts[nRoomIdx][nAccountIdx].SetCustomerId( BookingRecord.GetCustomerId() );
			}
			else
			{
				m_arrayAccounts[nRoomIdx][nAccountIdx].SetGuestCount(0);
				m_arrayAccounts[nRoomIdx][nAccountIdx].SetCustomerId(0);
			}

			m_arrayAccounts[nRoomIdx][nAccountIdx].ResetCustomerName();
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetBalanceTime( timeBalance );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalDepositPaid( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalPostCheckInPaid( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalAccomSpend( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalItemSpend( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalAllowance( 0.0 );
		}
	}
}

/**********************************************************************/

void CPMSRoomStatusTable::CheckOut( int nRoomIdx )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == TRUE )
	{
		m_arrayRooms[nRoomIdx].SetBookingId( 0 );
		m_arrayRooms[nRoomIdx].SetAccountCount(1);
		m_arrayRooms[nRoomIdx].SetStartDay( 0 );
		m_arrayRooms[nRoomIdx].SetNights( 1 );
		m_arrayRooms[nRoomIdx].SetMultipleAccountsFlag ( FALSE );

		CTime timeBalance = CTime ( 1980, 1, 1, 0, 0, 0, -1 );

		for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
		{
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetGuestCount(0);
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetCustomerId(0);
			m_arrayAccounts[nRoomIdx][nAccountIdx].ResetCustomerName();
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetBalanceTime( timeBalance );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalDepositPaid( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalPostCheckInPaid( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalAccomSpend( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalItemSpend( 0.0 );
			m_arrayAccounts[nRoomIdx][nAccountIdx].SetTotalAllowance( 0.0 );
		}
	}
}

/**********************************************************************/

void CPMSRoomStatusTable::InvalidateRoomStatus()
{
	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		CPMSRoomStatusInfo infoRoom;
		GetRoomInfo( nRoomIdx, infoRoom );
		infoRoom.SetRoomStatusValidFlag( FALSE );
		SetRoomInfo( nRoomIdx, infoRoom );
	}
}

/**********************************************************************/

bool CPMSRoomStatusTable::QueryRoomStatus( int nRoomIdx, CPMSRoomStatusInfo& infoRoom )
{
	if ( ( nRoomIdx < 0 ) || ( nRoomIdx > MAX_PMS_ROOMS ) )
		return FALSE;

	GetRoomInfo( nRoomIdx, infoRoom );
		
	if ( infoRoom.GetRoomStatusValidFlag() == FALSE )
		UpdateExpectedBookingId( nRoomIdx );

	GetRoomInfo( nRoomIdx, infoRoom );
	return infoRoom.GetRoomStatusValidFlag();
}

/**********************************************************************/

int CPMSRoomStatusTable::GetExpectedBookingId( int nRoomIdx )
{
	CPMSRoomStatusInfo infoRoom;
	if ( QueryRoomStatus( nRoomIdx, infoRoom ) == TRUE )
		return infoRoom.GetExpectedBookingId();
	else
		return 0;
}

/**********************************************************************/

const char* CPMSRoomStatusTable::GetExpectedBillingName( int nRoomIdx )
{
	CPMSRoomStatusInfo infoRoom;
	if ( QueryRoomStatus( nRoomIdx, infoRoom ) == TRUE )
		m_strExpectedBillingName = infoRoom.GetExpectedBillingName();
	else
		m_strExpectedBillingName = "";

	return m_strExpectedBillingName;
}

/**********************************************************************/

const char* CPMSRoomStatusTable::GetExpectedGuestName( int nRoomIdx )
{
	CPMSRoomStatusInfo infoRoom;
	if ( QueryRoomStatus( nRoomIdx, infoRoom ) == TRUE )
		m_strExpectedGuestName = infoRoom.GetExpectedGuestName();
	else
		m_strExpectedGuestName = "";

	return m_strExpectedGuestName;
}

/**********************************************************************/

int CPMSRoomStatusTable::GetAvailableNights( int nRoomIdx )
{
	CPMSRoomStatusInfo infoRoom;
	if ( QueryRoomStatus( nRoomIdx, infoRoom ) == TRUE )
		return infoRoom.GetAvailableNights();
	else
		return 0;
}

/**********************************************************************/

bool CPMSRoomStatusTable::GetClosureFlag( int nRoomIdx )
{
	CPMSRoomStatusInfo infoRoom;
	if ( QueryRoomStatus( nRoomIdx, infoRoom ) == TRUE )
		return infoRoom.GetClosureFlag();
	else
		return FALSE;
}

/**********************************************************************/

int CPMSRoomStatusTable::GetClosureId( int nRoomIdx )
{
	CPMSRoomStatusInfo infoRoom;
	if ( QueryRoomStatus( nRoomIdx, infoRoom ) == TRUE )
		return infoRoom.GetClosureId();
	else
		return 0;
}

/**********************************************************************/

void CPMSRoomStatusTable::UpdateExpectedBookingId( int nRoomIdx )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == FALSE )
		return;

	m_arrayRooms[ nRoomIdx ].SetExpectedBookingId(0);
	m_arrayRooms[ nRoomIdx ].SetExpectedBillingName( "" );
	m_arrayRooms[ nRoomIdx ].SetExpectedGuestName( "" );
	m_arrayRooms[ nRoomIdx ].SetAvailableNights(0);
	m_arrayRooms[ nRoomIdx ].SetClosureFlag( FALSE );

	CPMSBookingQueryInfo Query;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = PMSBooking::Nights.Max;
	Query.m_nRoomNo = m_arrayRooms[ nRoomIdx ].GetRoomNo();
	Query.m_nStartDay = PMSOptions.GetPMSDayNumber();

	CPMSBookingResponseInfo Response;
	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	int nId = Response.m_nBookingClashId;
	int nPart = Response.m_nBookingClashPart;

	if ( nId != 0 )
	{
		CPMSCalendarBooking CalendarBooking;
		PMSCalendarTable.GetAt( Response.m_nBookingClashArrayIdx, CalendarBooking );

		if ( PMS_BOOKING_STATUS_CLOSED  == CalendarBooking.m_nStatus )
		{
			m_arrayRooms[ nRoomIdx ].SetClosureFlag( TRUE );
			m_arrayRooms[ nRoomIdx ].SetClosureId( nId );
		}
		else
		{
			int nBookingIdx;
			CPMSBookingCSVRecord BookingRecord;
			if ( DataManagerNonDb.PMSBooking.FindBookingById( nId, nBookingIdx ) == TRUE )
			{
				DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

				if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
				{
					m_arrayRooms[ nRoomIdx ].SetExpectedBookingId( nId );

					int nCustomerIdx;
					int nCustomerId = BookingRecord.GetCustomerId();
					if ( DataManagerNonDb.PMSCustomer.FindCustomerById( nCustomerId, nCustomerIdx ) == TRUE )
					{
						CPMSCustomerCSVRecord Customer;
						DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, Customer );
						m_arrayRooms[ nRoomIdx ].SetExpectedBillingName( Customer.GetBillingName() );
						m_arrayRooms[ nRoomIdx ].SetExpectedGuestName( Customer.GetGuestName() );
					}
				}
			}
		}
	}
	else
	{
		int nAvailable = Response.m_nMaxNights;
		if ( nAvailable > PMSBooking::Nights.Max )
			nAvailable = PMSBooking::Nights.Max;

		m_arrayRooms[ nRoomIdx ].SetAvailableNights( nAvailable );
	}

	m_arrayRooms[ nRoomIdx ].SetRoomStatusValidFlag( TRUE );
}

/**********************************************************************/

const char* CPMSRoomStatusTable::GetDownloadFilename( int nRoomIdx, int nAccountIdx )
{
	m_strDownloadFilename = "";

	if ( ( nRoomIdx < 0 ) || ( nRoomIdx >= MAX_PMS_ROOMS ) )
		return m_strDownloadFilename;

	if ( ( nAccountIdx < 0 ) || ( nAccountIdx >= MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_strDownloadFilename;

	m_strDownloadFilename.Format( "%s\\Room%4.4d.%d",
		PMSOptions.GetPMSFolderDownload(),
		m_arrayRooms[nRoomIdx].GetRoomNo(),
		nAccountIdx + 1 );

	return m_strDownloadFilename;
}

/**********************************************************************/

void CPMSRoomStatusTable::WriteDownloadFileRoom( int nRoomIdx )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, 0 ) == TRUE )
		for ( int nAccountIdx = 0; nAccountIdx < MAX_PMS_ACCOUNTS_PER_ROOM; nAccountIdx++ )
			WriteDownloadFileAccount( nRoomIdx, nAccountIdx );
}

/**********************************************************************/

void CPMSRoomStatusTable::WriteDownloadFileAccount( int nRoomIdx, int nAccountIdx )
{
	if ( ValidateRoomAndAccountIdx( nRoomIdx, nAccountIdx ) == FALSE )
		return;
	
	bool bActive = ( m_arrayRooms[nRoomIdx].GetBookingId() != 0 );
	bActive &= ( nAccountIdx < m_arrayRooms[ nRoomIdx ].GetSafeAccountCount() );

	CString strDownloadFilename = GetDownloadFilename( nRoomIdx, nAccountIdx );

	CString strName = m_arrayAccounts[nRoomIdx][nAccountIdx].GetGuestName();
	if ( strName == "" )
		strName = "Guest";

	CSSFile fileDownload;
	fileDownload.Open( strDownloadFilename, "wb" );

	CCSV csv( ',', '\'', FALSE, FALSE, FALSE );
	csv.Add( bActive );
	csv.Add( m_arrayRooms[nRoomIdx].GetRoomNo() );
	csv.Add( strName );
	csv.Add( m_arrayAccounts[nRoomIdx][nAccountIdx].GetGuestCount() );
	csv.Add( 0 );
	csv.Add( 999999 );
	csv.Add( m_arrayRooms[nRoomIdx].GetBookingId() );
	csv.Add( "*" );
	csv.Add( "*" );
	csv.Add( "*" );
	fileDownload.WriteLine( csv.GetLine() );
}

/**********************************************************************/
#endif
/**********************************************************************/
