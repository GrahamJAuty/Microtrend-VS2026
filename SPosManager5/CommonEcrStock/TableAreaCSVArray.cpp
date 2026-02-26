/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/
#include "TableAreaCSVArray.h"
/**********************************************************************/

CTableAreaCSVRecord::CTableAreaCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTableAreaCSVRecord::ClearRecord()
{
	m_nAreaNo = 1;
	m_strAreaName = "";
	m_bAutoTabsOnly = FALSE;
	m_bSuppressCovers = FALSE;
	m_nTableOpenMacro = 0;
	m_nKPFlags = 0xFFFF;
	m_nTableNoLo = 0;
	m_nTableNoHi = 0;
}

/**********************************************************************/

void CTableAreaCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetAreaNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CTableAreaCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetAreaName					( csv.GetString(2) );
	SetAutoTabsOnlyFlag			( csv.GetBool(3) );
	SetSuppressCoversFlag		( csv.GetBool(4) );
	SetTableOpenMacro			( csv.GetInt(5) );
	SetKPFlags					( csv.GetInt(6) );
	SetTableNoLo				( csv.GetInt(7) );
	SetTableNoHi				( csv.GetInt(8) );
}

/**********************************************************************/

void CTableAreaCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nAreaNo );
	csv.Add ( TABLEAREA_VERSION );
	csv.Add ( m_strAreaName );
	csv.Add ( m_bAutoTabsOnly );
	csv.Add ( m_bSuppressCovers );
	csv.Add ( m_nTableOpenMacro );
	csv.Add ( m_nKPFlags );
	csv.Add ( m_nTableNoLo );
	csv.Add ( m_nTableNoHi );
}

/**********************************************************************/

void CTableAreaCSVRecord::SetAreaNo( int n )
{
	Set ( m_nAreaNo, n, SPosTableArea::AreaNo );
}

/**********************************************************************/

void CTableAreaCSVRecord::SetAreaName( const char* sz )
{
	Set ( m_strAreaName, sz, SPosTableArea::AreaName );
}

/**********************************************************************/

void CTableAreaCSVRecord::SetTableOpenMacro( int n )
{
	Set ( m_nTableOpenMacro, n, SPosTableArea::TableOpenMacro );
}

/**********************************************************************/

void CTableAreaCSVRecord::SetKPFlags( int n )
{
	Set ( m_nKPFlags, n, SPosTableArea::KPFlags );
}

/**********************************************************************/

void CTableAreaCSVRecord::SetTableNoHi( int n )
{
	Set ( m_nTableNoHi, n, SPosTableArea::TableNoHi );
}

/**********************************************************************/

void CTableAreaCSVRecord::SetTableNoLo( int n )
{
	Set ( m_nTableNoLo, n, SPosTableArea::TableNoLo );
}

/**********************************************************************/

bool CTableAreaCSVRecord::GetKPFlag( int n )
{
	if ( ( n >= 1 ) && ( n <= 16 ) )
	{
		int nMask = 1 << ( n - 1 );
		return ( ( m_nKPFlags & nMask ) != 0 );
	}
	else
	{
		return FALSE;
	}
}

/**********************************************************************/

void CTableAreaCSVRecord::SetKPFlag( int n, bool b )
{
	if ( ( n >= 1 ) && ( n <= 16 ) )
	{
		int nMask = 1 << ( n - 1 );
		
		if ( TRUE == b )
			m_nKPFlags |= nMask;
		else
			m_nKPFlags &= ( 0xFFFF - nMask );
	}
}

/**********************************************************************/

void CTableAreaCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTableAreaCSVArray::CTableAreaCSVArray() 
{
}

/**********************************************************************/

CTableAreaCSVArray::~CTableAreaCSVArray() 
{
}

/**********************************************************************/

int CTableAreaCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}
	
	int nLastAreaNo = 0;
	int nValidRecords = 0;
	
	CTableAreaCSVRecord Area;
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		int nAreaNo = GetAreaNo( nValidRecords );
		if ( ( nAreaNo <= nLastAreaNo ) || ( nAreaNo > SPosTableArea::AreaNo.Max ) )
			break;

		nLastAreaNo = nAreaNo;
	}

	while ( GetSize() > nValidRecords )
		RemoveAt( GetSize() - 1 );

	return nReply;
}

/**********************************************************************/

bool CTableAreaCSVArray::FindAreaByNumber( int nAreaNo, int& nAreaIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nAreaIdx = ( nStart + nEnd ) / 2;
		
		int nArrayNo = GetAreaNo(nAreaIdx);
			
		if ( nAreaNo == nArrayNo )
			return TRUE;

		if ( nAreaNo < nArrayNo )
			nEnd = nAreaIdx - 1;
		else
			nStart = nAreaIdx + 1;
	}

	nAreaIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CTableAreaCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	for ( int n = 0; n < GetSize(); n++ )
	{
		if ( GetAreaNo(n) > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if ( nResult == 0 )
		nResult = GetSize() + 1;

	if ( nResult < SPosTableArea::AreaNo.Min || nResult > SPosTableArea::AreaNo.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

int CTableAreaCSVArray::GetAreaNo( int nAreaIdx )
{
	int nAreaNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nAreaIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nAreaNo = csv.GetInt(0);
	}

	return nAreaNo;
}

/**********************************************************************/

