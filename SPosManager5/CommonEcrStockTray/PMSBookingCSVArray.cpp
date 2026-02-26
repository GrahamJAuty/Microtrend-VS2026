/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "FileRemover.h"
#include "MessageLogger.h"
#include "PMSCalendarTable.h"
#include "PMSDefines.h"
#include "PMSInvoiceNum.h"
#include "PMSOptions.h"
#include "PMSRoomCSVArray.h"
#include "PMSRoomStatusTable.h"
#include "PMSZeroNightStayArray.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PMSBookingCSVArray.h"
/**********************************************************************/

CPMSBookingPartInfo::CPMSBookingPartInfo()
{
	m_nRoomNo = 1;
	m_nStartDay = 0;
	m_nNights = 0;
}

/**********************************************************************/

void CPMSBookingPartInfo::SetRoomNo( int n )
{
	if ( ( n >= PMSRoom::RoomNo.Min ) && ( n <= PMSRoom::RoomNo.Max ) )
		m_nRoomNo = n;
}

/**********************************************************************/

void CPMSBookingPartInfo::SetStartDay( int n )
{
	if ( ( n >= PMSBooking::StartDay.Min ) && ( n <= PMSBooking::StartDay.Max ) )
		m_nStartDay = n;
}

/**********************************************************************/

void CPMSBookingPartInfo::SetNights( int n )
{
	if ( ( n >= PMSBooking::Nights.Min ) && ( n <= PMSBooking::Nights.Max ) )
		m_nNights = n;
}

/**********************************************************************/

CPMSDepositInfo::CPMSDepositInfo()
{
	m_nPMSDay = 0;
	m_nHour = 0;
	m_nMinute = 0;
	m_bRefund = FALSE;
	m_nPaymentType = 1;
	m_dValue = 0.0;
}

/**********************************************************************/

void CPMSDepositInfo::SetPMSDay( int n )
{
	if ( ( n >= PMSBooking::DepositDay.Min ) && ( n <= PMSBooking::DepositDay.Max ) )
		m_nPMSDay = n;
}

/**********************************************************************/

void CPMSDepositInfo::SetHour( int n )
{
	if ( ( n >= PMSBooking::DepositHour.Min ) && ( n <= PMSBooking::DepositHour.Max ) )
		m_nHour = n;
}

/**********************************************************************/

void CPMSDepositInfo::SetMinute( int n )
{
	if ( ( n >= PMSBooking::DepositMinute.Min ) && ( n <= PMSBooking::DepositMinute.Max ) )
		m_nMinute = n;
}

/**********************************************************************/

void CPMSDepositInfo::SetRefundFlag( bool b )
{
	m_bRefund = b;
}

/**********************************************************************/

void CPMSDepositInfo::SetPaymentType( int n )
{
	if ( ( n >= PMSBooking::DepositType.Min ) && ( n <= PMSBooking::DepositType.Max ) )
		m_nPaymentType = n;
}

/**********************************************************************/

void CPMSDepositInfo::SetValue( double d )
{
	if ( ( d >= PMSBooking::DepositValue.Min ) && ( d <= PMSBooking::DepositValue.Max ) )
		m_dValue = d;
}

/**********************************************************************/

void CPMSDepositInfo::SetIndex( int n )
{
	m_nIndex = n;
}

/**********************************************************************/

int CPMSDepositInfo::Compare( CPMSDepositInfo& infoOther )
{
	if ( m_nPMSDay > infoOther.GetPMSDay() )
		return 1;
	else if ( m_nPMSDay < infoOther.GetPMSDay() )
		return -1;
	else if ( m_nHour > infoOther.GetHour() )
		return 1;
	else if ( m_nHour < infoOther.GetHour() )
		return -1;
	else if ( m_nMinute > infoOther.GetMinute() )
		return 1;
	else if ( m_nMinute < infoOther.GetMinute() )
		return -1;
	else if ( m_nIndex > infoOther.GetIndex() )
		return 1;
	else if ( m_nIndex < infoOther.GetIndex() )
		return -1;
	else
		return 0;
}

/**********************************************************************/

CPMSBookingCSVRecord::CPMSBookingCSVRecord()
{
	ClearRecord( TRUE );
}

/**********************************************************************/

void CPMSBookingCSVRecord::ClearRecord( bool bClearDeposits )
{
	//QUICK ACCESS
	m_nBookingId = 1;
	m_nInternalStatus = PMS_BOOKING_STATUS_PENDING;
	m_nCheckoutBalanceType = PMS_CHECKOUT_BALANCE_ZERO;
	//BOOKING DETAIL
	m_nCustomerId = 1;
	m_nGuests = 1;
	m_nSearchStartDay = 0;
	m_nSearchNights = 1;
	m_nSearchRoomSleeps = 1;
	m_nSearchRoomSubType = 0;
	m_nTariffType = 0;
	m_nRoomRateType = PMS_BOOKING_RATE_ROOM;
	m_dRoomRate = 0.0;
	m_dExtraBedRate = 0.0;
	m_nInvoiceNum = 0;
	m_dDinnerAllowance = 0.0;
	m_strDinnerDepartments = "";
	//SUB ACCOUNTS
	m_bMultipleAccounts = FALSE;
	m_nAccountCount = 1;

	for ( int n = 0; n < MAX_PMS_ACCOUNTS_PER_ROOM; n++ )
		m_arrayCovers[n] = 1;
	//OTHER
	m_strBookingNote = "";
	m_nEmailStartDay = 0;
	m_nEmailNights = 0;

	m_nValidationError = 0;
	m_nValidationGuests = 1;
	m_nValidationCheckInDay = 0;
	m_nValidationCheckOutDay = 0;
	m_nValidationMaxCheckOutDay = 0;
	m_nValidationMinCheckOutDay = 0;

	m_arrayParts.RemoveAll();

	if ( TRUE == bClearDeposits )
		m_arrayDeposits.RemoveAll();
}

/**********************************************************************/

void CPMSBookingCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord( TRUE );
	SetDinnerAllowance( PMSOptions.GetDinnerAllowance() );
	SetDinnerDepartments( PMSOptions.GetDinnerDepartments() );

	SetBookingId			( csv.GetInt(0) );
	SetInternalStatus		( csv.GetInt(1) );
	SetCheckoutBalanceType	( csv.GetInt(2) );

	if ( csv.GetInt(3) >= 1 )
	{
		CPMSBookingPartInfo infoPart;
		infoPart.SetRoomNo( csv.GetInt(4) );
		infoPart.SetStartDay( csv.GetInt(5) );
		infoPart.SetNights( csv.GetInt(6) );
		AddPartInfo( infoPart );
	}
	
	switch ( csv.GetInt(7) )
	{
	case 1:		V1To5ConvertFromCSV	 ( csv, 1 );	break;
	case 2:		V1To5ConvertFromCSV	 ( csv, 2 );	break;
	case 3:		V1To5ConvertFromCSV	 ( csv, 3 );	break;
	case 4:		V1To5ConvertFromCSV	 ( csv, 4 );	break;
	case 5:		V1To5ConvertFromCSV	 ( csv, 5 );	break;
	case 6:		V6To7ConvertFromCSV	 ( csv, 6 );	break;
	case 7:		V6To7ConvertFromCSV	 ( csv, 7 );	break;
	case 8:		V8To10ConvertFromCSV ( csv, 8 );	break;
	case 9:		V8To10ConvertFromCSV ( csv, 9 );	break;
	case 10:	V8To10ConvertFromCSV ( csv, 10 );	break;
	}

	RemoveEmptyParts();
}

/**********************************************************************/

void CPMSBookingCSVRecord::V1To5ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 8;

	int nCount = csv.GetInt(3);
	for ( int nPart = 0; nPart < nCount - 1; nPart++ )
	{
		CPMSBookingPartInfo infoPart;
		infoPart.SetRoomNo( csv.GetInt( nOffset++ ) );
		infoPart.SetStartDay( csv.GetInt( nOffset++ ) );
		infoPart.SetNights( csv.GetInt( nOffset++ ) );
		AddPartInfo( infoPart );
	}

	SetCustomerId			( csv.GetInt(nOffset++) );
	SetGuests				( csv.GetInt(nOffset++) );
	SetSearchStartDay		( csv.GetInt(nOffset++) );
	SetSearchNights			( csv.GetInt(nOffset++) );
	SetSearchRoomSleeps		( csv.GetInt(nOffset++) );
	SetSearchRoomSubType	( csv.GetInt(nOffset++) );

	if ( nVer >= 5 )
		SetRoomRateType		( csv.GetInt(nOffset++) );

	if ( nVer >= 2 )
		SetRoomRate			( csv.GetDouble(nOffset++) );

	if ( nVer >= 3 )
		SetInvoiceNum		( csv.GetInt(nOffset++) );

	if ( nVer >= 4 )
	{
		CPMSDepositInfo infoDeposit;
		infoDeposit.SetPaymentType( csv.GetInt(nOffset++) );
		infoDeposit.SetValue( csv.GetDouble(nOffset++) );

		if ( CPriceHelpers::CompareDoubles( infoDeposit.GetValue(), 0.0, 3 ) != 0 )
			m_arrayDeposits.Add( infoDeposit );
	}

	SetMultipleAccountsFlag	( csv.GetBool(nOffset++) );
	SetAccountCount			( csv.GetInt(nOffset++) );

	for ( int n = 0; n < MAX_PMS_ACCOUNTS_PER_ROOM; n++ )
		SetAccountCovers	( n, csv.GetInt( nOffset++ ) );

	SetBookingNote			( csv.GetString(nOffset++ ) );
	SetEmailStartDay		( csv.GetInt(nOffset++) );
	SetEmailNights			( csv.GetInt(nOffset++) );
}

/**********************************************************************/

void CPMSBookingCSVRecord::V6To7ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 8;

	int nCount = csv.GetInt(3);
	for ( int nPart = 0; nPart < nCount - 1; nPart++ )
	{
		CPMSBookingPartInfo infoPart;
		infoPart.SetRoomNo( csv.GetInt( nOffset++ ) );
		infoPart.SetStartDay( csv.GetInt( nOffset++ ) );
		infoPart.SetNights( csv.GetInt( nOffset++ ) );
		AddPartInfo( infoPart );
	}

	SetCustomerId			( csv.GetInt(nOffset++) );
	SetGuests				( csv.GetInt(nOffset++) );
	SetSearchStartDay		( csv.GetInt(nOffset++) );
	SetSearchNights			( csv.GetInt(nOffset++) );
	SetSearchRoomSleeps		( csv.GetInt(nOffset++) );
	SetSearchRoomSubType	( csv.GetInt(nOffset++) );
	SetRoomRateType			( csv.GetInt(nOffset++) );
	SetRoomRate				( csv.GetDouble(nOffset++) );
	SetInvoiceNum			( csv.GetInt(nOffset++) );

	CPMSDepositInfo infoDeposit;
	infoDeposit.SetPaymentType ( csv.GetInt(nOffset++) );
	infoDeposit.SetValue ( csv.GetDouble( nOffset++ ) );
	infoDeposit.SetPMSDay ( csv.GetInt(nOffset++ ) );

	if ( CPriceHelpers::CompareDoubles( infoDeposit.GetValue(), 0.0, 3 ) != 0 )
		m_arrayDeposits.Add( infoDeposit );

	if ( nVer >= 7 )
	{
		SetDinnerAllowance	( csv.GetDouble(nOffset++) );
		SetDinnerDepartments( csv.GetString(nOffset++) );
	}

	SetMultipleAccountsFlag	( csv.GetBool(nOffset++) );
	SetAccountCount			( csv.GetInt(nOffset++) );

	for ( int n = 0; n < MAX_PMS_ACCOUNTS_PER_ROOM; n++ )
		SetAccountCovers	( n, csv.GetInt( nOffset++ ) );

	SetBookingNote			( csv.GetString(nOffset++ ) );
	SetEmailStartDay		( csv.GetInt(nOffset++) );
	SetEmailNights			( csv.GetInt(nOffset++) );
}

/**********************************************************************/

void CPMSBookingCSVRecord::V8To10ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 8;

	int nPartCount = csv.GetInt(3);
	for ( int nPart = 0; nPart < nPartCount - 1; nPart++ )
	{
		CPMSBookingPartInfo infoPart;
		infoPart.SetRoomNo( csv.GetInt( nOffset++ ) );
		infoPart.SetStartDay( csv.GetInt( nOffset++ ) );
		infoPart.SetNights( csv.GetInt( nOffset++ ) );
		AddPartInfo( infoPart );
	}

	SetCustomerId			( csv.GetInt(nOffset++) );
	SetGuests				( csv.GetInt(nOffset++) );
	SetSearchStartDay		( csv.GetInt(nOffset++) );
	SetSearchNights			( csv.GetInt(nOffset++) );
	SetSearchRoomSleeps		( csv.GetInt(nOffset++) );
	SetSearchRoomSubType	( csv.GetInt(nOffset++) );

	if ( nVer >= 9 )
		SetTariffType		( csv.GetInt(nOffset++) );

	SetRoomRateType			( csv.GetInt(nOffset++) );
	SetRoomRate				( csv.GetDouble(nOffset++) );

	if ( nVer >= 10 )
		SetExtraBedRate		( csv.GetDouble(nOffset++) );

	SetInvoiceNum			( csv.GetInt(nOffset++) );

	int nDepositCount = csv.GetInt( nOffset++ );

	for ( int n = 0; n < nDepositCount; n++ )
	{
		CPMSDepositInfo infoDeposit;
		infoDeposit.SetPMSDay( csv.GetInt(nOffset++) );
		infoDeposit.SetHour( csv.GetInt(nOffset++) );
		infoDeposit.SetMinute( csv.GetInt(nOffset++) );
		infoDeposit.SetRefundFlag( csv.GetBool(nOffset++) );
		infoDeposit.SetPaymentType( csv.GetInt(nOffset++) );
		infoDeposit.SetValue( csv.GetDouble(nOffset++) );
		m_arrayDeposits.Add( infoDeposit );
	}

	SetDinnerAllowance	( csv.GetDouble(nOffset++) );
	SetDinnerDepartments( csv.GetString(nOffset++) );
	SetMultipleAccountsFlag	( csv.GetBool(nOffset++) );
	SetAccountCount			( csv.GetInt(nOffset++) );

	for ( int n = 0; n < MAX_PMS_ACCOUNTS_PER_ROOM; n++ )
		SetAccountCovers	( n, csv.GetInt( nOffset++ ) );

	SetBookingNote			( csv.GetString(nOffset++ ) );
	SetEmailStartDay		( csv.GetInt(nOffset++) );
	SetEmailNights			( csv.GetInt(nOffset++) );
}

/**********************************************************************/

void CPMSBookingCSVRecord::ConvertToCSV ( CCSV& csv )
{
	RemoveEmptyParts();

	int nPartCount = m_arrayParts.GetSize();
	
	csv.Add ( m_nBookingId );
	csv.Add ( m_nInternalStatus );
	csv.Add ( m_nCheckoutBalanceType );
	csv.Add ( nPartCount );

	if ( nPartCount > 0 )
	{
		CPMSBookingPartInfo infoPart;
		GetPartInfo( 0, infoPart );
		csv.Add( infoPart.GetRoomNo() );
		csv.Add( infoPart.GetStartDay() );
		csv.Add( infoPart.GetNights() );
	}
	else
	{
		csv.Add(0);
		csv.Add(0);
		csv.Add(0);
	}

	csv.Add ( PMS_BOOKING_VERSION );
	
	for ( int nPart = 1; nPart < m_arrayParts.GetSize(); nPart++ )
	{
		CPMSBookingPartInfo infoPart;
		GetPartInfo( nPart, infoPart );
		csv.Add( infoPart.GetRoomNo() );
		csv.Add( infoPart.GetStartDay() );
		csv.Add( infoPart.GetNights() );
	}
	
	csv.Add ( m_nCustomerId );
	csv.Add ( m_nGuests );
	csv.Add ( m_nSearchStartDay );
	csv.Add ( m_nSearchNights );
	csv.Add ( m_nSearchRoomSleeps );
	csv.Add ( m_nSearchRoomSubType );
	csv.Add ( m_nTariffType );
	csv.Add ( m_nRoomRateType );
	csv.Add ( m_dRoomRate, 2 );
	csv.Add ( m_dExtraBedRate, 2 );
	csv.Add ( m_nInvoiceNum );

	int nSize = m_arrayDeposits.GetSize();
	
	csv.Add ( nSize );
	for ( int n = 0; n < nSize; n++ )
	{
		csv.Add( m_arrayDeposits[n].GetPMSDay() );
		csv.Add( m_arrayDeposits[n].GetHour() );
		csv.Add( m_arrayDeposits[n].GetMinute() );
		csv.Add( m_arrayDeposits[n].GetRefundFlag() );
		csv.Add( m_arrayDeposits[n].GetPaymentType() );
		csv.Add( m_arrayDeposits[n].GetValue(), 2 );
	}
	
	csv.Add ( m_dDinnerAllowance, 2 );
	csv.Add ( m_strDinnerDepartments );
	csv.Add ( m_bMultipleAccounts );
	csv.Add ( m_nAccountCount );

	for ( int n = 0; n < MAX_PMS_ACCOUNTS_PER_ROOM; n++ )
		csv.Add( GetAccountCovers(n) );

	csv.Add	( m_strBookingNote );
	csv.Add ( m_nEmailStartDay );
	csv.Add ( m_nEmailNights );
}

/**********************************************************************/

void CPMSBookingCSVRecord::RemoveEmptyParts()
{
	CPMSBookingPartInfo infoPart;

	while ( GetPartCount() > 1 )
	{
		GetFinalPartInfo( infoPart );
		if ( infoPart.GetNights() > 0 )
			break;

		RemoveFinalPartInfo();
	}
}

/**********************************************************************/

int CPMSBookingCSVRecord::GetAccountCovers( int n )
{
	if ( ( n >= 0 ) && ( n < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		return m_arrayCovers[n];
	else
		return 1;
}

/**********************************************************************/

double CPMSBookingCSVRecord::GetExtraBedRate()
{
	double dRate = 0.0;

	if ( PMS_BOOKING_RATE_FREE != m_nRoomRateType )
		if ( ( 2 == m_nTariffType ) || ( 3 == m_nTariffType ) )
			dRate = m_dExtraBedRate;

	return dRate;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetBookingId( int n )
{
	Set ( m_nBookingId, n, PMSBooking::BookingId );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetBookingStatus( int nStatus )
{
	int nTraining = ( m_nInternalStatus >= 100 ) ? 100 : 0;

	switch( nStatus )
	{
	case PMS_BOOKING_STATUS_PENDING:
	case PMS_BOOKING_STATUS_ACTIVE:
	case PMS_BOOKING_STATUS_COMPLETE:
	case PMS_BOOKING_STATUS_CANCELLED:
		m_nInternalStatus = nTraining + nStatus;
		break;
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetTrainingFlag( bool b )
{
	int nStatus = GetBookingStatus();
	if ( TRUE == b ) nStatus += 100;
	m_nInternalStatus = nStatus;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetCustomerId( int n )
{
	Set ( m_nCustomerId, n, PMSBooking::CustomerId );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetCheckoutBalanceType( int n )
{
	switch( n )
	{
	case PMS_CHECKOUT_BALANCE_ZERO:
	case PMS_CHECKOUT_BALANCE_DEBIT:
	case PMS_CHECKOUT_BALANCE_CREDIT:
	case PMS_CHECKOUT_BALANCE_MIXED:
		m_nCheckoutBalanceType = n;
		break;
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetGuests( int n )
{
	Set ( m_nGuests, n, PMSBooking::Guests );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetInternalStatus( int n )
{
	if ( ( n >= 0 ) && ( n <= 199 ) )
	{
		switch( n % 100 )
		{
		case PMS_BOOKING_STATUS_PENDING:
		case PMS_BOOKING_STATUS_ACTIVE:
		case PMS_BOOKING_STATUS_COMPLETE:
		case PMS_BOOKING_STATUS_CANCELLED:
			m_nInternalStatus = n;
			break;
		}
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetSearchStartDay( int n )
{
	Set ( m_nSearchStartDay, n, PMSBooking::StartDay );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetSearchNights( int n )
{
	Set ( m_nSearchNights, n, PMSBooking::Nights );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetSearchRoomSleeps( int n )
{
	Set( m_nSearchRoomSleeps, n, PMSBooking::RoomSleeps );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetSearchRoomSubType( int n )
{
	Set( m_nSearchRoomSubType, n, PMSRoom::RoomType );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetTariffType( int n )
{
	Set( m_nTariffType, n, PMSBooking::TariffType );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetRoomRateType( int n )
{
	switch( n )
	{
	case PMS_BOOKING_RATE_ROOM:
	case PMS_BOOKING_RATE_BB:
	case PMS_BOOKING_RATE_DBB:
	case PMS_BOOKING_RATE_FREE:
		m_nRoomRateType = n;
		break;
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetRoomRate( double d )
{
	if ( DealerFlags.GetPMSExtendedRoomRateFlag() == TRUE )
	{
		Set( m_dRoomRate, d, PMSRoom::ExtendedRoomRate );
	}
	else
	{
		Set( m_dRoomRate, d, PMSRoom::RoomRate );
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetExtraBedRate( double d )
{
	Set( m_dExtraBedRate, d, PMSRoom::RoomRate );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetInvoiceNum( int n )
{
	Set( m_nInvoiceNum, n, PMSBooking::InvoiceNum );
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetDepositInfo( int nIdx, CPMSDepositInfo& infoDeposit )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayDeposits.GetSize() ) )
		infoDeposit = m_arrayDeposits[nIdx];
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetDepositInfo( int nIdx, CPMSDepositInfo& infoDeposit )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayDeposits.GetSize() ) )
		m_arrayDeposits[nIdx] = infoDeposit;
}

/**********************************************************************/

void CPMSBookingCSVRecord::AddDepositInfo( CPMSDepositInfo& infoDeposit )
{
	m_arrayDeposits.Add( infoDeposit );
}

/**********************************************************************/

void CPMSBookingCSVRecord::RemoveDepositInfo( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayDeposits.GetSize() ) )
		m_arrayDeposits.RemoveAt( nIdx );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetDinnerAllowance( double d )
{
	Set( m_dDinnerAllowance, d, PMSBooking::DinnerAllowance );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetDinnerDepartments( const char* sz )
{
	m_strDinnerDepartments = sz;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetMultipleAccountsFlag( bool b )
{
	m_bMultipleAccounts = b;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetAccountCount( int n )
{
	if ( ( n >= 1 ) && ( n <= MAX_PMS_ACCOUNTS_PER_ROOM ) )
		m_nAccountCount = n;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetAccountCovers( int n, int nCovers )
{
	if ( ( n >= 0 ) && ( n < MAX_PMS_ACCOUNTS_PER_ROOM ) )
		if ( ( nCovers >= 1 ) && ( nCovers <= MAX_PMS_ACCOUNTS_PER_ROOM ) )
			m_arrayCovers[n] = nCovers;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetEmailStartDay( int n )
{
	if ( ( n >= 0 ) && ( n <= PMSBooking::StartDay.Max ) )
		m_nEmailStartDay = n;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetEmailNights( int n )
{
	if ( ( n >= 0 ) && ( n <= PMSBooking::Nights.Max ) )
		m_nEmailNights = n;
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetBookingNote( const char* sz )
{
	Set ( m_strBookingNote, sz, PMSBooking::BookingNote );
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetPartInfo( int nIdx, CPMSBookingPartInfo& infoPart )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayParts.GetCount() ) )
		infoPart = m_arrayParts.GetAt( nIdx );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetPartInfo( int nIdx, CPMSBookingPartInfo& infoPart )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayParts.GetCount() ) )
		m_arrayParts.SetAt( nIdx, infoPart );
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetFinalPartInfo( CPMSBookingPartInfo& infoPart )
{
	int nIdx = m_arrayParts.GetSize() - 1;

	if ( nIdx >= 0 )
		infoPart = m_arrayParts.GetAt( nIdx );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetFinalPartInfo( CPMSBookingPartInfo& infoPart )
{
	int nIdx = m_arrayParts.GetSize() - 1;

	if ( nIdx >= 0 )
		m_arrayParts.SetAt( nIdx, infoPart );
}

/**********************************************************************/

void CPMSBookingCSVRecord::AddPartInfo( CPMSBookingPartInfo& infoPart )
{
	m_arrayParts.Add( infoPart );
}

/**********************************************************************/

void CPMSBookingCSVRecord::RemoveFinalPartInfo()
{
	int nIdx = m_arrayParts.GetSize() - 1;

	if ( nIdx >= 0 )
		m_arrayParts.RemoveAt( nIdx );
}

/**********************************************************************/

void CPMSBookingCSVRecord::BuildRoomListForEditing( CArray<CPMSBookSearchListInfo,CPMSBookSearchListInfo>& arrayRooms )
{
	arrayRooms.RemoveAll();

	for ( int nIdx = 0; nIdx < GetPartCount(); nIdx++ )
	{
		CPMSBookingPartInfo infoPart;
		GetPartInfo( nIdx, infoPart );

		int nRoomIdx;
		CPMSRoomCSVRecord RoomRecord;
		if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoPart.GetRoomNo(), nRoomIdx ) == TRUE )
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, RoomRecord );
		
		COleDateTime date;
		int nDate = infoPart.GetStartDay() + infoPart.GetNights();
		GetOleDateSince2011( nDate, date );

		CString strDate;
		strDate.Format( "%2.2d/%2.2d/%4.4d",
			date.GetDay(),
			date.GetMonth(),
			date.GetYear() );

		CPMSBookSearchListInfo info;
		info.m_bFullStay = FALSE;
		info.m_nNights = infoPart.GetNights();
		info.m_nRoomIdx = nRoomIdx;
		info.m_nRoomNo = RoomRecord.GetRoomNo();
		info.m_nSleeps = RoomRecord.GetSleeps();
		info.m_nStartDay = infoPart.GetStartDay();
		info.m_strRoomName = RoomRecord.GetRoomName();
		info.m_strType = RoomRecord.GetRoomTypeName( FALSE );
		info.m_strDate = strDate;
		info.m_bExtraBed = RoomRecord.GetRoomAllowExtraBedFlag();
		info.m_bExtraCot = RoomRecord.GetRoomAllowExtraCotFlag();
		arrayRooms.Add( info );
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::ValidateForEditing( int nDayNow, bool bAllowClosed )
{
	m_nValidationError = 0;

	//BOOKING SHOULD CONTAIN AT LEAST ONE PART
	if ( GetPartCount() == 0 )
	{
		m_nValidationError = 1;
		return;
	}

	//BOOKING SHOULD BE ACTIVE
	if ( FALSE == bAllowClosed )
	{
		switch( GetBookingStatus() )
		{
		case PMS_BOOKING_STATUS_ACTIVE:
			break;

		default:
			m_nValidationError = 2;
			return;
		}
	}
	else
	{
		switch( GetBookingStatus() )
		{
		case PMS_BOOKING_STATUS_ACTIVE:
		case PMS_BOOKING_STATUS_COMPLETE:
			break;

		default:
			m_nValidationError = 2;
			return;
		}
	}

	bool bGotActivePart = FALSE;
	int nNextCheckInDay = -1;
	
	for ( int nPart = 0; nPart < GetPartCount(); nPart++ )
	{
		CPMSBookingPartInfo infoPart;
		GetPartInfo( nPart, infoPart );

		if ( infoPart.GetStartDay() < 0 )
		{
			m_nValidationError = 3;
			return;
		}

		if ( infoPart.GetNights() < 1 )
		{
			m_nValidationError = 4;
			return;
		}

		//DATES SHOULD FOLLOW IN SEQUENCE FOR CONSECUTIVE PARTS
		if ( nPart != 0 )
		{
			if ( infoPart.GetStartDay() != nNextCheckInDay )
			{
				m_nValidationError = 5;
				return;
			}
		}

		nNextCheckInDay = infoPart.GetStartDay() + infoPart.GetNights();
	}

	//GET INITIAL CHECK IN DAY FROM FIRST PART OF BOOKING
	CPMSBookingPartInfo infoPart;
	GetPartInfo( 0, infoPart );

	m_nValidationCheckInDay = infoPart.GetStartDay();
	m_nValidationGuests = m_nGuests;

	//CHECK IN DAY FOR THIS PART MUST NOT BE IN THE FUTURE
	GetFinalPartInfo( infoPart );
	if ( infoPart.GetStartDay() > nDayNow )
	{
		m_nValidationError = 6;
		return;
	}

	m_nValidationCheckOutDay = infoPart.GetStartDay() + infoPart.GetNights();
	m_nValidationMinCheckOutDay = infoPart.GetStartDay() + 1;
	m_nValidationMaxCheckOutDay = m_nValidationCheckOutDay;

	CPMSBookingQueryInfo Query;
	Query.m_nRoomNo = infoPart.GetRoomNo();
	Query.m_nStartDay = m_nValidationCheckOutDay;
	Query.m_nMinNights = 1;
	Query.m_nMaxNights = PMSBooking::Nights.Max;

	CPMSBookingResponseInfo Response;

	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( Response.m_nBookingClashId == 0 )
		m_nValidationMaxCheckOutDay += Response.m_nMaxNights;

	//CURRENT CHECK IN DAY MUST BE WITHIN VALID RANGE
	if ( ( m_nValidationCheckOutDay < m_nValidationMinCheckOutDay ) || ( m_nValidationCheckOutDay > m_nValidationMaxCheckOutDay ) )
		m_nValidationError = 7;

	//OVERALL CHECK IN DAY MUST BE BEFORE EARLIEST CHECK OUT DAY
	if ( m_nValidationCheckInDay >= m_nValidationMinCheckOutDay )
		m_nValidationError = 8;
}

/**********************************************************************/

bool CPMSBookingCSVRecord::CheckEmailDates()
{
	return ( ( m_nSearchStartDay == m_nEmailStartDay ) && ( m_nSearchNights == m_nEmailNights ) );
}

/**********************************************************************/

void CPMSBookingCSVRecord::SetEmailDates()
{
	m_nEmailStartDay = m_nSearchStartDay;
	m_nEmailNights = m_nSearchNights;
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetAccomodationTaxPeriods( CArray<CPMSBookingPartInfo,CPMSBookingPartInfo>& arrayParts )
{
	double dRoomSpend;
	double dExtraBedSpend;
	int nStartDay;
	int nNights;
	GetAccomodationSpend( dRoomSpend, dExtraBedSpend, nStartDay, nNights );
	DataManager.HistoricalTaxRates.SplitPMSDateRange( nStartDay, nNights, arrayParts );
}

/**********************************************************************/

bool CPMSBookingCSVRecord::GetAccomodationSpend( double& dRoomSpend, double& dExtraBedSpend, int& nStartDay, int& nStayLength )
{
	dRoomSpend = 0.0;
	dExtraBedSpend = 0.0;
	nStartDay = 0;
	nStayLength = 0;
	bool bGotSpend = FALSE;

	switch( GetBookingStatus() )
	{
	case PMS_BOOKING_STATUS_PENDING:
	case PMS_BOOKING_STATUS_CANCELLED:
		break;

	case PMS_BOOKING_STATUS_ACTIVE:
		{
			CPMSBookingPartInfo infoPart;
			GetPartInfo( 0, infoPart );
			
			nStayLength = PMSOptions.GetPMSDayNumberAccom() - infoPart.GetStartDay();
			
			if ( nStayLength > 0 )
			{
				dRoomSpend = ( m_dRoomRate * nStayLength );
				dExtraBedSpend = ( m_dExtraBedRate * nStayLength );
				nStartDay = infoPart.GetStartDay();
				bGotSpend = TRUE;
			}
			else
				nStayLength = 0;
		}
		break;

	case PMS_BOOKING_STATUS_COMPLETE:
		{
			CPMSBookingPartInfo infoStart, infoEnd;
			GetPartInfo( 0, infoStart );
			GetFinalPartInfo( infoEnd );

			nStayLength = infoEnd.GetStartDay() + infoEnd.GetNights() - infoStart.GetStartDay();
			
			if ( nStayLength > 0 )
			{
				dRoomSpend = ( m_dRoomRate * nStayLength );
				dExtraBedSpend = ( m_dExtraBedRate * nStayLength );
				nStartDay = infoStart.GetStartDay();
				bGotSpend = TRUE;
			}
			else
				nStayLength = 0;
		}
		break;
	}

	return bGotSpend;
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetAccomodationInvoiceLine( int nStartDay, int nNights, CPMSInvoiceLineInfo& infoTrans, CPMSInvoiceLineInfo& infoAccom )
{
	infoAccom.m_dateSale = COleDateTime( 1980, 1, 1, 0, 0, 0 );
	infoAccom.m_nTransactionNo = 0;
	infoAccom.m_nLineType = PMS_LINE_ACCOM;

	switch( m_nRoomRateType )
	{
	case PMS_BOOKING_RATE_BB:
		infoAccom.m_nBasePluNo = PMSOptions.GetEposReportRoomBBPlu();
		break;

	case PMS_BOOKING_RATE_DBB:
		infoAccom.m_nBasePluNo = PMSOptions.GetEposReportRoomDBBPlu();
		break;

	case PMS_BOOKING_RATE_ROOM:
	default:
		infoAccom.m_nBasePluNo = PMSOptions.GetEposReportRoomOnlyPlu();
		break;
	}

	infoAccom.m_nSection = -2;
	infoAccom.m_nDeptNo = 0;
	infoAccom.m_nGroupNo = 0;
	infoAccom.m_nTaxBand = 0;
	infoAccom.m_nModifier = 0;
	infoAccom.m_nLevel = 0;
	infoAccom.m_strDescription = "";
	infoAccom.m_dEposPluQty = 0.0;
	infoAccom.m_dValue = 0.0;
	infoAccom.m_dDiscount = 0.0;
	infoAccom.m_dAllowance = 0.0;

	infoTrans = infoAccom;
	infoTrans.m_nLineType = PMS_LINE_TRAN;

	double dSpend = m_dRoomRate * nNights;
	
	COleDateTime dateTrans;
	int nAccomDay = ( PMSOptions.GetAccomReportNextDayFlag() ? nStartDay + 1 : nStartDay );
	GetOleDateSince2011( nAccomDay, dateTrans );
	dateTrans += COleDateTimeSpan ( 0, PMSOptions.GetAccomReportHour(), PMSOptions.GetAccomReportMinute(), 0 );
	infoTrans.m_dateSale = dateTrans;
	infoAccom.m_dateSale = dateTrans;

	infoAccom.m_dBasePluQty = nNights;
	infoAccom.m_dEposPluQty = nNights;
	infoAccom.m_dValue = dSpend;
	infoTrans.m_dValue = dSpend;

	if ( 0 != infoAccom.m_nBasePluNo )
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( infoAccom.m_nBasePluNo, nPluIdx ) == TRUE )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			
			infoAccom.m_nDeptNo = PluRecord.GetBaseDeptNo();
			infoAccom.m_nTaxBand = PluRecord.GetBaseTaxBandInt();

			CReportHelpers ReportHelpers;
			infoAccom.m_strDescription = ReportHelpers.GetReportText( PluRecord );

			int nDeptIdx;
			if ( DataManager.Department.FindDeptByNumber( infoAccom.m_nDeptNo, nDeptIdx ) == TRUE )
			{
				CDepartmentCSVRecord Department;
				DataManager.Department.GetAt( nDeptIdx, Department );
				infoAccom.m_nGroupNo = Department.GetEposGroup();
			}
		}
	}
}

/**********************************************************************/

bool CPMSBookingCSVRecord::SplitAccommodationLine( CPMSInvoiceLineInfo& infoAccom, CPMSInvoiceLineInfo& infoBreakfast )
{
	//DO NOT CREATE BREAKFAST ENTRY FOR ROOM ONLY BOOKINGS
	switch( m_nRoomRateType )
	{
	case PMS_BOOKING_RATE_BB:
	case PMS_BOOKING_RATE_DBB:
		break;

	default:
		return FALSE;
	}

	//DO NOT CREATE BREAKFAST ENTRY IF NO BREAKFAST PLU HAS BEEN DEFINED
	__int64 nBreakfastPluNo = PMSOptions.GetEposReportBreakfastPlu();
	if ( 0 == nBreakfastPluNo )
		return FALSE;

	//DO NOT CREATE BREAKFAST ENTRY IF BREAKFAST PLU DOES NOT EXIST
	int nBreakfastPluIdx;
	if ( DataManager.Plu.FindPluByNumber( nBreakfastPluNo, nBreakfastPluIdx ) == FALSE )
		return FALSE;
	
	//DO NOT CREATE BREAKFAST ENTRY IF BREAKFAST VALUE IS NOT SET
	if ( CPriceHelpers::CompareDoubles( PMSOptions.GetEposReportBreakfastValue(), 0.0, 3 ) != 1 )
		return FALSE;

	//DO NOT CREATE BREAKFAST ENTRY IF IT WOULD EXHAUST THE ACCOMODATION COST
	double dBreakfastValue = PMSOptions.GetEposReportBreakfastValue() * infoAccom.m_dBasePluQty * ( (double) GetGuests() );
	if ( CPriceHelpers::CompareDoubles( infoAccom.m_dValue, dBreakfastValue, 3 ) != 1 )
		return FALSE;

	CPluCSVRecord BreakfastPluRecord;
	DataManager.Plu.GetAt( nBreakfastPluIdx, BreakfastPluRecord );
	
	//THE BREAKFAST ENTRY TAKES THE SAME TAX BAND AS THE ACCOMMODATION
	//ITEM, IRRESPECTIVE OF THE PLU DATABASE SETTING
	infoBreakfast = infoAccom;
	infoBreakfast.m_nLineType = PMS_LINE_BREAKFAST;
	infoBreakfast.m_nBasePluNo = nBreakfastPluNo;
	infoBreakfast.m_dValue = dBreakfastValue;
	infoBreakfast.m_nDeptNo = BreakfastPluRecord.GetBaseDeptNo();
	infoBreakfast.m_nGroupNo = 0;
	infoBreakfast.m_strDescription = ReportHelpers.GetReportText( BreakfastPluRecord );
	
	int nDeptIdx;
	if ( DataManager.Department.FindDeptByNumber( infoBreakfast.m_nDeptNo, nDeptIdx ) == TRUE )
	{
		CDepartmentCSVRecord Department;
		DataManager.Department.GetAt( nDeptIdx, Department );
		infoBreakfast.m_nGroupNo = Department.GetEposGroup();
	}
	
	infoAccom.m_dValue -= infoBreakfast.m_dValue;
	return TRUE;
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetExtraBedInvoiceLine( int nStartDay, int nNights, CPMSInvoiceLineInfo& infoTrans, CPMSInvoiceLineInfo& infoExtraBed )
{
	infoExtraBed.m_dateSale = COleDateTime( 1980, 1, 1, 0, 0, 0 );
	infoExtraBed.m_nTransactionNo = 0;
	
	if ( PMS_TARIFFTYPE_EXTRACOT == m_nTariffType )
	{
		infoExtraBed.m_nLineType = PMS_LINE_EXTRACOT;
		infoExtraBed.m_nBasePluNo = PMSOptions.GetEposReportExtraCotPlu();
	}
	else
	{
		infoExtraBed.m_nLineType = PMS_LINE_EXTRABED;
		infoExtraBed.m_nBasePluNo = PMSOptions.GetEposReportExtraBedPlu();
	}
	
	infoExtraBed.m_nSection = -2;
	infoExtraBed.m_nDeptNo = 0;
	infoExtraBed.m_nGroupNo = 0;
	infoExtraBed.m_nTaxBand = 0;
	infoExtraBed.m_nModifier = 0;
	infoExtraBed.m_nLevel = 0;
	infoExtraBed.m_strDescription = "";
	infoExtraBed.m_dEposPluQty = 0.0;
	infoExtraBed.m_dValue = 0.0;
	infoExtraBed.m_dDiscount = 0.0;
	infoExtraBed.m_dAllowance = 0.0;

	infoTrans = infoExtraBed;
	infoTrans.m_nLineType = PMS_LINE_TRAN;

	double dSpend = m_dExtraBedRate * nNights;
	
	COleDateTime dateTrans;
	int nAccomDay = ( PMSOptions.GetAccomReportNextDayFlag() ? nStartDay + 1 : nStartDay );
	GetOleDateSince2011( nAccomDay, dateTrans );
	dateTrans += COleDateTimeSpan ( 0, PMSOptions.GetAccomReportHour(), PMSOptions.GetAccomReportMinute(), 0 );
	infoTrans.m_dateSale = dateTrans;
	infoExtraBed.m_dateSale = dateTrans;

	infoExtraBed.m_dBasePluQty = nNights;
	infoExtraBed.m_dEposPluQty = nNights;
	infoExtraBed.m_dValue = dSpend;
	infoTrans.m_dValue = dSpend;

	if ( 0 != infoExtraBed.m_nBasePluNo )
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( infoExtraBed.m_nBasePluNo, nPluIdx ) == TRUE )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			
			infoExtraBed.m_nDeptNo = PluRecord.GetBaseDeptNo();
			infoExtraBed.m_nTaxBand = PluRecord.GetBaseTaxBandInt();

			CReportHelpers ReportHelpers;
			infoExtraBed.m_strDescription = ReportHelpers.GetReportText( PluRecord );

			int nDeptIdx;
			if ( DataManager.Department.FindDeptByNumber( infoExtraBed.m_nDeptNo, nDeptIdx ) == TRUE )
			{
				CDepartmentCSVRecord Department;
				DataManager.Department.GetAt( nDeptIdx, Department );
				infoExtraBed.m_nGroupNo = Department.GetEposGroup();
			}
		}
	}
}

/**********************************************************************/

void CPMSBookingCSVRecord::GetDepositInvoiceArray( CArray<CPMSInvoiceLineInfo,CPMSInvoiceLineInfo>& arrayDeposit )
{
	arrayDeposit.RemoveAll();

	for ( int n = 0; n < m_arrayDeposits.GetCount(); n++ )
	{
		CPMSDepositInfo infoDeposit;
		GetDepositInfo( n, infoDeposit );

		int nPaymentNo = infoDeposit.GetPaymentType();
		CString strPaymentType = DataManager.Payment.GetDisplayName( nPaymentNo );

		COleDateTime dateTrans;
		GetOleDateSince2011( infoDeposit.GetPMSDay(), dateTrans );
		dateTrans += COleDateTimeSpan( 0, infoDeposit.GetHour(), infoDeposit.GetMinute(), 0 );

		double dValue = infoDeposit.GetValue();
		if ( TRUE == infoDeposit.GetRefundFlag() )
			dValue *= -1;

		CPMSInvoiceLineInfo infoInvoiceDeposit;
		infoInvoiceDeposit.m_nSection = -1;
		infoInvoiceDeposit.m_dateSale = dateTrans;
		infoInvoiceDeposit.m_nTransactionNo = 0;
		infoInvoiceDeposit.m_nLineType = PMS_LINE_DEPOSIT;
		infoInvoiceDeposit.m_nBasePluNo = nPaymentNo;
		infoInvoiceDeposit.m_nDeptNo = 0;
		infoInvoiceDeposit.m_nGroupNo = 0;
		infoInvoiceDeposit.m_nTaxBand = 0;
		infoInvoiceDeposit.m_nModifier = 0;
		infoInvoiceDeposit.m_nLevel = 0;
		infoInvoiceDeposit.m_strDescription = strPaymentType;
		infoInvoiceDeposit.m_dEposPluQty = 0.0;
		infoInvoiceDeposit.m_dValue = dValue;
		infoInvoiceDeposit.m_dDiscount = 0.0;
		infoInvoiceDeposit.m_dAllowance = 0.0;

		CPMSInvoiceLineInfo infoInvoiceTrans;
		infoInvoiceTrans = infoInvoiceDeposit;
		infoInvoiceTrans.m_dValue = 0.0;
		infoInvoiceTrans.m_nLineType = PMS_LINE_TRAN;

		arrayDeposit.Add( infoInvoiceTrans );
		arrayDeposit.Add( infoInvoiceDeposit );
	}
}

/**********************************************************************/

bool CPMSBookingCSVRecord::GetDinnerAdjustmentLine( CString& strDate, double dCredit, CPMSInvoiceLineInfo& infoTrans, CPMSInvoiceLineInfo& infoCredit, bool bCredit )
{
	if ( strDate.GetLength() != 8 )
		return FALSE;

	COleDateTime dateTrans = COleDateTime( atoi( strDate.Left(4) ), atoi( strDate.Mid(4,2) ), atoi( strDate.Right(2) ), 23, 59, 0 );
	if ( dateTrans.m_status != COleDateTime::valid )
		return FALSE;

	infoCredit.m_dateSale = dateTrans;
	infoCredit.m_nTransactionNo = 0;
	infoCredit.m_nLineType = PMS_LINE_DINNER;
	infoCredit.m_nBasePluNo = PMSOptions.GetEposReportDinnerPlu();
	infoCredit.m_nDeptNo = 0;
	infoCredit.m_nGroupNo = 0;
	infoCredit.m_nTaxBand = 0;
	infoCredit.m_nModifier = 0;
	infoCredit.m_nLevel = 0;
	infoCredit.m_strDescription = "";
	infoCredit.m_dEposPluQty = 1.0;
	infoCredit.m_dBasePluQty = 1.0;
	infoCredit.m_dValue = -dCredit;
	infoCredit.m_dDiscount = 0.0;
	infoCredit.m_dAllowance = 0.0;

	if ( 0 != infoCredit.m_nBasePluNo )
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( infoCredit.m_nBasePluNo, nPluIdx ) == TRUE )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			
			infoCredit.m_nDeptNo = PluRecord.GetBaseDeptNo();
			infoCredit.m_nTaxBand = PluRecord.GetBaseTaxBandInt();

			if ( TRUE == bCredit )
				infoCredit.m_strDescription = PluRecord.GetRepText();
			else
				infoCredit.m_strDescription = PluRecord.GetEposText();

			int nDeptIdx;
			if ( DataManager.Department.FindDeptByNumber( infoCredit.m_nDeptNo, nDeptIdx ) == TRUE )
			{
				CDepartmentCSVRecord Department;
				DataManager.Department.GetAt( nDeptIdx, Department );
				infoCredit.m_nGroupNo = Department.GetEposGroup();
			}
		}
	}

	infoTrans = infoCredit;
	infoTrans.m_dValue = 0.0;
	infoTrans.m_nLineType = PMS_LINE_TRAN;

	return TRUE;
}

/**********************************************************************/

bool CPMSBookingCSVRecord::UpdateInvoiceNum()
{
	bool bResult = FALSE;

	if ( GetInvoiceNum() == 0 )
	{
		CPMSInvoiceNum InvoiceNum;
		SetInvoiceNum( InvoiceNum.GetInvoiceNum() );
		InvoiceNum.NextInvoiceNum();
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CPMSBookingCSVRecord::CheckDinnerSettings( int& nDinnerTaxBand )
{
	//DINNER ALLOWANCE MUST NOT BE ZERO
	if ( CPriceHelpers::CompareDoubles( m_dDinnerAllowance, 0.0, 3 ) != 1 )
		return FALSE;

	//DINNER PLU MUST BE SPECIFIED
	__int64 nDinnerPlu = PMSOptions.GetEposReportDinnerPlu(); 
	if ( 0 == nDinnerPlu )
		return FALSE;

	//DINNER PLU MUST EXIST
	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( nDinnerPlu, nPluIdx ) == FALSE )
		return FALSE;
	
	CPluCSVRecord DinnerPlu;
	DataManager.Plu.GetAt( nPluIdx, DinnerPlu );
	nDinnerTaxBand = DinnerPlu.GetBaseTaxBandInt();
	return TRUE;
}

/**********************************************************************/

double CPMSBookingCSVRecord::CalculateDepositValue()
{
	double dValue = 0.0;

	for ( int n = 0; n < m_arrayDeposits.GetSize(); n++ )
	{
		if ( m_arrayDeposits[n].GetRefundFlag() == FALSE )
			dValue += m_arrayDeposits[n].GetValue();
		else
			dValue -= m_arrayDeposits[n].GetValue();
	}

	return dValue;
}

/**********************************************************************/

int CPMSBookingCSVRecord::SortDepositTransactions( int nTransactionToTrack )
{
	int nCount = m_arrayDeposits.GetCount();
	int nResult = nCount - 1;

	if ( nCount >= 2 )
	{
		//MARK DEPOSIT INDICES BEFORE SORTING
		for ( int n = 0; n < nCount; n++ )
			m_arrayDeposits[n].SetIndex(n);

		//INSERTION SORT OF DEPOSITS
		for ( int i = 1; i < nCount; i++ )
		{
			CPMSDepositInfo infoDeposit = m_arrayDeposits[i];

			int j = i - 1;

			while ( ( j >= 0 ) && ( m_arrayDeposits[j].Compare( infoDeposit ) == 1 ) )
			{
				m_arrayDeposits[ j + 1 ] = m_arrayDeposits[ j ];
				j--;
			}

			m_arrayDeposits[j + 1] = infoDeposit;
		}

		//FIND NEW POSITION OF TRACKED DEPOSIT
		for ( int n = 0; n < m_arrayDeposits.GetCount(); n++ )
		{
			if ( m_arrayDeposits[n].GetIndex() == nTransactionToTrack )
			{
				nResult = n;
				break;
			}
		}
	}

	return nResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPMSBookingCSVArray::CPMSBookingCSVArray() 
{
	m_bRepairDatabase = FALSE;
}

/**********************************************************************/

CPMSBookingCSVArray::~CPMSBookingCSVArray() 
{
}

/**********************************************************************/

int CPMSBookingCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	return nReply;
}

/**********************************************************************/

void CPMSBookingCSVArray::QuickGetAt ( int nIdx, CPMSBookingCSVRecord& BookingRecord, bool bBuildCalendar )
{
	BookingRecord.ClearRecord( TRUE );

	CString strTemp;
	if ( ( strTemp = GetAt ( nIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 7 );

		BookingRecord.SetBookingId			( csv.GetInt(0) );
		BookingRecord.SetInternalStatus		( csv.GetInt(1) );
		BookingRecord.SetCheckoutBalanceType( csv.GetInt(2) );

		int nPartCount = csv.GetInt(3);
		if ( nPartCount >= 1 )
		{
			CPMSBookingPartInfo infoPart;
			infoPart.SetRoomNo( csv.GetInt(4) );
			infoPart.SetStartDay( csv.GetInt(5) );
			infoPart.SetNights( csv.GetInt(6) );
			BookingRecord.AddPartInfo( infoPart );
		}
	
		if ( TRUE == bBuildCalendar )
			if ( ( nPartCount > 1 ) || ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_ACTIVE ) )
				GetAt( nIdx, BookingRecord );
	}
}

/**********************************************************************/

int CPMSBookingCSVArray::GetBookingId ( int nIdx )
{
	int nId = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nId = csv.GetInt(0);
	}

	return nId;
}

/**********************************************************************/

bool CPMSBookingCSVArray::FindBookingById( int nId, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		int nArrayId = GetBookingId( nIndex );

		if ( nId < nArrayId )
			nEnd = nIndex - 1;
		else if ( nId > nArrayId )
			nStart = nIndex + 1;
		else
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CPMSBookingCSVArray::FindFirstFreeId()
{
	int nResult = 0;

	int nSize = GetSize();
	if ( 0 == nSize )
		nResult = 1;
	else
	{
		int nId = GetBookingId( nSize - 1 );
		nResult = nId + 1;
	}

	if ( nResult < PMSBooking::BookingId.Min || nResult > PMSBooking::BookingId.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

void CPMSBookingCSVArray::InsertAndSaveBooking( CPMSBookingCSVRecord& BookingRecord )
{
	int nPos;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( BookingRecord.GetBookingId(), nPos ) == TRUE )
		DataManagerNonDb.PMSBooking.SetAt( nPos, BookingRecord );
	else	
		DataManagerNonDb.PMSBooking.InsertAt( nPos, BookingRecord );

	CFilenameUpdater FnUp( SysFiles::PMSBooking );
	DataManagerNonDb.PMSBooking.Write( FnUp.GetFilenameToUse() );
	
	CString strFilename;
	strFilename.Format( "%s\\%d.dat",
		PMSOptions.GetPMSFolderBooking(),
		BookingRecord.GetBookingId() );

	CCSV csv;
	BookingRecord.ConvertToCSV( csv );

	CSSFile fileBooking;
	if ( fileBooking.Open( strFilename, "wb" ) == TRUE )
		fileBooking.WriteLine( csv.GetLine() );
}

/**********************************************************************/

void CPMSBookingCSVArray::RemoveAndSaveBooking( int nBookingId )
{
	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
	{
		DataManagerNonDb.PMSBooking.RemoveAt( nBookingIdx );

		CFilenameUpdater FnUp( SysFiles::PMSBooking );
		DataManagerNonDb.PMSBooking.Write( FnUp.GetFilenameToUse() );
		
		CString strFilename;
		strFilename.Format( "%s\\%d.dat",
			PMSOptions.GetPMSFolderBooking(),
			nBookingId );

		CFileRemover FileRemover( strFilename );
	}
}

/**********************************************************************/

void CPMSBookingCSVArray::BuildInternalTables()
{
	m_bRepairDatabase = FALSE;

	PMSRoomStatusTable.InitialiseRooms();

	StatusProgress.Lock( TRUE, "Building PMS Calendar" );

	int nLastBookingId = 0;

	int nSize = GetSize();
	for ( int nBookingIdx = 0; nBookingIdx < nSize; nBookingIdx++ )
	{
		StatusProgress.SetPos( nBookingIdx, nSize );

		CPMSBookingCSVRecord BookingRecord;
		QuickGetAt( nBookingIdx, BookingRecord, TRUE );

		int nBookingId = BookingRecord.GetBookingId();
		
		if ( ( nBookingId < PMSBooking::BookingId.Min ) || ( nBookingId > PMSBooking::BookingId.Max ) )
		{
			m_bRepairDatabase = TRUE;
			break;
		}

		if ( nBookingId <= nLastBookingId )
		{
			m_bRepairDatabase = TRUE;
			break;
		}

		nLastBookingId = nBookingId;
		
		switch( BookingRecord.GetBookingStatus() )
		{
		case PMS_BOOKING_STATUS_PENDING:
			PMSCalendarTable.ApplyBooking( BookingRecord );
			break;
		
		case PMS_BOOKING_STATUS_ACTIVE:
			PMSCalendarTable.ApplyBooking( BookingRecord );
			PMSRoomStatusTable.LoadRoomAccountSettingsLive( BookingRecord, TRUE );
			break;

		case PMS_BOOKING_STATUS_COMPLETE:
			if ( BookingRecord.GetTrainingFlag() == FALSE )
				PMSCalendarTable.ApplyBooking( BookingRecord );
			break;
		
		case PMS_BOOKING_STATUS_CANCELLED:
		default:
			break;
		}
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CPMSBookingCSVArray::RebuildFromBookingFiles()
{
	RemoveAt( 0, GetSize() );

	CString strFilePath = "";
	strFilePath += PMSOptions.GetPMSFolderBooking();
	strFilePath += "\\";
	
	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strFilePath + "*.*" );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDirectory() == TRUE )
			continue;

		if ( finder.IsDots() == TRUE )
			continue;

		CString strFileName = finder.GetFileName();
		strFileName.MakeUpper();

		int nLength = strFileName.GetLength();
		if ( nLength < 5 )
			continue;

		if ( strFileName.Right(4) != ".DAT" )
			continue;

		if ( ::TestNumeric( strFileName.Left( nLength - 4 ) ) == FALSE )
			continue;

		CSSFile fileBooking;
		if ( fileBooking.Open( strFilePath + strFileName, "rb" ) == FALSE )
			continue;

		CString strBuffer;
		fileBooking.ReadString( strBuffer );
		fileBooking.Close();

		CCSV csv( strBuffer );

		CPMSBookingCSVRecord BookingRecord;
		BookingRecord.ConvertFromCSV( csv );

		int nBookingIdx;
		int nBookingId = BookingRecord.GetBookingId();
		if ( FindBookingById( nBookingId, nBookingIdx ) == FALSE )
			InsertAt( nBookingIdx, BookingRecord );
	}
}

/**********************************************************************/

void CPMSBookingCSVArray::ValidateBookingDatabase()
{
	int nError = 0;
	if ( DataManagerNonDb.PMSBooking.GetRepairDatabaseFlag() == TRUE )
		nError = 1;
	else if ( DataManagerNonDb.PMSBooking.GetSize() == 0 )
	{
		CString strTestFile;
		strTestFile.Format( "%s\\1.dat", PMSOptions.GetPMSFolderBooking() );
		
		if ( ::FileExists( strTestFile ) == TRUE )
			nError = 2;
		else
			nError = 3;
	}

	if ( 0 != nError ) 
	{
		int nSizeOld = DataManagerNonDb.PMSBooking.GetSize();

		switch( nError )
		{
		case 1:
		case 2:
			{
				CString strMsg = "";

#ifndef STOCKMAN_SYSTEM
				strMsg += "Ecr Manager has detected a problem in the PMS booking index file.\n\n";
#else
				strMsg += "Stock Manager has detected a problem in the PMS booking index file.\n\n";
#endif
				strMsg += "The index file will be rebuilt from individual booking records.";
				Prompter.Error( strMsg );
			}
			break;
		}

		switch( nError )
		{
		case 1:
			PMSCalendarTable.RemoveAll();
			PMSZeroNightStayArray.RemoveAll();
			DataManagerNonDb.PMSBooking.RebuildFromBookingFiles();
			DataManagerNonDb.PMSClosure.BuildInternalTables();
			DataManagerNonDb.PMSBooking.BuildInternalTables();
			break;

		case 2:
		case 3:
			DataManagerNonDb.PMSBooking.RebuildFromBookingFiles();
			DataManagerNonDb.PMSBooking.BuildInternalTables();
			break;
		}

		int nSizeNew = DataManagerNonDb.PMSBooking.GetSize();
		
		CFilenameUpdater FnUp( SysFiles::PMSBooking );
		DataManagerNonDb.PMSBooking.Write( FnUp.GetFilenameToUse() );
		
		if ( ( 3 != nError ) || ( DataManagerNonDb.PMSBooking.GetSize() > 0 ) )
		{
			CMessageLoggerInfo infoMsg;
			infoMsg.m_strMessage.Format ( "Rebuilt PMS Booking File [%d:%d:%d]", nError, nSizeOld, nSizeNew );
			infoMsg.m_strStatus = "";
			infoMsg.m_nHideLevel = 2;
			infoMsg.m_nNetworkIdx = 0;
			infoMsg.m_bAddDate = TRUE;
			infoMsg.m_bAddTimeOfDay = TRUE;
			infoMsg.m_bAddNetworkName = FALSE;
			MessageLogger.LogSchedulerMessage( infoMsg, FALSE, TRUE, FALSE );
		}
	}
}

/**********************************************************************/

int CPMSBookingCSVArray::GetCurrentDayNumber()
{
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	return GetDayNumberSince2011( dateNow );
}

/**********************************************************************/
#endif
/**********************************************************************/
