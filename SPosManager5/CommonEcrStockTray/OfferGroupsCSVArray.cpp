/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "OfferGroupsCSVArray.h"
/**********************************************************************/

COfferGroupsCSVRecord::COfferGroupsCSVRecord( int nNodeType )
{
	m_nNodeType = nNodeType;
	ClearRecord();
}

/**********************************************************************/

void COfferGroupsCSVRecord::ClearRecord()
{
	m_nGroupNo = 1;
	m_strGroupName = "";
}

/**********************************************************************/

void COfferGroupsCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord(); 

	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void COfferGroupsCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetGroupNo( csv.GetInt(1) );
	SetGroupName( csv.GetString(2) );
}

/**********************************************************************/
	
void COfferGroupsCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( OFFERGROUPS_VERSION );
	csv.Add ( m_nGroupNo );
	csv.Add ( m_strGroupName );
}

/**********************************************************************/

const char* COfferGroupsCSVRecord::GetDisplayName()
{
	m_strDisplayName = m_strGroupName;
	::TrimSpaces( m_strDisplayName, FALSE );

	if ( m_strDisplayName == "" )
	{
		if ( NODE_PLUFILTER_NAME == m_nNodeType )
			m_strDisplayName.Format( "Plu Filter %d", m_nGroupNo );
		else
			m_strDisplayName.Format( "Offer Group %d", m_nGroupNo );
	}
		
	return m_strDisplayName;
}

/**********************************************************************/

void COfferGroupsCSVRecord::SetGroupNo( int n )
{
	Set ( m_nGroupNo, n, OfferGroups::GroupNo );
}

/**********************************************************************/

void COfferGroupsCSVRecord::SetGroupName( const char* sz )
{
	CString str = sz;
	::TrimSpaces( str, FALSE );

	Set( m_strGroupName, str, OfferGroups::GroupName );

	if ( m_strGroupName == "" )
	{
		if ( NODE_PLUFILTER_NAME == m_nNodeType )
			m_strGroupName.Format( "Plu Filter %d", m_nGroupNo );
		else
			m_strGroupName.Format( "Offer Group %d", m_nGroupNo );
	}	
}

/**********************************************************************/

COfferGroupsCSVArray::COfferGroupsCSVArray( int nNodeType ) 
{
	m_nNodeType = nNodeType;
}

/**********************************************************************/

COfferGroupsCSVArray::~COfferGroupsCSVArray() 
{
}

/**********************************************************************/

bool COfferGroupsCSVArray::FindTableByNumber( int nTableNo, int& nTableIdx )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nTableIdx = ( nStart + nEnd ) / 2;
		
		COfferGroupsCSVRecord Group( m_nNodeType );
		GetAt ( nTableIdx, Group );
		int nArrayNo = Group.GetGroupNo();
			
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

int COfferGroupsCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	COfferGroupsCSVRecord Group( m_nNodeType );
	for ( int n = 0; n < GetSize(); n++ )
	{
		GetAt( n, Group );
		if ( Group.GetGroupNo() > n + 1 )
		{
			nResult = n + 1;
			break;
		}
	}

	if ( nResult == 0 )
		nResult = GetSize() + 1;

	if ( nResult < OfferGroups::GroupNo.Min || nResult > OfferGroups::GroupNo.Max )
		nResult = 0;

	return nResult;
}

/**********************************************************************/

int COfferGroupsCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nLastGroupNo = 0;
	int nValidRecords = 0;
	
	COfferGroupsCSVRecord Group( m_nNodeType );
	for ( ; nValidRecords < GetSize(); nValidRecords++ )
	{
		GetAt( nValidRecords, Group );

		int nGroupNo = Group.GetGroupNo();
		if ( nGroupNo <= nLastGroupNo || nGroupNo > OfferGroups::GroupNo.Max )
			break;

		nLastGroupNo = nGroupNo;
	}

	while ( GetSize() > nValidRecords )
		RemoveAt( GetSize() - 1 );

	return nReply;
}

/**********************************************************************/

