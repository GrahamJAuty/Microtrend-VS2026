/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/
#include "TableNameCSVArray.h"
/**********************************************************************/

CTableNameCSVRecord::CTableNameCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTableNameCSVRecord::ClearRecord()
{
	m_nTableNo = 1;
	m_strTableName = "";
	m_bShowAsFilter = TRUE;
	m_bAccountTable = TRUE;
	m_bEnableLimit = TRUE;
	m_nLimitPounds = 0;
	m_bAutoStatusEnable = FALSE;
	m_bAutoStatusUnusedAfterPayment = FALSE;
	m_nReportGroup[0] = 1;
	m_nReportGroup[1] = 1;
	m_nReportGroup[2] = 1;
	m_nReportGroup[3] = 1;
}

/**********************************************************************/

void CTableNameCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetTableNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CTableNameCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetTableName							( csv.GetString(2) );
	SetShowAsFilterFlag						( csv.GetBool(3) );
	SetAccountTableFlag						( csv.GetBool(4) );
	SetEnableLimitFlag						( csv.GetBool(5) );
	SetLimitPounds							( csv.GetInt(6) );
	SetAutoStatusEnableFlag					( csv.GetBool(7) );
	SetAutoStatusUnusedAfterPaymentFlag		( csv.GetBool(8) );
	SetReportGroup							( 1, csv.GetInt(9) );
	SetReportGroup							( 2, csv.GetInt(10) );
	SetReportGroup							( 3, csv.GetInt(11) );
}

/**********************************************************************/

void CTableNameCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nTableNo );
	csv.Add ( TABLENAME_VERSION );
	csv.Add ( m_strTableName );
	csv.Add ( m_bShowAsFilter );
	csv.Add ( m_bAccountTable );
	csv.Add ( m_bEnableLimit );
	csv.Add ( m_nLimitPounds );
	csv.Add ( m_bAutoStatusEnable );
	csv.Add ( m_bAutoStatusUnusedAfterPayment );
	csv.Add ( m_nReportGroup[1] );
	csv.Add ( m_nReportGroup[2] );
	csv.Add ( m_nReportGroup[3] );
}

/**********************************************************************/

int CTableNameCSVRecord::GetReportGroup( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_nReportGroup[n];
	else
		return 0;
}

/**********************************************************************/

void CTableNameCSVRecord::SetTableNo( int n )
{
	Set ( m_nTableNo, n, SPosTableName::TableNo );
}

/**********************************************************************/

void CTableNameCSVRecord::SetTableName( const char* sz )
{
	Set ( m_strTableName, sz, SPosTableName::TableName );
}

/**********************************************************************/

void CTableNameCSVRecord::SetLimitPounds( int n )
{
	Set ( m_nLimitPounds, n, SPosTableName::LimitPounds );
}

/**********************************************************************/

void CTableNameCSVRecord::SetReportGroup( int n, int nGrp )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
	{
		Set ( m_nReportGroup[n], nGrp, SPosTableName::ReportGroup );
	}
}

/**********************************************************************/

void CTableNameCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTableNameCSVArray::CTableNameCSVArray() 
{
}

/**********************************************************************/

CTableNameCSVArray::~CTableNameCSVArray() 
{
}

/**********************************************************************/

int CTableNameCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	int nLastTableNo = 0;
	int nValidRecords = 0;
	
	CTableNameCSVRecord Table;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		int nTableNo = GetTableNo( nValidRecords );
		if ( ( nTableNo <= nLastTableNo ) || ( nTableNo > SPosTableName::TableNo.Max ) )
			break;

		nLastTableNo = nTableNo;
	}

	while ( GetSize() > nValidRecords )
		RemoveAt( GetSize() - 1 );

	return nReply;
}

/**********************************************************************/

bool CTableNameCSVArray::FindTableByNumber( int nTableNo, int& nTableIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nTableIdx = ( nStart + nEnd ) / 2;
		
		int nArrayNo = GetTableNo(nTableIdx);
			
		if ( nTableNo == nArrayNo )
			return TRUE;

		if ( nTableNo < nArrayNo )
			nEnd = nTableIdx - 1;
		else
			nStart = nTableIdx + 1;
	}

	nTableIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CTableNameCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	for ( int n = 0; n < GetSize(); n++ )
	{
		if ( GetTableNo(n) > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if ( nResult == 0 )
		nResult = GetSize() + 1;

	if ( nResult < SPosTableName::TableNo.Min || nResult > SPosTableName::TableNo.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

int CTableNameCSVArray::GetTableNo( int nTableIdx )
{
	int nTableNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nTableIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nTableNo = csv.GetInt(0);
	}

	return nTableNo;
}

/**********************************************************************/

