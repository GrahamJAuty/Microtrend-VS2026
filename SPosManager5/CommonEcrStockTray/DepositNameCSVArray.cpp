 /**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "DepositNameCSVArray.h"
/**********************************************************************/

CDepositNameCSVRecord::CDepositNameCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CDepositNameCSVRecord::ClearRecord()
{
	m_nDepositID = 1;
	m_nServerNo = 0;
	m_nDepositAmount = 0;
	m_strTaxBand = "";
	m_nTaxAmount = 0;
	m_strBuyerName = "";
	m_strBookingDate = "";
	m_strBookingTime = "";

	for ( int n = 0; n < DEPOSITINFO_LINES; n++ )
		m_strCustomerInfo[n] = "";
}

/**********************************************************************/

void CDepositNameCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetDepositID( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1to4ConvertFromCSV ( csv, 1 );		break;
	case 2:	V1to4ConvertFromCSV ( csv, 2 );		break;
	case 3:	V1to4ConvertFromCSV ( csv, 3 );		break;
	case 4:	V1to4ConvertFromCSV ( csv, 4 );		break;
	}
}

/**********************************************************************/

void CDepositNameCSVRecord::V1to4ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nPos = 2;
	if ( nVer >= 4 )
	{
		SetServerNo( csv.GetInt(nPos++) );
		SetDepositAmount( csv.GetInt(nPos++) );
		SetTaxBand( csv.GetString(nPos++) );
		SetTaxAmount( csv.GetInt(nPos++) );
	}

	SetBuyerName	( csv.GetString(nPos++) );

	if ( nVer >= 2 )
	{
		SetBookingDate( csv.GetString(nPos++) );
		SetBookingTime( csv.GetString(nPos++) );
	}

	if ( nVer >= 3 )
	{
		for ( int n = 0; n < DEPOSITINFO_LINES; n++ )
			SetCustomerInfo( n, csv.GetString( nPos++ ) );
	}
}

/**********************************************************************/
	
void CDepositNameCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nDepositID );
	csv.Add ( DEPOSITNAME_VERSION );
	csv.Add ( m_nServerNo );
	csv.Add ( m_nDepositAmount );
	csv.Add ( m_strTaxBand );
	csv.Add ( m_nTaxAmount );
	csv.Add ( m_strBuyerName );
	csv.Add ( m_strBookingDate );
	csv.Add ( m_strBookingTime );

	for ( int n = 0; n < DEPOSITINFO_LINES; n++ )
		csv.Add ( m_strCustomerInfo[n] );
}

/**********************************************************************/

const char* CDepositNameCSVRecord::GetCustomerInfo ( int n )
{
	if ( ( n >= 0 ) && ( n < DEPOSITINFO_LINES ) )
		m_strCustomerInfoOut = m_strCustomerInfo[n];
	else
		m_strCustomerInfoOut = "";

	return m_strCustomerInfoOut;
}

/**********************************************************************/

void CDepositNameCSVRecord::SetDepositID( int n )
{
	Set ( m_nDepositID, n, DepositName::DepositID );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetServerNo( int n )
{
	Set ( m_nServerNo, n, DepositName::ServerNo );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetDepositAmount( int n )
{
	Set ( m_nDepositAmount, n, DepositName::DepositAmount );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetTaxBand( const char* sz )
{
	Set ( m_strTaxBand, sz, DepositName::TaxBand );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetTaxAmount( int n )
{
	Set ( m_nTaxAmount, n, DepositName::TaxAmount );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetBuyerName ( const char* szName )
{
	Set ( m_strBuyerName, szName, DepositName::BuyerName );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetBookingDate ( const char* szInfo )
{
	Set ( m_strBookingDate, szInfo, DepositName::BookingDate );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetBookingTime ( const char* szInfo )
{
	Set ( m_strBookingTime, szInfo, DepositName::BookingTime );
}

/**********************************************************************/

void CDepositNameCSVRecord::SetCustomerInfo ( int n, const char* szInfo )
{
	if ( ( n >= 0 ) && ( n < DEPOSITINFO_LINES ) )
		Set ( m_strCustomerInfo[n], szInfo, DepositName::CustomerInfo );
}

/**********************************************************************/

const char* CDepositNameCSVRecord::GetCustomerInfoDisplayLine()
{
	CString strTemp = "";
	for ( int n = 0; n < DEPOSITINFO_LINES; n++ )
	{
		CString strInfoLine = GetCustomerInfo(n);
		::TrimSpaces( strInfoLine, TRUE );

		if ( strInfoLine != "" )
		{
			if ( strTemp != "" )
				strTemp += " ";

			strTemp += strInfoLine;
		}
	}
	
	m_strCustomerInfoOut = strTemp;
	return m_strCustomerInfoOut;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CDepositNameCSVArray::CDepositNameCSVArray() 
{
}

/**********************************************************************/

CDepositNameCSVArray::~CDepositNameCSVArray() 
{
}

/**********************************************************************/

int CDepositNameCSVArray::Open( const char* szFilename, int nMode )
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

bool CDepositNameCSVArray::FindDepositByID( int nDepositID, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CDepositNameCSVRecord DepositName;
		GetAt ( nIndex, DepositName );
		int nArrayID = DepositName.GetDepositID();
			
		if ( nDepositID == nArrayID )
			return TRUE;

		if ( nDepositID < nArrayID )
			nEnd = nIndex - 1;
		else
			nStart = nIndex + 1;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

void CDepositNameCSVArray::GetDepositByID( int nDepositID, CDepositNameCSVRecord& DepositRecord )
{
	int nIdx;
	if ( FindDepositByID( nDepositID, nIdx ) == TRUE )
		GetAt( nIdx, DepositRecord );
	else
		DepositRecord.ClearRecord();
}

/**********************************************************************/

const char* CDepositNameCSVArray::GetBuyerNameByDepositID( int nDepositID )
{
	m_strBuyerName = "";

	int nIdx;
	if ( FindDepositByID( nDepositID, nIdx ) == TRUE )
	{
		CDepositNameCSVRecord DepositRecord;
		GetAt( nIdx, DepositRecord );
		m_strBuyerName = DepositRecord.GetBuyerName();
	}

	return m_strBuyerName;
}

/**********************************************************************/

