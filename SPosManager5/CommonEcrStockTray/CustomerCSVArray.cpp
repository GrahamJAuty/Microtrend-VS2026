/**********************************************************************/
#include "math.h"
/**********************************************************************/
#include "..\CommonEcrStock\ListDataCustomerAccountDlg.h"
#include "LocationCSVArray.h"
#include "PMSBookingCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CustomerCSVArray.h"
/**********************************************************************/

CCustomerDeletionNote::CCustomerDeletionNote()
{
	Reset();
}

/**********************************************************************/

void CCustomerDeletionNote::Reset()
{
	m_nCustomerID = 0;
	m_strDate = "";
}

/**********************************************************************/

void CCustomerDeletionNote::Add ( CCustomerDeletionNote& source )
{
	m_strDate = source.m_strDate;
}

/**********************************************************************/

int CCustomerDeletionNote::Compare( CCustomerDeletionNote& source, int nHint )
{
	if ( m_nCustomerID != source.m_nCustomerID )
		return ( m_nCustomerID > source.m_nCustomerID ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCustomerCSVRecord::CCustomerCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CCustomerCSVRecord::ClearRecord()
{
	m_nCustomerID = 1;
	m_nLineType = CUSTOMER_LINETYPE_CUSTOMER;
	m_strCustomerName1 = "";
	m_strCustomerName2 = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strPostcode = "";
	m_strEmail = "";
	m_bEmailRedirect = FALSE;
	m_strPhone1 = "";
	m_strPhone2 = "";
	m_strPhone3 = "";
	m_bSpecifyStartDay = FALSE;
	m_nStartDay = 0;
	m_nStartHour = 0;
	m_nStartMinute = 0;
	m_dStartBalance = 0.0;
	m_nPosCreationDay = -5000;
	m_nPosCreationTime = 0;
	m_nRecordCreationDay = 0;
	m_nRecordCreationTime = -1;
	m_bAutoDeleteFlag = FALSE;
	m_nAutoDeleteDay = 0;

	m_nDeletionStatus = CUSTOMER_DELETIONSTATUS_CANDELETE;
	m_strTempDate = "";
	m_bMoveToOtherArray = FALSE;
}

/**********************************************************************/

void CCustomerCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetCustomerID( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	case 2:	V2ConvertFromCSV ( csv );		break;
	case 3:	V3ConvertFromCSV ( csv );		break;
	case 4:	V4To9ConvertFromCSV ( csv, 4 );	break;
	case 5:	V4To9ConvertFromCSV ( csv, 5 );	break;
	case 6:	V4To9ConvertFromCSV ( csv, 6 );	break;
	case 7:	V4To9ConvertFromCSV ( csv, 7 );	break;
	case 8:	V4To9ConvertFromCSV ( csv, 8 );	break;
	case 9:	V4To9ConvertFromCSV ( csv, 9 );	break;
	}
}

/**********************************************************************/

void CCustomerCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetCustomerName1	( csv.GetString(2) );
	SetStartDay			( csv.GetInt(3) );
	SetStartHour		( csv.GetInt(4) );
	SetStartMinute		( csv.GetInt(5) );
	SetStartBalance		( csv.GetDouble(6) );
}

/**********************************************************************/

void CCustomerCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	SetCustomerName1	( csv.GetString(2) );
	SetCustomerName2	( csv.GetString(3) );
	SetStartDay			( csv.GetInt(4) );
	SetStartHour		( csv.GetInt(5) );
	SetStartMinute		( csv.GetInt(6) );
	SetStartBalance		( csv.GetDouble(7) );
}

/**********************************************************************/

void CCustomerCSVRecord::V3ConvertFromCSV ( CCSV& csv )
{
	SetLineType				( csv.GetInt(2) );
	SetCustomerName1		( csv.GetString(3) );
	SetCustomerName2		( csv.GetString(4) );
	SetSpecifyStartDayFlag	( csv.GetBool(5) );
	SetStartDay				( csv.GetInt(6) );
	SetStartHour			( csv.GetInt(7) );
	SetStartMinute			( csv.GetInt(8) );
	SetStartBalance			( csv.GetDouble(9) );
}

/**********************************************************************/

void CCustomerCSVRecord::V4To9ConvertFromCSV ( CCSV& csv, int nVer )
{
	SetLineType				( csv.GetInt(2) );
	SetCustomerName1		( csv.GetString(3) );
	SetCustomerName2		( csv.GetString(4) );
	SetAddress1				( csv.GetString(5) );
	SetAddress2				( csv.GetString(6) );
	SetAddress3				( csv.GetString(7) );
	SetPostcode				( csv.GetString(8) );

	int nOffset = 9;

	if ( nVer >= 5 )
	{
		SetEmail			( csv.GetString( nOffset++ ) );
	}

	if ( nVer >= 6 )
	{
		SetEmailRedirectFlag( csv.GetBool( nOffset++ ) );
	}

	if ( nVer >= 8 )
	{
		SetPhone1				( csv.GetString(nOffset++) );
		SetPhone2				( csv.GetString(nOffset++) );
		SetPhone3				( csv.GetString(nOffset++) );
	}

	SetSpecifyStartDayFlag	( csv.GetBool(nOffset++) );
	SetStartDay				( csv.GetInt(nOffset++) );
	SetStartHour			( csv.GetInt(nOffset++) );
	SetStartMinute			( csv.GetInt(nOffset++) );
	SetStartBalance			( csv.GetDouble(nOffset++) );

	if ( ( 7 == nVer ) || ( 8 == nVer ) )
	{
		SetRecordCreationDay( csv.GetInt(nOffset++) );
	}
	
	if ( nVer >= 9 )
	{
		SetPosCreationDay( csv.GetInt(nOffset++) );
		SetPosCreationTime( csv.GetInt(nOffset++) );
		SetRecordCreationDay( csv.GetInt(nOffset++) );
		SetRecordCreationTime( csv.GetInt(nOffset++) );
	}

	if ( nVer >= 7 )
	{
		SetAutoDeleteFlag	( csv.GetBool(nOffset++) );
		SetAutoDeleteDay	( csv.GetInt(nOffset++) );
	}

	SetDeletionStatus		( csv.GetInt(nOffset++) );
	SetTempDate				( csv.GetString(nOffset++) );
	SetMoveToOtherArrayFlag ( csv.GetBool(nOffset++) );
}

/**********************************************************************/

void CCustomerCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nCustomerID );
	csv.Add ( CUSTOMER_VERSION );
	csv.Add ( m_nLineType );
	csv.Add ( m_strCustomerName1 );
	csv.Add ( m_strCustomerName2 );
	csv.Add ( m_strAddress1 );
	csv.Add ( m_strAddress2 );
	csv.Add ( m_strAddress3 );
	csv.Add ( m_strPostcode );
	csv.Add ( m_strEmail );
	csv.Add ( m_bEmailRedirect );
	csv.Add ( m_strPhone1 );
	csv.Add ( m_strPhone2 );
	csv.Add ( m_strPhone3 );
	csv.Add ( m_bSpecifyStartDay );
	csv.Add ( m_nStartDay );
	csv.Add ( m_nStartHour );
	csv.Add ( m_nStartMinute );
	csv.Add ( m_dStartBalance, 2 );
	csv.Add ( m_nPosCreationDay );
	csv.Add ( m_nPosCreationTime );
	csv.Add ( m_nRecordCreationDay );
	csv.Add ( m_nRecordCreationTime );
	csv.Add ( m_bAutoDeleteFlag );
	csv.Add ( m_nAutoDeleteDay );
	csv.Add ( m_nDeletionStatus );
	csv.Add ( m_strTempDate );
	csv.Add ( m_bMoveToOtherArray );
}

/**********************************************************************/

void CCustomerCSVRecord::SetCustomerID( int nID )
{
	Set ( m_nCustomerID, nID, Customer::CustomerID );
}

/**********************************************************************/

void CCustomerCSVRecord::SetLineType( int n )
{
	Set ( m_nLineType, n, Customer::LineType );
}

/**********************************************************************/

void CCustomerCSVRecord::SetCustomerName1 ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strCustomerName1, strName, Customer::CustomerName1 );
}

/**********************************************************************/

void CCustomerCSVRecord::SetCustomerName2 ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strCustomerName2, strName, Customer::CustomerName2 );
}

/**********************************************************************/

void CCustomerCSVRecord::SetAddress1 ( const char* sz )
{
	CString strAddress = sz;
	::TrimSpaces ( strAddress, FALSE );
	Set( m_strAddress1, strAddress, Customer::Address );
}

/**********************************************************************/

void CCustomerCSVRecord::SetAddress2 ( const char* sz )
{
	CString strAddress = sz;
	::TrimSpaces ( strAddress, FALSE );
	Set( m_strAddress2, strAddress, Customer::Address );
}

/**********************************************************************/

void CCustomerCSVRecord::SetAddress3 ( const char* sz )
{
	CString strAddress = sz;
	::TrimSpaces ( strAddress, FALSE );
	Set( m_strAddress3, strAddress, Customer::Address );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPhone1 ( const char* sz )
{
	CString strPhone = sz;
	::TrimSpaces ( strPhone, FALSE );
	Set( m_strPhone1, strPhone, Customer::Phone );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPhone2 ( const char* sz )
{
	CString strPhone = sz;
	::TrimSpaces ( strPhone, FALSE );
	Set( m_strPhone2, strPhone, Customer::Phone );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPhone3 ( const char* sz )
{
	CString strPhone = sz;
	::TrimSpaces ( strPhone, FALSE );
	Set( m_strPhone3, strPhone, Customer::Phone );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPostcode ( const char* sz )
{
	CString strPostcode = sz;
	::TrimSpaces ( strPostcode, FALSE );
	Set( m_strPostcode, strPostcode, Customer::Postcode );
}

/**********************************************************************/

void CCustomerCSVRecord::SetEmail ( const char* sz )
{
	CString strEmail = sz;
	::TrimSpaces ( strEmail, FALSE );
	Set( m_strEmail, strEmail, Customer::Email );
}

/**********************************************************************/

void CCustomerCSVRecord::SetEmailRedirectFlag( bool b )
{
	m_bEmailRedirect = b;
}

/**********************************************************************/

void CCustomerCSVRecord::SetSpecifyStartDayFlag( bool b )
{
	m_bSpecifyStartDay = b;
}

/**********************************************************************/

void CCustomerCSVRecord::SetStartDay( int n )
{
	Set ( m_nStartDay, n, Customer::StartDay );
}

/**********************************************************************/

void CCustomerCSVRecord::SetStartHour( int n )
{
	Set ( m_nStartHour, n, Customer::StartHour );
}

/**********************************************************************/

void CCustomerCSVRecord::SetStartMinute( int n )
{
	Set ( m_nStartMinute, n, Customer::StartMinute );
}

/**********************************************************************/

void CCustomerCSVRecord::SetStartBalance( double d )
{
	Set ( m_dStartBalance, d, Customer::StartBalance );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPosCreationTime( double d )
{
	int nDay = int( floor ( d ) );
	
	d -= (double) nDay;
	d *= 1440.0;

	int nTime = int ( floor( d ) );

	SetPosCreationDay( nDay - 40542 );
	SetPosCreationTime( nTime );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPosCreationDay( int n )
{
	Set ( m_nPosCreationDay, n, Customer::PosCreationDay );
}

/**********************************************************************/

void CCustomerCSVRecord::SetPosCreationTime( int n )
{
	Set ( m_nPosCreationTime, n, Customer::PosCreationTime );
}

/**********************************************************************/

void CCustomerCSVRecord::SetRecordCreationDay( int n )
{
	Set ( m_nRecordCreationDay, n, Customer::RecordCreationDay );
}

/**********************************************************************/

void CCustomerCSVRecord::SetRecordCreationTime( int n )
{
	Set ( m_nRecordCreationTime, n, Customer::RecordCreationTime );
}

/**********************************************************************/

void CCustomerCSVRecord::SetAutoDeleteDay( int n )
{
	Set ( m_nAutoDeleteDay, n, Customer::AutoDeleteDay );
}

/**********************************************************************/

void CCustomerCSVRecord::SetDeletionStatus( int n )
{
	Set ( m_nDeletionStatus, n, Customer::DeletionStatus );
}

/**********************************************************************/

void CCustomerCSVRecord::SetTempDate( const char* sz )
{
	Set ( m_strTempDate, sz, Customer::TempDate );
}

/**********************************************************************/

void CCustomerCSVRecord::SetMoveToOtherArrayFlag( bool b )
{
	m_bMoveToOtherArray = b;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayName1()
{
	CString strTemp1 = m_strCustomerName1;
	::TrimSpaces ( strTemp1, FALSE );

	CString strTemp2 = m_strCustomerName2;
	::TrimSpaces ( strTemp2, FALSE );

	if ( ( strTemp1 == "" ) && ( strTemp2 == "" ) )
		m_strDisplayName1.Format ( "Customer %d", m_nCustomerID );
	else
		m_strDisplayName1 = strTemp1;
	
	return m_strDisplayName1;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayName2()
{
	m_strDisplayName2 = m_strCustomerName2;
	::TrimSpaces ( m_strDisplayName2, FALSE );
	return m_strDisplayName2;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayNameFull()
{
	CString strName1 = GetDisplayName1();
	CString strName2 = GetDisplayName2();

	m_strDisplayNameFull = strName1;

	if ( strName2 != "" )
	{
		if ( strName1 != "" )
			m_strDisplayNameFull += " ";

		m_strDisplayNameFull += strName2;
	}

	return m_strDisplayNameFull;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayDateBalance()
{
	if ( FALSE == m_bSpecifyStartDay )
		m_strDisplayDateBalance = "";
	else
	{
		COleDateTime date;
		GetOleDateSince2011( m_nStartDay, date );
	
		m_strDisplayDateBalance.Format( "%2.2d/%2.2d/%4.4d",
			date.GetDay(),
			date.GetMonth(),
			date.GetYear() );
	}

	return m_strDisplayDateBalance;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayPosDateCreated()
{
	if ( m_nPosCreationDay <= Customer::PosCreationDay.Min )
		m_strDisplayPosDateCreated = "----";
	else
	{
		COleDateTime date;
		GetOleDateSince2011( m_nPosCreationDay, date );
	
		m_strDisplayPosDateCreated.Format( "%2.2d/%2.2d/%4.4d",
			date.GetDay(),
			date.GetMonth(),
			date.GetYear() );

		if ( ( m_nPosCreationTime >= 0 ) && ( m_nPosCreationTime <= 1439 ) )
		{
			CString strExtra;
			strExtra.Format( "  %2.2d:%2.2d", m_nPosCreationTime / 60, m_nPosCreationTime % 60 );
			m_strDisplayPosDateCreated += strExtra;
		}
	}

	return m_strDisplayPosDateCreated;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayRecordDateCreated()
{
	if ( m_nRecordCreationDay <= 0 )
		m_strDisplayRecordDateCreated = "----";
	else
	{
		COleDateTime date;
		GetOleDateSince2011( m_nRecordCreationDay, date );
	
		m_strDisplayRecordDateCreated.Format( "%2.2d/%2.2d/%4.4d",
			date.GetDay(),
			date.GetMonth(),
			date.GetYear() );

		if ( ( m_nRecordCreationTime >= 0 ) && ( m_nRecordCreationTime <= 1439 ) )
		{
			CString strExtra;
			strExtra.Format( "  %2.2d:%2.2d", m_nRecordCreationTime / 60, m_nRecordCreationTime % 60 );
			m_strDisplayRecordDateCreated += strExtra;
		}
	}

	return m_strDisplayRecordDateCreated;
}

/**********************************************************************/

const char* CCustomerCSVRecord::GetDisplayDateAutoDelete()
{
	if ( FALSE == m_bAutoDeleteFlag )
		m_strDisplayDateAutoDelete = "----";
	else
	{
		COleDateTime date;
		GetOleDateSince2011( m_nAutoDeleteDay, date );
	
		m_strDisplayDateAutoDelete.Format( "%2.2d/%2.2d/%4.4d",
			date.GetDay(),
			date.GetMonth(),
			date.GetYear() );
	}

	return m_strDisplayDateAutoDelete;
}

/**********************************************************************/

void CCustomerCSVRecord::InitCreateAndDeleteDates( bool bOneOff )
{
	COleDateTime dateNow = COleDateTime::GetCurrentTime();
	m_nRecordCreationDay = ::GetDayNumberSince2011( dateNow );
	m_nRecordCreationTime = ( dateNow.GetHour() * 60 ) + dateNow.GetMinute();

	int nBaseDay = GetLatestCreationDay();
	
	if ( FALSE == bOneOff )
	{
		if ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE )
		{
			m_bAutoDeleteFlag = TRUE;
			m_nAutoDeleteDay = nBaseDay + EcrmanOptions.GetFeaturesCustomerAccountDelDay();
		}
		else
		{
			m_bAutoDeleteFlag = FALSE;
			m_nAutoDeleteDay = 0;
		}
	}
	else
	{
		if ( EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() == TRUE )
		{
			m_bAutoDeleteFlag = TRUE;
			m_nAutoDeleteDay = nBaseDay + EcrmanOptions.GetFeaturesCustomerOneOffDelDay();
		}
		else
		{
			m_bAutoDeleteFlag = FALSE;
			m_nAutoDeleteDay = 0;
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CCustomerCSVArray::CCustomerCSVArray() 
{
}

/**********************************************************************/

CCustomerCSVArray::~CCustomerCSVArray() 
{
}

/**********************************************************************/

int CCustomerCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	int nLastCustomerID = 0;
	int nValidRecords = 1;
	bool bDeletionBlock = FALSE;
	bool bIsValid = TRUE;
	
	CCustomerCSVRecord CustomerRecord;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		GetAt( nValidRecords, CustomerRecord );

		if ( FALSE == bDeletionBlock )
		{
			if ( CustomerRecord.GetDeletionStatus() == CUSTOMER_DELETIONSTATUS_DELETED )
				bDeletionBlock = TRUE;
		}

		if ( FALSE == bDeletionBlock )
		{
			int nCustomerID = CustomerRecord.GetCustomerID();
			bIsValid = ( ( nCustomerID > nLastCustomerID ) && ( nCustomerID <= Customer::CustomerID.Max ) );
			nLastCustomerID = nCustomerID;
		}
		else
			bIsValid = ( CustomerRecord.GetDeletionStatus() == CUSTOMER_DELETIONSTATUS_DELETED );

		if ( FALSE == bIsValid )
			break;
	}

	while ( GetSize() > nValidRecords )
		RemoveAt( GetSize() - 1 );

	return nReply;
}

/**********************************************************************/

int CCustomerCSVArray::GetCustomerID ( int nIndex )
{
	int nID = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nID = csv.GetInt ( 0 );
	}
	
	return nID;
}

/**********************************************************************/

const char* CCustomerCSVArray::GetDisplayName1( int nCustomerID )
{
	if ( nCustomerID == 0 )
		m_strDisplayName1 = "None";
	else
	{
		m_strDisplayName1.Format ( "Customer %6.6d", nCustomerID );

		int nCustomerIdx;
		if ( FindCustomerByID( nCustomerID, nCustomerIdx ) == TRUE )
		{
			CCustomerCSVRecord Customer;
			GetAt ( nCustomerIdx, Customer );
			m_strDisplayName1 = Customer.GetDisplayName1();
		}
	}
	
	return m_strDisplayName1;
}

/**********************************************************************/

const char* CCustomerCSVArray::GetDisplayName2( int nCustomerID )
{
	if ( nCustomerID == 0 )
		m_strDisplayName2 = "";
	else
	{
		int nCustomerIdx;
		if ( FindCustomerByID( nCustomerID, nCustomerIdx ) == TRUE )
		{
			CCustomerCSVRecord Customer;
			GetAt ( nCustomerIdx, Customer );
			m_strDisplayName2 = Customer.GetDisplayName2();
		}
	}
	
	return m_strDisplayName2;
}

/**********************************************************************/

const char* CCustomerCSVArray::GetDisplayNameFull( int nCustomerID )
{
	if ( nCustomerID == 0 )
		m_strDisplayNameFull = "";
	else
	{
		int nCustomerIdx;
		if ( FindCustomerByID( nCustomerID, nCustomerIdx ) == TRUE )
		{
			CCustomerCSVRecord Customer;
			GetAt ( nCustomerIdx, Customer );
			m_strDisplayNameFull = Customer.GetDisplayNameFull();
		}
	}
	
	return m_strDisplayNameFull;
}

/**********************************************************************/

bool CCustomerCSVArray::FindCustomerByID ( int nCustomerID, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		int nArrayCustomerID = GetCustomerID ( nIndex );
		
		if ( nArrayCustomerID < nCustomerID )
			nStart = nIndex + 1;

		if ( nArrayCustomerID > nCustomerID )
			nEnd = nIndex - 1;

		if ( nArrayCustomerID == nCustomerID )
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

void CCustomerCSVArray::CreateCustomerReport( bool bOneOff, int nLocIdx, COleDateTime oleDateFrom, COleDateTime oleDateTo, bool bSPosDate, bool bEcrmanDate, CReportFile& ReportFile )
{
	CString strTitle = "";

	if ( TRUE == bOneOff )
	{
		strTitle = "One Off Customer Names";
	}
	else
	{
		strTitle = "Customer Account Details";
	}

	{
		CString strExtra = "";
		strExtra.Format( " (%s)", dbLocation.GetName( nLocIdx ) );
		strTitle + strExtra;
	}

	ReportFile.SetStyle1 ( strTitle );
		
	ReportFile.AddColumn ( "ID", TA_LEFT, 200 );
	
	if ( TRUE == bSPosDate )
	{
		ReportFile.AddColumn ( "SPOS Creation Date", TA_LEFT, 500 );
	}

	if ( TRUE == bEcrmanDate )
	{
		if ( SysInfo.IsStockSystem() == FALSE )
		{
			ReportFile.AddColumn ( "Ecrman Import Date", TA_LEFT, 500 );
		}
		else
		{
			ReportFile.AddColumn ( "Stockman Import Date", TA_LEFT, 500 );
		}
	}

	if ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE )
	{
		ReportFile.AddColumn ( "Auto Delete Date", TA_LEFT, 400 );
	}

	ReportFile.AddColumn ( "First Name", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Last Name", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Address1", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Address2", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Address3", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Postcode", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Email", TA_LEFT, 450 );
	ReportFile.AddColumn ( "Phone1", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Phone2", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Phone3", TA_LEFT, 300 );

	if ( FALSE == bOneOff )
	{
		ReportFile.AddColumn ( "Balance Start Date", TA_LEFT, 400 );
		ReportFile.AddColumn ( "Starting Balance", TA_RIGHT, 300 );
	}

	int nSize = 0;
	if ( TRUE == bOneOff )
	{
		nSize = DataManagerNonDb.CustomerNameTable.OneOff.GetSize();
	}
	else
	{
		nSize = DataManagerNonDb.CustomerNameTable.Account.GetSize();
	}

	bool bGotCustomer = FALSE;
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		int nCustomerIdx = 0;
		if ( TRUE == bOneOff )
		{
			nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( nIndex );
		}
		else
		{
			nCustomerIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( nIndex );
		}
		
		CCustomerCSVRecord CustomerRecord;
		GetAt ( nCustomerIdx, CustomerRecord );

		bool bAcceptDate = TRUE;

		if ( ( TRUE == bSPosDate ) || ( TRUE == bEcrmanDate ) )
		{
			bAcceptDate = FALSE;

			if ( TRUE == bSPosDate )
			{
				COleDateTime oleDateCreated;
				GetOleDateSince2011( CustomerRecord.GetPosCreationDay(), oleDateCreated );
				bAcceptDate = ( oleDateCreated >= oleDateFrom ) && ( oleDateCreated <= oleDateTo );
			}

			if ( ( TRUE == bEcrmanDate ) && ( FALSE == bAcceptDate ) )
			{
				COleDateTime oleDateCreated;
				GetOleDateSince2011( CustomerRecord.GetRecordCreationDay(), oleDateCreated );
				bAcceptDate = ( oleDateCreated >= oleDateFrom ) && ( oleDateCreated <= oleDateTo );
			}
		}

		if ( TRUE == bAcceptDate )
		{
			CString strID;
			strID.Format( "%8.8d", CustomerRecord.GetCustomerID() );

			CString strEmail;
			if ( ( FALSE == bOneOff ) && ( CustomerRecord.GetEmailRedirectFlag() == TRUE ) )
			{
				strEmail = "Redirect to printer";
			}
			else
			{
				strEmail = CustomerRecord.GetEmail();
			}

			CCSV csvOut ( '\t' );
			csvOut.Add ( strID );

			if ( TRUE == bSPosDate )
			{
				csvOut.Add ( CustomerRecord.GetDisplayPosDateCreated() );
			}

			if ( TRUE == bEcrmanDate )
			{
				csvOut.Add ( CustomerRecord.GetDisplayRecordDateCreated() );
			}
			
			if ( EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag() == TRUE )
			{
				csvOut.Add ( CustomerRecord.GetDisplayDateAutoDelete() );
			}

			csvOut.Add ( CustomerRecord.GetDisplayName1() );
			csvOut.Add ( CustomerRecord.GetDisplayName2() );
			csvOut.Add ( CustomerRecord.GetAddress1() );
			csvOut.Add ( CustomerRecord.GetAddress2() );
			csvOut.Add ( CustomerRecord.GetAddress3() );
			csvOut.Add ( CustomerRecord.GetPostcode() );
			csvOut.Add ( strEmail );
			csvOut.Add ( CustomerRecord.GetPhone1() );
			csvOut.Add ( CustomerRecord.GetPhone2() );
			csvOut.Add ( CustomerRecord.GetPhone3() );

			if ( FALSE == bOneOff )
			{
				csvOut.Add ( CustomerRecord.GetDisplayDateBalance() );
				csvOut.Add ( CustomerRecord.GetStartBalance(), 2 );
			}

			ReportFile.WriteLine ( csvOut.GetLine() );
			bGotCustomer = TRUE;
		}
	}

	if ( FALSE == bGotCustomer )
	{
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "<..>No customers were found matching creation date filter" );
	}
}

/**********************************************************************/

void CCustomerCSVArray::Sync( CCustomerCSVArray& source, CCustomerCSVArray& block, bool bAdd, bool bRemove, int& nAdded, int& nRemoved, int& nFirstNewID, bool bAuto )
{
	nAdded = 0;
	nRemoved = 0;
	nFirstNewID = 0;

	//DELETE CUSTOMERS THAT ARE NOT PRESENT IN THE CASHRSP DATA
	if ( TRUE == bRemove )
	{
		for ( int n = GetSize() - 1; n >= 0; n-- )
		{
			int nID = GetCustomerID(n);

			int nIdx;
			if ( source.FindCustomerByID( nID, nIdx ) == FALSE )
			{
				nRemoved++;
				RemoveAt(n);
			}
		}
	}

	//ADD NEW CUSTOMERS FROM THE CASHRSP DATA
	if ( TRUE == bAdd )
	{
		for ( int n = 0; n < source.GetSize(); n++ )
		{
			int nID = source.GetCustomerID(n);

			int nCustomerIdx;
			if ( FindCustomerByID( nID, nCustomerIdx ) == FALSE )
			{
				if ( ( FALSE == bAuto ) || ( IsRecentlyDeleted( nID ) == FALSE ) )
				{
					int nBlockIdx;
					if ( block.FindCustomerByID( nID, nBlockIdx ) == FALSE )
					{
						CCustomerCSVRecord CustomerRecord;
						source.GetAt( n, CustomerRecord );
						CustomerRecord.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_CANDELETE );
						CustomerRecord.InitCreateAndDeleteDates(FALSE);
						InsertAt( nCustomerIdx, CustomerRecord );
						
						if ( 0 == nAdded++ )
							nFirstNewID = nID;
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CCustomerCSVArray::LoadHeaderStartDateRecord()
{
	m_StartDateRecord.ClearRecord();

	if ( GetSize() != 0 )
	{
		CCustomerCSVRecord CustomerRecord;
		GetAt( 0, CustomerRecord );

		if ( CustomerRecord.GetLineType() == CUSTOMER_LINETYPE_HEADER )
		{
			GetAt( 0, m_StartDateRecord );
			RemoveAt( 0 );
		}
	}

	m_StartDateRecord.SetLineType( CUSTOMER_LINETYPE_HEADER );
	m_StartDateRecord.SetSpecifyStartDayFlag( TRUE );
}

/**********************************************************************/

void CCustomerCSVArray::SaveHeaderStartDateRecord()
{
	m_StartDateRecord.SetLineType( CUSTOMER_LINETYPE_HEADER );
	m_StartDateRecord.SetSpecifyStartDayFlag( TRUE );

	if ( GetSize() != 0 )
	{
		CCustomerCSVRecord CustomerRecord;
		GetAt( 0, CustomerRecord );

		if ( CustomerRecord.GetLineType() == CUSTOMER_LINETYPE_HEADER )
			SetAt( 0, m_StartDateRecord );
		else
			InsertAt( 0, m_StartDateRecord );
	}
}

/**********************************************************************/

void CCustomerCSVArray::GetStartDateRecord( CCustomerCSVRecord& Record )
{
	CCustomerCSVArray arrayTemp;
	arrayTemp.Add( m_StartDateRecord );
	arrayTemp.GetAt( 0, Record );

	Record.SetLineType( CUSTOMER_LINETYPE_HEADER );
	Record.SetSpecifyStartDayFlag( TRUE );
}

/**********************************************************************/

void CCustomerCSVArray::SetStartDateRecord( CCustomerCSVRecord& Record )
{
	Record.SetLineType( CUSTOMER_LINETYPE_HEADER );
	Record.SetSpecifyStartDayFlag( TRUE );
	
	CCustomerCSVArray arrayTemp;
	arrayTemp.Add( Record );
	arrayTemp.GetAt( 0, m_StartDateRecord );
}

/**********************************************************************/

void CCustomerCSVArray::SetDeletionStatus( int nStatus )
{
	switch( nStatus )
	{
	case CUSTOMER_DELETIONSTATUS_PROTECTED:
	case CUSTOMER_DELETIONSTATUS_CANDELETE:
	case CUSTOMER_DELETIONSTATUS_DELETENOW:
	case CUSTOMER_DELETIONSTATUS_DELETED:
		{
			for ( int n = 0; n < GetSize(); n++ )
			{
				CCustomerCSVRecord Customer;
				GetAt( n, Customer );
				Customer.SetDeletionStatus(nStatus);
				SetAt( n, Customer );
			}
		}
		break;
	}
}

/**********************************************************************/

bool CCustomerCSVArray::IsRecentlyDeleted( int nID )
{
	CCustomerDeletionNote info;
	info.m_nCustomerID = nID;

	int nPos;
	return m_arrayRecentDeletions.Find( info, nPos );
}

/**********************************************************************/

void CCustomerCSVArray::FlagDeletion( int nCustomerID, const char* szDate )
{
	CCustomerDeletionNote info;
	info.m_nCustomerID = nCustomerID;
	info.m_strDate = szDate;
	m_arrayRecentDeletions.Consolidate(info);
}

/**********************************************************************/

void CCustomerCSVArray::MoveDeletionsToHelperArray()
{
	m_arrayRecentDeletions.RemoveAll();

	COleDateTime dateCutOff = COleDateTime::GetCurrentTime() - COleDateTimeSpan( 7, 0, 0, 0 );

	CString strDateCutOff;
	strDateCutOff.Format( "%4.4d%2.2d%2.2d", dateCutOff.GetYear(), dateCutOff.GetMonth(), dateCutOff.GetDay() );

	bool bDeletionBlock = TRUE;
	for ( int n = GetSize() - 1; ( n >= 0 ) && ( TRUE == bDeletionBlock ); n-- )
	{
		CCustomerCSVRecord Customer;
		GetAt( n, Customer );

		bDeletionBlock = ( Customer.GetDeletionStatus() == CUSTOMER_DELETIONSTATUS_DELETED );

		if ( TRUE == bDeletionBlock ) 
		{
			RemoveAt(n);

			CString strCustomerDate = Customer.GetTempDate();
			if ( strDateCutOff < strCustomerDate )
			{
				CCustomerDeletionNote info;
				info.m_nCustomerID = Customer.GetCustomerID();
				info.m_strDate = strCustomerDate;
				m_arrayRecentDeletions.Consolidate(info);
			}
		}
	}
}

/**********************************************************************/

void CCustomerCSVArray::MoveDeletionsToMainArray()
{
	COleDateTime dateCutOff = COleDateTime::GetCurrentTime() - COleDateTimeSpan( 7, 0, 0, 0 );

	CString strDateCutOff;
	strDateCutOff.Format( "%4.4d%2.2d%2.2d", dateCutOff.GetYear(), dateCutOff.GetMonth(), dateCutOff.GetDay() );

	for ( int n = m_arrayRecentDeletions.GetSize() - 1; n >= 0; n-- )
	{
		CCustomerDeletionNote info;
		m_arrayRecentDeletions.GetAt( n, info );

		if ( strDateCutOff < info.m_strDate )
		{
			CCustomerCSVRecord Customer;
			Customer.SetCustomerID( info.m_nCustomerID );
			Customer.SetTempDate( info.m_strDate );
			Customer.SetDeletionStatus( CUSTOMER_DELETIONSTATUS_DELETED );
			Add( Customer );
		}
		else
			m_arrayRecentDeletions.RemoveAt(n);
	}
}

/**********************************************************************/

void CCustomerCSVArray::ClearMoveToOtherArrayFlags()
{
	for ( int n = 0; n < GetSize(); n++ )
	{
		CCustomerCSVRecord Customer;
		GetAt( n, Customer );
		Customer.SetMoveToOtherArrayFlag(FALSE);
		SetAt( n, Customer );
	}
}

/**********************************************************************/

void CCustomerCSVArray::MoveCustomers( CCustomerCSVArray& Destination )
{
	for ( int n = GetSize() - 1; n >= 0; n-- )
	{
		CCustomerCSVRecord Customer;
		GetAt( n, Customer );

		if ( Customer.GetMoveToOtherArrayFlag() == TRUE )
		{
			RemoveAt(n);
			Customer.SetMoveToOtherArrayFlag( FALSE );
			
			int nIdx;
			if ( Destination.FindCustomerByID( Customer.GetCustomerID(), nIdx ) == FALSE )
				Destination.InsertAt( nIdx, Customer );
			else
				Destination.SetAt( nIdx, Customer );
		}
	}	
}

/**********************************************************************/

void CCustomerCSVArray::AddFromIDList( CArray<int,int>& arrayIDs, CCustomerCSVArray& arrayBlock, bool bOneOff )
{
	for ( int n = 0; n < arrayIDs.GetSize(); n++ )
	{
		int nID = arrayIDs.GetAt(n);
		if ( ( nID >= Customer::CustomerID.Min ) && ( nID <= Customer::CustomerID.Max ) )
		{
			int nPos;
			if ( arrayBlock.FindCustomerByID( nID, nPos ) == FALSE )
			{
				if ( FindCustomerByID( nID, nPos ) == FALSE )
				{
					CCustomerCSVRecord Customer;
					Customer.SetCustomerID( nID );
					Customer.SetSpecifyStartDayFlag( FALSE );
					Customer.InitCreateAndDeleteDates( bOneOff );
					InsertAt( nPos, Customer );
				}
			}
		}
	}
}

/**********************************************************************/
