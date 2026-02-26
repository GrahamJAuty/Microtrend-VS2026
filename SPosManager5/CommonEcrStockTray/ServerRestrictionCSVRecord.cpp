/**********************************************************************/
 
/**********************************************************************/
#include "ServerCSVArray.h"
/**********************************************************************/
#include "ServerRestrictionCSVArray.h"
/**********************************************************************/

CServerRestrictionCSVRecord::CServerRestrictionCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CServerRestrictionCSVRecord::ClearRecord()
{
	m_nServerNo = 1;
	m_strTimeStamp = "";
	m_strRestrictions = "";
}

/**********************************************************************/

void CServerRestrictionCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nServerNo );
	csv.Add ( m_strTimeStamp );
	csv.Add ( SERVERRESTRICTION_VERSION );
	csv.Add ( m_strRestrictions );
}

/**********************************************************************/

void CServerRestrictionCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	
	SetServerNo( csv.GetInt(0) );
	SetTimeStamp( csv.GetString(1) );

	switch ( csv.GetInt(2) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CServerRestrictionCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetRestrictions( csv.GetString(3) );
}
	
/**********************************************************************/
/*	Set Routines													  */
/**********************************************************************/

void CServerRestrictionCSVRecord::SetServerNo( int n )
{
	Set ( m_nServerNo, n, Server::ServerNo );
}

/**********************************************************************/

void CServerRestrictionCSVRecord::SetTimeStamp( const char* sz )
{
	Set ( m_strTimeStamp, sz, Server::TimeStamp );
}

/**********************************************************************/

void CServerRestrictionCSVRecord::SetRestrictions( const char* sz )
{
	Set ( m_strRestrictions, sz, ServerRestriction::Restrictions );
}

/**********************************************************************/